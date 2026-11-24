# Spoked wheel

[日本語](README.ja.md)

These are imported, unbuilt prototype artifacts, not a change to the settled architecture. Units: mm. Source: `generate.py` (Python 3.12; install `requirements.txt` in a virtual environment). It writes ignored `generated/` outputs with the original export names. Review them, then use [import-map.csv](../../import-map.csv) to update the corresponding repository files together. The map is the import-time snapshot; regenerate checks after geometry changes. Do not commit generated caches or ZIP bundles. No Onshape document was created; STEP import supports geometry editing without native feature history. See [prototype-build.md](../../prototype-build.md).

Print two 65 mm wheels only after the axle-fit test. [STL and fitting instructions](../../3d-print/spoked-wheel/). The clearance assembly uses the actual lower deck and a conservative rectangular upper-deck envelope, not the complete robot. The nominal wheel centre spacing is 180 mm (outside width 206 mm). Verify loaded rolling radius and track before configuring firmware.
