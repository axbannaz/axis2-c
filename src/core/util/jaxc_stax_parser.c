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
#include <jaxc.h>

jaxc_t* jaxc_create(void)
{
	/*FIX */
	jaxc_t* parser = malloc(sizeof(struct jaxc_s));
	parser->badgerfish_reader = NULL;
	parser->mapped_reader = NULL;

	return parser;
}

void jaxc_free(jaxc_t* parser)
{
	/* Memory Deallocation */
	if (NULL != parser->badgerfish_reader)
		badgerfish_reader_free(parser->badgerfish_reader);
	if (NULL != parser->mapped_reader)
		mapped_reader_free(parser->mapped_reader);

	free(parser);
}


void jaxc_initiate_reader(jaxc_t* parser, int convention, struct json_object* obj)
{
	parser->jaxc_convention = convention;
	if (0 == convention)
	{
		/*Initiate Badgerfish Reader */
		parser->badgerfish_reader = badgerfish_reader_create(obj);
	}

	else if (1 == convention)
	{
		/*Initiate Mapped Reader */
		parser->mapped_reader = mapped_reader_create(obj);
	}
	else 
	{
		printf ("ERROR : Invalid Convention\n");
	}
}

int jaxc_next(jaxc_t* parser)
{
	if (parser->jaxc_convention == 0)
	{	
		return badgerfish_reader_read(parser->badgerfish_reader);	
	}
	return -1;
}

void* jaxc_get_reader (jaxc_t* parser)
{
	return parser->badgerfish_reader;
}

int jaxc_get_reader_type(jaxc_t* parser)
{
	return parser->jaxc_convention;
}
