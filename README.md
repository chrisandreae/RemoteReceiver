## Arduino library for receiving data from wireless remote controls with an EV1527 or similar chipset.

To use, call `RemoteReceiver::start()` to initialize with the interrupt number corresponding to the hardware interrupt pin the receiver is connected to. For
AVR Arduino hardware, this is interrupt 0 for digital pin 2 and interrupt 1 for digital pin 3. Call `RemoteReceiver::dataReady()` to check whether a message has
been recieved, and `RemoteReceiver::getData()` to retrieve it and listen for the next.
