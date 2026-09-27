#include "line_reader.h"

namespace rover {

constexpr size_t LineReader::kMaxLineLength;

LineReader::LineReader()
    : buf_{}, length_(0), discarding_(false), line_ready_(false) {}

bool LineReader::feed(char c) {
  if (line_ready_) {
    length_ = 0;
    buf_[0] = '\0';
    line_ready_ = false;
  }

  if (c == '\n') {
    const bool complete = !discarding_ && length_ > 0;
    discarding_ = false;
    if (!complete) {
      length_ = 0;
      return false;
    }
    if (buf_[length_ - 1] == '\r') {
      --length_;
    }
    buf_[length_] = '\0';
    line_ready_ = true;
    return length_ > 0;
  }

  if (discarding_) {
    return false;
  }
  // Keep one byte for the newline's place, which holds the NUL.
  if (c == '\0' || length_ >= kMaxLineLength - 1) {
    discarding_ = true;
    length_ = 0;
    return false;
  }
  buf_[length_++] = c;
  return false;
}

const char* LineReader::line() const { return buf_; }

}  // namespace rover
