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
#ifndef JAXC_WRITER_ELEMENT_H
#define JAXC_WRITER_ELEMENT_H

#include <stdio.h>
#include <stdlib.h>

#include <axiom.h>
#include <axutil_utils.h>
#include <json-c/json.h>

/* Writer Element contains the iterator and the relevent JSON object. */
typedef struct jaxc_writer_element_s
{
	axiom_children_iterator_t *children_iter;	/*Current Iterator */
	struct json_object* json_obj;				/*Current JSON Obj */
	axiom_node_t* writer_node;

}jaxc_writer_element_t;

jaxc_writer_element_t* jaxc_writer_element_create(axiom_children_iterator_t *children_iter, axiom_node_t* writer_node, const axutil_env_t *env);
void jaxc_writer_element_set_json_object(jaxc_writer_element_t* writer_element, struct json_object* json_obj);

void jaxc_writer_element_free(jaxc_writer_element_t* writer_element, const axutil_env_t *env);

#endif
