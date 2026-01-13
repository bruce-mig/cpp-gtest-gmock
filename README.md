# C++ Unit Testing Template

[![CI Testing](https://github.com/bruce-mig/cpp-gtest-gmock/actions/workflows/ci-testing.yaml/badge.svg)](https://github.com/bruce-mig/cpp-gtest-gmock/actions/workflows/ci-testing.yaml)
[![CI Dev Container](https://github.com/bruce-mig/cpp-gtest-gmock/actions/workflows/ci-devcontainer.yaml/badge.svg)](https://github.com/bruce-mig/cpp-gtest-gmock/actions/workflows/ci-devcontainer.yaml)
[![Docker Pulls](https://img.shields.io/docker/pulls/bmigeri/devcon-cpp)](https://hub.docker.com/r/bmigeri/devcon-cpp)
[![Docker Image Size](https://img.shields.io/docker/image-size/bmigeri/devcon-cpp/latest)](https://hub.docker.com/r/bmigeri/devcon-cpp)
[![Docker Image Version](https://img.shields.io/docker/v/bmigeri/devcon-cpp?sort=semver)](https://hub.docker.com/r/bmigeri/devcon-cpp)

A reference implementation for C++ unit testing with [Google Test](https://github.com/google/googletest) and [Google Mock](https://github.com/google/googletest/tree/main/googlemock), featuring a containerized development environment.

## Purpose

This repository provides a **starter template** for C++ projects that require:

- Unit testing with GTest/GMock
- Interface-based design for testable code
- Mocking external dependencies
- Containerized, reproducible development environment
- CI/CD pipeline for automated testing

The example code demonstrates testing patterns using a simple library with injected dependencies (logger, noise model), showing how to isolate units under test from their collaborators.

## Project Structure

```bash
├── .devcontainer/          # Dev container configuration
│   ├── Dockerfile          # C++ development image
│   ├── devcontainer.json   # VS Code integration
│   └── init-volumes.sh     # Volume setup script
├── .github/workflows/      # CI/CD pipeline
├── app/                    # Example application
│   └── app.cc
├── lib/                    # Example library (unit under test)
│   ├── algebraClass.hh/cc  # Class with injectable dependencies
│   ├── ILogger.hh          # Logger interface (abstract)
│   ├── INoise.hh           # Noise model interface (abstract)
│   └── FileLogger.hh       # Concrete logger implementation
├── tst/                    # Test suite
│   ├── tst.cc              # GTest test cases
│   ├── logger_mock.hh      # GMock implementation of ILogger
│   └── noise_mock.hh       # GMock implementation of INoise
├── CMakeLists.txt
└── README.md
```

## What's Demonstrated

### Interface-Based Design

The `AlgebraClass` depends on abstract interfaces, not concrete implementations:

```cpp
class AlgebraClass {
public:
    AlgebraClass(const INoise* noise) : noise_(noise) {}
    void setLogger(ILogger* logger) { logger_ = logger; }
    // ...
};
```

This allows injecting mock implementations during testing.

### Mocking with GMock

Mock classes implement interfaces and allow setting expectations:

```cpp
class MockNoise : public INoise {
public:
    MOCK_METHOD(float, addNoise, (), (const, override));
};

// In test:
MockNoise mock_noise;
EXPECT_CALL(mock_noise, addNoise).WillOnce(testing::Return(1.0));
```

### Test Cases

| Test | What it verifies |
|------|------------------|
| `SquareTwo` | Basic computation without dependencies |
| `SquareTwoNoiseThrow` | Exception when required dependency is missing |
| `SquareTwoNoise` | Computation with mocked noise injection |
| `testLongOpLogging` | Correct logging calls during operations |

## Getting Started

### Option 1: Dev Container (Recommended)

Requires [Docker](https://www.docker.com/get-started) and [VS Code](https://code.visualstudio.com/) with [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers).

1. Clone this repository
2. Open in VS Code
3. Click "Reopen in Container" when prompted

### Option 2: Local Setup

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake ninja-build libgtest-dev

# macOS
brew install cmake ninja googletest
```

## Build & Test

```bash
# Configure
cmake -B build -DBUILD_TESTS=ON -GNinja

# Build
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure

# Or using ninja
cd build
ninja && ./tests
```

### Code Coverage

```bash
cmake -B build -DBUILD_TESTS=ON -DCMAKE_CXX_FLAGS="--coverage" -GNinja
cmake --build build
ctest --test-dir build

lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage-report
```

## Dev Container

The included container provides a complete C++ development environment:

- **Compilers:** GCC, Clang
- **Build:** CMake, Ninja
- **Testing:** GTest, GMock (pre-built)
- **Analysis:** clang-tidy, cppcheck, iwyu
- **Debugging:** GDB, LLDB, Valgrind
- **Coverage:** lcov, gcovr
- **Libraries:** Eigen3, Boost, libcurl, OpenSSL, fmt, spdlog

### Shell Aliases

```bash
cmake-debug      # Configure debug build
cmake-release    # Configure release build
cmake-coverage   # Configure with coverage flags
run-tests        # Run tests with verbose output
```

## CI/CD

GitHub Actions workflow includes:

- Container image build and test
- Trivy security scanning
- SBOM generation
- Docker Hub push (on main branch)

## Technologies

| Component | Version |
|-----------|---------|
| C++ | 17 |
| CMake | 3.20+ |
| Google Test/Mock | 1.17.0 |
| Docker | Latest |

## Adapting This Template

1. Replace `lib/` contents with your own library code
2. Define interfaces for external dependencies
3. Create mock implementations in `tst/`
4. Write tests using the demonstrated patterns
5. Update `CMakeLists.txt` with your source files

## Resources

- [GoogleTest Primer](https://google.github.io/googletest/primer.html)
- [GoogleMock for Dummies](https://google.github.io/googletest/gmock_for_dummies.html)
- [Dev Containers Specification](https://containers.dev/)
