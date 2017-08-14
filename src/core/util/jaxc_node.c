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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include <jaxc_node.h>
#include <json-c/arraylist.h>



struct jaxc_node
{
	struct lh_table *lhtable;
	struct array_list* json_array_list;
	int current_array_index;
	char* current_element;
	char* current_string;

};

jaxc_node_t* jaxc_node_create(struct lh_table *tmp_lhtable, struct array_list* tmp_json_array_list)
{
	jaxc_node_t *node = NULL;
	node = (jaxc_node_t*)malloc (sizeof (struct jaxc_node));
	
	node->lhtable = tmp_lhtable;
	node->json_array_list = tmp_json_array_list;
	node->current_array_index = 0;

	node->current_element = "NOT_SET";
	node->current_string = "NOT_SET";

	return node;
}


struct lh_table* jaxc_node_get_lhtable (jaxc_node_t *node)
{
	return node->lhtable;
}

struct array_list* jaxc_node_get_array_list (jaxc_node_t *node)
{
	return node->json_array_list;
}

int jaxc_node_get_array_index(jaxc_node_t *node)
{
	return node->current_array_index;
}

void jaxc_node_set_array_index(jaxc_node_t *node, int value)
{
	node->current_array_index = value;
}


/* Current Element */
void jaxc_node_set_current_element(jaxc_node_t *node, char* element_name)
{
	node->current_element = (char*) malloc( strlen(element_name) + 1 );
	strcpy(node->current_element, element_name);
}


char* jaxc_node_get_current_element(jaxc_node_t *node)
{
	return node->current_element;
}

/* Current String */
void jaxc_node_set_current_string (jaxc_node_t *node, const char* custom_str)
{
	node->current_string = (char*) malloc (strlen(custom_str) + 1);
	strcpy(node->current_string, custom_str); 
}
char* jaxc_node_get_current_string (jaxc_node_t *node)
{
	return node->current_string;
}


/* Memory Deallocation */
void jaxc_node_free(jaxc_node_t *node)
{
	free(node);
}
