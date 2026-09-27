// Detects that velocity commands have stopped arriving, so the body can stop
// the motors on its own (see "Command timeout" in docs/architecture.md).
#pragma once

#include <stdint.h>

namespace rover {

class CommandTimeout {
 public:
  explicit CommandTimeout(uint32_t timeout_ms);

  // Call whenever a valid command arrives. now_ms is the value of millis().
  void feed(uint32_t now_ms);

  // True if no command has arrived within the timeout, or none has arrived
  // yet. Once expired it stays expired until the next feed(), so a long gap
  // cannot look short after millis() wraps around (about every 49.7 days).
  // Call it on every loop iteration.
  bool expired(uint32_t now_ms);

 private:
  uint32_t timeout_ms_;
  uint32_t last_feed_ms_;
  bool active_;
};

}  // namespace rover
