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

#include <jaxc_badgerfish_writer.h>

jaxc_badgerfish_writer_t* jaxc_badgerfish_writer_create(const axutil_env_t *env)
{
	jaxc_badgerfish_writer_t* temp_badgerfish_writer = (jaxc_badgerfish_writer_t*)AXIS2_MALLOC(env->allocator, sizeof(jaxc_badgerfish_writer_t));
	temp_badgerfish_writer->base_node = NULL;
	temp_badgerfish_writer->converted_xml_string = NULL;
	temp_badgerfish_writer->converted_json_obj = NULL;

	temp_badgerfish_writer->writer_element_stack = axutil_stack_create(env);

	return temp_badgerfish_writer;
}

void jaxc_badgerfish_writer_write (jaxc_badgerfish_writer_t* badgerfish_writer, axiom_node_t* root_node, const axutil_env_t *env)
{
	axiom_element_t* root_element;
	axiom_element_t* temp_element;
	axiom_node_t* temp_node;
	axiom_children_iterator_t* children_iter;
	jaxc_writer_element_t* temp_writer_element;
	
	/* JSON Objects */
	struct json_object* json_obj_str;
	struct json_object* json_obj_badgerfish_str;
	struct json_object* json_obj_local_obj;
	struct json_object* json_obj_base_obj;
	int initial_iter_flag;

json_obj_base_obj = NULL; /* XAX this is probably bogus. (gcc was complaining about not initialized - looks like that's probably true) */
	/* Set up the root node of the writer */
	badgerfish_writer->base_node = root_node;

	/* Get root element and the iterator for the root node */
	root_element = axiom_node_get_data_element(badgerfish_writer->base_node, env);
	children_iter = axiom_element_get_children(root_element, env, badgerfish_writer->base_node);

	/* Push the root element to the stack */
	/* Create an new element of jaxc_writer_element */
	temp_writer_element = jaxc_writer_element_create(children_iter, root_node, env);
	temp_writer_element->json_obj = json_object_new_object();	/*empty json object instance */
	axutil_stack_push(badgerfish_writer->writer_element_stack, env, temp_writer_element);
	
	initial_iter_flag = 0;	 /* This flag is used to specifically identify the very first node */

	while (NULL != children_iter)
	{
		temp_node = axiom_children_iterator_next(children_iter, env);
	
		/* Processing and Finalizing the Final JSON string - @ the end of the root node-->Follows */
		if ((1 == axutil_stack_size(badgerfish_writer->writer_element_stack, env)))
		{
			temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(badgerfish_writer->writer_element_stack, env);
			if (temp_writer_element)
			{
				temp_element = (axiom_element_t*) axiom_node_get_data_element(temp_writer_element->writer_node, env);
				badgerfish_writer->converted_json_obj = json_object_new_object();
				
				if(0 == initial_iter_flag)
				{
					/* Initial JSON Object set up */
					struct json_object* json_str_obj;
					struct json_object* json_bf_str_obj;


					if (axiom_element_get_text(temp_element, env, temp_writer_element->writer_node))
					{					
						json_str_obj = json_object_new_string(axiom_element_get_text(temp_element, env, temp_writer_element->writer_node));
						json_bf_str_obj = json_object_new_object();
						json_object_object_add(json_bf_str_obj ,"$" , json_str_obj);
						json_object_object_add(badgerfish_writer->converted_json_obj, 
											axiom_element_get_localname(temp_element, env), 
											json_bf_str_obj);
					}
					else 
					{
						json_object_object_add(badgerfish_writer->converted_json_obj, 
											axiom_element_get_localname(temp_element, env), 
											temp_writer_element->json_obj);

					}

					initial_iter_flag = 1;

				}
				else {
					json_object_object_add(badgerfish_writer->converted_json_obj, 
										axiom_element_get_localname(temp_element, env), 
									temp_writer_element->json_obj);
								
				}
				
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

				axutil_stack_push(badgerfish_writer->writer_element_stack, env, temp_writer_element);

				

			}


			if(axiom_node_get_node_type(temp_node, env) == AXIOM_TEXT)
			{
				/* Local Varibles -> Free within this block */
				axiom_element_t* local_element;
				axiom_node_t* local_node;
				axis2_char_t* local_name;

				/*Attribute Processing */
				axutil_hash_t* temp_hash;
				axiom_attribute_t* temp_attr = NULL;
				struct json_object* attr_obj = NULL;
				
				temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(badgerfish_writer->writer_element_stack, env);
				local_node = temp_writer_element->writer_node;
				local_element = (axiom_element_t*) axiom_node_get_data_element(local_node, env);
				
				local_name = axiom_element_get_localname(local_element, env);

				/* Forming JSON Objects */
				json_obj_str = json_object_new_string(axiom_element_get_text(local_element, env, local_node));
				json_obj_badgerfish_str = json_object_new_object();
				json_object_object_add(json_obj_badgerfish_str ,"$" , json_obj_str); 


				/* Attr proc--------------------------------------------------------------- */
				temp_hash = axiom_element_get_all_attributes(local_element, env);
				if (NULL != temp_hash)
				{
					axutil_hash_index_t *hi = NULL;
					void *val = NULL;	/*Attribute value */
					size_t attr_name_len = 0;

					for (hi = axutil_hash_first(temp_hash, env); hi; hi = axutil_hash_next(env, hi))
					{
						axutil_hash_this(hi, NULL, NULL, &val);
						if (val)
						{
							temp_attr = (axiom_attribute_t*)val;

							attr_obj = json_object_new_string(axiom_attribute_get_value(temp_attr, env));
							
							attr_name_len = strlen(axiom_attribute_get_localname(temp_attr, env));
							if (axiom_attribute_get_localname(temp_attr, env))
							{
								char* s = (char*) calloc((attr_name_len+1), sizeof(char));
								strcat(s,"@");
								strcat(s, axiom_attribute_get_localname(temp_attr, env));
								json_object_object_add(json_obj_badgerfish_str ,s , attr_obj);
							}	
						}
					}
				}
				/* End Attr proc ------------------------------------------------------------------ */
				
				json_obj_local_obj = json_object_new_object();
				

				/* --- End Forming JSON objects */
				/* Popoing the writer_element from the stack */
				axutil_stack_pop(badgerfish_writer->writer_element_stack, env);

				/* Update the PEEK element */
				temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(badgerfish_writer->writer_element_stack, env);
				
				if (NULL != temp_writer_element)
				{
					local_node = temp_writer_element->writer_node;
					children_iter = temp_writer_element->children_iter;
					if (NULL == temp_writer_element->json_obj)
						temp_writer_element->json_obj = json_object_new_object();
					json_object_object_add(temp_writer_element->json_obj, local_name, json_obj_badgerfish_str);
					
				}
				
				if (!axiom_children_iterator_has_next(children_iter, env))
				{
					/* If no non-visited elements in the relevent breanch --> Follows */
					if (temp_writer_element)
					{
						temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(badgerfish_writer->writer_element_stack, env);
						local_node = temp_writer_element->writer_node;
						local_element = (axiom_element_t*) axiom_node_get_data_element(local_node, env);
						local_name = axiom_element_get_localname(local_element, env);
						json_obj_base_obj = temp_writer_element->json_obj;
					}

					axutil_stack_pop(badgerfish_writer->writer_element_stack, env);
					temp_writer_element = (jaxc_writer_element_t*)axutil_stack_get(badgerfish_writer->writer_element_stack, env);
					
					if (temp_writer_element)
					{
						if (temp_writer_element)
						{	
							/*Attr Proc */
							axutil_hash_t* temp_hash;
							axiom_attribute_t* temp_attr = NULL;
							struct json_object* attr_obj = NULL;
														
							temp_hash = axiom_element_get_all_attributes(local_element, env);
						
							if (NULL != temp_hash)
							{
								axutil_hash_index_t *hi = NULL;
								void *val = NULL;	/*Attribute value */
								size_t attr_name_len = 0;


								for (hi = axutil_hash_first(temp_hash, env); hi; hi = axutil_hash_next(env, hi))
								{
									axutil_hash_this(hi, NULL, NULL, &val);
									if (val)
									{
										temp_attr = (axiom_attribute_t*)val;
										attr_obj = json_object_new_string(axiom_attribute_get_value(temp_attr, env));
										
										attr_name_len = strlen(axiom_attribute_get_localname(temp_attr, env));
										if (axiom_attribute_get_localname(temp_attr, env))
										{
											char* s = (char*) calloc((attr_name_len+1), sizeof(char));
											strcat(s,"@");
											strcat(s, axiom_attribute_get_localname(temp_attr, env));
											json_object_object_add(json_obj_base_obj ,s , attr_obj);
										}	
									}
								}
							}
							json_object_object_add(temp_writer_element->json_obj, local_name,json_obj_base_obj);
							/*END Attribute Processing----------- */
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

	/* END All.. */
	/* Convert the Created JSON Object to JSON String  */
	badgerfish_writer->converted_xml_string = json_object_to_json_string(badgerfish_writer->converted_json_obj);
	/*badgerfish_writer->converted_xml_string ="asd"; */
}


void jaxc_badgerfish_writer_free(jaxc_badgerfish_writer_t* badgerfish_writer, const axutil_env_t *env)
{	
	AXIS2_FREE(env->allocator, badgerfish_writer);
}

/* This Method returns the converted xml string in the form of a JSON string */
axis2_char_t* jaxc_badgerfish_writer_get_converted_xml_str (jaxc_badgerfish_writer_t* badgerfish_writer, const axutil_env_t *env)
{
	return badgerfish_writer->converted_xml_string;
}
