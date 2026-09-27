# Brain–body message protocol

The messages exchanged between the brain and the ESP32 body. They implement
the brain–body contract in [architecture.md](architecture.md): the brain sends
only velocity, and the body reports cumulative motion and its state.

The same messages are used over Wi-Fi (UDP) and USB serial, and by any
controller that drives the body directly.

## Framing

- ASCII text, one message per line. Senders end each line with `\n`;
  receivers also accept `\r\n`.
- A line is at most 96 bytes including the newline. Receivers discard longer
  lines.
- Over UDP, each datagram carries exactly one line. Receivers also accept a
  datagram without the trailing newline.
- Over serial, lines follow each other in the byte stream.

## Fields

- Fields are separated by one or more spaces.
- The first field is the message type, a single upper-case letter.
- Integers are unsigned decimal unless stated otherwise.
- Real numbers are written as an optional `-`, digits, and an optional `.`
  followed by digits, for example `0.20`, `-0.5` or `1`. No `+` sign, no
  exponent, no `nan` or `inf`.
- Receivers ignore extra fields at the end of a line, so fields can be added
  later without breaking older receivers.
- Receivers silently discard lines with an unknown type or a malformed field.

## `V`: velocity command (brain → body)

```
V <seq> <linear> <angular>
```

| Field | Type | Meaning |
| --- | --- | --- |
| `seq` | integer, 0 to 4294967295 | Increases by one per message and wraps around. Used to spot lost or reordered messages. |
| `linear` | real, m/s | Forward speed. Positive is forward. |
| `angular` | real, rad/s | Turning speed. Positive turns left (counter-clockwise), as in ROS. |

Example: `V 42 0.20 -0.50` drives forward at 0.2 m/s while turning right at
0.5 rad/s.

- The brain sends `V` continuously, at about 20 Hz, including `V <seq> 0 0`
  while the robot should stand still.
- If no valid `V` arrives within the command timeout (for example 0.3 s), the
  body stops the motors and reports `TIMEOUT`. It drives again as soon as a
  valid `V` arrives.
- The body limits wheel speeds to what the hardware can do, scaling both
  wheels together so the robot keeps the commanded turning radius.

## `O`: odometry and state (body → brain)

```
O <seq> <time> <left> <right> <battery> <state>
```

| Field | Type | Meaning |
| --- | --- | --- |
| `seq` | integer, 0 to 4294967295 | Increases by one per message and wraps around. |
| `time` | integer, ms | Body clock (`millis()`) when the values were sampled. |
| `left` | signed integer | Cumulative encoder ticks of the left wheel since the body started. Forward is positive. |
| `right` | signed integer | Cumulative encoder ticks of the right wheel since the body started. Forward is positive. |
| `battery` | integer, mV | Battery voltage. |
| `state` | word | `OK`, `TIMEOUT`, `ESTOP` or `LOWBAT`; see below. |

Example: `O 1234 56789 10234 10180 11800 OK`.

- The body sends `O` at about 20 Hz, whether or not it is receiving commands.
- Tick counts are totals, never differences, so a lost message is recovered
  by the next one. The brain computes distance and speed from the change
  between messages, using `time` for the interval.
- When the body restarts, `time` and the tick counts start again from zero.
  The brain detects this by `time` going backwards.

| State | Meaning |
| --- | --- |
| `OK` | Driving, or ready to drive. |
| `TIMEOUT` | No valid `V` within the command timeout; the motors are stopped. |
| `ESTOP` | The emergency stop is active; the motors have no power. |
| `LOWBAT` | The battery is too low; the motors are stopped. |

## Transports

- **UDP.** The body listens on a fixed port and sends each `O` to the address
  and port of the last valid `V` it received. Before the first `V`, it sends
  nothing. The port number is set in the firmware configuration.
- **USB serial.** The same lines in both directions. The baud rate is set in
  the firmware configuration.

The port number and the baud rate are documented here once the ESP32
firmware is brought up on the chosen board.

## Changing the protocol

- Add information by appending fields to an existing message, or by adding a
  new message type. Older receivers keep working.
- Changing the meaning or the order of existing fields breaks every
  implementation. Propose it first and update the firmware and all brain-side
  code together.
