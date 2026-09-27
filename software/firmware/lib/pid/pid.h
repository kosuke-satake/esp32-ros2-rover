// PID controller used to make each wheel track its target speed.
#pragma once

namespace rover {

struct PidGains {
  float kp;
  float ki;
  float kd;
  float kff;  // Feedforward: output per unit of setpoint.
};

class Pid {
 public:
  Pid(const PidGains& gains, float output_min, float output_max);

  // Returns the output for one control step of dt_s seconds, clamped to
  // [output_min, output_max]. A non-positive dt_s skips the integral and
  // derivative terms for that step.
  float update(float setpoint, float measurement, float dt_s);

  // Clears the integral and derivative history. Call it whenever the loop is
  // interrupted, for example after the command timeout stopped the motors.
  void reset();

  void setGains(const PidGains& gains);

 private:
  PidGains gains_;
  float output_min_;
  float output_max_;
  float integral_term_;
  float previous_measurement_;
  bool has_previous_measurement_;
};

}  // namespace rover
