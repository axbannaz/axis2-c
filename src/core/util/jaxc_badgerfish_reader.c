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
#include <jaxc_badgerfish_reader.h>
#include <jaxc_node.h>


jaxc_badgerfish_reader_t* badgerfish_reader_create(struct json_object* obj)
{
	jaxc_node_t* temp_node;
	jaxc_badgerfish_reader_t* reader;
	reader = (jaxc_badgerfish_reader_t*)malloc(sizeof(struct jaxc_badgerfish_reader_s));

	reader->root_json_object = obj;
	reader->current_json_object = obj;
	
	reader->current_array_index = 0;
	reader->current_json_array = NULL;

	/* Stack Creation */
	reader->node_stack = jaxc_stack_create();			/*Node Stack */
	reader->attributes_stack = jaxc_stack_create();		/*Attribute Stack */

	/*Setting Initial StAX Status */
	reader->jaxc_event = JAXC_START_DOCUMENT;
	
	/*jaxc_stack_push(reader->lh_element_stack, json_object_get_object(reader->current_json_object)); */
	temp_node = jaxc_node_create(json_object_get_object(reader->current_json_object), NULL);
	jaxc_stack_push (reader->node_stack, temp_node); 

	/*Setting up the initial current element */
	jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack),
											json_object_get_object(reader->current_json_object)->head->k);

	return reader;
}

void badgerfish_reader_free(jaxc_badgerfish_reader_t* reader)
{
	/*FIX->Axis free */
	if (reader->node_stack !=NULL)
		free(reader->node_stack);
	
	/* free nodes */
	free(reader);
}


int badgerfish_reader_read(jaxc_badgerfish_reader_t* reader)
{	
	struct json_object *temp_json_obj;
	char* local_current_key = NULL;
	struct lh_table *temp_lhtable;
	jaxc_node_t* temp_node; 

	/*Process JSON Array */
	struct array_list* temp_array_list;
	int temp_array_index;

	/*Process Attribute at the same level with elements */
	char* attr_key;
	struct json_object *temp_attribute;

	if(JAXC_START_DOCUMENT == reader->jaxc_event)
	{
		reader->jaxc_event = JAXC_START_ELEMENT;
		badgerfish_reader_process_attributes(reader);
	}

	else if(JAXC_START_ELEMENT == reader->jaxc_event)
	{	
		badgerfish_reader_process_attributes(reader);
		
		if (jaxc_node_get_array_list(jaxc_stack_get(reader->node_stack)))
		{
			if ((array_list_length(jaxc_node_get_array_list(jaxc_stack_get(reader->node_stack)))
			>  jaxc_node_get_array_index(jaxc_stack_get(reader->node_stack))))
			{
				temp_array_list = jaxc_node_get_array_list(jaxc_stack_get(reader->node_stack));
				temp_array_index = jaxc_node_get_array_index(jaxc_stack_get(reader->node_stack));

				reader->current_json_object = array_list_get_idx(temp_array_list, temp_array_index);
				
				/*IF json obj is a normal json obj -- Fix for Json arrays */
				temp_node = jaxc_node_create( json_object_get_object(reader->current_json_object) , NULL);
				jaxc_node_set_array_index(jaxc_stack_get(reader->node_stack), temp_array_index + 1); 
				
				jaxc_stack_push(reader->node_stack, temp_node);
				jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack),
							json_object_get_object(reader->current_json_object)->head->k);
				
		
			}
			else
			{
				jaxc_stack_pop(reader->node_stack);
				if (jaxc_node_get_lhtable(jaxc_stack_get(reader->node_stack)))
				{
					reader->current_json_object = reader->root_json_object;

					reader->current_json_object  = 
						(struct json_object *)json_object_get_object(reader->current_json_object)->head->v;
				}
				reader->jaxc_event = JAXC_END_ELEMENT;
				
			}
		}

		/*Generic JSON Object  */
		else if (NULL != json_object_get_object(reader->current_json_object))
		{	
			temp_json_obj = (struct json_object*)json_object_get_object(reader->current_json_object)->head->v;
			if (!strcmp("$", json_object_get_object(reader->current_json_object)->head->k ))
			{
				struct json_object *v =
					(struct json_object *)json_object_get_object(reader->current_json_object)->head->v;
				reader->jaxc_event = JAXC_CHARACTER;

			
				/* Set the Current String ->iff event is CHARACTER */
				jaxc_node_set_current_string(jaxc_stack_get(reader->node_stack),
					json_object_get_string(v));
				
				return reader->jaxc_event;
			}

			
			/* Set  the current element name ->Set Top Node of the stack	 */
			jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack),
											json_object_get_object(reader->current_json_object)->head->k);
										
			/*for Normal JSON Objects */
			if (NULL != json_object_get_object(temp_json_obj))
			{
				local_current_key = json_object_get_object(temp_json_obj)->head->k;
				if (!strcmp("$", local_current_key))
				{
					struct json_object *v =
						(struct json_object *)json_object_get_object(temp_json_obj)->head->v;
					reader->jaxc_event = JAXC_CHARACTER;
						if (json_object_is_type(v, json_type_string))
					/* Set the Current String ->iff event is CHARACTER */
					jaxc_node_set_current_string(jaxc_stack_get(reader->node_stack),
						json_object_get_string(v));
					
									
					if (json_object_get_object(temp_json_obj)->head->next)
					{
						attr_key = json_object_get_object(temp_json_obj)->head->next->k;
						if ( '@' == attr_key [0] )
						{	/*Attribute Found */
							v = (struct json_object *)json_object_get_object(temp_json_obj)->head->next->v;
							temp_attribute = json_object_new_object();
							json_object_object_add(temp_attribute, attr_key, v);
							badgerfish_reader_process_attributes(reader);
						}
					}
				}

				else{
					reader->current_json_object = temp_json_obj;
					temp_node = jaxc_node_create(json_object_get_object(temp_json_obj), NULL);
					jaxc_stack_push(reader->node_stack, temp_node);
					jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack), 
													json_object_get_object(temp_json_obj)->head->k);
				}
			}

			/*For JSON Array */
			else if(json_object_is_type(temp_json_obj, json_type_array))
			{
				reader->jaxc_event = JAXC_START_ELEMENT;
				temp_node = jaxc_node_create(NULL, json_object_get_array(temp_json_obj));
				
				jaxc_stack_push(reader->node_stack, temp_node);
				reader->current_json_object = temp_json_obj;
				jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack),
											"END_ARRAY_ELEMENT");
				badgerfish_reader_read(reader);	/* Recursion ... Critical  */
	
			}
		}
	}

	else if (JAXC_END_ELEMENT == reader->jaxc_event)
	{
		jaxc_stack_pop(reader->node_stack);
		temp_node = (jaxc_node_t*)jaxc_stack_get(reader->node_stack);
		if (temp_node != NULL)
		{
			temp_lhtable = jaxc_node_get_lhtable(temp_node);
		
			if (temp_lhtable)
			{
				struct json_object *v = (struct json_object *)temp_lhtable->head->v;
				if (json_object_get_object(v))
				{
					temp_lhtable = json_object_get_object(v);
					
					if (temp_lhtable->head)
					{
						if (temp_lhtable->head->next)
						{
							temp_node = jaxc_node_create(temp_lhtable, NULL);
							jaxc_stack_push(reader->node_stack, temp_node);
							jaxc_node_set_current_element( jaxc_stack_get(reader->node_stack),
								temp_lhtable->head->next->k);

							reader->jaxc_event = JAXC_START_ELEMENT;
							/* Process Attributes for new StartElement */
							temp_lhtable->head = temp_lhtable->head->next;
						}
					}
				}
			}
			else 
			{
				reader->jaxc_event = JAXC_START_ELEMENT;
			}
		}
	}

	else if (JAXC_CHARACTER == reader->jaxc_event /*|| jaxc_stack_size(reader->node_stack) > 1*/)
	{
		reader->jaxc_event = JAXC_END_ELEMENT;	
	}
	else
		return reader->jaxc_event = JAXC_END_DOCUMENT;
	
	return reader->jaxc_event;
}


void badgerfish_reader_set_event(jaxc_badgerfish_reader_t* reader, int new_event)
{
	reader->jaxc_event = new_event;
}

int badgerfish_reader_get_event (jaxc_badgerfish_reader_t* reader)
{
	return reader->jaxc_event;
}

void badgerfish_reader_process_attributes(jaxc_badgerfish_reader_t* reader)
{
	struct lh_table *temp_lhtable;
	jaxc_node_t* temp_node;	/* JAXC NODE */
	struct json_object *val; 
	struct json_object *temp_attribute;
	char *key;
	struct lh_entry *entry;
	
	temp_node = jaxc_stack_get(reader->node_stack);
	if (!temp_node)
		return;

	temp_lhtable = jaxc_node_get_lhtable(temp_node);
	
	if (temp_lhtable)
	{
		struct json_object *v = (struct json_object *)temp_lhtable->head->v;
		temp_lhtable = json_object_get_object(v);
	}

	
	if(temp_lhtable)
	{
		for(entry = temp_lhtable->head;
		 (entry ? (key = (char*)entry->k, val = (struct json_object*)entry->v, entry) : 0); entry = entry->next) 
		{
			if ( '@' == key [0])
			{
				struct json_object* json_obj_str;

				temp_attribute = json_object_new_object();

				json_obj_str = json_object_new_string(json_object_get_string(val));
				json_object_object_add(temp_attribute, key, json_obj_str);

				jaxc_stack_push(reader->attributes_stack,temp_attribute);
				/*printf ("Attribute <pushed> %s\n", json_object_to_json_string(temp_attribute)); */
				lh_table_delete_entry(temp_lhtable, entry);
				entry = temp_lhtable->head;
			}	
		}
	}
	
}

char* badgerfish_reader_get_current_element(jaxc_badgerfish_reader_t* reader)
{
	if (NULL != jaxc_stack_get(reader->node_stack))
		return jaxc_node_get_current_element(jaxc_stack_get(reader->node_stack));
	else
		return NULL;
}

char* badgerfish_reader_get_current_string(jaxc_badgerfish_reader_t* reader)
{
	if (NULL != jaxc_stack_get(reader->node_stack))
		return jaxc_node_get_current_string(jaxc_stack_get(reader->node_stack));
	else
		return NULL;
}


jaxc_stack_t* badgerfish_reader_get_current_attributes(jaxc_badgerfish_reader_t* reader)
{
	return reader->attributes_stack;

}
