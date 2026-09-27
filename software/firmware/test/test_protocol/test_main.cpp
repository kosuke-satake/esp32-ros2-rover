#include <protocol.h>
#include <string.h>
#include <unity.h>

using rover::protocol::BodyState;
using rover::protocol::kMaxLineLength;
using rover::protocol::Odometry;
using rover::protocol::VelocityCommand;

namespace {

constexpr float kTolerance = 1e-6f;

bool parse(const char* line, VelocityCommand* out) {
  return rover::protocol::parseVelocityCommand(line, out);
}

bool rejects(const char* line) {
  VelocityCommand command{7, 1.0f, 2.0f};
  const bool ok = parse(line, &command);
  // A rejected line must leave the previous command untouched.
  return !ok && command.seq == 7 && command.linear_mps == 1.0f &&
         command.angular_radps == 2.0f;
}

void test_parses_the_example_from_the_spec() {
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse("V 42 0.20 -0.50", &c));
  TEST_ASSERT_TRUE(c.seq == 42);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 0.2f, c.linear_mps);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -0.5f, c.angular_radps);
}

void test_accepts_newline_and_crlf() {
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse("V 1 0.1 0.2\n", &c));
  TEST_ASSERT_TRUE(parse("V 2 0.1 0.2\r\n", &c));
  TEST_ASSERT_TRUE(c.seq == 2);
}

void test_accepts_integers_and_runs_of_spaces() {
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse("V  3   1  -1", &c));
  TEST_ASSERT_TRUE(c.seq == 3);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, 1.0f, c.linear_mps);
  TEST_ASSERT_FLOAT_WITHIN(kTolerance, -1.0f, c.angular_radps);
}

void test_ignores_extra_trailing_fields() {
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse("V 4 0.1 0.2 future fields 1 2 3 4 5 6 7 8", &c));
  TEST_ASSERT_TRUE(c.seq == 4);
}

void test_accepts_the_largest_sequence_number() {
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse("V 4294967295 0 0", &c));
  TEST_ASSERT_TRUE(c.seq == 4294967295u);
}

void test_rejects_wrong_or_missing_type() {
  TEST_ASSERT_TRUE(rejects("X 1 0 0"));
  TEST_ASSERT_TRUE(rejects("v 1 0 0"));
  TEST_ASSERT_TRUE(rejects("VV 1 0 0"));
  TEST_ASSERT_TRUE(rejects(""));
  TEST_ASSERT_TRUE(rejects("\n"));
}

void test_rejects_missing_fields() {
  TEST_ASSERT_TRUE(rejects("V"));
  TEST_ASSERT_TRUE(rejects("V 1"));
  TEST_ASSERT_TRUE(rejects("V 1 0.1"));
}

void test_rejects_malformed_sequence_numbers() {
  TEST_ASSERT_TRUE(rejects("V -1 0 0"));
  TEST_ASSERT_TRUE(rejects("V 1.5 0 0"));
  TEST_ASSERT_TRUE(rejects("V 4294967296 0 0"));
  TEST_ASSERT_TRUE(rejects("V 99999999999999999999 0 0"));
  TEST_ASSERT_TRUE(rejects("V x 0 0"));
}

void test_rejects_numbers_outside_the_allowed_form() {
  TEST_ASSERT_TRUE(rejects("V 1 +0.1 0"));
  TEST_ASSERT_TRUE(rejects("V 1 .5 0"));
  TEST_ASSERT_TRUE(rejects("V 1 1. 0"));
  TEST_ASSERT_TRUE(rejects("V 1 1e3 0"));
  TEST_ASSERT_TRUE(rejects("V 1 0x10 0"));
  TEST_ASSERT_TRUE(rejects("V 1 nan 0"));
  TEST_ASSERT_TRUE(rejects("V 1 inf 0"));
  TEST_ASSERT_TRUE(rejects("V 1 - 0"));
  TEST_ASSERT_TRUE(rejects("V 1 0.1x 0"));
}

void test_rejects_two_lines_run_together() {
  TEST_ASSERT_TRUE(rejects("V 1 0.2 0.0V 2 0.2 0.0"));
}

void test_rejects_control_and_non_ascii_characters() {
  TEST_ASSERT_TRUE(rejects("V\t1 0 0"));
  TEST_ASSERT_TRUE(rejects("V 1 0 0\r"));
  TEST_ASSERT_TRUE(rejects("V 1 0 0 \xe2\x9c\x93"));
}

void test_enforces_the_line_length_limit() {
  // Longest valid line: 95 characters plus the newline.
  char line[kMaxLineLength + 2];
  memset(line, ' ', sizeof(line));
  memcpy(line, "V 1 0 0", 7);
  line[kMaxLineLength - 1] = '\n';
  line[kMaxLineLength] = '\0';
  VelocityCommand c{};
  TEST_ASSERT_TRUE(parse(line, &c));

  // Without the newline, 96 characters are one too many.
  line[kMaxLineLength - 1] = ' ';
  TEST_ASSERT_TRUE(rejects(line));

  // 96 characters plus a newline are too long as well.
  line[kMaxLineLength] = '\n';
  line[kMaxLineLength + 1] = '\0';
  TEST_ASSERT_TRUE(rejects(line));
}

void test_formats_the_example_from_the_spec() {
  const Odometry msg{1234, 56789, 10234, 10180, 11800, BodyState::kOk};
  char buf[kMaxLineLength];
  const size_t n = rover::protocol::formatOdometry(msg, buf, sizeof(buf));
  TEST_ASSERT_TRUE(strcmp(buf, "O 1234 56789 10234 10180 11800 OK\n") == 0);
  TEST_ASSERT_TRUE(n == strlen(buf));
}

void test_formats_negative_ticks_and_every_state() {
  char buf[kMaxLineLength];
  Odometry msg{1, 2, -3, -4, 5, BodyState::kTimeout};
  rover::protocol::formatOdometry(msg, buf, sizeof(buf));
  TEST_ASSERT_TRUE(strcmp(buf, "O 1 2 -3 -4 5 TIMEOUT\n") == 0);
  msg.state = BodyState::kEstop;
  rover::protocol::formatOdometry(msg, buf, sizeof(buf));
  TEST_ASSERT_TRUE(strcmp(buf, "O 1 2 -3 -4 5 ESTOP\n") == 0);
  msg.state = BodyState::kLowBattery;
  rover::protocol::formatOdometry(msg, buf, sizeof(buf));
  TEST_ASSERT_TRUE(strcmp(buf, "O 1 2 -3 -4 5 LOWBAT\n") == 0);
}

void test_longest_odometry_line_fits_the_limit() {
  const Odometry msg{4294967295u, 4294967295u, INT32_MIN, INT32_MIN,
                     4294967295u, BodyState::kTimeout};
  char buf[kMaxLineLength];
  const size_t n = rover::protocol::formatOdometry(msg, buf, sizeof(buf));
  TEST_ASSERT_TRUE(n > 0);
  TEST_ASSERT_TRUE(n <= kMaxLineLength);
}

void test_small_buffer_fails_without_overflowing() {
  const Odometry msg{1234, 56789, 10234, 10180, 11800, BodyState::kOk};
  char buf[16];
  buf[10] = '#';
  TEST_ASSERT_TRUE(rover::protocol::formatOdometry(msg, buf, 10) == 0);
  TEST_ASSERT_TRUE(buf[10] == '#');
}

void test_invalid_state_is_never_reported() {
  const Odometry msg{1, 2, 3, 4, 5, static_cast<BodyState>(99)};
  char buf[kMaxLineLength];
  TEST_ASSERT_TRUE(rover::protocol::formatOdometry(msg, buf, sizeof(buf)) == 0);
  TEST_ASSERT_TRUE(rover::protocol::bodyStateName(msg.state) == nullptr);
}

}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_parses_the_example_from_the_spec);
  RUN_TEST(test_accepts_newline_and_crlf);
  RUN_TEST(test_accepts_integers_and_runs_of_spaces);
  RUN_TEST(test_ignores_extra_trailing_fields);
  RUN_TEST(test_accepts_the_largest_sequence_number);
  RUN_TEST(test_rejects_wrong_or_missing_type);
  RUN_TEST(test_rejects_missing_fields);
  RUN_TEST(test_rejects_malformed_sequence_numbers);
  RUN_TEST(test_rejects_numbers_outside_the_allowed_form);
  RUN_TEST(test_rejects_two_lines_run_together);
  RUN_TEST(test_rejects_control_and_non_ascii_characters);
  RUN_TEST(test_enforces_the_line_length_limit);
  RUN_TEST(test_formats_the_example_from_the_spec);
  RUN_TEST(test_formats_negative_ticks_and_every_state);
  RUN_TEST(test_longest_odometry_line_fits_the_limit);
  RUN_TEST(test_small_buffer_fails_without_overflowing);
  RUN_TEST(test_invalid_state_is_never_reported);
  return UNITY_END();
}
