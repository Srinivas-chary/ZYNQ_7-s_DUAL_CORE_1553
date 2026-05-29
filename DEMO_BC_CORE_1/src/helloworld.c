/*
 * Empty C++ Application
 */

#include "xtime_l.h"
#include <mil.h>


#define INTERVAL_100MS (COUNTS_PER_SECOND / 1)

typedef union{
	u8 bytes[72];
	struct{
		union {
			u16 cmdword;
			struct{
				u16 wordcount:5; //lsb
				u16 sub_addr:5;
				u16 direction:1;
				u16 rt_addr:5;
			}details;
		}cmd;
		u16 payload[32];
		u16 msgid;
		u16 channel;
		s16 result;
	}msg;
}mil_metadata_t;
mil_metadata_t metadata_1553[3];

void initglobals()
{
	metadata_1553[0].msg.cmd.details.rt_addr = 1;
	metadata_1553[0].msg.cmd.details.sub_addr = 2;
	metadata_1553[0].msg.cmd.details.direction = 0;
	metadata_1553[0].msg.cmd.details.wordcount = 32;
	metadata_1553[0].msg.channel = BUS_A;
	metadata_1553[0].msg.msgid = 1;

	for(int i=0; i<32; i++)
		metadata_1553[0].msg.payload[i] =  i;

	metadata_1553[1].msg.cmd.details.rt_addr = 1;
	metadata_1553[1].msg.cmd.details.sub_addr = 2;
	metadata_1553[1].msg.cmd.details.direction = 1;
	metadata_1553[1].msg.cmd.details.wordcount = 32;
	metadata_1553[1].msg.channel = BUS_A;
	metadata_1553[1].msg.msgid = 2;

	for(int i=0; i<32; i++)
		metadata_1553[1].msg.payload[i] =  i;


}

// Declare the dummy array with 32 words
uint16_t dummy_data[32] = {
    0xFACE, 0x0001, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666,
    0x7777, 0x8888, 0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE,
    0x1010, 0x2020, 0x3030, 0x4040, 0x5050, 0x6060, 0x7070, 0x8080,
    0x9090, 0xA0A0, 0xB0B0, 0xC0C0, 0xD0D0, 0xE0E0, 0xF0F0, 0xED01
};

/*
 * Entry point for application
 *
 * */
int main()
{

	uint16_t hh = 0;
	XTime tEnd, tCur;

	initglobals();
	/* 1553B Initilization*/
	init1553_Bcmode(MOD2);


	while(1) {
	    // 1. Get current time to start the 1 second window
	    XTime_GetTime(&tEnd);
	    tEnd += COUNTS_PER_SECOND;

	    // 2. Update Dummy Data
	    dummy_data[1] = hh++;
	    if(hh > 255) hh = 0; // Reset counter at 255

	    // 3. Update Metadata Payload
	    // Note: 32 words = 64 bytes. Ensure metadata_1553[0] is a pointer or object correctly.
	    memcpy(metadata_1553[0].msg.payload, dummy_data, 64);

	    // 4. Trigger 1553 Transmission
	    // Ensure you are passing the pointer if that's what your function expects
	    bc_send_1553_msg(&metadata_1553[0]);

	    // 5. Precise Wait using Core Clock (Non-blocking window)
	    do {
	        XTime_GetTime(&tCur);
	    } while (tCur < tEnd);

	    // Optional: Print status to Core 0 terminal
	    // xil_printf("Sent Packet ID: %d\r\n", dummy_data[1]);
	}
	return 0;
}
