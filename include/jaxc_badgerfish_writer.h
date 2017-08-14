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

#ifndef JAXC_BADGERFISH_WRITER
#define JAXC_BADGERFISH_WRITER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <axutil_utils.h>
#include <axiom.h>
#include <axutil_stack.h>

#include <json-c/json.h>
#include <jaxc_writer_element.h>

typedef struct jaxc_badgerfish_writer_s
{
	struct json_object* converted_json_obj;
	axis2_char_t* converted_xml_string;
	axiom_node_t* base_node;
	axutil_stack_t* writer_element_stack;

}jaxc_badgerfish_writer_t;

jaxc_badgerfish_writer_t* jaxc_badgerfish_writer_create(const axutil_env_t *env);
void jaxc_badgerfish_writer_write (jaxc_badgerfish_writer_t* badgerfish_writer, axiom_node_t* root_node, const axutil_env_t *env);
void jaxc_badgerfish_writer_free(jaxc_badgerfish_writer_t* badgerfish_writer, const axutil_env_t *env);

/* This Method returns the converted xml string in the form of a JSON string */
axis2_char_t* jaxc_badgerfish_writer_get_converted_xml_str (jaxc_badgerfish_writer_t* badgerfish_writer, const axutil_env_t *env);

#endif
