#include "protocol.h"

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace rover {
namespace protocol {
namespace {

// V uses four fields. Anything after them is ignored, as the protocol
// allows fields to be appended later.
constexpr size_t kVelocityFields = 4;

bool isDigit(char c) { return c >= '0' && c <= '9'; }

// Splits line in place at spaces and returns the number of fields found, up
// to max_fields. Fields after that are left unsplit and ignored.
size_t splitFields(char* line, char* fields[], size_t max_fields) {
  size_t count = 0;
  char* p = line;
  while (*p != '\0') {
    while (*p == ' ') {
      *p++ = '\0';
    }
    if (*p == '\0' || count == max_fields) {
      break;
    }
    fields[count++] = p;
    while (*p != ' ' && *p != '\0') {
      ++p;
    }
  }
  return count;
}

// Unsigned decimal integer that fits in 32 bits.
bool parseUint32(const char* text, uint32_t* out) {
  if (*text == '\0') {
    return false;
  }
  uint64_t value = 0;
  for (const char* p = text; *p != '\0'; ++p) {
    if (!isDigit(*p)) {
      return false;
    }
    value = value * 10 + static_cast<uint64_t>(*p - '0');
    if (value > UINT32_MAX) {
      return false;
    }
  }
  *out = static_cast<uint32_t>(value);
  return true;
}

// Real number in the form -?[0-9]+(\.[0-9]+)? as required by the protocol.
// Checking the form first rejects what strtof would also accept, such as
// exponents, hexadecimal, "nan" and "inf".
bool parseReal(const char* text, float* out) {
  const char* p = text;
  if (*p == '-') {
    ++p;
  }
  if (!isDigit(*p)) {
    return false;
  }
  while (isDigit(*p)) {
    ++p;
  }
  if (*p == '.') {
    ++p;
    if (!isDigit(*p)) {
      return false;
    }
    while (isDigit(*p)) {
      ++p;
    }
  }
  if (*p != '\0') {
    return false;
  }
  const float value = strtof(text, nullptr);
  if (!isfinite(value)) {
    return false;
  }
  *out = value;
  return true;
}

}  // namespace

bool parseVelocityCommand(const char* line, VelocityCommand* out) {
  char buf[kMaxLineLength + 1];
  const size_t length = strlen(line);
  if (length > kMaxLineLength) {
    return false;
  }
  memcpy(buf, line, length + 1);

  size_t end = length;
  if (end > 0 && buf[end - 1] == '\n') {
    buf[--end] = '\0';
    if (end > 0 && buf[end - 1] == '\r') {
      buf[--end] = '\0';
    }
  }
  // The limit includes the newline, even when a UDP datagram omits it.
  if (end >= kMaxLineLength) {
    return false;
  }
  for (size_t i = 0; i < end; ++i) {
    if (buf[i] < 0x20 || buf[i] > 0x7e) {
      return false;
    }
  }

  char* fields[kVelocityFields];
  const size_t count = splitFields(buf, fields, kVelocityFields);
  if (count < kVelocityFields || strcmp(fields[0], "V") != 0) {
    return false;
  }

  VelocityCommand command;
  if (!parseUint32(fields[1], &command.seq) ||
      !parseReal(fields[2], &command.linear_mps) ||
      !parseReal(fields[3], &command.angular_radps)) {
    return false;
  }
  *out = command;
  return true;
}

size_t formatOdometry(const Odometry& msg, char* buf, size_t buf_size) {
  const char* state = bodyStateName(msg.state);
  if (state == nullptr) {
    return 0;
  }
  const int written =
      snprintf(buf, buf_size,
               "O %" PRIu32 " %" PRIu32 " %" PRId32 " %" PRId32 " %" PRIu32
               " %s\n",
               msg.seq, msg.time_ms, msg.left_ticks, msg.right_ticks,
               msg.battery_mv, state);
  if (written < 0 || static_cast<size_t>(written) >= buf_size) {
    return 0;
  }
  return static_cast<size_t>(written);
}

const char* bodyStateName(BodyState state) {
  switch (state) {
    case BodyState::kOk:
      return "OK";
    case BodyState::kTimeout:
      return "TIMEOUT";
    case BodyState::kEstop:
      return "ESTOP";
    case BodyState::kLowBattery:
      return "LOWBAT";
  }
  return nullptr;
}

}  // namespace protocol
}  // namespace rover
