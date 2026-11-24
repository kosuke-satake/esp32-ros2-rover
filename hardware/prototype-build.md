# Mechanical prototype: build and review notes

This is a technical summary of the earlier mechanical work, imported for team review. It is **not a change to the settled architecture or an approved manufacturing release**. The CAD, DXF, STL and previews use nominal dimensions; no physical build, print-fit test or electrical load test is recorded.

## Start here

| Need | Location |
| --- | --- |
| Chassis assembly and preview | [cad/robot-assembly/](cad/robot-assembly/) |
| Individual editable STEP parts | [cad/](cad/) |
| Laser files | [laser-cut/](laser-cut/) |
| Printed motor saddles | [3d-print/tt-motor-saddle/](3d-print/tt-motor-saddle/) |
| Printed spoked wheels and axle tests | [3d-print/spoked-wheel/](3d-print/spoked-wheel/) |
| Candidate bill of materials | [prototype-parts.csv](prototype-parts.csv) |
| File provenance and checksums | [import-map.csv](import-map.csv) |

Filenames are normalized to the repository's lower-case, hyphenated convention. The import map records original filenames, destinations and SHA-256 hashes. Binary manufacturing files are copied unchanged. Text/source imports remove local filesystem dependencies, adjust names and link to this repository. ZIP downloads, installed packages, caches, scratch packaging scripts and conversation logs are intentionally excluded; the unique deliverables they contained are unpacked here.

The source generators use CadQuery, with STEP exports for Onshape or other CAD tools. No online Onshape document was created. STEP does not preserve a native feature tree or functional assembly mates; DXFs can be inserted into sketches and extruded. Source scripts and their dependency lists sit beside their design exports. Regeneration writes ignored `generated/` folders, preserving the original export names; review and map the results to the tracked names before replacing exports.

## What the artifacts represent

- The chassis assembly is the earlier revision B: corrected cable-tie routes and solid wheel clearance envelopes. It includes simplified purchased-part shapes and hardware; most wiring and screws are absent.
- The printed saddle is a separate mounting alternative, with its own one-motor clearance reference.
- The spoked wheel is another separate alternative. Its clearance reference includes the lower deck and a conservative upper-deck envelope, not a complete robot.
- The main assembly has **not** been updated to combine the printed saddles and spoked wheels. The wheel's source check moves the rear saddle from X=-55 to X=-56; the older saddle reference remains at X=-55.
- Validation JSON files are preserved import-time results. They demonstrate the checks listed in those files, not a fresh physical validation or complete tolerance analysis. Filenames inside those snapshots use original export names; use the import map to trace them.

## Nominal chassis and coordinates

All dimensions are mm. X points forward, Y left and Z upward; ground is Z=0.

| Feature | Nominal geometry |
| --- | --- |
| Two plywood decks | 180 x140 x3, clipped corners |
| Lower deck | underside Z=46, top Z=49 |
| Upper deck | underside Z=79, top Z=82 |
| Deck supports | four M3 standoffs, 30 long, at X=±75 and Y=±57 |
| Motor bodies in original assembly | 65 x18 x20; centres X=-28, Y=±52; bottom Z=22.5 |
| Axles | X=-45, Z=32.5 |
| Original wheel envelopes | diameter 65, width 26; centres Y=±90 |
| Wheel spacing for this prototype | centre-to-centre 180; outside width 206 |

These are design assumptions. The supplied TT motor description matches a product whose published body dimensions are 70 x22 x18, so do not assume the original motor envelope or motor packing fits the actual product. The specific exact development-board variant, motor shape, output shaft and caster have not been measured.

The software team must verify the loaded rolling radius and wheel centre spacing before using these values in firmware. These imports do not set firmware constants.

## Laser-cut parts and material

Cut one each of chassis-base, chassis-top, esp-carrier, servo-horn-plate and sensor-face. Caster-spacer is an optional 3 mm shim; kerf-coupon is a test piece, not an installed robot part. The combined DXF includes one of every profile in a 300 x320 usable layout. Check clamp margins and bed size.

The profiles assume 3 mm plywood. Lab-approved 3 mm MDF is an alternative candidate, but its actual thickness and laser compatibility must be checked. It is more vulnerable around narrow slots/tabs and to crushed screw seats; use broad washers and gentle tightening. Material choice is not finalized by this import.

**Kerf** is the width removed by the laser. All supplied paths are nominal boundaries, with no compensation. Cut the coupon first using the same sheet and machine settings. Its four slots, left to right, are 2.9, 3.0, 3.1 and 3.2 wide, plus a 3.4 round hole. Test a measured piece of the sheet and an M3 screw.

Apply compensation only once in CAM: external boundaries outward by half the measured kerf, internal holes/slots inward by half the kerf. Without compensation, tabs shrink and openings grow. Set finished tab sockets to measured thickness plus approximately 0.1–0.2 clearance and tab width plus approximately 0.2. Update tab depth and solid thickness if sheet thickness changes. Do not assume a generic kerf value is correct.

Check 1:1 mm scale, closed outlines and cutter preview. A deck must read 180 x140. Cut internal details before external contours and follow the lab's material, ventilation and supervision requirements. Sand char from glue faces. No cutter power/speed settings have been established.

## What the holes and slots do

- Corner round holes: deck screws and standoffs.
- Narrow slots: adjustable mounting or ties for motors, battery and electronics; unused slots are intentional flexibility, not extra components.
- Rounded central openings: wiring between decks; protect wire insulation at edges.
- Front rectangular opening on top: servo body.
- Rear small rectangular pilot: power switch, requiring adjustment to the actual switch. This is **not** a designed emergency-stop mount or proof of a suitable e-stop.
- Two large sensor-face circles: ultrasonic transducers.

## Motor attachment options

**Cable ties:** two 2.5 mm ties per motor, around stationary body surfaces through opposing slots at Y=39/65 or -39/-65. Rear tie X=-55 uses 6 x3.5 slots to avoid the axle at X=-45; forward tie X=-10 uses 12 x3.5 slots. Route the heads and tails clear of both shaft ends, hubs, terminals and wheels. Use approximately 3.5 total packing above the nominal 20 mm body, including any thin rubber, to retain the intended axle height. Measure before using this value. Tighten evenly and add a fitted gearbox stop if the motor can slide or twist.

**Printed saddles:** four identical saddles, two per motor, use the same slot pairs. They require eight M3 heat-set inserts with verified pilot fit, screws and washers. Print one saddle and the insert-fit coupon before all four. The nominal cradle width is 18.6 and motor height assumption is 20. Detailed insert, printing and screw-length instructions are in the saddle folder. With the spoked wheels, move each rear saddle **1 mm rearward to X=-56** within its slot; the forward saddle stays at X=-10. Imported wheel checks show 1.5 mm nominal rear saddle clearance at that position. Confirm the saddle still bears on the real body.

**Commercial brackets:** an alternative only after their motor fit, height and hole pattern are measured. No commercial bracket pattern is present in these files. Motor mounting selection remains open for the encoder-equipped Stage 1 motors.

## Spoked wheels

Print two 65 mm wheels, with six curved through-openings, recessed spoke web and tread grooves. The 26 mm rim has a 7 mm nominal side gap to the deck. An inward-projecting, tapered hub reaches toward the motor shaft while clearing the deck. Imported checks report 4.61 mm minimum wheel-to-lower-deck distance in the nominal model and no saddle intersection after the rear mounting adjustment.

The assumed shaft is double-flat, 5.4 major diameter and 3.6 across flats. Actual printed fit must be tested: the supplied full wheel uses a 5.55 x3.75 nominal bore, 12 deep. Tight/nominal/loose coupon sizes are 5.40/3.60, 5.55/3.75 and 5.70/3.90 respectively. If a different coupon fits best, change the source allowance and regenerate the full wheel; do not scale the complete STL.

Retention is a snug keyed push-fit, with no wheel inserts or set screws. Do not use a loose wheel, force it onto the plastic shaft or hammer it. Nominal engagement is 8, based on assumed shaft projection; measure actual engagement and gearbox-boss clearance. A rigid printed tread may have poor grip on smooth floors. A tyre changes rolling diameter and requires rechecking caster level and clearance.

Print the flat outer face down, hub upward. Starting suggestions are 0.4 nozzle, 0.2 layers, four walls, five top/bottom layers and about 40% infill, with PETG as a candidate. These are not verified machine settings. Earlier time estimates were rough planning estimates; use the actual slicer's estimate rather than treating them as specifications.

## Other component mounts

| Component | Proposed attachment and measurement needed |
| --- | --- |
| Battery holder | lower deck at (-44,0), assumed 60 x58 x19; two straps through X=-60/-25, Y=±32. Nominal overhead space 11; leave cell access and lead strain relief. Screws must not contact cells. |
| ESP32-S3 | 72 x52 plywood carrier at X=-50, bottom Z=88; two 6 mm M3 spacers and screws near (-70,±20). Board assumed 64 x28, supported by insulating 5 mm pads/rails. Use nylon standoffs if actual PCB holes exist, otherwise insulated clips/ties. Keep USB and antenna clear. |
| Motor driver | nominal 22 x20 x8 at (-12,-40); at least 4 mm insulating supports and secure clips/ties or actual board-hole standoffs. |
| Regulator | nominal 35 x22 x12 at (25,33); insulated supports, connector access and ventilation. |
| Switch | pilot opening 8.4 x15.4 at (-76,0); match actual nut/clip retention and body size. |
| Wiring | anchor with ties, sleeve wood edges, keep motor wiring away from sensor wiring and moving shafts; slack loop for servo. |

## Servo-to-sensor mount and spacers

The SG90 body passes through a nominal 24 x13 opening at (60,0). Its mounting ears rest on the top deck; small M2 or supplied servo screws fit the actual ears. The model's blue ear blocks are simplified and are not separate parts to print or purchase. Their shape and placement have not been verified against a servo drawing. Measure the real body, ears, shaft offset and output height before cutting the final opening/slots.

The supplied plastic horn fits the spline and uses its own retaining screw. Two **10 mm M2 spacers**, nominally at ±8 from the shaft, connect the horn to the horizontal plywood plate. The plate bottom is Z=104 in the nominal layout. Verify horn-hole spacing, horn elevation, screw lengths and retaining-screw access. A central tube alone is not a torque coupling.

The sensor face is 50 x30, with two 8 x3 tabs entering 8.2 x3.1 rectangular sockets. Dry-fit square, then glue. The 16.4 holes assume 16 diameter transducers on 26 pitch. Small ties through two 5 x2.5 bottom slots and insulating pads retain the PCB behind the face; leave the header accessible and do not stress solder joints. Start with ±45 degrees pan, testing any extension toward ±60 only after checking stops, fasteners and cable slack.

## Caster and chassis spacers

- Between decks: four **M3 female-to-female standoffs, 30 mm long**, with appropriate screws and broad washers.
- ESP carrier: two **6 mm M3 spacers**.
- Servo horn to sensor plate: two **10 mm M2 spacers**, subject to actual horn geometry.
- Caster: four **21 mm M3 risers** only if the caster measures 25 from floor to mounting-plate top. Required riser =46 minus measured caster height. Optional 3 mm plywood shims replace part of that length; do not add them above the full riser stack. Nominal mounting pitch 24 x24; slots adjust longitudinal spacing, not arbitrary lateral spacing.

Set the caster on a flat surface so both drive wheels and the ball contact the floor with level decks. Screw lengths depend on the real stack; avoid bottoming or exposed tips near electronics and batteries.

## Electrical discussion: candidate only

The earlier candidate used four AA cells and separate motor and logic branches with common ground. Its 5 V logic/servo supply used a **buck-boost**, since a four-cell pack may fall below 5 V. This is not automatically compatible with the settled step-down/phone-power plan: pack, converter and loads require a maintainer decision. A nominal target of 2 A output is not a measured load budget. At 4 V input, 10 W output at 85% efficiency alone draws about 2.94 A, before motors. Verify cell, holder, switch, connector, wire and thermal ratings; choose the battery fuse from measured currents and wire limits.

TB6612FNG was an earlier candidate, not the final driver. Verify actual motor startup/stall current against continuous, pulse and thermal ratings; do not rely on duty limiting as current protection. The specified TT motor family can exceed the driver's continuous rating when stalled. Stock non-encoder TT motors do not meet Stage 1 odometry requirements without a suitable encoder solution.

ESP GPIO is 3.3 V logic. Do not connect 5 V ultrasonic Echo directly. The earlier divider candidate was Echo ->10 kOhm ->GPIO ->15 kOhm ->ground (1%); verify worst-case voltage/input limits on the actual board. Check Trigger and servo input-high thresholds; use an appropriate one-way buffer if needed, without feeding 5 V back into ESP pins. If TB6612 is selected, 3.3 V logic VCC and a standby pull-down are candidate arrangements, with all motor inputs verified stopped during boot/reset.

Candidate suppression: 100 nF across motor terminals, short twisted motor leads, local bypass capacitors and a starting 470–1000 uF near the servo rail. Follow the selected regulator's stability/capacitor requirements. Check voltage, polarity, brownouts and USB backfeed before connecting the board. No exact GPIO mapping, schematic, fuse rating or regulator model has been approved.

The current project additionally requires a physical motor-power emergency stop, hardware watchdog, stopped-by-default driver inputs, battery monitoring and the existing command timeout. The generic power-switch hole and old assembly do not demonstrate those safety layers. Preserve the protocol, cumulative odometry and velocity-only brain/body contract. Obstacle detection alone is not a safety substitute and cannot reliably detect all surfaces or drop-offs.

## Review before fabrication

1. Select encoder-capable motors and measure them, their shafts, the exact board, battery holder, caster and servo.
2. Resolve the battery/converter and driver current budget, plus hardware safety integration.
3. Choose a motor mount; verify the wheel neck, shaft engagement, bracket spacing and caster level together.
4. Print small fit tests and cut the kerf coupon before full parts.
5. Dry-assemble, verify wheel and servo travel by hand, then test electrical rails and driver default state with wheels lifted.
6. Record actual verified dimensions and tests before adopting any candidate in the architecture or firmware configuration.

General lab stock may cover basic hardware, resistors, wire and tools; availability of the exact motors, encoder solution, converter, caster, inserts and standoffs must be checked. Stock availability is not established by these artifacts.

## Reference information retained

- [Supplied TT motor description and manufacturer information](https://www.adafruit.com/product/3777)
- [TB6612 hookup information](https://learn.sparkfun.com/tutorials/tb6612fng-hookup-guide/all?print=1)
- [ESP32-S3 datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [SG90 manufacturer](https://towerpro.com.tw/product/sg90-7/)
- [Ultrasonic sensor logic-level information](https://cdn-learn.adafruit.com/downloads/pdf/ultrasonic-sonar-distance-sensors.pdf)
- [Onshape file formats](https://cad.onshape.com/help/Content/File/supported_file_formats.htm)

References were used during the earlier prototype work; this import does not reverify live product specifications or stock.
