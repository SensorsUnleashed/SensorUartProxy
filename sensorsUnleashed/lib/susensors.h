/*
 * Copyright (c) 2009, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "contiki.h"
#include "lib/list.h"
#include "../../apps/uartsensors/uart_protocolhandler.h"

#define DEVICES_MAX		10

extern const char* suEventAboveEventString;
extern const char* suEventBelowEventString;
extern const char* suEventChangeEventString;
/* This is common to all devices and has its own function */
struct susensors_sensor;
int suconfig(struct susensors_sensor* this, int type, void* data);

enum susensors_configcmd {
	SUSENSORS_EVENTSETUP_SET,
	SUSENSORS_EVENTSETUP_GET,
	SUSENSORS_CEVENT_GET,
	SUSENSORS_AEVENT_GET,
	SUSENSORS_BEVENT_GET,
	SUSENSORS_RANGEMAX_GET,
	SUSENSORS_RANGEMIN_GET,
	SUSENSORS_EVENTSTATE_GET,
};

enum susensors_event_cmd {
	SUSENSORS_ABOVE_EVENT_SET,
	SUSENSORS_BELOW_EVENT_SET,
	SUSENSORS_CHANGE_EVENT_SET,
};

/* some constants for the configure API */
#define SUSENSORS_HW_INIT 	128 /* internal - used only for initialization */
#define SUSENSORS_ACTIVE 	129 /* ACTIVE => 0 -> turn off, 1 -> turn on */
#define SUSENSORS_READY 	130 /* read only */
#define SUSENSORS_EXTRAS 	131	/* Get a struct with further details */
#define SUSENSORS_MAX_AGE	132	/* How long will the value be valid for */

/* Used for extra material needed for using a sensor */
struct extras{
	int type;
	void* config;
	void* runtime;
};

struct susensors_sensor {
	struct susensors_sensor* next;
	char *       type;

	unsigned char event_flag;

	/* Set device values */
	int          (* value)     	(struct susensors_sensor* this, int type, void* data);
	/* Get/set device hardware specific configuration */
	int          (* configure) 	(struct susensors_sensor* this, int type, int value);
	/* Get device values */
	int          (* status)    	(struct susensors_sensor* this, int type, void* data);
	/* Received an event from another device - handle it */
	int 		   (* eventhandler)	(struct susensors_sensor* this, int type, int len, uint8_t* payload);
	/* Get the date from the last event emittet */
	int 		   (* getActiveEventMsg)	(struct susensors_sensor* this, const char** eventstr, uint8_t* payload);
	/* Get/set device suconfig (common to all devices) */
	int          (* suconfig)  	(struct susensors_sensor* this, int type, void* data);

	struct extras data;

	LIST_STRUCT(pairs);
};

typedef struct susensors_sensor susensors_sensor_t;

void initSUSensors();
susensors_sensor_t* addSUDevices(susensors_sensor_t* device);
susensors_sensor_t* susensors_find(const char *type, unsigned short len);
susensors_sensor_t* susensors_next(susensors_sensor_t* s);
susensors_sensor_t* susensors_first(void);

void susensors_changed(susensors_sensor_t* s);

extern process_event_t susensors_event;

PROCESS_NAME(susensors_process);

#endif /* SENSORS_H_ */
