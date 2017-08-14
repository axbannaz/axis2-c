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

// Test cases for JSON Writer
// sample xml file : source.xml 
// Test cases : - Badgerfish Writer
//				- Mapped Writer

#include <stdio.h>
#include <stdlib.h>

#include <axiom.h>
#include <axutil_utils.h>

#include <json.h>
#include <jaxc_json_writer.h>
#include <jaxc_writer_element.h>


int main()
{
	// Create axiom object model using the test xml file.
	// 
	int option;
	axutil_env_t * env = axutil_env_create_all("jaxc.log",4);
	axiom_xml_reader_t *xml_reader = NULL;
	axiom_stax_builder_t *xml_stax_builder = NULL; 
	axiom_document_t *om_document = NULL;	
	axiom_node_t* root_node = NULL;
	axiom_node_t* created_node = NULL;
	axiom_node_t* created_node_2 = NULL;

	
	// Testing follows
	axiom_node_t* temp_node = NULL;
	axiom_element_t *temp_element = NULL;
	axiom_children_iterator_t *children_iter = NULL;


	// JSON_C 
	jaxc_json_writer_t* json_badgerfish_writer;
	jaxc_json_writer_t* json_mapped_writer;
	
	printf ("---------------Test---------------------\n");

	root_node = axiom_node_create(env);
	temp_node = axiom_node_create(env);
	created_node = axiom_node_create(env);
	created_node_2 = axiom_node_create(env);
	

	xml_reader = axiom_xml_reader_create_for_file(env, "./source.xml", NULL);
	xml_stax_builder = axiom_stax_builder_create(env, xml_reader);
	om_document = axiom_document_create( env, NULL, xml_stax_builder);


	created_node = axiom_document_build_all(om_document, env);
	created_node_2 = axiom_document_build_all(om_document, env);

	//JSON writers
	// Creating Badgerfish Writer

	printf ("--------------- JAxC - Writer : XML to JSON Convertion ---------------------\n");
	printf ("--------------- Test Case : source.xml                 ---------------------\n");
	printf (" \nSelect Option : \n\t0 - Badgerfish Writer\n\t1 - Mapped Writer\n Option : >> ");
	scanf ("%d", &option);
	if (0 == option)
	{
		json_badgerfish_writer = json_writer_create(0, env);
		json_writer_write(json_badgerfish_writer, created_node, env);
		printf("\n------- Badgerfish JSON Writer ->\n");
		printf ("-------Converted Badgerfish JSON String ------------------------------- \n%s\n\n", 
		json_writer_get_converted_xml_str (json_badgerfish_writer, env));
		json_writer_free(json_badgerfish_writer, env);
	}
	// Creating Mapped Writer
	if (1 == option)
	{
		json_mapped_writer = json_writer_create(1, env);
		json_writer_write(json_mapped_writer, created_node_2, env);
		printf("\n------- Mapped JSON Writer ->\n");
		printf ("-------Converted Mapped JSON String -------------------------------\n %s\n\n", 
		json_writer_get_converted_xml_str (json_mapped_writer, env));
		json_writer_free(json_mapped_writer, env);
	}
	printf ("-------------------------------------------------------------------------------\n");



	

	// Deallocation
	AXIS2_FREE(env->allocator, xml_reader);
	AXIS2_FREE(env->allocator, xml_stax_builder);
	AXIS2_FREE(env->allocator, om_document);
	printf ("---------------End Test---------------------\n");
	
	return 0;
}