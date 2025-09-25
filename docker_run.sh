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

IMAGE_TAG="kali-re-tools:arm64"
DOCKERFILE_SHA="$(sha256sum Dockerfile | awk '{print $1}')"
HASH_TAG="${IMAGE_TAG}-${DOCKERFILE_SHA}"

# build only when hash-tagged image is missing
if ! docker image inspect "$HASH_TAG" >/dev/null 2>&1; then
  echo "[build] building $HASH_TAG"
  docker buildx build --platform linux/arm64 -t "$HASH_TAG" --load .
else
  echo "[build] up to date ($HASH_TAG)"
fi

# stable alias for compose
docker tag "$HASH_TAG" "$IMAGE_TAG"

# run
docker compose run --rm kali