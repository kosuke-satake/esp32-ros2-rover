#include <pid.h>
#include <unity.h>

using rover::Pid;
using rover::PidGains;

namespace {

constexpr float kTolerance = 1e-5f;
constexpr float kDt = 0.01f;

void test_proportional_only() {
  Pid pid(PidGains{2.0f, 0.0f, 0.0f, 0.0f}, -1.0f, 1.0f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.4f, pid.update(0.5f, 0.3f, kDt));
}

void test_feedforward_scales_the_setpoint() {
  Pid pid(PidGains{0.0f, 0.0f, 0.0f, 1.5f}, -1.0f, 1.0f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.3f, pid.update(0.2f, 0.2f, kDt));
}

void test_output_is_clamped() {
  Pid pid(PidGains{10.0f, 0.0f, 0.0f, 0.0f}, -1.0f, 1.0f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 1.0f, pid.update(1.0f, 0.0f, kDt));
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -1.0f, pid.update(-1.0f, 0.0f, kDt));
}

void test_integral_accumulates_error_over_time() {
  Pid pid(PidGains{0.0f, 2.0f, 0.0f, 0.0f}, -1.0f, 1.0f);
  pid.update(0.5f, 0.0f, 0.1f);                // 2 * 0.5 * 0.1 = 0.1
  const float out = pid.update(0.5f, 0.0f, 0.1f);  // + 0.1 = 0.2
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.2f, out);
}

void test_integral_does_not_wind_up_while_saturated() {
  Pid pid(PidGains{1.0f, 1.0f, 0.0f, 0.0f}, -1.0f, 1.0f);
  // The wheel is stalled: a large error for a long time saturates the output.
  for (int i = 0; i < 1000; ++i) {
    pid.update(2.0f, 0.0f, kDt);
  }
  // Once the error reverses, the output must follow it immediately instead of
  // staying positive until a wound-up integral drains.
  const float out = pid.update(0.0f, 0.5f, kDt);
  TEST_ASSERT_TRUE(out < 0.0f);
}

void test_derivative_ignores_setpoint_steps() {
  Pid pid(PidGains{0.0f, 0.0f, 1.0f, 0.0f}, -10.0f, 10.0f);
  pid.update(0.0f, 0.0f, kDt);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.0f, pid.update(1.0f, 0.0f, kDt));
}

void test_derivative_opposes_measurement_change() {
  Pid pid(PidGains{0.0f, 0.0f, 0.1f, 0.0f}, -10.0f, 10.0f);
  pid.update(0.0f, 0.0f, kDt);
  // Measurement rises by 0.02 in 0.01 s: -0.1 * 0.02 / 0.01 = -0.2.
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -0.2f, pid.update(0.0f, 0.02f, kDt));
}

void test_non_positive_dt_skips_integral_and_derivative() {
  Pid pid(PidGains{1.0f, 100.0f, 100.0f, 0.0f}, -10.0f, 10.0f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.5f, pid.update(0.5f, 0.0f, 0.0f));
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.5f, pid.update(0.5f, 0.0f, -1.0f));
}

void test_reset_clears_the_integral() {
  Pid pid(PidGains{0.0f, 1.0f, 0.0f, 0.0f}, -1.0f, 1.0f);
  pid.update(1.0f, 0.0f, 0.5f);
  pid.reset();
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.0f, pid.update(0.0f, 0.0f, kDt));
}

}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_proportional_only);
  RUN_TEST(test_feedforward_scales_the_setpoint);
  RUN_TEST(test_output_is_clamped);
  RUN_TEST(test_integral_accumulates_error_over_time);
  RUN_TEST(test_integral_does_not_wind_up_while_saturated);
  RUN_TEST(test_derivative_ignores_setpoint_steps);
  RUN_TEST(test_derivative_opposes_measurement_change);
  RUN_TEST(test_non_positive_dt_skips_integral_and_derivative);
  RUN_TEST(test_reset_clears_the_integral);
  return UNITY_END();
}
