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
        gcc \
        git \
        vim \
        ipython3 \
        binwalk \
        john \
        wordlists && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /work
ENTRYPOINT ["/bin/bash"]
