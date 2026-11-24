# Robot assembly

[日本語](README.ja.md)

These are imported, unbuilt prototype artifacts, not a change to the settled architecture. Units: mm. Source: `generate.py` (Python 3.12; install `requirements.txt` in a virtual environment). It writes ignored `generated/` outputs with the original export names. Review them, then use [import-map.csv](../../import-map.csv) to update the corresponding repository files together. The map is the import-time snapshot; regenerate checks after geometry changes. Do not commit generated caches or ZIP bundles. No Onshape document was created; STEP import supports geometry editing without native feature history. See [prototype-build.md](../../prototype-build.md).

The assembly contains the earlier solid wheel envelopes and cable-tie routes. The later printed saddles and spoked wheels are separate alternatives, not silently merged into this assembly. `parameters.json` is an output inventory, not an input configuration. The preview and validation correspond to the imported chassis revision B.
