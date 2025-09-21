#!/usr/bin/env bash
set -euo pipefail

# Ensure host can execute arm64 binaries (idempotent). Faster to skip if already present.
if [[ ! -e /proc/sys/fs/binfmt_misc/qemu-aarch64 ]]; then
  echo "[binfmt] installing qemu handlers on host."
  docker run --rm --privileged tonistiigi/binfmt --install all
fi

# Make sure a buildx builder exists and is ready.
docker buildx create --use --name multi >/dev/null 2>&1 || docker buildx use multi
docker buildx inspect --bootstrap >/dev/null 2>&1 || true

# Build ARM64 image and load it into the local daemon for compose to reuse.
docker buildx build --platform linux/arm64 -t kali-re-tools:arm64 --load .

# Run the workshop container.
docker compose run --rm kali