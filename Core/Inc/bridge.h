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
	volatile uint16_t tx_index;
	uint8_t *data;
} msg_t;


typedef struct {
	msg_t* uart_rx_msg;
	msg_t* uart_tx_msg;
	msg_t* spi_rx_msg;
	msg_t* spi_tx_msg;
	msg_t *no_data_msg;
	uint16_t MAX_MSG_LENGHT;
} bridge_t;



msg_t* getNewMsgPtr(uint16_t size);
void freeMsgPtr(msg_t *message);
void bridgeInit(bridge_t *bridge_ptr, uint16_t max_msg_lenght);




#endif /* BRIDGE_H_ */
