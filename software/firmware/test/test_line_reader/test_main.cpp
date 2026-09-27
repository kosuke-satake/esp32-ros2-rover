#include <line_reader.h>
#include <string.h>
#include <unity.h>

using rover::LineReader;

namespace {

// Feeds text byte by byte and returns how many lines it completed. The last
// completed line is left in reader.line().
int feedAll(LineReader& reader, const char* text, size_t length) {
  int lines = 0;
  for (size_t i = 0; i < length; ++i) {
    if (reader.feed(text[i])) {
      ++lines;
    }
  }
  return lines;
}

int feedAll(LineReader& reader, const char* text) {
  return feedAll(reader, text, strlen(text));
}

void test_completes_a_line_at_the_newline() {
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, "V 1 0 0") == 0);
  TEST_ASSERT_TRUE(reader.feed('\n'));
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 1 0 0") == 0);
}

void test_strips_carriage_return() {
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, "V 1 0 0\r\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 1 0 0") == 0);
}

void test_reads_consecutive_lines() {
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, "V 1 0 0\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 1 0 0") == 0);
  TEST_ASSERT_TRUE(feedAll(reader, "V 2 0.5 0\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 2 0.5 0") == 0);
}

void test_skips_empty_lines() {
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, "\n\r\n\n") == 0);
  TEST_ASSERT_TRUE(feedAll(reader, "V 1 0 0\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 1 0 0") == 0);
}

void test_accepts_the_longest_line() {
  char text[LineReader::kMaxLineLength + 1];
  memset(text, 'a', LineReader::kMaxLineLength - 1);
  text[LineReader::kMaxLineLength - 1] = '\n';
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, text, LineReader::kMaxLineLength) == 1);
  TEST_ASSERT_TRUE(strlen(reader.line()) == LineReader::kMaxLineLength - 1);
}

void test_drops_a_line_that_is_too_long_and_recovers() {
  char text[LineReader::kMaxLineLength + 1];
  memset(text, 'a', LineReader::kMaxLineLength);
  text[LineReader::kMaxLineLength] = '\n';
  LineReader reader;
  TEST_ASSERT_TRUE(feedAll(reader, text, sizeof(text)) == 0);
  TEST_ASSERT_TRUE(feedAll(reader, "V 1 0 0\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 1 0 0") == 0);
}

void test_drops_a_much_longer_line_whole() {
  LineReader reader;
  for (int i = 0; i < 1000; ++i) {
    TEST_ASSERT_FALSE(reader.feed('V'));
  }
  TEST_ASSERT_FALSE(reader.feed('\n'));
  TEST_ASSERT_TRUE(feedAll(reader, "V 2 0 0\n") == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 2 0 0") == 0);
}

void test_drops_a_line_containing_nul() {
  LineReader reader;
  const char text[] = "V 1\0 0 0\nV 2 0 0\n";
  TEST_ASSERT_TRUE(feedAll(reader, text, sizeof(text) - 1) == 1);
  TEST_ASSERT_TRUE(strcmp(reader.line(), "V 2 0 0") == 0);
}

}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_completes_a_line_at_the_newline);
  RUN_TEST(test_strips_carriage_return);
  RUN_TEST(test_reads_consecutive_lines);
  RUN_TEST(test_skips_empty_lines);
  RUN_TEST(test_accepts_the_longest_line);
  RUN_TEST(test_drops_a_line_that_is_too_long_and_recovers);
  RUN_TEST(test_drops_a_much_longer_line_whole);
  RUN_TEST(test_drops_a_line_containing_nul);
  return UNITY_END();
}
