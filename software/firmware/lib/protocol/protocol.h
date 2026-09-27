// Messages between the brain and the body, as specified in docs/protocol.md.
//
// The body parses V (velocity command) lines and formats O (odometry and
// state) lines. The same text is used over UDP and serial.
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace rover {
namespace protocol {

// Longest valid line, including the trailing newline.
constexpr size_t kMaxLineLength = 96;

struct VelocityCommand {
  uint32_t seq;
  float linear_mps;
  float angular_radps;
};

enum class BodyState {
  kOk,
  kTimeout,     // No valid V within the command timeout.
  kEstop,       // The emergency stop is active.
  kLowBattery,  // The battery is too low to drive.
};

struct Odometry {
  uint32_t seq;
  uint32_t time_ms;
  int32_t left_ticks;   // Cumulative since the body started.
  int32_t right_ticks;  // Cumulative since the body started.
  uint32_t battery_mv;
  BodyState state;
};

// Parses one V line. A trailing "\n" or "\r\n" is allowed. Returns false,
// leaving *out unchanged, if the line is not a well-formed V message.
bool parseVelocityCommand(const char* line, VelocityCommand* out);

// Writes an O line, including the trailing "\n" and a terminating NUL, into
// buf. Returns the number of characters written without the NUL, or 0 if
// buf is too small or msg.state is not a valid state.
size_t formatOdometry(const Odometry& msg, char* buf, size_t buf_size);

// The word used for a state in O lines, for example "OK", or nullptr if
// state is not a valid value. An invalid state is never reported as "OK".
const char* bodyStateName(BodyState state);

}  // namespace protocol
}  // namespace rover
