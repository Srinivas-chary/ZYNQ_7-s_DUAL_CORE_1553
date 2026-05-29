/*
 * timerInterrupt.h
 *
 *  Created on: Nov 23, 2025
 *      Author: checkout
 */

#ifndef SRC_TIMERINTERRUPT_H_
#define SRC_TIMERINTERRUPT_H_


#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include <stdbool.h>


#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR


int initInterrupt(float timeout_msec);


#endif /* SRC_TIMERINTERRUPT_H_ */
