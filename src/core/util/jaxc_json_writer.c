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

#include <jaxc_json_writer.h>

AXIS2_EXTERN jaxc_json_writer_t* AXIS2_CALL json_writer_create(int init_param, const axutil_env_t *env)
{
	jaxc_json_writer_t* temp_json_writer = (jaxc_json_writer_t*) AXIS2_MALLOC(env->allocator, sizeof(jaxc_json_writer_t));
	
	temp_json_writer->current_convention = init_param; /* Convention */

	if (0 == temp_json_writer->current_convention) 
	{
		/* Creating Badgerfish Writer */
		temp_json_writer->badgerfish_writer = jaxc_badgerfish_writer_create(env);
	}
	if (1 == temp_json_writer->current_convention)
	{
		/* Creating Mapped Writer */
		temp_json_writer->mapped_writer = jaxc_mapped_writer_create(env);
	}

	return temp_json_writer;
}


AXIS2_EXTERN void AXIS2_CALL json_writer_free(jaxc_json_writer_t* writer, const axutil_env_t *env)
{
	if (0 == writer->current_convention)
		jaxc_badgerfish_writer_free(writer->badgerfish_writer, env);
	if (1 == writer->current_convention)
		jaxc_mapped_writer_free(writer->mapped_writer, env);

	AXIS2_FREE(env->allocator, writer);
}

AXIS2_EXTERN void AXIS2_CALL json_writer_write(jaxc_json_writer_t* writer, const axiom_node_t* root_node, const axutil_env_t *env)
{
	if ( 0 == writer->current_convention)
	{
		/* Badgerfish Convention */
		jaxc_badgerfish_writer_write (writer->badgerfish_writer, (axiom_node_t*) root_node, env);
	}
	else if ( 1 == writer->current_convention)
	{
		jaxc_mapped_writer_write(writer->mapped_writer, root_node, env);
	}
	else 
	{
		printf ("ERROR : Invalid Convention -> Init Parameter\n");
	}
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL json_writer_get_converted_xml_str (jaxc_json_writer_t* writer, const axutil_env_t *env)
{
	if (0 == writer->current_convention)
		return jaxc_badgerfish_writer_get_converted_xml_str(writer->badgerfish_writer,env);
	if (1 == writer->current_convention)
		return jaxc_mapped_writer_get_converted_xml_str(writer->mapped_writer, env);
/* XAX wtf?  no else? */
	return NULL; /* XAX does this make sense? */
}
