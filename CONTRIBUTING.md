## Contributing

Contributions are welcome. This is a solo-maintained project, so response times will vary, but every issue and PR gets read.

### Reporting bugs

Open a [GitHub issue](../../issues/new) with:

- **Hardware:** Shelly 1 Gen4 hardware revision (printed on the PCB), and the firmware version you're running
- **Ecosystem:** Apple Home, Google Home, Alexa, Home Assistant, etc., and the Thread Border Router(s) on your network
- **What happened:** What you expected, what actually occurred, and steps to reproduce
- **Logs:** Serial monitor output at 115200 baud, especially anything around the failure point

If your device won't commission, won't boot, or behaves unexpectedly after flashing, serial logs are usually the fastest path to a diagnosis.

Firmware filename convention: automatous-io-{model}-{variant}-{version}.bin
- {model}: shelly-1-gen4, shelly-1pm-gen4, etc.
- {variant}: light, switch, dimmer
- {version}: v1.0.0