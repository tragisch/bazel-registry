#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BENCH_DIR="$ROOT_DIR/tools/openblas_compare"
RESULT_DIR="${RESULT_DIR:-/tmp/openblas_compare_results}"
RUN_DIR="${RUN_DIR:-$RESULT_DIR/$(date +%Y%m%d_%H%M%S)}"
THREADS="${THREADS:-$(nproc)}"
REPEATS="${REPEATS:-3}"
BAZEL_FLAGS=(
  --registry="file://$ROOT_DIR"
  --registry="https://bcr.bazel.build"
  --spawn_strategy=local
  --action_env=PATH=/home/linuxbrew/.linuxbrew/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
  --compilation_mode=opt
  --define=OPENBLAS_PROFILE=haswell
)

mkdir -p "$RUN_DIR"

cd "$BENCH_DIR"

bazel build "${BAZEL_FLAGS[@]}" "$@" //:registry_bench //:homebrew_bench

run_case() {
  local label="$1"
  local binary="$2"
  local threads="$3"
  local run="$4"
  local logfile="$RUN_DIR/${label}_threads_${threads}_run_${run}.log"

  export OPENBLAS_NUM_THREADS="$threads"
  export OMP_NUM_THREADS="$threads"
  export LD_LIBRARY_PATH="/home/linuxbrew/.linuxbrew/opt/openblas/lib:${LD_LIBRARY_PATH:-}"

  echo "running $label with $threads thread(s), run $run/$REPEATS"
  "$binary" | tee "$logfile"
}

thread_cases=("1")
if [[ "$THREADS" != "1" ]]; then
  thread_cases+=("$THREADS")
fi

for run in $(seq 1 "$REPEATS"); do
  for threads in "${thread_cases[@]}"; do
    run_case "registry" "./bazel-bin/registry_bench" "$threads" "$run"
    run_case "homebrew" "./bazel-bin/homebrew_bench" "$threads" "$run"
  done
done

awk '
  /^provider=/ {
    split($1, p, "=")
    split($2, b, "=")
    split($3, s, "=")
    split($4, t, "=")
    provider = p[2]
    benchmark = b[2]
    size = s[2]
    threads = t[2]
  }
  /^B\|/ {
    for (i = 1; i <= NF; ++i) {
      if ($i == "throughput") {
        split($(i + 1), value, "=")
        throughput = value[2]
      }
    }
    printf "%s\t%s\t%s\t%s\t%.3f\n", provider, benchmark, size, threads, throughput / 1000000000.0
  }
' "$RUN_DIR"/*.log | sort > "$RUN_DIR/summary_gflops.tsv"

echo "results written to $RUN_DIR"
echo "summary written to $RUN_DIR/summary_gflops.tsv"
