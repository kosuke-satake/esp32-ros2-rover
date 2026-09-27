# Open questions

Decisions that are deliberately left open. Each one is settled when the stage
that needs it begins, not earlier. Once decided, move the result into
[architecture.md](architecture.md) and remove it from this list.

## Phone for the brain

Which old Android phone to use as the brain.
If it needs to be rooted, note that carrier-locked models often cannot unlock
the bootloader.

Needed by: Stage 4.

## Running ROS 2 on the phone

How to run ROS 2 on Android. Candidates include rooting plus a chroot, or
Termux plus proot.

Needed by: Stage 4.

## ROS 2 distribution and middleware

- Which ROS 2 distribution and which middleware (RMW) to use.
- How to pass ROS 2 traffic between the Mac's Docker environment and other
  machines. Docker on macOS runs containers inside a VM, so it does not talk
  to other machines on the network easily out of the box.

Needed by: Stage 2 for the distribution; the cross-machine link once a second
machine joins the ROS 2 network.

## Link to the ESP32: custom protocol or micro-ROS

Whether to keep the self-made text protocol or switch to micro-ROS.
If micro-ROS is considered, check its compatibility with the chosen
middleware.

Needed by: Stage 2.

## SLAM sensor

- **iPhone LiDAR**: stream point clouds and pose to ROS 2 with Record3D and
  build the map with RTAB-Map. The iPhone only sees forward, which fits poorly
  with 2D SLAM methods that assume a 360° scan. Once mounted on the robot, it
  can no longer serve as the controller.
- **Low-cost 2D LiDAR** (around USD 50).

Needed by: Stage 5.

## Phone camera into ROS

How to bring the phone's camera stream into ROS 2.

Needed by: Stage 4 or 5, whichever first uses the camera.

## Parts

Specific parts such as the motors, the motor driver and the battery.

Needed by: Stage 1.

## CAD tool

Which CAD tool the hardware team uses. Likely Onshape or SolidWorks,
possibly Inventor. The rules in `hardware/cad/` work for any of them.

Needed by: Stage 1, before the chassis is designed.
