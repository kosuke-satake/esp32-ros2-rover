#include "diff_drive.h"

#include <math.h>

namespace rover {
namespace {

constexpr float kTwoPi = 6.28318530718f;

}  // namespace

WheelSpeeds wheelSpeedsFromTwist(float linear_mps, float angular_radps,
                                 float track_width_m) {
  const float half_track = 0.5f * track_width_m;
  return WheelSpeeds{linear_mps - angular_radps * half_track,
                     linear_mps + angular_radps * half_track};
}

WheelSpeeds limitWheelSpeeds(WheelSpeeds speeds, float max_wheel_mps) {
  if (max_wheel_mps <= 0.0f) {
    return WheelSpeeds{0.0f, 0.0f};
  }
  const float peak = fmaxf(fabsf(speeds.left), fabsf(speeds.right));
  if (peak <= max_wheel_mps) {
    return speeds;
  }
  const float scale = max_wheel_mps / peak;
  return WheelSpeeds{speeds.left * scale, speeds.right * scale};
}

float metersPerTick(float wheel_radius_m, float ticks_per_wheel_rev) {
  if (ticks_per_wheel_rev <= 0.0f) {
    return 0.0f;
  }
  return kTwoPi * wheel_radius_m / ticks_per_wheel_rev;
}

}  // namespace rover
