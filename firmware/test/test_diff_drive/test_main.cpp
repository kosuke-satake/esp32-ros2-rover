#include <diff_drive.h>
#include <unity.h>

using rover::WheelSpeeds;

namespace {

constexpr float kTolerance = 1e-6f;
constexpr float kTrackWidth = 0.20f;

void test_straight_line_drives_both_wheels_equally() {
  const WheelSpeeds s = rover::wheelSpeedsFromTwist(0.2f, 0.0f, kTrackWidth);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.2f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.2f, s.right);
}

void test_positive_angular_turns_left_in_place() {
  const WheelSpeeds s = rover::wheelSpeedsFromTwist(0.0f, 1.0f, kTrackWidth);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -0.1f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.1f, s.right);
}

void test_arc_combines_linear_and_angular() {
  const WheelSpeeds s = rover::wheelSpeedsFromTwist(0.2f, 0.5f, kTrackWidth);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.15f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.25f, s.right);
}

void test_limit_keeps_speeds_within_range_unchanged() {
  const WheelSpeeds s = rover::limitWheelSpeeds({0.1f, -0.2f}, 0.3f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.1f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -0.2f, s.right);
}

void test_limit_scales_both_wheels_to_keep_the_turn_radius() {
  const WheelSpeeds s = rover::limitWheelSpeeds({0.3f, 0.6f}, 0.3f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.15f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.3f, s.right);
}

void test_limit_uses_the_magnitude_of_reverse_speeds() {
  const WheelSpeeds s = rover::limitWheelSpeeds({-0.6f, 0.2f}, 0.3f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -0.3f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.1f, s.right);
}

void test_limit_with_non_positive_max_stops_the_wheels() {
  const WheelSpeeds s = rover::limitWheelSpeeds({0.3f, 0.3f}, 0.0f);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.0f, s.left);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.0f, s.right);
}

void test_meters_per_tick_is_circumference_over_ticks() {
  TEST_ASSERT_FLOAT_WITHIN(1e-9f, 2.0f * 3.14159265f * 0.0325f / 1000.0f,
                           rover::metersPerTick(0.0325f, 1000.0f));
}

void test_meters_per_tick_rejects_non_positive_ticks() {
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.0f, rover::metersPerTick(0.03f, 0.0f));
}

}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_straight_line_drives_both_wheels_equally);
  RUN_TEST(test_positive_angular_turns_left_in_place);
  RUN_TEST(test_arc_combines_linear_and_angular);
  RUN_TEST(test_limit_keeps_speeds_within_range_unchanged);
  RUN_TEST(test_limit_scales_both_wheels_to_keep_the_turn_radius);
  RUN_TEST(test_limit_uses_the_magnitude_of_reverse_speeds);
  RUN_TEST(test_limit_with_non_positive_max_stops_the_wheels);
  RUN_TEST(test_meters_per_tick_is_circumference_over_ticks);
  RUN_TEST(test_meters_per_tick_rejects_non_positive_ticks);
  return UNITY_END();
}
