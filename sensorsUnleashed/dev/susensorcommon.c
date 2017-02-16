/*
 * susensorcommon.c
 *
 *  Created on: 25/12/2016
 *      Author: omn
 */
#include "susensorcommon.h"

int suconfig(struct susensors_sensor* this, int type, void* data){

	int ret = 1;
	struct resourceconf* config = (struct resourceconf*)this->data.config;
	enum susensors_configcmd cmd = (enum susensors_configcmd)type;
	if(cmd == SUSENSORS_EVENTSETUP_SET){

		/* The eventsetup is received as a fixed list of cmp_objects:
		 * 1. AboveEventAt
		 * 2. BelowEventAt
		 * 3. ChangeEvent
		 * 4. eventsActive
		 * */

		/* Returns
		 *  0: Success
		 *  1: AboveEventAt was not right
		 *  2: BelowEventAt was not right
		 *	3: ChangeEvent was not right
		 *	4: eventsActive was not right
		 * */
		uint8_t* payload = (uint8_t*)data;
		uint32_t bufindex;
		int len = 0;

		cmp_object_t newval;
		/* Read the AboveEventAt object */
		if(cp_decodeObject((uint8_t*)payload + len, &newval, &bufindex) == 0){
			len += bufindex;
			if(newval.type == config->AboveEventAt.type){
				config->AboveEventAt = newval;
			}
			else{
				return 1;
			}
		}
		else{
			return 1;
		}

		/* Read the BelowEventAt object */
		if(cp_decodeObject((uint8_t*)payload + len, &newval, &bufindex) == 0){
			len += bufindex;
			if(newval.type == config->BelowEventAt.type){
				config->BelowEventAt = newval;
			}
			else{
				return 2;
			}
		}
		else{
			return 2;
		}
		/* Read the ChangeEvent object */
		if(cp_decodeObject((uint8_t*)payload + len, &newval, &bufindex) == 0){
			len += bufindex;
			if(newval.type == config->ChangeEvent.type){
				config->ChangeEvent = newval;
			}
			else{
				return 3;
			}
		}
		else{
			return 3;
		}
		/* Read the eventsActive object */
		if(cp_decodeObject((uint8_t*)payload + len, &newval, &bufindex) == 0){
			len += bufindex;
			if(newval.type == CMP_TYPE_UINT8){
				config->eventsActive = newval.as.u8;
			}
			else{
				return 4;
			}
		}
		else{
			return 4;
		}
		return 0;
	}
	else if(cmd == SUSENSORS_EVENTSETUP_GET){
		uint8_t* bufptr = (uint8_t*)data;
		cmp_object_t eventsActive;
		eventsActive.type = CMP_TYPE_UINT8;
		eventsActive.as.u8 = config->eventsActive;

		bufptr += cp_encodeObject(bufptr, &config->AboveEventAt);
		bufptr += cp_encodeObject(bufptr, &config->BelowEventAt);
		bufptr += cp_encodeObject(bufptr, &config->ChangeEvent);
		bufptr += cp_encodeObject(bufptr, &eventsActive);

		ret = (uint8_t*)data - bufptr;
	}
	else if(cmd == SUSENSORS_CEVENT_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		*obj = config->ChangeEvent;
		ret = 0;
	}
	else if(cmd == SUSENSORS_AEVENT_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		*obj = config->AboveEventAt;
		ret = 0;
	}
	else if(cmd == SUSENSORS_BEVENT_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		*obj = config->BelowEventAt;
		ret = 0;
	}
	else if(cmd == SUSENSORS_RANGEMAX_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		*obj = config->RangeMax;
		ret = 0;
	}
	else if(cmd == SUSENSORS_RANGEMIN_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		*obj = config->RangeMin;
		ret = 0;
	}
	else if(cmd == SUSENSORS_EVENTSTATE_GET){
		cmp_object_t* obj = (cmp_object_t*)data;
		obj->type = CMP_TYPE_UINT8;
		obj->as.u8 = config->eventsActive;
		ret = 0;
	}
	return ret;
}

int getActiveEventMsg(struct susensors_sensor* this, uint8_t* payload){
	struct relayRuntime* d = (struct relayRuntime*)this->data.runtime;
	int len = cp_encodeU8(payload, d->hasEvent);
	len += cp_encodeObject(payload + len, &d->LastEventValue);
	return len;
}

/**
 *
 * @param this
 * 	The actual sensor instance
 * @param dir
 * 	The direction of the last value
 * @param step
 * 	The step from from last time the actuator was set
 * @return
 */
void setEventU8(struct susensors_sensor* this, int dir, uint8_t step){
	struct resourceconf* c = (struct resourceconf*)(this->data.config);
	struct relayRuntime* r = (struct relayRuntime*)(this->data.runtime);

	r->hasEvent = NoEventActive;
	if(dir < 0){
		if(r->LastValue.as.u8 <= c->BelowEventAt.as.u8 && (c->eventsActive & BelowEventActive)){
			r->hasEvent = BelowEventActive;
		}
	}
	else{
		if(r->LastValue.as.u8 >= c->AboveEventAt.as.u8 && (c->eventsActive & AboveEventActive)){
			r->hasEvent = AboveEventActive;
		}
	}

	r->ChangeEventAcc.as.u8 += step;
	if(c->ChangeEvent.as.u8 <= r->ChangeEventAcc.as.u8){
		r->ChangeEventAcc.as.u8 = 0;
		if(c->eventsActive & ChangeEventActive){
			r->hasEvent |= ChangeEventActive;
		}
	}

	if(r->hasEvent != NoEventActive){
		r->LastEventValue = r->LastValue;
		susensors_changed(this);
	}
}

void setEventU16(struct susensors_sensor* this, int dir, uint8_t step){
	struct resourceconf* c = (struct resourceconf*)(this->data.config);
	struct relayRuntime* r = (struct relayRuntime*)(this->data.runtime);

	r->hasEvent = NoEventActive;
	if(dir < 0){
		if(r->LastValue.as.u16 <= c->BelowEventAt.as.u16 && (c->eventsActive & BelowEventActive)){
			r->hasEvent = BelowEventActive;
		}
	}
	else{
		if(r->LastValue.as.u16 >= c->AboveEventAt.as.u16 && (c->eventsActive & AboveEventActive)){
			r->hasEvent = AboveEventActive;
		}
	}

	r->ChangeEventAcc.as.u16 += step;
	if(c->ChangeEvent.as.u16 <= r->ChangeEventAcc.as.u16){
		r->ChangeEventAcc.as.u16 = 0;
		if(c->eventsActive & ChangeEventActive){
			r->hasEvent |= ChangeEventActive;
		}
	}

	if(r->hasEvent != NoEventActive){
		r->LastEventValue = r->LastValue;
		susensors_changed(this);
	}
}
