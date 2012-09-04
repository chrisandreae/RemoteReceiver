#include "RemoteReceiver.h"

#include "Arduino.h"

volatile uint8_t RemoteReceiver::ready;
volatile uint32_t RemoteReceiver::data;

#if REMOTE_RECEIVER_USE_ATTACH_INTERRUPT
void RemoteReceiver::start(uint8_t intr)
#else
void RemoteReceiver::start()
#endif
{
  ready = 0;
  data = 0;
#if REMOTE_RECEIVER_USE_ATTACH_INTERRUPT
  attachInterrupt(intr, &RemoteReceiver::interrupt, CHANGE);
#endif
}

uint8_t RemoteReceiver::dataReady(){
  return ready;
}

uint32_t RemoteReceiver::getData(){
  if(ready){
	ready = 0;
	return data;
  }
  else{
	return 0;
  }
}

#define SYNC -1
void RemoteReceiver::interrupt(){
   static int8_t bits = SYNC;

   static unsigned int dataPulseLength;

   static unsigned long lastTime = 0;

   if(ready){ return; } // We've got a value to be consumed, don't clobber it (alternatively, store the most recent, alternatively, store a ringbuffer)

   unsigned long now = micros();
   unsigned int length = now - lastTime;
   lastTime = now;

   if(bits == SYNC){ // synchronizing
	  static unsigned int lastLength = 0;
	  unsigned int expectedMin = (lastLength - (lastLength >> 2)) * 31;
	  unsigned int expectedMax = (lastLength + (lastLength >> 2)) * 31;

	  if(length > 5000 && lastLength && length > expectedMin && length < expectedMax){
		// success, continue to read state
		dataPulseLength = length / 31;
		lastLength = 0;
		bits = 0;
	  }
	  else{
		lastLength = length;
	  }
   }
   else{
	  // read data bits
	  static unsigned long int current_data = 0;
	  static unsigned int pulse0 = 0;
	  if(pulse0 == 0){
	   pulse0 = length;
	  }
	  else{
		unsigned int minPulse = dataPulseLength - (dataPulseLength >> 2);
		unsigned int maxPulse = dataPulseLength + (dataPulseLength >> 2);

		if(pulse0 > minPulse * 3 && pulse0 < maxPulse * 3 &&
		   length > minPulse     && length < maxPulse){
			  // read high bit
			  current_data = (current_data << 1) | 1;
			  ++bits;

		}
		else if(length > minPulse * 3 && length < maxPulse * 3 &&
			   pulse0 > minPulse      && pulse0 < maxPulse){
			  // read low bit
			  current_data = (current_data << 1);
			  ++bits;

		}
		else{
		  // failure, return to waiting for sync
		  bits = SYNC;
		  current_data = 0;
		  dataPulseLength = 0;
		}

		pulse0 = 0; // always reset pulse 0

		if(bits == 24){
		  // Save data
		  data = current_data;
		  ready = 1;

		  // return to sync
		  current_data = 0;
		  dataPulseLength = 0;
		  bits = SYNC;
		}
	  }
   }
}
