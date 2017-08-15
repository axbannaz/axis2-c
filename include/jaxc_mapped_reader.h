/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef JAXC_MAPPED_READER
#define JAXC_MAPPED_READER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jaxc_stack.h>
#include <jaxc_badgerfish_reader.h>

typedef struct jaxc_mapped_reader_s
{
	struct json_object* root_json_object;
	struct json_object* current_json_object;
	struct array_list* current_json_array;	/* ArrayList to store the current JSON array */
	jaxc_stack_t* attributes_stack;	/* Store attributes */
	jaxc_stack_t* namespace_stack;	/* Store namespaces */
	jaxc_stack_t* node_stack;		/* Store the Nodes <lhtable | arraylist> */

	int current_array_index;				/* Store the current index of the array (if exsists) */
	enum jaxc_stax_event_types jaxc_event;
	
}jaxc_mapped_reader_t;


/*Create Reader */
jaxc_mapped_reader_t* mapped_reader_create(struct json_object*);

/*Free Reader */
void mapped_reader_free(jaxc_mapped_reader_t*);

/*Functions */
struct json_object* mapped_reader_get_staxpointer(jaxc_mapped_reader_t*);
struct json_object* mapped_reader_get_localpointer(jaxc_mapped_reader_t*);

int mapped_reader_read(jaxc_mapped_reader_t* reader);
void mapped_reader_set_event(jaxc_mapped_reader_t* reader, int new_event);
int mapped_reader_get_event (jaxc_mapped_reader_t* reader);

void mapped_reader_process_attributes(jaxc_mapped_reader_t* reader);

char* mapped_reader_get_current_element(jaxc_mapped_reader_t* reader);
char* mapped_reader_get_current_string (jaxc_mapped_reader_t* reader);

jaxc_stack_t* mapped_reader_get_current_attributes(jaxc_mapped_reader_t* reader); 



#endif
