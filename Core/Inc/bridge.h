/*
 * bridge.h
 *
 *  Created on: Jun 9, 2024
 *      Author: Andrey
 */

#ifndef BRIDGE_H_
#define BRIDGE_H_

#include "stm32f4xx_hal.h"

typedef struct {
	volatile uint16_t size;
	uint8_t *data;
} msg_t;


typedef struct {
	msg_t* uart_rx_msg;
	msg_t* uart_tx_msg;
	msg_t* spi_rx_msg;
	msg_t* spi_tx_msg;
} bridge_t;



msg_t* getNewMsgPtr(uint16_t size);
void freeMsgPtr(msg_t *message);




#endif /* BRIDGE_H_ */
