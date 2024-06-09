/*
 * bridge.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Andrey
 */

#include "bridge.h"


const uint16_t MAX_MESSAGE_LENGHT = 1000;


msg_t* getNewMsgPtr(uint16_t size)
{
	if(size > MAX_MESSAGE_LENGHT) return NULL;	//too long lenght

	msg_t *new_message = malloc(sizeof(msg_t));
	if( NULL == new_message) return NULL;

	new_message->data = malloc(size);
	if(NULL == new_message->data)
	{
		free(new_message);
		return NULL;
	}

	new_message->size = 0;
	return new_message;
}

void freeMsgPtr(msg_t *message)
{
	free(message->data);
	free(message);
}
