#include <stdint.h>

class RemoteReceiver{
  public:
	static void start(uint8_t intr);
	static uint8_t dataReady();
	static uint32_t getData();

 private:
   static void interrupt();
   static volatile uint8_t ready;
   static volatile uint32_t data;
};
