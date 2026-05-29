#define INTERVAL 10


#include <timerInterrupt.h>
#include <mil.h>


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
// Static test packet: Starts with 2, Ends with 3, Data 1-34 in between
uint8_t static_packet[36] = { 0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		0x20, 0x21, 0x22, 0x03 };


bool service_mil = false, service_serial = false;
RtMt mil_meta;
mil_metadata_t mil_metadata0, mil_metadata1;


void init_globals() {
	initInterrupt(INTERVAL);
}


void demo() {
	for (int i = 0; i < 18; i++) {
		mil_metadata0.msg.payload[i] = (static_packet[i * 2] << 8)
								| static_packet[i * 2 + 1];
	}

	mil_metadata0.msg.cmd.details.rt_addr = 1;
	mil_metadata0.msg.cmd.details.sub_addr = 2;
	mil_metadata0.msg.cmd.details.direction = 1;
	mil_metadata0.msg.cmd.details.wordcount = 18;
	rtmt_write_1553_block(&mil_meta, &mil_metadata0);
}

int main() {
	init_globals();


	mil_meta.rt =2;
	mil_meta.module_no = MOD0;
	mil_meta.msg_counter = 0;
	mil_meta.selective = false;
	init1553_RTMTmode(&mil_meta);

	memset(mil_metadata0.msg.payload, 'A', sizeof mil_metadata0.msg.payload);

	while (1) {
		if (interrupt_flag) {
			interrupt_flag = false;

//			xil_printf("0x%02X \r\n",mil_metadata0.msg.payload[0]);
			mil_meta.msg_counter = getNoOfMessagesRTMT(&mil_meta);

			while (mil_meta.msg_counter > 0) {
				rtmt_read_1553_msg(&mil_meta, &mil_metadata0,--mil_meta.msg_counter);

				if (mil_metadata0.msg.cmd.details.rt_addr ==2) {
					switch (mil_metadata0.msg.cmd.details.direction) {
					case TRANSMIT:
//					for(int i = 0; i < 18; i++) mil_metadata0.msg.payload[i]=(static_packet[i*2] << 8) | static_packet[i*2 + 1];
						mil_metadata0.msg.cmd.details.sub_addr = 2;
						rtmt_write_1553_block(&mil_meta, &mil_metadata0);
						break;
					case RECEIVE:
						for(int jj=0;jj<32;jj++)
						{
							xil_printf("0x%02X  ",mil_metadata0.msg.payload[jj]);
						}
						xil_printf("\r\n");
						mil_metadata0.msg.cmd.details.sub_addr = 2;
						rtmt_write_1553_block(&mil_meta, &mil_metadata0);
						break;
					default:
						break;
					}
				} //end of if
				else {

				}
			} //endof while(counter);

		} // end of interrupt
	} // endof outer while(1)

} //end of main
