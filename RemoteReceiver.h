#include <stdint.h>

#ifndef REMOTE_RECEIVER_USE_ATTACH_INTERRUPT
#define REMOTE_RECEIVER_USE_ATTACH_INTERRUPT 1
#endif

class RemoteReceiver{
  public:
#if REMOTE_RECEIVER_USE_ATTACH_INTERRUPT
	static void start(uint8_t intr);
#else
	static void start();
#endif
	static uint8_t dataReady();
	static uint32_t getData();

#if REMOTE_RECEIVER_USE_ATTACH_INTERRUPT
 private:
#endif
   static void interrupt();
   static volatile uint8_t ready;
   static volatile uint32_t data;
};
