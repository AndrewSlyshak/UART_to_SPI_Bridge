/*
 * bridge.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Andrey
 */

#include "bridge.h"

void bridgeInit(bridge_t *bridge_ptr, uint16_t max_msg_lenght)
{
	bridge_ptr->MAX_MSG_LENGHT = max_msg_lenght;
	//no data message for empty transmition
	bridge_ptr->no_data_msg = getNewMsgPtr(1);

	//prepare messages
	bridge_ptr->spi_rx_msg = getNewMsgPtr(bridge_ptr->MAX_MSG_LENGHT);
	bridge_ptr->uart_rx_msg = getNewMsgPtr(bridge_ptr->MAX_MSG_LENGHT);
	bridge_ptr->uart_tx_msg = NULL;
	bridge_ptr->spi_tx_msg = bridge_ptr->no_data_msg;
}

msg_t* getNewMsgPtr(uint16_t size)
{

	msg_t *new_message = malloc(sizeof(msg_t));
	if( NULL == new_message) return NULL;

	new_message->data = malloc(size);
	if(NULL == new_message->data)
	{
		free(new_message);
		return NULL;
	}

	new_message->size = 0;
	new_message->tx_index = 0;
	new_message->data[0] = 0;	//for empty message
	return new_message;
}

void freeMsgPtr(msg_t *message)
{
	free(message->data);
	free(message);
}
