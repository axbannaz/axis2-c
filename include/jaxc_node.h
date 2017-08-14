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
#ifndef JAXC_NODE_H
#define JAXC_NODE_H

typedef struct jaxc_node jaxc_node_t;

jaxc_node_t* jaxc_node_create(struct lh_table *tmp_lhtable, struct array_list* tmp_json_array_list);

struct lh_table* jaxc_node_get_lhtable (jaxc_node_t *node);
struct array_list* jaxc_node_get_array_list (jaxc_node_t *node);

int jaxc_node_get_array_index(jaxc_node_t *node);
void jaxc_node_set_array_index(jaxc_node_t *node, int value);

void jaxc_node_free(jaxc_node_t *node);

/* Current Element */
void jaxc_node_set_current_element(jaxc_node_t *node, char* element_name);
char* jaxc_node_get_current_element(jaxc_node_t *node);

/* Current String if jaxc event is CHARACTER */
void jaxc_node_set_current_string (jaxc_node_t *node, const char* current_string);
char* jaxc_node_get_current_string (jaxc_node_t *node);







#endif
