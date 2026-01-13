# syntax=docker/dockerfile:1.4
# ==============================================================================
# devcon-cpp: C/C++ Development Container
# Streamlined for GTest/GMock unit testing workflows
# ==============================================================================

# ------------------------------------------------------------------------------
# Stage 1: Build GoogleTest/GoogleMock from source
# ------------------------------------------------------------------------------
FROM ubuntu:22.04 AS gtest-builder

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC

RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

ARG GTEST_VERSION=v1.17.0
ENV GTEST_INSTALL_PREFIX=/opt/gtest

WORKDIR /tmp/gtest

RUN git clone --depth 1 --branch ${GTEST_VERSION} \
    https://github.com/google/googletest.git . && \
    cmake -B build -S . \
    -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_PREFIX} \
    -DCMAKE_CXX_STANDARD=17 \
    -DBUILD_GMOCK=ON \
    -DINSTALL_GTEST=ON && \
    cmake --build build --target install && \
    rm -rf /tmp/gtest

# ------------------------------------------------------------------------------
# Stage 2: Final development image
# ------------------------------------------------------------------------------
FROM ubuntu:22.04 AS development

LABEL org.opencontainers.image.title="devcon-cpp" \
    org.opencontainers.image.description="C/C++ Testing Container with GTest/GMock" \
    org.opencontainers.image.version="1.0.0" \
    org.opencontainers.image.authors="bmigeri@gmail.com"

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC

# Install development tools, libraries, and dependencies in a single layer
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt-get update && apt-get install -y --no-install-recommends \
    # Build essentials
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    git \
    # Compilers
    gcc \
    g++ \
    clang \
    # Debugging & profiling tools
    gdb \
    lldb \
    valgrind \
    strace \
    linux-tools-generic \
    # Code quality and static analysis
    clangd \
    clang-format \
    clang-tidy \
    cppcheck \
    iwyu \
    # Code coverage
    lcov \
    gcovr \
    # Documentation
    doxygen \
    graphviz \
    # Essential C++ libraries
    libeigen3-dev \
    libboost-dev \
    libboost-system-dev \
    libboost-filesystem-dev \
    libboost-thread-dev \
    libboost-program-options-dev \
    libboost-test-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    libjsoncpp-dev \
    libfmt-dev \
    libspdlog-dev \
    # Utilities
    curl \
    wget \
    vim \
    nano \
    bash-completion \
    sudo \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

# Copy GoogleTest from build stage
COPY --from=gtest-builder /opt/gtest /opt/gtest

# Setup environment paths
ENV PKG_CONFIG_PATH=/opt/gtest/lib/pkgconfig:${PKG_CONFIG_PATH} \
    LD_LIBRARY_PATH=/opt/gtest/lib:${LD_LIBRARY_PATH} \
    CMAKE_PREFIX_PATH=/opt/gtest:${CMAKE_PREFIX_PATH}

# Create non-root user for development
ARG USERNAME=developer
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} -s /bin/bash \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME} \
    && chmod 0440 /etc/sudoers.d/${USERNAME}

# Create workspace with correct ownership
WORKDIR /workspace
RUN chown ${USERNAME}:${USERNAME} /workspace

# Switch to non-root user
USER ${USERNAME}

SHELL ["/bin/bash", "-c"]

# Configure shell environment for developer
RUN echo 'alias ll="ls -lah"' >> ~/.bashrc \
    && echo 'alias cmake-debug="cmake -DCMAKE_BUILD_TYPE=Debug -GNinja"' >> ~/.bashrc \
    && echo 'alias cmake-release="cmake -DCMAKE_BUILD_TYPE=Release -GNinja"' >> ~/.bashrc \
    && echo 'alias cmake-coverage="cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=\"--coverage\" -GNinja"' >> ~/.bashrc \
    && echo 'alias run-tests="ctest --output-on-failure"' >> ~/.bashrc \
    && echo 'export PS1="\[\e[32m\]\u@devcon-cpp\[\e[m\]:\[\e[34m\]\w\[\e[m\]\$ "' >> ~/.bashrc

HEALTHCHECK --interval=60s --timeout=10s --start-period=5s --retries=3 \
    CMD g++ --version && cmake --version && test -f /opt/gtest/lib/libgtest.a || exit 1

CMD ["/bin/bash"]
