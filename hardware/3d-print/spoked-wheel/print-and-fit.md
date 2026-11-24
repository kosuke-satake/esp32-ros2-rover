> Prototype reference only. The Stage 1 encoder-motor selection and safety integration are still unresolved. STEP, source and PNG files mentioned below live in ../../cad/spoked-wheel/.

# 65 mm six-spoke TT wheel — nominal printable prototype

Two identical wheels, with six open curved windows, a recessed spoke web, three circumferential tread grooves and an inward-projecting hub. This is actual printable CAD, not an illustration or a solid cylinder. The diameter stays 65 mm to preserve the earlier nominal axle and caster height.

## Files

- `spoked-wheel.stl`: print TWO after the fit test succeeds.
- Matching `.step`: editable solid for Onshape import; imported STEP has no native feature history.
- `axle-fit-tight.stl`, `axle-fit-nominal.stl`, `axle-fit-loose.stl`: small hub tests; print these before spending time on full wheels.
- `wheel-clearance.step`: both wheels, the lower deck and the upper deck envelope at their assembled coordinates, plus nominal shafts. This is a clearance reference, not a complete rebuilt robot.
- `spoked-wheel.png`, `wheel-clearance.png`: renders of actual geometry.
- `validation.json`: nominal collision, STEP and STL checks.
- `generate.py`: editable CadQuery source and dimensions. Requires Python 3.12 and dependencies in requirements.txt. To change the shaft fit, edit SHAFT_D, SHAFT_FLATS and FIT_ALLOWANCE and regenerate. If the source is run without the earlier deck file present, it uses a conservative rectangular 180 x140 deck envelope for checking.

## Chassis clearance and hub reach

Existing chassis edges are Y=±70. Rim inner faces are Y=±77 and outer faces ±103, leaving **7 mm nominal lateral clearance** between rim and chassis. Both axle centres stay at X=-45, Z=32.5. Overall track outside-to-outside remains 206. Rim width is 26.

The hub projects 14 mm inward beyond the rim face: its inner mouth is Y=±63, so the motor shaft can enter while the rim stays outside the chassis. A 9 mm diameter inboard neck and tapered transition provide space for the earlier motor mounts. Maximum hub diameter is 20, so even its conservative cylindrical envelope has 3.5 mm vertical clearance below the lower deck. See validation.json for the actual minimum distances. Do not add a spacer to the shaft without rechecking engagement and clearance.

Nominal motor side face is at Y=±61 and shaft extends to ±71; this gives 2 mm gap between motor body and hub mouth, 8 mm shaft engagement and 4 mm bore-bottom clearance. **These shaft/body positions are assumptions, not measurements of your actual motor.** Check the gearbox boss as well as the shaft itself. If the real shaft cannot provide adequate engagement at this rim position, change the hub projection/shaft location; do not simply slide the rim inward until it rubs the chassis. Also check that the actual motor mounting and packing place the axle at the intended height.

The supplied motor name matches Adafruit product 3777, whose body specification is 70 x22 x18 rather than the original generic 65 x18 x20 envelope. The wheel check uses the original chassis axle coordinates; it does not certify the earlier generic motor packing or bracket against that exact motor. Verify the real axle-to-top distance and mount before cutting or installing.

**If using the earlier printed saddles:** move each rear saddle 1 mm rearward, from X=-55 to X=-56, within the existing 6 mm chassis slots. Keep the forward saddle at X=-10. This increases the nominal neck-to-rear-saddle clearance from 0.5 to 1.5 mm without moving the motor or wheel. Check that the saddle still bears on the actual motor case, tighten at that position and verify a full rotation. The updated validation uses this rear saddle position. Cable-tie rear routes remain at X=-55 and do not need this shift.

## Shaft fit

Nominal shaft assumption: double-flat profile, 5.4 major diameter and 3.6 across flats. These are editable nominal values that must be checked on the actual shaft. The default wheel bore is 5.55 major x3.75 across flats, blind depth 12. Coupon bores:

| Coupon | Major diameter | Across flats |
|---|---:|---:|
| tight | 5.40 | 3.60 |
| nominal | 5.55 | 3.75 |
| loose | 5.70 | 3.90 |

Printing errors can change hole fit in either direction. The chosen coupon must fit snugly with light hand pressure, resist pulling off and have negligible rotational play. Do not hammer or force a tight print onto the plastic shaft. The coupons have through bores so a shaft can be pushed back out. If a different coupon fits, update the source allowance and regenerate the wheel; do not scale the entire STL, which changes diameter and chassis clearance. The nominal clearance setting is a test starting point, not a promise of a press fit.

## Retention

This version uses a snug keyed push-fit, with no set screw or inserts. A narrow inboard neck keeps rotating material away from the existing motor mounts. The double-flat profile transmits torque; fit friction provides axial retention. Do not use a loose bore in service. Select and verify the fit on a coupon, then regenerate the wheel if needed. If a secure removable push-fit cannot be obtained, provide actual shaft measurements before adding a different retention mechanism; do not drill into the motor axle or glue a wheel permanently as a workaround.

## Printing and use

Print with the supplied flat OUTER face on the bed: local Z=0 down, long hub pointing up. The spoke web is at Z=0…6, rim at 0…26, and hub at 0…40. Six openings go fully through the spoke web and open into the recessed wheel cavity. This orientation avoids large unsupported spoke bridges. The blind vertical axle bore opens at the top. The neck walls are about 1.7 mm minimum at the major-diameter sides; inspect the slicer to ensure continuous walls there.

Starting settings: 0.4 nozzle, 0.2 layers, at least 4 walls, 5 top/bottom layers and about 40% infill. Prefer PETG for a functional prototype; use the lab printer's approved settings. A rigid printed tread has limited traction compared with rubber, particularly on smooth floors. Adding a tyre changes the rolling diameter and may require caster-height adjustment; retain the chassis side clearance and keep tyre edges from bulging inward.

Print a hub test before a full wheel. Check both wheels by hand with power off, through multiple revolutions; inspect rim, hub, motor brackets, tie routes and motor-boss clearance. Leave at least 3 mm rim side clearance in the actual build. Check for wobble and lateral shaft play. Start powered tests with wheels lifted, then at low speed on the floor. Recheck push-fit retention after the first run. Where the earlier printed saddle STEP is available, the validation script also checks its nominal positions; small mount clearances still require a real fit test.

Geometry checks do not establish strength, concentricity, balance, print quality or exact clone-motor compatibility. No physical wheel or fit test has been performed.

Reference for the motor identification and body specification: https://www.adafruit.com/product/3777 . Manufacturer drawing: https://cdn-shop.adafruit.com/product-files/3777/3777_diagram.jpg . Shaft fit remains explicitly nominal until measured.
