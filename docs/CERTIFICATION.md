# Matter and Thread Certification

[← Back to README](../README.md) · [Report an issue](../../../issues/new)

This document explains the certification status of the Automatous Matter over Thread firmware, what "uncertified Matter device" means for users, and the trademarks referenced in this project.

---

## Contents

- [Certification Status](#certification-status)
- [What This Means for Users](#what-this-means-for-users)
- [Test Credentials](#test-credentials)
- [Trademarks](#trademarks)

---

## Certification Status

This firmware is **not** Matter Certified or Thread Certified.

It implements the Matter protocol via Espressif's [esp-matter](https://github.com/espressif/esp-matter) SDK and Thread via [OpenThread](https://openthread.io/), but has not been submitted to the Connectivity Standards Alliance (CSA) or Thread Group for certification.

Certification requires CSA membership ($7,000+/year) and Authorized Test Lab evaluation, which are out of scope for an open-source community project.

---

## What This Means for Users

Devices flashed with this firmware appear in Matter ecosystems as **uncertified Matter devices**. Most ecosystems show a one-time warning during commissioning:

- **Apple Home:** "Add Anyway" prompt
- **Google Home:** Similar uncertified device warning
- **Amazon Alexa:** Similar uncertified device warning
- **Home Assistant:** Generally accepts uncertified devices without an extra prompt

This is normal and expected behavior. Once commissioned, the device functions identically to a certified Matter device within your fabric.

---

## Test Credentials

This firmware uses the publicly available ESP-Matter test VID/PID and setup code: 3497-011-2332

These test credentials are acceptable for personal use and development but **cannot be used in commercial certified products**. All devices running this firmware share the same setup code, which is why the [commissioning instructions](../README.md#before-commissioning) specify commissioning one device at a time.

Once a device is commissioned to your Matter fabric, the setup code is no longer used for authentication — your Matter ecosystem manages credentials going forward.

---

## Trademarks

- "**Matter**" is a trademark of the Connectivity Standards Alliance.
- "**Thread**" is a trademark of the Thread Group.
- "**Shelly**" is a trademark of Allterco Robotics.

This project is not affiliated with, endorsed by, or sponsored by any of these organizations.

---

[← Back to README](../README.md)