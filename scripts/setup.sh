#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
# SPDX-FileCopyrightText: Duszku

set -euo pipefail

EXECUTABLES="clang mold git meson"
source "$(dirname "${0}")/common"

pushd "${TOPDIR}"
git config --add core.hooksPath scripts/hooks
CC=clang LDFLAGS="-B$(which mold)" meson setup build
popd
