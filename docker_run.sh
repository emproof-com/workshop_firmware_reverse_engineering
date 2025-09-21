#!/usr/bin/env bash
set -euo pipefail

# 0) Register ARM64 emulation once (idempotent). This installs the binfmt
#    handler so an x86_64 host can execute arm64 binaries during the build.
docker compose run --rm --profile setup binfmt >/dev/null 2>&1 || true

# 1) Ensure Buildx (multi-arch builder) is active and warmed up.
#    If a builder named "multi" doesn't exist, create+use it; otherwise, use it.
docker buildx create --use --name multi >/dev/null 2>&1 || docker buildx use multi
docker buildx inspect --bootstrap >/dev/null

# 2) Force ARM64 for build and compose. BuildKit + CLI integration enable
#    modern multi-arch builds; DOCKER_DEFAULT_PLATFORM pins the target arch.
export DOCKER_BUILDKIT=1
export COMPOSE_DOCKER_CLI_BUILD=1
export DOCKER_DEFAULT_PLATFORM=linux/arm64

# 3) Build the image explicitly as ARM64 (QEMU will be used on x86_64 hosts).
docker buildx build --platform linux/arm64 -t kali-re-tools:arm64 .

# 4) Run the container. docker compose will reuse the locally built ARM64 image.
docker compose run --rm kali