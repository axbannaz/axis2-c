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


#include <stdlib.h>
#include <string.h>

#include <jaxc_stack.h>

#define STACK_DEFAULT_CAPACITY 10
#define JAXC_TRUE 1
#define JAXC_FALSE 0

struct jaxc_stack
{
    void **data;
    /** current number of elements */
    int size;
    /** total capacity */
    int capacity;
    int is_empty_stack; /* boo */
};

jaxc_stack_t *jaxc_stack_create()
{
    jaxc_stack_t *stack = NULL;
	stack = (jaxc_stack_t*)malloc(sizeof(struct jaxc_stack));


    stack->data = NULL;
    stack->size = 0;
    stack->capacity = STACK_DEFAULT_CAPACITY;
    stack->is_empty_stack = JAXC_TRUE;

    stack->data = malloc(sizeof(void*) *STACK_DEFAULT_CAPACITY);
 
    return stack;
}

void jaxc_stack_free(jaxc_stack_t *stack)
{
   
    if (stack->data)
    {
        free(stack->data);
    }
    free(stack);
    return;
}

void* jaxc_stack_pop(jaxc_stack_t *stack)
{
    void *value = NULL;
   
    if (stack->is_empty_stack == JAXC_TRUE ||
        stack->size == 0)
    {
        return NULL;
    }
    if (stack->size > 0)
    {
        value = stack->data[stack->size -1 ];
        stack->data[stack->size-1] = NULL;
        stack->size--;
        if (stack->size == 0)
        {
            stack->is_empty_stack = JAXC_TRUE;
        }
    }
    return value;
}

void jaxc_stack_push(jaxc_stack_t *stack, void* value)
{
    
    if ((stack->size  < stack->capacity) && (stack->capacity > 0))
    {
        stack->data[stack->size++] = value;
    }
    else
    {
        void **new_data = NULL;

        int new_capacity = stack->capacity + STACK_DEFAULT_CAPACITY;

        new_data = malloc(sizeof(void*) * new_capacity);
       
        memset(new_data, 0, sizeof(void*)*new_capacity);
        memcpy(new_data, stack->data, sizeof(void*) *(stack->capacity));
        stack->capacity = new_capacity;

        free(stack->data);
        stack->data = new_data;

        stack->data[stack->size++] = value;
    }
    stack->is_empty_stack = JAXC_FALSE;
    return;
}

int jaxc_stack_size(jaxc_stack_t *stack)
{
    return stack->size;
}

void* jaxc_stack_get(jaxc_stack_t *stack)
{
	if (stack->size > 0)
    {
        return stack->data[stack->size-1];
    }
    return NULL;
}

void* jaxc_stack_get_at(jaxc_stack_t *stack, int i)
{
    if ((stack->size == 0) || (i < 0) || (i >= stack->size))
    {
        return NULL;
    }
    return stack->data[i];
}
