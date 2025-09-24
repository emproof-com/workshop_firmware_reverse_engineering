FROM kalilinux/kali-rolling:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        libssl-dev \
        zlib1g-dev \
        libzstd-dev \
        pkg-config \
        unzip \
        binutils \
        strace \
        ltrace \
        patchelf \
        gcc \
        gcc-arm-linux-gnueabihf \
        binutils-arm-linux-gnueabihf \
        git \
        vim \
        ipython3 \
        binwalk \
        john \
        wordlists && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN dpkg --add-architecture armhf \
    && apt-get update && apt-get install -y \
        libssl-dev:armhf \
        zlib1g-dev:armhf \
        libzstd-dev:armhf \
        libc6:armhf \
        libgcc-s1:armhf \
        libstdc++6:armhf && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /work
ENTRYPOINT ["/bin/bash"]
