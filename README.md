# VeloDB

[![Language: C++20](https://img.shields.io/badge/language-C%2B%2B20-blue.svg)](https://isocpp.org/)
[![Build System: CMake](https://img.shields.io/badge/build-CMake-informational.svg)](https://cmake.org/)
[![Deps: Conan](https://img.shields.io/badge/deps-Conan-green.svg)](https://conan.io/)

VeloDB is a minimalist, high-performance **in-memory key-value database** built for real-time use cases.

This repository currently ships a small C++20 core (`kvdb`) plus a TCP server with a simple, Redis-like text protocol.

## What you get today

- **Fast in-memory KV store** (string keys, string values)
- **TTL support** via `EXPIRE`
- **Async TCP server** powered by Asio, with a simple text protocol
- **Tiny command set** you can extend easily
- **CMake + Conan** build scripts for repeatable builds

## Protocol (quick peek)

The server speaks a simple, line-based protocol (space-separated arguments):

- `PING`
- `SET <key> <value>`
- `GET <key>`
- `DEL <key>`
- `EXPIRE <key> <seconds>`
- `INFO`

## Build

### Prerequisites

- A C++20 compiler (Apple Clang / GCC / Clang)
- CMake (>= 3.16)
- Conan (v2 recommended)

### Release build

```bash
cd kvdb
./scripts/build_release.sh
```

### Debug build

```bash
cd kvdb
./scripts/build_debug.sh
```

## Run the server

### Default (release)

```bash
cd kvdb
./scripts/run_server.sh
```

By default, the server listens on port **6379**.

### Custom port

```bash
cd kvdb
./build/release/kvdb-server 6380
```

## Try it locally

In another terminal:

```bash
nc 127.0.0.1 6379
```

Then type commands:

```text
PING
SET hello world
GET hello
EXPIRE hello 3
INFO
```

## Project status

VeloDB is **early-stage / WIP** and currently focused on being small, understandable, and fast.

## Roadmap (rough ideas)

- **[ ]** Simple persistence (snapshots)
- **[ ]** More commands (increment, lists, etc.)
- **[ ]** Basic auth / access control
- **[ ]** Benchmarks and comparison docs
- **[ ]** Docker image for easy running

## License

See `LICENSE`.
