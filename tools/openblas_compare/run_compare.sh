#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BENCH_DIR="$ROOT_DIR/tools/openblas_compare"
RESULT_DIR="${RESULT_DIR:-/tmp/openblas_compare_results}"
THREADS="${THREADS:-$(nproc)}"
BAZEL_FLAGS=(
  --registry="file://$ROOT_DIR"
  --registry="https://bcr.bazel.build"
  --spawn_strategy=local
  --action_env=PATH=/home/linuxbrew/.linuxbrew/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
)

mkdir -p "$RESULT_DIR"

cd "$BENCH_DIR"

bazel build "${BAZEL_FLAGS[@]}" //:registry_bench //:homebrew_bench

run_case() {
  local label="$1"
  local binary="$2"
  local threads="$3"
  local logfile="$RESULT_DIR/${label}_threads_${threads}.log"

  export OPENBLAS_NUM_THREADS="$threads"
  export OMP_NUM_THREADS="$threads"
  export LD_LIBRARY_PATH="/home/linuxbrew/.linuxbrew/opt/openblas/lib:${LD_LIBRARY_PATH:-}"

  echo "running $label with $threads thread(s)"
  "$binary" | tee "$logfile"
}

run_case "registry" "./bazel-bin/registry_bench" "1"
run_case "homebrew" "./bazel-bin/homebrew_bench" "1"
run_case "registry" "./bazel-bin/registry_bench" "$THREADS"
run_case "homebrew" "./bazel-bin/homebrew_bench" "$THREADS"

echo "results written to $RESULT_DIR"
