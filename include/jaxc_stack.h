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

#ifndef JAXC_STACK_H
#define JAXC_STACK_H

typedef struct jaxc_stack jaxc_stack_t;

jaxc_stack_t* jaxc_stack_create();

void jaxc_stack_free(jaxc_stack_t *stack);

void* jaxc_stack_pop(jaxc_stack_t *stack);

void jaxc_stack_push(jaxc_stack_t *stack, void* value);

int jaxc_stack_size(jaxc_stack_t *stack);

void* jaxc_stack_get(jaxc_stack_t *stack);

void* jaxc_stack_get_at(jaxc_stack_t *stack, int i);

#endif

