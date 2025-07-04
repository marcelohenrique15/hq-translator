FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Sao_Paulo

# INSTALAR DEPENDÊNCIAS DO SISTEMA
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    libgif-dev \
    libwebp-dev \
    libopenjp2-7-dev \
    zlib1g-dev \
    nlohmann-json3-dev \
    wget \
    ca-certificates \
    libicu-dev \
    libarchive-dev \
    libcurl4-openssl-dev \
    libpango1.0-dev \
    libcairo2-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libssl-dev \
    libgtk-3-dev \ 
    x11-xserver-utils \ 
    x11-apps \
    mesa-utils \
    xauth \
    curl \
    && rm -rf /var/lib/apt/lists/*

# INSTALAR BIBLIOTECAS DO PROJETO
WORKDIR /tmp/build_libs

RUN ( cd /tmp/build_libs && \
    wget https://github.com/DanBloomberg/leptonica/releases/download/1.83.1/leptonica-1.83.1.tar.gz && \
    tar -xzvf leptonica-1.83.1.tar.gz && \
    cd leptonica-1.83.1 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    ldconfig )

RUN ( cd /tmp/build_libs && \
    wget https://github.com/tesseract-ocr/tesseract/archive/refs/tags/5.3.0.tar.gz && \
    tar -xzvf 5.3.0.tar.gz && \
    cd tesseract-5.3.0/ && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    ldconfig )

WORKDIR /
RUN rm -rf /tmp/build_libs

WORKDIR /usr/local/share/tessdata

RUN wget https://raw.githubusercontent.com/tesseract-ocr/tessdata_best/main/eng.traineddata

WORKDIR /hq-translator

COPY . .