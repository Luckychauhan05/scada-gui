#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

cmake --build build -j"$(nproc)"
QT_QPA_PLATFORM=xcb ./build/IndustrialScadaQt
