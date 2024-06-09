/*
 * bridge.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Andrey
 */

#include "bridge.h"


/**
 * @brief Initializes the bridge structure with specified message length.
 * @param bridge_ptr Pointer to the bridge structure to initialize.
 * @param max_msg_lenght Maximum length for the messages to be handled by the bridge.
 */
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

/**
 * @brief Allocates and initializes a new message structure.
 * @param size The size of the data buffer to allocate for the message.
 * @return msg_t* Pointer to the newly created message or NULL if allocation fails.
 */
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

/**
 * @brief Handles received UART byte and prepares message for queue.
 * @param bridge_ptr Pointer to the bridge structure containing UART message details.
 * @return msg_t* Pointer to the message to be queued if complete, otherwise NULL.
 */
msg_t* UARTRxByteHandler(bridge_t *bridge_ptr)
{
	if('\0' == bridge_ptr->uart_rx_msg->data[bridge_ptr->uart_rx_msg->size])
	{
		//check if not empty
		if( 0 != bridge_ptr->uart_rx_msg->size )
		{
			//prepare for push in queue msg and get new ptr
			msg_t *to_queue_ptr = bridge_ptr->uart_rx_msg;
			bridge_ptr->uart_rx_msg = getNewMsgPtr(bridge_ptr->MAX_MSG_LENGHT);

			return to_queue_ptr;
			//TODO add memory optimization for small message
		}
	}
	else
	{
		bridge_ptr->uart_rx_msg->size++;
		if(bridge_ptr->uart_rx_msg->size > bridge_ptr->MAX_MSG_LENGHT) bridge_ptr->uart_rx_msg->size = 0;	//over
	}

	return NULL;
}


/**
 * @brief Handles received SPI byte and prepares message for queue.
 * @param bridge_ptr Pointer to the bridge structure containing SPI message details.
 * @return msg_t* Pointer to the message to be queued if complete, otherwise NULL.
 */
msg_t* SPIRxByteHandler(bridge_t *bridge_ptr)
{
	//rx check
	if( '\0' == bridge_ptr->spi_rx_msg->data[bridge_ptr->spi_rx_msg->size] )
	{
		//check if not empty
		if( 0 != bridge_ptr->spi_rx_msg->size )
		{
			//prepare for push in queue msg and get new ptr
			msg_t *to_queue_ptr = bridge_ptr->spi_rx_msg;
			bridge_ptr->spi_rx_msg = getNewMsgPtr(bridge_ptr->MAX_MSG_LENGHT);

			return to_queue_ptr;
		}
	}
	else
	{
		bridge_ptr->spi_rx_msg->size++;	//read next byte
		if(bridge_ptr->spi_rx_msg->size > bridge_ptr->MAX_MSG_LENGHT) bridge_ptr->spi_rx_msg->size = 0;	//over
	}

	//transmitting real message
	if(bridge_ptr->spi_tx_msg != bridge_ptr->no_data_msg)
	{
		//tx done check
		if( bridge_ptr->spi_tx_msg->tx_index >= bridge_ptr->spi_tx_msg->size )
		{
			freeMsgPtr(bridge_ptr->spi_tx_msg);
			bridge_ptr->spi_tx_msg = bridge_ptr->no_data_msg;
		}
		else
		{
			bridge_ptr->spi_tx_msg->tx_index++;
		}
	}

	return NULL;
}


