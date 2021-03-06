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
#include "ledindicator.h"
#include "contiki.h"
#include "dev/leds.h"

#include "../../apps/uartsensors/uart_protocolhandler.h"
#include "rest-engine.h"
#include "susensorcommon.h"
#include "board.h"

typedef enum su_basic_actions su_led_actions;

struct resourceconf ledindicatorconfig = {
		.resolution = 1,
		.version = 1,
		.flags = METHOD_GET | METHOD_PUT | IS_OBSERVABLE | HAS_SUB_RESOURCES,
		.max_pollinterval = 2,
		.eventsActive = ChangeEventActive,
		.AboveEventAt = {
				.type = CMP_TYPE_UINT8,
				.as.u8 = 1
		},
		.BelowEventAt = {
				.type = CMP_TYPE_UINT8,
				.as.u8 = 0
		},
		.ChangeEvent = {
				.type = CMP_TYPE_UINT8,
				.as.u8 = 1
		},
		.RangeMin = {
				.type = CMP_TYPE_UINT16,
				.as.u8 = 0
		},
		.RangeMax = {
				.type = CMP_TYPE_UINT16,
				.as.u8 = 1
		},

		.unit = "",
		.spec = "LED indicator",
		//.type = LED_INDICATOR,
		.attr = "title=\"LED indicator\" ;rt=\"Indicator\"",
};

static int set(struct susensors_sensor* this, int type, void* data){
	int ret = 1;

	if(type == setToggle){
		leds_toggle(((struct ledRuntime*)(this->data.runtime))->mask);
		ret = 0;
	}
	else if(type == setOn){
		leds_on(((struct ledRuntime*)(this->data.runtime))->mask);
		ret = 0;
	}
	else if(type == setOff){
		leds_off(((struct ledRuntime*)(this->data.runtime))->mask);
		ret = 0;
	}

	return ret;
}

static int configure(struct susensors_sensor* this, int type, int value){
	return 0;
}

int get(struct susensors_sensor* this, int type, void* data){
	int ret = 1;
	cmp_object_t* obj = (cmp_object_t*)data;

	if((enum up_parameter) type == ActualValue){
		obj->type = CMP_TYPE_UINT8;
		obj->as.u8 = (leds_get() & ((struct ledRuntime*)(this->data.runtime))->mask) > 0;
		ret = 0;
	}
	return ret;
}

/* An event was received from another device - now act on it */
static int eventHandler(struct susensors_sensor* this, int len, uint8_t* payload){
	uint8_t event;
	uint32_t parselen = len;
	cmp_object_t eventval;
	if(cp_decodeU8(payload, &event, &parselen) != 0) return 1;
	payload += parselen;
	parselen = len - parselen;
	if(cp_decodeObject(payload, &eventval, &parselen) != 0) return 2;

	if(event & AboveEventActive){
		this->value(this, setOn, NULL);
	}
	else if(event & BelowEventActive){
		this->value(this, setOff, NULL);
	}

	if(event & ChangeEventActive){
		this->value(this, setToggle, NULL);
	}

	return 0;
}

static int  setOnhandler(struct susensors_sensor* this, int len, const uint8_t* payload){
	this->value(this, setOn, NULL);
	return 0;
}
static int  setOffhandler(struct susensors_sensor* this, int len, const uint8_t* payload){
	this->value(this, setOff, NULL);
	return 0;
}
static int  setChangehandler(struct susensors_sensor* this, int len, const uint8_t* payload){
	this->value(this, setToggle, NULL);
	return 0;
}


/* Return the function to call when a specified trigger is in use */
static void* getFunctionPtr(su_led_actions trig){

	if(trig >= setOn && trig <= setToggle){
		switch(trig){
		case setOn:
			return setOnhandler;
			break;
		case setOff:
			return setOffhandler;
			break;
		case setToggle:
			return setChangehandler;
			break;
		default:
			return NULL;
		}
	}

	return NULL;
}

static int setEventhandlers(struct susensors_sensor* this, int8_t triggers[]){

	this->aboveEventhandler = getFunctionPtr(triggers[aboveEvent]);
	this->belowEventhandler = getFunctionPtr(triggers[belowEvent]);
	this->changeEventhandler = getFunctionPtr(triggers[changeEvent]);

	return 0;
}

susensors_sensor_t* addASULedIndicator(const char* name, struct resourceconf* config, struct ledRuntime* extra){
	susensors_sensor_t d;
	d.type = (char*)name;
	d.status = get;
	d.value = set;
	d.configure = configure;
	d.eventhandler = eventHandler;
	d.suconfig = suconfig;

	d.aboveEventhandler = NULL;
	d.belowEventhandler = NULL;
	d.changeEventhandler = NULL;
	d.setEventhandlers = setEventhandlers;

	d.data.config = config;
	d.data.runtime = extra;
	d.data.runtime = (void*) extra;

	config->type = (char*)name;

	return addSUDevices(&d);
}
