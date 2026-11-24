> Prototype reference only. For the spoked-wheel option, place the rear saddle at X=-56, not the earlier X=-55. See hardware/prototype-build.md. STEP and PNG files mentioned below live in ../../cad/tt-motor-saddle/.

# TT motor saddle — nominal printable prototype

Print four identical saddles: two per motor. This mount uses M3 brass heat-set inserts; it is not intended for M3 screws driven directly into plastic. The 10 mm wide rear saddle sits behind the axle at X=-55; the other sits ahead at X=-10. Both use the revision-B chassis slots, Y=39/65 or -39/-65. No new plywood holes are nominally required.

## Before printing

Measure your TT motor. This design assumes 18 mm body width, 20 mm height, rear body edge X=-60.5, and axle at X=-45 (relative to the existing assembly). The saddle has 18.6 mm internal width and sits against the underside of the chassis. Confirm neither saddle covers a terminal, end cap, vent, gearbox rib or shaft on your actual motor. Nominal clearance alone cannot guarantee fit with a clone motor.

Print one saddle first and try it on the actual motor. Do not print all four until this fits. STL units are millimetres; the outside size is 10 x34 x26.8. The supplied STEP is editable; motor-saddle-clearance.step shows the two saddles relative to one nominal motor and shaft, not a complete robot assembly.

## Printing

- Prefer PETG; PLA is acceptable for a cool indoor fit trial but can creep under clamp load or motor heat.
- Start with a 0.4 mm nozzle, 0.2 mm layers, 4 perimeter walls, 5 top/bottom layers and about 40% infill. Review the slicer preview with your lab technician.
- Keep the supplied orientation: broad saddle bottom on the bed, U-shaped cavity opening upward. The outer flange undersides have 45-degree supports. No generated support should normally be needed; use a brim if adhesion is poor.
- Print four copies of tt-motor-saddle.stl after the first fit succeeds. Print one insert coupon first if your printer/insert combination is untested.

## Inserts and hardware

You need 8 M3 heat-set inserts, 8 M3 screws and broad washers against the plywood. The nominal insert pocket is 4.0 diameter x5.3 deep, intended as a pilot for inserts approximately 4.5–4.6 outside diameter and 5 long. **Use the insert manufacturer's recommended pilot diameter and measure actual inserts.** Shorter or larger inserts require a source change. Heat-setting should be done by someone familiar with the equipment.

The coupon has blind pockets of 3.8,4.0,4.2 diameter from left to right (X=-12,0,+12), each 5.3 deep. Test the actual insert on it, including screw engagement and resistance to pulling out. This is an insert-fit coupon for a 3D printer; it is separate from the plywood laser kerf coupon.

Install inserts from the flange top, flush, with saddles off the motors. Let cool before loading. Through 3 mm plywood plus about 1 mm washer, M3x8 gives about 4 mm engagement; verify the real stack. Never bottom screws in blind pockets or let screw tips reach a motor. Tighten gently by hand; printed parts and plywood are easy to damage.

## Installation

1. Place two saddles around each motor from below, at the reference positions. The flange tops contact the lower plywood deck underside.
2. Include rigid packing above each motor. The saddle floor-to-chassis space is 23.8. For a 20 mm motor, use approximately 3.5 mm rigid packing plus 0.3 mm compliant anti-slip material. Actual dimensions determine the final pad thickness. Do not force an oversized motor into the cradle.
3. Insert M3 screws and washers from above the plywood through the existing slots and into the inserts. Centre them over the pockets and tighten evenly. Pads provide light clamping; no cable ties are required for normal retention once fit is verified.
4. Keep the rear saddle behind the shaft. Check both ends of the axle, wheel hub and rotating wheel through full revolutions. Trim no structural walls to make a bad fit work; revise the CAD instead.
5. Check for motor sliding/twisting by hand. Add a fitted stop block if necessary; this generic cradle does not key into every TT gearbox shape. Test with wheels lifted, then under gentle load. Recheck screws and clamp security after use.

The motor remains nominally at axle Z=32.5 to match 65 mm wheels and the existing caster setup. Check the actual robot on a flat surface. This is an untested printable prototype; the validation file checks nominal solid validity, motor overlap and shaft clearance, not strength, print quality or insert retention.
