/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 */

/**
 * \file
 *      Erbium (Er) REST Engine example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"

#include "dev/button-sensor.h"
#include "coap_proxy.h"

/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t res_toggle;

PROCESS(er_uart_server, "Erbium Uart Server");
AUTOSTART_PROCESSES(&er_uart_server);

PROCESS_THREAD(er_uart_server, ev, data)
{
	PROCESS_BEGIN();

	PROCESS_PAUSE();

	/* Initialize the REST engine. */
	rest_init_engine();

	proxy_init();
	/*
	 * Bind the resources to their Uri-Path.
	 * WARNING: Activating twice only means alternate path, not two instances!
	 * All static variables are the same for each URI path.
	 */
	rest_activate_resource(&res_toggle, "actuators/toggle");




//	char count;
//	if(req_received(resource_count, &count) == 0){
//		for(int i=0; i<count; i++){
//			req_resource(resource_url, (void*)i, 1);
//			PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
//		}
//	}



//	req_resource(resource_count);
//	PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
//	req_resource(resource_count);
//	PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);

	//req_resource(resource_count);

	/* Define application-specific events here. */
	while(1) {
		PROCESS_WAIT_EVENT();
	}                             /* while (1) */

	PROCESS_END();
}
