#include <RemoteReceiver.h>

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  
  // start the remote receiver using interrupt 0 (pin 2)
  RemoteReceiver::start(0);
}

void loop() {
  if(RemoteReceiver::dataReady()){
    Serial.print("Read: ");
    Serial.println(RemoteReceiver::getData());
  }
}

