#include "pid.h"

namespace rover {
namespace {

float clamp(float value, float low, float high) {
  if (value < low) {
    return low;
  }
  if (value > high) {
    return high;
  }
  return value;
}

}  // namespace

Pid::Pid(const PidGains& gains, float output_min, float output_max)
    : gains_(gains),
      output_min_(output_min),
      output_max_(output_max),
      integral_term_(0.0f),
      previous_measurement_(0.0f),
      has_previous_measurement_(false) {}

float Pid::update(float setpoint, float measurement, float dt_s) {
  const float error = setpoint - measurement;
  const float feedforward = gains_.kff * setpoint;
  const float proportional = gains_.kp * error;

  // Differentiate the measurement rather than the error, so a step in the
  // setpoint does not cause a spike in the output.
  float derivative = 0.0f;
  if (dt_s > 0.0f) {
    if (has_previous_measurement_) {
      derivative = -gains_.kd * (measurement - previous_measurement_) / dt_s;
    }
    previous_measurement_ = measurement;
    has_previous_measurement_ = true;

    // Anti-windup: keep the new integral only if it does not push an
    // already saturated output further past its limit.
    const float increment = gains_.ki * error * dt_s;
    const float candidate = integral_term_ + increment;
    const float unclamped = feedforward + proportional + candidate + derivative;
    const bool winds_up_high = unclamped > output_max_ && increment > 0.0f;
    const bool winds_up_low = unclamped < output_min_ && increment < 0.0f;
    if (!winds_up_high && !winds_up_low) {
      integral_term_ = clamp(candidate, output_min_, output_max_);
    }
  }

  return clamp(feedforward + proportional + integral_term_ + derivative,
               output_min_, output_max_);
}

void Pid::reset() {
  integral_term_ = 0.0f;
  previous_measurement_ = 0.0f;
  has_previous_measurement_ = false;
}

void Pid::setGains(const PidGains& gains) { gains_ = gains; }

}  // namespace rover
