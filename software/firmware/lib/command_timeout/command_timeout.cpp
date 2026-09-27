#include "command_timeout.h"

namespace rover {

CommandTimeout::CommandTimeout(uint32_t timeout_ms)
    : timeout_ms_(timeout_ms), last_feed_ms_(0), active_(false) {}

void CommandTimeout::feed(uint32_t now_ms) {
  last_feed_ms_ = now_ms;
  active_ = true;
}

bool CommandTimeout::expired(uint32_t now_ms) {
  // Unsigned subtraction gives the elapsed time even across a wraparound.
  if (active_ && static_cast<uint32_t>(now_ms - last_feed_ms_) >= timeout_ms_) {
    active_ = false;
  }
  return !active_;
}

}  // namespace rover
