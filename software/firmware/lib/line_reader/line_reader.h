// Assembles lines from a byte stream such as serial input, one byte at a
// time, so the main loop can read only what has arrived and never block.
#pragma once

#include <protocol.h>
#include <stddef.h>

namespace rover {

class LineReader {
 public:
  // Longest line accepted, including the newline (see docs/protocol.md).
  static constexpr size_t kMaxLineLength = protocol::kMaxLineLength;

  LineReader();

  // Feeds one received byte. Returns true when it completes a non-empty
  // line, which line() then returns until the next call to feed().
  // Lines that are too long or contain a NUL byte are dropped whole.
  bool feed(char c);

  // The last completed line, without the newline or a preceding "\r".
  const char* line() const;

 private:
  char buf_[kMaxLineLength];
  size_t length_;
  bool discarding_;
  bool line_ready_;
};

}  // namespace rover
