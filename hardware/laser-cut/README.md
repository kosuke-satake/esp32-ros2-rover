# Laser cutting

Cutting files for parts made on a laser cutter.

[日本語](README.ja.md)

## What goes here

Files exported from the designs in [`../cad/`](../cad), named

```
<part>-<material>-<thickness>mm.dxf
```

for example `chassis-base-mdf-3mm.dxf`. SVG is fine when the laser software
prefers it.

## Before committing a file

- Exported at 1:1 scale, in millimetres.
- Only the lines to cut or engrave are left. Dimensions, notes and
  construction lines are removed.
- Outlines are closed, with no duplicate or overlapping lines; the laser
  would cut them twice.
- The part name matches a design in `../cad/`.

## Tips

- The laser burns away a thin strip of material (the kerf, typically around
  0.1 to 0.3 mm). Slots and tabs meant to press-fit need to allow
  for it; say in the pull request whether the design compensates.
- When you find settings that cut a material cleanly, add them to this file
  as a table: material, thickness, machine, power, speed and passes.
