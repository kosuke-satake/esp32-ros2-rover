# Firmware

Firmware for the ESP32 body, written with the Arduino framework and built
with PlatformIO.

[日本語](README.ja.md)

## Setup

1. Install [VS Code](https://code.visualstudio.com/) and its PlatformIO IDE
   extension.
2. On a Mac, the host tests also need a C++ compiler. Install the Xcode
   Command Line Tools with `xcode-select --install`.
3. Open this folder (`software/firmware`) in VS Code, or `cd` into it.

## Run the tests

```sh
pio test -e native
```

This runs the unit tests on your computer; no ESP32 is needed. CI runs the
same tests on every pull request.

Building and flashing the ESP32 will be added here once the board is chosen.

## Layout

| Path | Contents |
| --- | --- |
| `platformio.ini` | Build environments. Only `native` (host tests) for now. |
| `lib/` | Hardware-independent logic: kinematics, PID, command timeout. No Arduino code. |
| `test/` | Unit tests for `lib/`, one folder per module. |

To add a module, create `lib/<name>/<name>.h` and `lib/<name>/<name>.cpp`,
and its tests in `test/test_<name>/test_main.cpp`.

## Rules

From [docs/architecture.md](../../docs/architecture.md):

- Never use `delay()`. Run periodic work on a fixed schedule with `millis()`.
- Read serial and UDP input only as far as data has arrived; never block the
  loop.
- Stop the motors when commands stop arriving.
- Read encoders with interrupts or a hardware pulse counter.
- Keep hardware-independent logic in `lib/`, free of Arduino code, with tests
  in `test/`.
- Put Wi-Fi passwords and other secrets in `secrets.h`, which Git ignores.
  Commit only `secrets.example.h`.
- Do not weaken the safety layers, for example by lengthening or disabling
  the command timeout, without a decision from the maintainer.
