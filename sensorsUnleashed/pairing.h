/*******************************************************************************
 * Copyright (c) 2017, Ole Nissen.
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  1. Redistributions of source code must retain the above copyright 
 *  notice, this list of conditions and the following disclaimer. 
 *  2. Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following
 *  disclaimer in the documentation and/or other materials provided
 *  with the distribution. 
 *  3. The name of the author may not be used to endorse or promote
 *  products derived from this software without specific prior
 *  written permission.  
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This file is part of the Sensors Unleashed project
 *******************************************************************************/
/*
 * pairing.h
 *
 *  Created on: 12/11/2016
 *      Author: omn
 */

#ifndef SENSORSUNLEASHED_PAIRING_H_
#define SENSORSUNLEASHED_PAIRING_H_

#include "contiki.h"
#include "mmem.h"
#include "net/ip/uiplib.h"
//#include "uartsensors.h"
#include "lib/list.h"
#include "lib/susensors.h"

//We need to have a way to keep track of which sensor a notification belongs to
enum datatype_e{
	uartsensor,
	susensor
};

struct __attribute__ ((__packed__)) joinpair_s{
	struct joinpair_s *next;	/* for LIST, points to next resource defined */

	struct mmem dsturl;
	char* dsturlAbove;
	char* dsturlBelow;
	char* dsturlChange;
	struct mmem srcurl;	//Used only to determine at boot, which resource we are a pair of.
	int8_t triggers[3];
	enum datatype_e devicetype;
	void* deviceptr;
	uip_ip6addr_t destip;
	char nodediscuri[25];
};

typedef struct joinpair_s joinpair_t;

uint8_t parseMessage(joinpair_t* pair);

list_t pairing_get_pairs(void);
//joinpair_t* getUartSensorPair(uartsensors_device_t* p);
//void activateUartSensorPairing(uartsensors_device_t* p);
void activateSUSensorPairing(susensors_sensor_t* p);

uint8_t pairing_assembleMessage(const uint8_t* data, uint32_t len, uint32_t num);
int16_t pairing_getlist(susensors_sensor_t* s, uint8_t* buffer, uint16_t len, int32_t *offset);
uint8_t pairing_remove_all(susensors_sensor_t* s);
uint8_t pairing_remove(susensors_sensor_t* s, uint32_t len, uint8_t* indexbuffer);
uint8_t pairing_handle(susensors_sensor_t* s);
void store_SensorPair(susensors_sensor_t* s, uint8_t* data, uint32_t len);
void restore_SensorPairs(susensors_sensor_t* s);


#endif /* SENSORSUNLEASHED_PAIRING_H_ */
