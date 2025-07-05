
FROM ubuntu:22.04

# Build arguments
ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=America/Los_Angeles
ARG VCPK_VERSION=2024.01.12

# Labels for image
LABEL org.opencontainers.image.title="ATLAS Development Environment"
LABEL org.opencontainers.image.description="Pre-built development environment for ATLAS"
LABEL org.opencontainers.image.vendor="CSU Fresno Unmanned Systems Research Team"
LABEL org.opencontainers.image.source="https://github.com/CSUFresno-UnmannedSystemsResearchTeam/UTM-Software_atlas"


# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Los_Angeles
ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"
ENV CMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"

# Install essential packages
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    # Build essentials
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    ccache \
    # Version control and tools
    git-all \
    curl \
    wget \ 
    unzip \
    zip \
    tar \
    # Development
    gdb \ 
    lldb \
    clang \
    clang-format \
    clang-tidy \
    cppcheck \
    valgrind \
    strace \
    # Python development
    python3 \
    python3-pip \
    python3-venv \
    python3-dev \
    # Qt 6 development framework
    qt6-base-dev \
    qt6-tools-dev \
    qt6-declarative-dev \
    qt6-multimedia-dev \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6quick6 \
    libqt6multimedia6 \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    # Database development libraries
    postgresql-client \
    redis-tools \
    libpq-dev \
    libhiredis-dev \
    # Protocol Buffers and gRPC
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    # C++ libraries commonly used
    libboost-all-dev \
    libeigen3-dev \
    libopencv-dev \
    libfmt-dev \
    libspdlog-dev \ 
    libjsoncpp-dev \
    # Testing frameworks
    libgtest-dev \
    libgmock-dev \
    # OpenSSL for communications
    libssl-dev \
    # Audo/Video libraries 
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    # GNU Radio
    gnuradio 
