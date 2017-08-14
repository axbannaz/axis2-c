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

#ifndef JAXC_JSON_READER_H
#define JAXC_JSON_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_stack.h>
#include <axutil_utils.h>
#include <axiom.h>

#include <json-c/json.h>
#include <jaxc.h>
#include <jaxc_badgerfish_reader.h>
#include <jaxc_node.h>

#ifdef WIN32
#define AXIS2_EXTERN __declspec(dllexport)
#endif

typedef struct jaxc_json_reader_s
{
	axiom_xml_writer_t* axiom_xml_serializer;
	char* current_xml_element;
	char* converted_xml_buf;
	struct json_object *current_json_obj;
	struct json_object *val; 
	jaxc_t* parser;
	axutil_stack_t* stack;
	
}jaxc_json_reader_t;

/*Create Reader */
/* init_param : Convention */
AXIS2_EXTERN jaxc_json_reader_t* AXIS2_CALL json_reader_create(const char* current_json_str, int init_param, const axutil_env_t *env);


/*Free Reader */
AXIS2_EXTERN void AXIS2_CALL json_reader_free(jaxc_json_reader_t* obj, const axutil_env_t *env);

/* Read Complete JSON String */
AXIS2_EXTERN void AXIS2_CALL json_reader_read(jaxc_json_reader_t* obj, const axutil_env_t *env);

AXIS2_EXTERN char* AXIS2_CALL json_reader_get_converted_xml_str(jaxc_json_reader_t* obj, const axutil_env_t *env);


AXIS2_EXTERN void AXIS2_CALL json_reader_badgerfish_read(jaxc_json_reader_t* json_reader, const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL json_reader_mapped_read(jaxc_json_reader_t* json_reader, const axutil_env_t *env);

#endif
