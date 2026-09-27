# Software

Everything that runs as code.

[日本語](README.ja.md)

## Layout

| Path | Runs on | Stage | Contents |
| --- | --- | --- | --- |
| [`firmware/`](firmware) | ESP32 (the body) | 1 | Motor control, encoders, safety stop. A PlatformIO project. |
| `tools/` | Mac | 1 | Small tools to drive the robot before ROS 2. Added when that work starts. |
| `ros2/` | The brain (a Mac for now, later a phone) | 2 | ROS 2 workspace. Added when that work starts. |

Each folder has its own guide. Read it before you start.

## The rule across folders

The firmware and the brain talk only through the brain–body contract in
[docs/architecture.md](../docs/architecture.md):

- The brain sends only a linear and an angular velocity, never PWM values
  or per-motor commands.
- The body reports motion as cumulative totals, plus sensor data.
- The body stops the motors by itself when commands stop arriving.

The exact messages are specified in [docs/protocol.md](../docs/protocol.md).
Every implementation, on the body or the brain, follows that file.

Do not add shortcuts around the contract, even for testing. It is what lets
the brain be replaced without touching the firmware.

## Workflow

See [CONTRIBUTING.md](../CONTRIBUTING.md). CI runs the firmware tests on
every pull request.
