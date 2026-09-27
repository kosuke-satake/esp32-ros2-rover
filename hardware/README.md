# Hardware

Mechanical design and parts of the robot.

[日本語](README.ja.md)

## Layout

| Path | Contents |
| --- | --- |
| [`cad/`](cad) | CAD data and STEP exports for each part and assembly |
| [`laser-cut/`](laser-cut) | Cutting files for the laser cutter |
| `parts.md` | Parts list: name, model, quantity, where to buy and price. Added once the parts are chosen. |

Folders for wiring diagrams and 3D-printed parts are added when that work
starts.

## Rules

- Units are millimetres.
- Name files and folders after the part, in lower-case words joined by
  hyphens, for example `chassis-base`. Use the same name in `cad/` and
  `laser-cut/`, so a cutting file can be traced back to its design.
- Do not put versions in file names (`-v2`, `-final`). Overwrite the file and
  commit; Git keeps every earlier version.
- Keep each file under 50 MB. GitHub warns above 50 MB and rejects files over
  100 MB. Ask before adding anything larger.
- The firmware uses the wheel radius and the distance between the two wheels.
  When either changes, tell the software team.
- Keep the safety parts from [docs/architecture.md](../docs/architecture.md)
  in the design: an emergency-stop switch that is easy to reach and cuts motor
  power, and separate power branches for the motors and the electronics.

## Workflow

1. Create a branch, for example `feat/chassis-base`.
2. Add or update files as described in the guide of each folder.
3. Open a pull request with a screenshot or photo of the change.
4. After review, the pull request is merged into `main`.

See [CONTRIBUTING.md](../CONTRIBUTING.md) for the full workflow.
