/*
 * mil.c
 *
 *  Created on: Nov 23, 2025
 *      Author: checkout
 */



/*
 * mil.cpp
 *
 *  Created on: 11-Feb-2024
 *      Author: SES
 */

#include "mil.h"


WORD getNoOfMessagesRTMT(RtMt* rtmt){
	return GetNoOfMTMsgs_RTMT(rtmt->module_no);
}

void init1553_RTMTmode(RtMt* rtmt)
{
	SelectModule(rtmt->module_no);
	if(rtmt->selective){
		ConfigRT_MT_SelectiveMsg(rtmt->module_no, rtmt->rt, rtmt->noOfCmdWords, rtmt->cmdwords);
	}else{
		ConfigRTMT(rtmt->module_no, rtmt->rt);
	}
}


void rtmt_read_1553_msg(RtMt* rtmt, mil_metadata_t *t1, WORD msgno)
{
	SelectModule(rtmt->module_no);
	MESSAGEBLOCK *temp = 0;
	cmd_t cmd;
	WORD res, timetag, bsw;
	res=ReadMTMsg_RTMT(rtmt->module_no, msgno, temp);
	rtmt->nResult=res;
#ifdef DEBUG
	cout<<"res="<<res<<endl;
#endif
	timetag=temp->TimeTag;
	bsw=temp->BSW;
	cmd.cmdword = temp->CmdWord;
	t1->msg.msgid = msgno;
	t1->msg.cmd.cmdword = cmd.cmdword;
	t1->msg.result = res;
	t1->msg.timeTag = timetag;
	t1->msg.bsw = bsw;
	memcpy(t1->msg.payload, temp->DataPacket, 64);
	if(cmd.details.direction)
	{

	}
}

void rtmt_write_1553_block(RtMt* rtmt, mil_metadata_t* t1){
	SelectModule(rtmt->module_no);
	WORD ret=WriteDataBlock(t1->msg.cmd.details.sub_addr, t1->msg.payload);
	t1->msg.result=ret;
	rtmt->nResult=ret;
}


