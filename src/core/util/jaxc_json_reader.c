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

#include <jaxc_json_reader.h>


AXIS2_EXTERN jaxc_json_reader_t* AXIS2_CALL json_reader_create(const char* current_json_str, int init_param, const axutil_env_t *env)
{
	jaxc_json_reader_t* temp_json_reader;
	temp_json_reader = (jaxc_json_reader_t*) malloc(sizeof(jaxc_json_reader_t));
	
	/*temp_json_reader->guththila_xml_serializer = guththila_create_xml_stream_writer_for_memory(env); */
	temp_json_reader->axiom_xml_serializer = axiom_xml_writer_create_for_memory(env, NULL, 0, 0, 0);

	temp_json_reader->parser = jaxc_create();
	temp_json_reader->current_json_obj = json_tokener_parse(current_json_str);
	jaxc_initiate_reader(temp_json_reader->parser, init_param, temp_json_reader->current_json_obj);

	return temp_json_reader;
}

AXIS2_EXTERN void AXIS2_CALL json_reader_free(jaxc_json_reader_t* json_reader, const axutil_env_t *env)
{
	if (json_reader->current_xml_element)
		free (json_reader->current_xml_element);
	if (json_reader->converted_xml_buf)
		free (json_reader->converted_xml_buf);
	
	if (json_reader->axiom_xml_serializer)
		axiom_xml_writer_free(json_reader->axiom_xml_serializer, env);

	if (json_reader->current_json_obj)
		json_object_put(json_reader->current_json_obj);

	if (json_reader->parser)
		free (json_reader->parser);

	if (json_reader)
		free (json_reader);

}

AXIS2_EXTERN void AXIS2_CALL json_reader_read(jaxc_json_reader_t* json_reader, const axutil_env_t *env)
{
	if (0 == jaxc_get_reader_type(json_reader->parser))
	{
		/* Badgerfish Reader	 */
		json_reader_badgerfish_read(json_reader, env);
	}
	else if (1 == jaxc_get_reader_type(json_reader->parser))
	{
		/* Mapped Reader */
		json_reader_mapped_read(json_reader, env);
	}
}

AXIS2_EXTERN char* AXIS2_CALL json_reader_get_converted_xml_str(jaxc_json_reader_t* obj, const axutil_env_t *env)
{
	return obj->converted_xml_buf;
}

AXIS2_EXTERN void AXIS2_CALL json_reader_badgerfish_read(jaxc_json_reader_t* json_reader, const axutil_env_t *env)
{
	int parser_state;
	/* This is for Badgerfish convention */

	for (;;)
	{
		parser_state = jaxc_next(json_reader->parser);
		
		
		if (NULL == badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser)))
		{
			break;
		}

		
		if (JAXC_START_ELEMENT == parser_state)
		{	
			if (0 == strcmp("$", badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
			{

				while( 0 !=strcmp(badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser)), 
					json_reader->current_xml_element))
				{
					parser_state = jaxc_next(json_reader->parser);
					if (JAXC_CHARACTER == parser_state)
					{
						axiom_xml_writer_write_characters(json_reader->axiom_xml_serializer, env,
							badgerfish_reader_get_current_string(jaxc_get_reader(json_reader->parser)));
					}
					else if (parser_state == JAXC_END_ELEMENT)
					{
						
						if (0 != strcmp(json_reader->current_xml_element, badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
						{
							axiom_xml_writer_write_end_element(json_reader->axiom_xml_serializer, env);
						}

					
					}

					else if (parser_state == JAXC_START_ELEMENT)
					{
						jaxc_stack_t* temp_stack;
						struct json_object* temp_obj;
						char* attr_name; 
						char* attr_val;
						struct lh_table *temp_lhtable;

						if (0 == strcmp("$", badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
						{
							axiom_xml_writer_write_start_element(json_reader->axiom_xml_serializer,env,
								json_reader->current_xml_element);

							while (0 != jaxc_stack_size(badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
							{
								temp_stack = badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
								temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
								
								/* Remove @ sign from the JSON attribute  */
								temp_obj = json_object_new_object();
								json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
								
								attr_name = temp_lhtable->head->k;
								attr_name = strtok(attr_name, "@");
								attr_val = json_object_get_string(temp_lhtable->head->v);
										
								axiom_xml_writer_write_attribute(json_reader->axiom_xml_serializer, 
									env, attr_name, attr_val);

								jaxc_stack_pop(temp_stack);
							}
						}	
						/* */
					}
				}/* End While	 */
			}

			else
			{
				jaxc_stack_t* temp_stack;
				struct json_object* temp_obj;
				char* attr_name; 
				char* attr_val;
				struct lh_table *temp_lhtable;

				axiom_xml_writer_write_start_element(json_reader->axiom_xml_serializer,env,
								badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser)));

				/* Attr--- */
				while (0 != jaxc_stack_size(badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
				{
					temp_stack = badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
					temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
					
					/* Remove @ sign from the JSON attribute  */
					temp_obj = json_object_new_object();
					json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
					
					attr_name = temp_lhtable->head->k;
					attr_name = strtok(attr_name, "@");
					attr_val = json_object_get_string(temp_lhtable->head->v);
				

					axiom_xml_writer_write_attribute(json_reader->axiom_xml_serializer, 
									env, attr_name, attr_val);
				
					jaxc_stack_pop(temp_stack);
				}
		
		
				/* Attrt test end */

				json_reader->current_xml_element = badgerfish_reader_get_current_element(jaxc_get_reader(json_reader->parser));
			}
		}
		
		else if (JAXC_CHARACTER == parser_state)
		{
			jaxc_stack_t* temp_stack;
			struct json_object* temp_obj;
			char* attr_name; 
			char* attr_val;
			struct lh_table *temp_lhtable;

			while (0 != jaxc_stack_size(badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
			{
				temp_stack = badgerfish_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
				temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
				
				/* Remove @ sign from the JSON attribute  */
				temp_obj = json_object_new_object();
				json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
				
				attr_name = temp_lhtable->head->k;
				attr_name = strtok(attr_name, "@");
				attr_val = json_object_get_string(temp_lhtable->head->v);
			
				
				axiom_xml_writer_write_attribute(json_reader->axiom_xml_serializer, 
								env, attr_name, attr_val);

				jaxc_stack_pop(temp_stack);
			}

			axiom_xml_writer_write_characters(json_reader->axiom_xml_serializer, env,
							badgerfish_reader_get_current_string(jaxc_get_reader(json_reader->parser)));

		}

		else if (JAXC_END_ELEMENT == parser_state)
		{
			axiom_xml_writer_write_end_element(json_reader->axiom_xml_serializer, env);
		}

	}

	axiom_xml_writer_write_end_document(json_reader->axiom_xml_serializer, env);

	json_reader->converted_xml_buf = (char*)axiom_xml_writer_get_xml(json_reader->axiom_xml_serializer, env);

}

AXIS2_EXTERN void AXIS2_CALL json_reader_mapped_read(jaxc_json_reader_t* json_reader, const axutil_env_t *env)
{
/*
XXX argh!  why is *EVERY* single line it's own comment?!?

	// -------------------------------
	// *******************************
	// *** IMPORTANT ***
	// This version of Mapped Reader Contains few bugs
	// 

	//int parser_state;
	//// This is for mapped convention
	//guththila_write_start_document (json_reader->guththila_xml_serializer, env);

	//for (;;)
	//{
	//	parser_state = jaxc_next(json_reader->parser);
	//	
	//	if ( NULL != mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser)))
	//		printf ("\t\tCURRENT ELEMENT : %s\n", mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser)));
	//
	//	if (NULL == mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser)))
	//	{
	//		// TEST - continue;
	//		break;
	//	}

	//	if (JAXC_START_DOCUMENT == parser_state)
	//	{
	//		printf ("\t\tJAXC_START_DOCUMENT\n");
	//		guththila_write_start_document (json_reader->guththila_xml_serializer, env);
	//		
	//	}
	//	
	//	if (JAXC_START_ELEMENT == parser_state)
	//	{	printf ("\t\tJAXC_START_ELEMENT\n");

	//		if (0 == strcmp("$", mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
	//		{
	//			printf ("--------------- %s -----------------------", json_reader->current_xml_element);
	//			// Found JSON Array

	//			while( 0 !=strcmp(mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser)), 
	//				json_reader->current_xml_element))
	//			{
	//				parser_state = jaxc_next(json_reader->parser);
	//				if (JAXC_CHARACTER == parser_state)
	//				{
	//					guththila_write_characters (json_reader->guththila_xml_serializer, 
	//					mapped_reader_get_current_string(jaxc_get_reader(json_reader->parser)),
	//					env);
	//				}
	//				else if (parser_state == JAXC_END_ELEMENT)
	//				{
	//					
	//					if (0 != strcmp(json_reader->current_xml_element, mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
	//					{
	//						guththila_write_end_element(json_reader->guththila_xml_serializer, env);
	//					}

	//				
	//				}

	//				else if (parser_state == JAXC_START_ELEMENT)
	//				{
	//					jaxc_stack_t* temp_stack;
	//					struct json_object* temp_obj;
	//					char* attr_name; 
	//					char* attr_val;
	//					struct lh_table *temp_lhtable;

	//					if (0 == strcmp("$", mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser))))
	//					{
	//						guththila_write_start_element ( json_reader->guththila_xml_serializer,
	//							json_reader->current_xml_element, 
	//							env);

	//						while (0 != jaxc_stack_size(mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
	//						{
	//							temp_stack = mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
	//							temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
	//							
	//							// Remove @ sign from the JSON attribute 
	//							temp_obj = json_object_new_object();
	//							json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
	//							
	//							attr_name = temp_lhtable->head->k;
	//							attr_name = strtok(attr_name, "@");
	//							attr_val = json_object_get_string(temp_lhtable->head->v);
	//						
	//							printf ("Attribute writing @ %s\n", attr_name);
	//							guththila_write_attribute (json_reader->guththila_xml_serializer
	//														, attr_name, attr_val, env);
	//							jaxc_stack_pop(temp_stack);
	//						}

	//					}
	//					//
	//					
	//		
	//					
	//					//
	//				}
	//			}// End While	
	//		}

	//		else
	//		{
	//			jaxc_stack_t* temp_stack;
	//			struct json_object* temp_obj;
	//			char* attr_name; 
	//			char* attr_val;
	//			struct lh_table *temp_lhtable;

	//			guththila_write_start_element ( json_reader->guththila_xml_serializer,
	//					mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser)), 
	//					env);
	//			
	//			// Attr---
	//			while (0 != jaxc_stack_size(mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
	//			{
	//				temp_stack = mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
	//				temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
	//				
	//				// Remove @ sign from the JSON attribute 
	//				temp_obj = json_object_new_object();
	//				json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
	//				
	//				attr_name = temp_lhtable->head->k;
	//				attr_name = strtok(attr_name, "@");
	//				attr_val = json_object_get_string(temp_lhtable->head->v);
	//			
	//				guththila_write_attribute (json_reader->guththila_xml_serializer
	//											, attr_name, attr_val, env);
	//				jaxc_stack_pop(temp_stack);
	//			}
	//	
	//	
	//			// Attrt test end

	//			json_reader->current_xml_element = mapped_reader_get_current_element(jaxc_get_reader(json_reader->parser));
	//		}
	//	}
	//	
	//	else if (JAXC_CHARACTER == parser_state)
	//	{
	//		jaxc_stack_t* temp_stack;
	//		struct json_object* temp_obj;
	//		char* attr_name; 
	//		char* attr_val;
	//		struct lh_table *temp_lhtable;

	//		while (0 != jaxc_stack_size(mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser))))
	//		{
	//			temp_stack = mapped_reader_get_current_attributes(jaxc_get_reader(json_reader->parser));
	//			temp_lhtable = json_object_get_object(jaxc_stack_get(temp_stack));
	//			
	//			// Remove @ sign from the JSON attribute 
	//			temp_obj = json_object_new_object();
	//			json_object_object_add(temp_obj, temp_lhtable->head->k, temp_lhtable->head->v);
	//			
	//			attr_name = temp_lhtable->head->k;
	//			attr_name = strtok(attr_name, "@");
	//			attr_val = json_object_get_string(temp_lhtable->head->v);
	//		
	//			guththila_write_attribute (json_reader->guththila_xml_serializer
	//										, attr_name, attr_val, env);
	//			jaxc_stack_pop(temp_stack);
	//		}

	//		printf ("\t\tJAXC_CHARACTER\n");


	//		guththila_write_characters (json_reader->guththila_xml_serializer, 
	//					mapped_reader_get_current_string(jaxc_get_reader(json_reader->parser)),
	//					env);
	//	}

	//	else if (JAXC_END_ELEMENT == parser_state)
	//	{
	//		printf ("\t\tJAXC_END_ELEMENT\n");
	//		guththila_write_end_element(json_reader->guththila_xml_serializer, env);
	//	}

	//}

	//guththila_write_end_document ( json_reader->guththila_xml_serializer, env);
	//json_reader->converted_xml_buf = guththila_get_memory_buffer (json_reader->guththila_xml_serializer, env);
	//
*/
}


