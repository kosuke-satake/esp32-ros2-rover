# AGENTS.md

Instructions for AI agents working in this repository.
This file is the canonical source. `CLAUDE.md` and `GEMINI.md` only import it.
It is written to be self-contained: agents running in cloud sessions cannot
see the maintainer's local configuration, so everything needed is here.

## Project

`esp32-ros2-rover` is a low-cost, small differential-drive (two-wheel) mobile
robot, built up in stages. The long-term goals are ROS 2, camera-based
perception with neural networks, SLAM with LiDAR or similar sensors,
autonomous driving, and natural-language interaction through an LLM.

The repository currently holds design documents only. No firmware or ROS 2
code has been written yet.

- `docs/architecture.md`: decisions that are settled, with rationale.
- `docs/open-questions.md`: decisions that are deliberately still open.

Directories such as `firmware/`, `ros2/` and `hardware/` are created when real
work on them starts. Do not add empty directories or placeholder files.

## Architecture

The robot is split into a brain and a body.

- **Body**: an ESP32, programmed with the Arduino framework. It owns motor
  control (PID), encoders, sensors, battery monitoring and the safety stop.
  It must run on its own, without a brain.
- **Brain**: for now a Mac running ROS 2 in Docker. The target is an old
  Android phone (camera, screen, IMU, Wi-Fi and NN inference at low cost).
  The Mac may stay in use for heavy processing.
- **Controller**: an iPhone, for example as a remote that sets speed by
  tilting. An iPhone with LiDAR is also a candidate SLAM sensor.
- **Development**: on the Mac. The brain is reached over SSH (for example VS
  Code Remote-SSH). The ESP32 is flashed from the Mac.

The same split is used by TurtleBot3 (Raspberry Pi + OpenCR) and OpenBot
(smartphone + Arduino Nano or ESP32).

## Brain–body contract (most important)

Any change on either side must keep this contract.

1. **Brain → body sends velocity only**: a linear velocity and an angular
   velocity, equivalent to ROS `/cmd_vel`. Never send PWM values or per-motor
   commands from the brain. Conversion to left and right wheel speeds, and the
   PID loops, live in the body.
2. **Body → brain reports motion and sensor data.** Motion (encoder counts or
   equivalent) is reported as cumulative totals, never as deltas, so a lost
   UDP packet is recovered by the next one.
3. **Command timeout**: if commands stop arriving for a set time (for example
   0.3 s), the body stops the motors by itself.
4. **Transport independence**: the same text message must work unchanged
   over Wi-Fi (UDP) and over USB serial. Do not tie the message format to one
   link.

Because of this contract, the brain can be replaced (Mac, phone, Raspberry Pi)
without changing the ESP32 firmware. OpenBot sends motor PWM from the phone;
this project deliberately does not, so the body can drive standalone, an
iPhone can drive it directly, and the brain stays replaceable.

## Safety rules

When a layer fails, the layer below it stops the robot.

- A physical emergency-stop switch cuts motor power. It is the lowest layer.
- If the ESP32 hangs, its PWM hardware keeps driving the motors, so a hardware
  watchdog must reset it.
- If the brain or the link fails, the ESP32 detects missing commands and
  stops.
- If remote processing (for example on the Mac) fails, the brain detects it
  and stops the robot.
- Fast reactions such as stopping just before an obstacle run on the ESP32
  alone, independent of network latency.

Never weaken these layers, for example by lengthening or disabling the
command timeout, without an explicit decision from the maintainer.

## Communication and power

- Start with a simple, self-made, text-based protocol. Wi-Fi (UDP) is the
  first choice. The exact message syntax is defined when Stage 1 begins.
- Bandwidth is not a concern; camera video never passes through the ESP32.
- Typical problems are blocking calls (`delay()`, `pulseIn()`), missed
  encoder counts and I2C wiring, not the link itself.
- Managed Wi-Fi networks often block traffic between devices. The robot uses
  its own network, such as a phone hotspot. Most ESP32 modules are 2.4 GHz
  only.
- Motors are on a separate power branch from the logic (a step-down DC-DC
  converter feeds the ESP32 and the phone). One battery is fine if split this
  way. All branches share a common ground.

## ESP32 firmware rules

- Never use `delay()`. Run periodic work on a fixed schedule with `millis()`.
- Read serial and UDP input only as far as data has arrived; never block the
  loop.
- Stop the motors when commands stop arriving.
- Read encoders with interrupts or a hardware pulse counter.
- Put Wi-Fi passwords and other secrets in `secrets.h` (ignored by Git).
  Commit only `secrets.example.h`.

## Stages

1. ESP32-only RC car: laser-cut chassis, two motors with encoders, driven from
   the Mac.
2. ROS 2 on the Mac sends velocity commands and receives motion data.
3. iPhone controller.
4. Move the brain to an old Android phone. This is exploratory: ROS 2 on
   Android is not a well-trodden path. If it stalls, keep the Mac or switch
   to a Raspberry Pi; the body does not change.
5. Extensions: camera perception (color tracking first, NN later), SLAM, LLM
   conversation (cloud API).

## How to make decisions

- Decide details only when the current stage needs them. Do not pick
  specific hardware models, OS or ROS 2 versions, middleware, libraries or
  similar details ahead of time. If a task seems to require one, propose
  options and let the maintainer decide.
- Record undecided items in `docs/open-questions.md`. When one is decided,
  move it into `docs/architecture.md`.
- The design in `docs/architecture.md` and in this file is settled. Do not
  change it on your own. If you think it should change, propose the change
  with your reasons.
- When the design changes, update `docs/architecture.md`, this file and the
  READMEs together.

## How to report

- Focus on the overall direction and the next step, not on exhaustive detail.
- Put the conclusion and recommendation first, then the reasons.
- Keep what you verified separate from what you assume or infer.

## Commands

There is no code yet, so there are no build, flash or test commands.
Add them here stage by stage as they are introduced (for example, how to
build and flash the firmware in Stage 1, and how to build and run the ROS 2
workspace in Stage 2).

## Language

- Everything committed to this repository is in English: code, comments,
  README, this file, docs, commit messages and pull request descriptions.
- The only exception is `*.ja.md` files, which are Japanese versions of
  documents (for example `README.ja.md`).
- When you change a document that has a `*.ja.md` counterpart, update both in
  the same change, following the wording and conventions already used in
  each file.

## Git

- Never push directly to `main`. Work on a branch named
  `<type>/<short-description>`, for example `feat/…`, `fix/…`, `refactor/…`,
  `docs/…` or `chore/…`.
- Split commits into meaningful units. Write the subject as a short
  imperative sentence in English. For important changes, explain why in the
  body. Avoid boilerplate or generic wording.
- Open a pull request for review. Do not merge it yourself.
- Never commit secrets (passwords, API keys, tokens), `.DS_Store`, build
  output, caches, `.label.toml`, or conversation logs and personal notes.

## Public repository

This repository is public.

- Do not write personal information: people's names, affiliations (such as
  schools, clubs or employers), budget figures, or the specific models of
  devices the maintainer owns. Describe devices generically, for example
  "an old Android phone" or "an iPhone with LiDAR".
- Keep credentials out of the history entirely. If one is committed by
  mistake, tell the maintainer; removing it from later commits is not enough.
