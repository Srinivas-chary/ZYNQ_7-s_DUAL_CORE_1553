#include "mil.h"
#include <unistd.h>
#include <string.h>

u16 frameitems[36];
u16 nResult = 0;
extern mil_metadata_t metadata_1553[3];
void bc_createMsg(mil_metadata_t* data) {
    // Message_AB defines the transaction structure in the ACE RAM
    Message_AB(
        data->msg.msgid,
        data->msg.channel,
        data->msg.cmd.details.rt_addr,
        data->msg.cmd.details.sub_addr,
        data->msg.cmd.details.direction, // 1=Transmit(RT->BC), 0=Receive(BC->RT)
        data->msg.cmd.details.wordcount ? data->msg.cmd.details.wordcount : 32,
        data->msg.payload
    );
}

void init1553_Bcmode(int module)
{
    InitBC(module);

    bc_createMsg(&metadata_1553[0]);

    // Initializing the first message in the frame
    frameitems[0] = metadata_1553[0].msg.msgid;
    nResult = DefFrame(1, frameitems);

    // Note: RunBc starts the stack processing.
    // If you plan to trigger messages manually with StartBcNoWait,
    // ensure this doesn't create a conflict.
    SelectModule(module);
    nResult = RunBc(module, 1, 1, 1);
    usleep(1000);
}

void bc_send_1553_msg(mil_metadata_t *t1)
{
    MSG_RESULT temp_data;
    u16 wc = 0;

    // 1. Prepare the message hardware descriptor
    bc_createMsg(t1);

    // 2. Define the frame with this message ID
    frameitems[0] = t1->msg.msgid;
    nResult = DefFrame(1, frameitems);

    // 3. Clear payload if we are receiving data from an RT
    if(t1->msg.cmd.details.direction == 1) { // RT to BC
        wc = t1->msg.cmd.details.wordcount ? t1->msg.cmd.details.wordcount : 32;
        memset(t1->msg.payload, 0, sizeof(t1->msg.payload));
    }

    // 4. Trigger execution
    SelectModule(MOD2);
    StartBcNoWait(1);

    // 5. Wait for the transaction to complete on the physical bus
    // 1553 messages take ~20us to ~700us depending on word count
    usleep(1500);

    // 6. Retrieve results
    memset(&temp_data, 0, sizeof(temp_data));
    nResult = ReadMsgResult(0, frameitems[0], &temp_data);

    // 7. Error Checking via Block Status Word (BSW)
    // Check for Format Error, No Response, or Loopback Error
    if(!temp_data.bsw_bits.err_flag &&
       !temp_data.bsw_bits.stat_wrd_err &&
       !temp_data.bsw_bits.bus)
    {
        // If it's a Transmit Command (RT to BC), copy data from hardware to t1
        if(t1->msg.cmd.details.direction == 1)
        {
            for(int i = 0; i < wc && i < 32; i++) {
                t1->msg.payload[i] = temp_data.data[i];
            }
        }
        t1->msg.result = 0; // Success
    }
    else {
        t1->msg.result = -1; // Hardware or Protocol Error
    }
}
