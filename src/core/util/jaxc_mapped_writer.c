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

#include <jaxc_mapped_writer.h>

jaxc_mapped_writer_t* jaxc_mapped_writer_create(const axutil_env_t *env)
{
	jaxc_mapped_writer_t* temp_mapped_writer = (jaxc_mapped_writer_t*)AXIS2_MALLOC(env->allocator, sizeof(jaxc_mapped_writer_t));
	temp_mapped_writer->base_node = NULL;
	temp_mapped_writer->converted_xml_string = NULL;
	temp_mapped_writer->converted_json_obj = NULL;

	temp_mapped_writer->writer_element_stack = axutil_stack_create(env);

	return temp_mapped_writer;
}

void jaxc_mapped_writer_write (jaxc_mapped_writer_t* mapped_writer,const axiom_node_t* root_node, const axutil_env_t *env)
{
	axiom_element_t* root_element;
	axiom_element_t* temp_element;
	axiom_node_t* temp_node;
	axiom_children_iterator_t* children_iter;
	jaxc_writer_element_t* temp_writer_element;
	
	/* JSON Objects */
	struct json_object* json_obj_str;
	struct json_object* json_obj_mapped_str;
	struct json_object* json_obj_base_obj;

	json_obj_base_obj = NULL; /* XAX probably bogus */
	/* Set up the root node of the writer */
	mapped_writer->base_node = ( axiom_node_t*)root_node;

	/* Get root element and the iterator for the root node */
	root_element = (axiom_element_t*) axiom_node_get_data_element(mapped_writer->base_node, env);
	children_iter = axiom_element_get_children(root_element, env, mapped_writer->base_node);

	/* Push the root element to the stack */
	/* Create an new element of jaxc_writer_element */
	temp_writer_element = jaxc_writer_element_create(children_iter, mapped_writer->base_node, env);
	temp_writer_element->json_obj = json_object_new_object();	/*empty json object instance */
	axutil_stack_push(mapped_writer->writer_element_stack, env, temp_writer_element);
	

	while (NULL != children_iter)
	{

		temp_node = axiom_children_iterator_next(children_iter, env);

		/* Processing and Finalizing the Final JSON string - @ the end of the root node-->Follows */
		if (1 == axutil_stack_size(mapped_writer->writer_element_stack, env))
		{
			/* Get peek element -> i.e. relevent to root elemented */
			temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(mapped_writer->writer_element_stack, env);
			if (temp_writer_element)
			{
				temp_element = (axiom_element_t*) axiom_node_get_data_element(temp_writer_element->writer_node, env);
				mapped_writer->converted_json_obj = json_object_new_object();
				json_object_object_add(mapped_writer->converted_json_obj, 
										axiom_element_get_localname(temp_element, env), 
										temp_writer_element->json_obj);
				/* Created JSON string */
				mapped_writer->converted_xml_string = (axis2_char_t *)json_object_to_json_string(mapped_writer->converted_json_obj);
			}

		}
		if (NULL != temp_node )
		{
			temp_element = (axiom_element_t*) axiom_node_get_data_element(temp_node, env);
			children_iter = axiom_element_get_children(temp_element, env, temp_node);

			if (axiom_node_get_node_type(temp_node, env) == AXIOM_ELEMENT)
			{
				/*Processing AXIOM Element */
				temp_writer_element = jaxc_writer_element_create(children_iter, temp_node, env);
				temp_writer_element->json_obj = json_object_new_object();
				axutil_stack_push(mapped_writer->writer_element_stack, env, temp_writer_element);
			}

			if(axiom_node_get_node_type(temp_node, env) == AXIOM_TEXT)
			{
				/* Local Varibles -> Free within this block */
				axiom_element_t* local_element;
				axiom_node_t* local_node;
				axis2_char_t* local_name;
				
				temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(mapped_writer->writer_element_stack, env);
				local_node = temp_writer_element->writer_node;
				local_element = (axiom_element_t*) axiom_node_get_data_element(local_node, env);
				
				local_name = axiom_element_get_localname(local_element, env);

				/* Forming JSON Objects */
				json_obj_str = json_object_new_string(axiom_element_get_text(local_element, env, local_node));
				json_obj_mapped_str = json_object_new_object();
				/*json_object_object_add(json_obj_mapped_str ,"$" , json_obj_str);  */
				
				/*------ Mapped Convention does not have $ format ----- */
				json_obj_mapped_str = json_obj_str;
				/*------ End Mapped Convention ------------------------ */
				
				/* --- End Forming JSON objects */


				/* Popoing the writer_element from the stack */
				axutil_stack_pop(mapped_writer->writer_element_stack, env);

				/* Update the PEEK element */
				temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(mapped_writer->writer_element_stack, env);
				
				if (NULL != temp_writer_element)
				{
					local_node = temp_writer_element->writer_node;
					children_iter = temp_writer_element->children_iter;
					if (NULL == temp_writer_element->json_obj)
						temp_writer_element->json_obj = json_object_new_object();
					json_object_object_add(temp_writer_element->json_obj, local_name, json_obj_mapped_str);
					
				}
				
				if (!axiom_children_iterator_has_next(children_iter, env))
				{
					/* If no non-visited elements in the relevent breanch --> Follows */

					if (temp_writer_element)
					{
						temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(mapped_writer->writer_element_stack, env);
						local_node = temp_writer_element->writer_node;
						local_element = (axiom_element_t*) axiom_node_get_data_element(local_node, env);
						local_name = axiom_element_get_localname(local_element, env);
						json_obj_base_obj = temp_writer_element->json_obj;
					}

					axutil_stack_pop(mapped_writer->writer_element_stack, env);
					temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(mapped_writer->writer_element_stack, env);
					
					if (temp_writer_element)
					{
						if (temp_writer_element)
						{
							json_object_object_add(temp_writer_element->json_obj, local_name,json_obj_base_obj);
						}
						children_iter = temp_writer_element->children_iter;
					}
				}
			}
		}
		else
		{
			children_iter = NULL;
		}
	}	
}


void jaxc_mapped_writer_free(jaxc_mapped_writer_t* mapped_writer, const axutil_env_t *env)
{
	AXIS2_FREE(env->allocator, mapped_writer);
}

axis2_char_t* jaxc_mapped_writer_get_converted_xml_str (jaxc_mapped_writer_t* mapped_writer, const axutil_env_t *env)
{
	return mapped_writer->converted_xml_string;
}
