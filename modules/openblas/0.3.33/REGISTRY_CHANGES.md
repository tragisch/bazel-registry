# OpenBLAS Registry - Profilunterstützung (Phase 1)

## Datum: 2026-06-14

## Implementierte Änderungen

### 1. Bug-Fix: config.h - Doppelte Include-Guards
**Datei**: `/home/uwe/Dokumente/01_projects/bazel-registry/modules/openblas/0.3.33/overlay/bazel/config.h`

**Problem**: 
- Zeilen 1-2 und 4-5 hatten identische Include-Guards
- Führte zu "unterminated conditional directive" Compiler-Fehler

**Lösung**:
```c
// Vorher (fehlerhaft):
#ifndef OPENBLAS_BAZEL_CONFIG_H
#define OPENBLAS_BAZEL_CONFIG_H

#ifndef OPENBLAS_BAZEL_CONFIG_H    // <-- doppelt!
#define OPENBLAS_BAZEL_CONFIG_H    // <-- doppelt!

// Nachher (korrekt):
#ifndef OPENBLAS_BAZEL_CONFIG_H
#define OPENBLAS_BAZEL_CONFIG_H
```

**Neue Checksumme**: `sha256-K7ybk5oLQ1RQgm4Lyw2mLeUhbQM0ytaNoZzdmP1ktoA=`

---

### 2. Feature: Profilumschaltung aktiviert
**Datei**: `/home/uwe/Dokumente/01_projects/bazel-registry/modules/openblas/0.3.33/overlay/BUILD.bazel`

**Problem**:
- `config_haswell.h` existierte, aber wurde nie gefunden
- `#include "config_haswell.h"` in `config.h` schlug fehl: "file not found"
- Grund: `bazel/` Verzeichnis nicht im Include-Pfad

**Lösung**:
```python
# In OPENBLAS_COMMON_INCLUDES hinzugefügt:
OPENBLAS_COMMON_INCLUDES = [
    "bazel",              # <-- NEU: Damit config_haswell.h gefunden wird
    "driver/level3",
    "kernel/generic",
    "lapack/laswp/generic",
]
```

**Neue Checksumme**: `sha256-bRMKzdfHF/dkzqO8bpmr3cJVn5d8AVBn0diDD0MUcl4=`

---

### 3. Checksummen aktualisiert
**Datei**: `/home/uwe/Dokumente/01_projects/bazel-registry/modules/openblas/0.3.33/source.json`

```json
"overlay": {
    "BUILD.bazel": "sha256-bRMKzdfHF/dkzqO8bpmr3cJVn5d8AVBn0diDD0MUcl4=",
    "bazel/config.h": "sha256-K7ybk5oLQ1RQgm4Lyw2mLeUhbQM0ytaNoZzdmP1ktoA=",
    ...
}
```

---

## Verwendung

### Profil-Defines

Das Registry-Modul nutzt bereits vorhandene `config_setting`s:

```python
config_setting(
    name = "profile_haswell",
    values = {"define": "OPENBLAS_PROFILE=haswell"},
)

config_setting(
    name = "profile_generic",
    values = {"define": "OPENBLAS_PROFILE=generic"},
)
```

### Build-Kommandos

```bash
# Generic-Profil (portabel, keine CPU-spezifischen Optimierungen)
bazel build --define=OPENBLAS_PROFILE=generic //your:target

# Haswell-Profil (x86_64, AVX2, FMA3, optimierte Parameter)
bazel build --define=OPENBLAS_PROFILE=haswell //your:target
```

### Integration in abhängigen Projekten

In `.bazelrc`:
```bash
# OpenBLAS-Profile
build:openblas-generic --config=openblas --define=OPENBLAS_PROFILE=generic
build:openblas-haswell --config=openblas --define=OPENBLAS_PROFILE=haswell

# Linux Default
build:linux --config=openblas-haswell
```

---

## Technische Details

### Mechanismus der Profilumschaltung

1. **Build-Zeit**: `--define=OPENBLAS_PROFILE=haswell` aktiviert `profile_haswell` config_setting
2. **Compiler-Flag**: `OPENBLAS_COMMON_COPTS` fügt via `select()` `-DOPENBLAS_PROFILE_HASWELL` hinzu
3. **Präprozessor**: `config.h` prüft dieses Define:
   ```c
   #if defined(OPENBLAS_PROFILE_HASWELL)
   #include "config_haswell.h"
   #else
   // ... generic config
   #endif
   ```

### config_haswell.h Parameter

```c
#define CORE_haswell 1
#define CHAR_CORENAME "haswell"

// Cache-Parameter für Haswell
#define L1_DATA_SIZE 32768
#define L1_DATA_LINESIZE 64
#define L2_SIZE 262144
#define L2_LINESIZE 64
#define DTB_SIZE 4096
#define L2_ASSOCIATIVE 8

// Threading
#define MAX_CPU_NUMBER 56
#define MAX_PARALLEL_NUMBER 56
#define GEMM_MULTITHREAD_THRESHOLD 1

// OpenMP
#define USE_OPENMP 1
#define SMP 1
#define SMP_SERVER 1
```

---

## Bekannte Einschränkungen

### 1. LAPACK Parallel-Funktionen fehlen
**Symptom**: Linker-Fehler für `*_parallel` Symbole (z.B. `spotrf_L_parallel`)

**Ursache**: Registry-Port enthält nicht alle LAPACK-Thread-Targets

**Status**: Separates Problem, existiert unabhängig von Profilumschaltung

### 2. Kernel-Pfade noch nicht optimiert
**Status**: Diese Änderungen aktivieren nur die Profilumschaltung

**Nächste Schritte** (zukünftige Phasen):
- Phase 2: SGEMM-Kernel auf x86_64 umstellen
- Phase 3: Level-2 BLAS (gemv, symv) optimieren
- Phase 4: Level-1 BLAS optimieren

---

## Verifikation

### Compiler-Flags prüfen

```bash
bazel build --config=benchmark --define=OPENBLAS_PROFILE=haswell -s @openblas//:openblas 2>&1 | grep DOPENBLAS_PROFILE
```

Erwartete Ausgabe:
```
-DOPENBLAS_PROFILE_HASWELL -DOPENBLAS_BAZEL -DNO_LAPACKE ...
```

### Smoke-Test

```bash
bazel test @openblas_lapack//:openblas_lapack_smoke_test \
  --config=benchmark \
  --define=OPENBLAS_PROFILE=haswell \
  --test_output=errors
```

---

## Git-Commit-Vorlage

```
feat(openblas): Aktiviere CPU-Profilumschaltung im Registry-Port

- Fix: Doppelte Include-Guards in config.h entfernt
- Feature: bazel/ zu OPENBLAS_COMMON_INCLUDES hinzugefügt
  - Ermöglicht Include von config_haswell.h
- Update: source.json Checksummen aktualisiert

Profile verfügbar:
- generic: Portabel, keine CPU-spezifischen Optimierungen
- haswell: x86_64, AVX2, FMA3, Haswell-Parameter

Verwendung: --define=OPENBLAS_PROFILE={generic|haswell}

Phase 1 von geplanten Kernel-Optimierungen.
```
