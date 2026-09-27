# CAD

Design data for every part and assembly of the robot.

[日本語](README.ja.md)

The CAD tool is not fixed yet (see
[docs/open-questions.md](../../docs/open-questions.md)). The rules below work
for any of them.

## What goes here

One folder per part or assembly, named after it:

```
cad/
  chassis-base/
    chassis-base.step     STEP export (always)
    chassis-base.sldprt   native file, when the CAD tool saves files
    chassis-base.png      preview image
    README.md             only when something needs explaining
```

- **STEP, always.** Every design has an up-to-date STEP export, so anyone
  can open it in any CAD tool.
- **Native files.** With a file-based tool such as SolidWorks (`.sldprt`,
  `.sldasm`, `.slddrw`) or Inventor (`.ipt`, `.iam`, `.idw`), commit the
  native files next to the STEP.
- **Onshape.** The design lives in Onshape, not in files. Put a link to the
  Onshape document in the folder's `README.md`, and commit the STEP export as
  with any other tool.
- **Preview.** A PNG screenshot, so people can see the part on GitHub without
  opening CAD.

## Updating a design

1. Change the design in the CAD tool.
2. Export the STEP again, and update the native file and the preview.
3. Commit them together, so they always match.
4. If the change affects a cutting file, update [`../laser-cut/`](../laser-cut)
   in the same pull request.

## Notes

- Units are millimetres.
- Assemblies find their parts by file name. Do not rename part files without
  updating the assemblies that use them.
- Lock and backup files that CAD tools create (`~$*`, `*.lck`,
  `OldVersions/`) are ignored by Git.
