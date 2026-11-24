# Parts

Parts of the ESP32-only prototype (Stage 1). The prototype may be rebuilt,
chassis and sensors included, when the ROS 2 stage starts, so this list will
change.

| Part | Model | Qty | Notes |
| --- | --- | --- | --- |
| Microcontroller board | ESP32-S3 development board (exact model to be confirmed) | 1 | The exact model sets the PlatformIO board and the pin map. |
| DC gear motor | TT motor, without encoder | 2 | No speed feedback, so the motors run open loop. |
| Motor driver | L298N module | 1 | See the notes below. |
| Ultrasonic sensor | HC-SR04 | 1 | See the notes below. |
| Battery | to be decided | | Motors and electronics need separate power branches (see [architecture](../docs/architecture.md)). |
| Emergency-stop switch | to be decided | 1 | Required by the safety rules: it cuts the motor power. |
| Chassis | laser-cut, designed in `cad/` and `laser-cut/` | 1 | |

Where to buy and the prices are added here as the parts are bought.

## Notes

These notes come from general knowledge of these modules. They have not been
checked against the modules in hand, so check the datasheet and the markings
on your own board.

- **L298N.**
  - The small jumper caps on ENA and ENB keep the enable inputs high. Remove
    them and drive ENA and ENB with PWM to set the speed.
  - While the ESP32 resets or boots, its pins float. Pull ENA, ENB and IN1 to
    IN4 down to GND with resistors (about 10 kΩ) so that the motors stay off
    in that state (see the safety rules in [architecture](../docs/architecture.md)).
  - The module drops roughly 1.5 to 2 V, so the motors get a lower voltage
    than the battery. Choose the battery with the motors' rated voltage in
    mind.
- **HC-SR04.** The ECHO pin outputs 5 V, but the ESP32-S3 pins work at 3.3 V
  and are not 5 V tolerant. Put a voltage divider (for example two resistors,
  1 kΩ and 2 kΩ) between ECHO and the ESP32, or use a 3.3 V version of the
  sensor.
- **ESP32-S3 pins.** Do not use the strapping pins (GPIO0, GPIO3, GPIO45,
  GPIO46), the pins of the USB port (GPIO19, GPIO20) or the UART used for
  flashing (GPIO43, GPIO44). Depending on the module, some further pins are
  taken by the flash and PSRAM. The pin map is decided once the exact board is
  known.
