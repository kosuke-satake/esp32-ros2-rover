#include <command_timeout.h>
#include <unity.h>

using rover::CommandTimeout;

namespace {

constexpr uint32_t kTimeoutMs = 300;

void test_expired_before_the_first_command() {
  CommandTimeout timeout(kTimeoutMs);
  TEST_ASSERT_TRUE(timeout.expired(0));
  TEST_ASSERT_TRUE(timeout.expired(5000));
}

void test_active_right_after_a_command() {
  CommandTimeout timeout(kTimeoutMs);
  timeout.feed(1000);
  TEST_ASSERT_FALSE(timeout.expired(1000));
  TEST_ASSERT_FALSE(timeout.expired(1299));
}

void test_expires_when_commands_stop() {
  CommandTimeout timeout(kTimeoutMs);
  timeout.feed(1000);
  TEST_ASSERT_TRUE(timeout.expired(1300));
}

void test_a_new_command_restarts_the_timer() {
  CommandTimeout timeout(kTimeoutMs);
  timeout.feed(1000);
  TEST_ASSERT_TRUE(timeout.expired(1400));
  timeout.feed(1400);
  TEST_ASSERT_FALSE(timeout.expired(1600));
}

void test_works_across_millis_wraparound() {
  CommandTimeout timeout(kTimeoutMs);
  timeout.feed(0xFFFFFF00u);
  TEST_ASSERT_FALSE(timeout.expired(0x00000010u));  // 272 ms later
  TEST_ASSERT_TRUE(timeout.expired(0x00000040u));   // 320 ms later
}

void test_stays_expired_even_if_the_clock_wraps_back_near_the_last_command() {
  CommandTimeout timeout(kTimeoutMs);
  timeout.feed(1000);
  TEST_ASSERT_TRUE(timeout.expired(2000));
  // About 49.7 days later millis() is close to 1000 again.
  TEST_ASSERT_TRUE(timeout.expired(1100));
}

}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_expired_before_the_first_command);
  RUN_TEST(test_active_right_after_a_command);
  RUN_TEST(test_expires_when_commands_stop);
  RUN_TEST(test_a_new_command_restarts_the_timer);
  RUN_TEST(test_works_across_millis_wraparound);
  RUN_TEST(test_stays_expired_even_if_the_clock_wraps_back_near_the_last_command);
  return UNITY_END();
}
