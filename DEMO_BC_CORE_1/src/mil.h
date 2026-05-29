/*
 * mil.h
 *
 *  Created on: Nov 23, 2025
 *      Author: checkout
 */

#ifndef SRC_MIL_H_
#define SRC_MIL_H_

#include <global.h>
#include <Mil1553B.h>


typedef struct {
	WORD module_no;
	WORD rt;
	WORD nResult;
	WORD msg_counter;
}Rt;


typedef struct {
	WORD module_no;
	WORD nResult;
	UINT32 msg_counter;
	bool selective;
	WORD noOfCmdWords;
	WORD cmdwords[32];
}Mt;

typedef struct {
	WORD module_no;
	WORD rt;
	WORD nResult;
	WORD msg_counter;
	bool selective;
	WORD noOfCmdWords;
	WORD cmdwords[32];
}RtMt;

typedef struct{
	WORD module_no;
	WORD frameitems[32];
	WORD nResult;
}Bc;




WORD getNoOfMessagesRT(Rt*);
void init1553_RTmode(Rt*);
void rt_read_1553_msg(Rt*, mil_metadata_t *, WORD);
void rt_write_1553_block(Rt*, mil_metadata_t *);


UINT32 getNoOfMessagesMT(Mt*);
void init1553_MTmode(Mt*);
void mt_read_1553_msg(Mt*, mil_metadata_t *, WORD);


WORD getNoOfMessagesRTMT(RtMt*);
void init1553_RTMTmode(RtMt*);
void rtmt_read_1553_msg(RtMt*, mil_metadata_t *, WORD);
void rtmt_write_1553_block(RtMt*, mil_metadata_t *);

//void init1553_Bcmode(Bc*);
void init1553_Bcmode();
void bc_send_1553_msgs(Bc*, mil_metadata_t**, int);
//void bc_send_1553_msg(Bc*, mil_metadata_t*);
void bc_send_1553_msg( mil_metadata_t*);
void bc_check_1553_msg_status(Bc*, mil_metadata_t**, int);
//int bc_createMsg(Bc*, mil_metadata_t*);
void bc_createMsg(mil_metadata_t*);

#endif /* SRC_MIL_H_ */
