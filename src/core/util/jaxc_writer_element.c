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

#include <jaxc_writer_element.h>

jaxc_writer_element_t* jaxc_writer_element_create(axiom_children_iterator_t *children_iter, axiom_node_t* writer_node, const axutil_env_t *env)
{
	jaxc_writer_element_t* temp_writer_element = (jaxc_writer_element_t*)AXIS2_MALLOC(env->allocator,sizeof(jaxc_writer_element_t));

	temp_writer_element->children_iter = children_iter;
	temp_writer_element->writer_node = writer_node;
	temp_writer_element->json_obj = NULL;

	return temp_writer_element;
}

void jaxc_writer_element_set_json_object(jaxc_writer_element_t* writer_element, struct json_object* json_obj)
{
	writer_element->json_obj = json_obj;
}

void jaxc_writer_element_free(jaxc_writer_element_t* writer_element, const axutil_env_t *env)
{
	AXIS2_FREE(env->allocator, writer_element);
}
