#include <stdint.h>
#include <printf.h>
#include <trace.h>

#define TCSM0_BANK0 0x132B0000

#define AUX_MESG 0x132A0014 /* Raise interrupt to MAIN core */
#define AUX_ACK 0x132A0018

int main(void)
{
	unsigned int i;

	printf("Hello world!\n");
	printf("Running test...\n");

	for (i = 0; i < 16; i++)
		((uint32_t *)TCSM0_BANK0)[i] = i;

	printf("Test done.\n");

	/* Raise interrupt */
	*((volatile int *)(AUX_MESG)) = 0xdeadcafe;

	return 0;
}
