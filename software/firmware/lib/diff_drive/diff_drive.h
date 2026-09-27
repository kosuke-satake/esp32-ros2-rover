// Differential-drive kinematics for the body.
//
// The brain sends only a linear and an angular velocity (see
// docs/architecture.md). This module turns that command into left and right
// wheel speeds, which the per-wheel PID loops then track.
#pragma once

namespace rover {

struct WheelSpeeds {
  float left;   // Wheel surface speed in m/s, positive is forward.
  float right;  // Wheel surface speed in m/s, positive is forward.
};

// Wheel speeds for a body velocity command. Positive angular velocity turns
// counter-clockwise (to the left), as in ROS.
WheelSpeeds wheelSpeedsFromTwist(float linear_mps, float angular_radps,
                                 float track_width_m);

// Scales both wheel speeds by the same factor so that neither exceeds
// max_wheel_mps. Keeping the ratio keeps the turning radius, so the robot
// follows the commanded path more slowly instead of drifting off it.
WheelSpeeds limitWheelSpeeds(WheelSpeeds speeds, float max_wheel_mps);

// Distance travelled by the wheel surface per encoder tick.
float metersPerTick(float wheel_radius_m, float ticks_per_wheel_rev);

}  // namespace rover
