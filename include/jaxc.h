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
#ifndef JAXC_JSON_PULL_PARSER
#define JAXC_JSON_PULL_PARSER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jaxc_badgerfish_reader.h>
#include <jaxc_mapped_reader.h>




typedef struct jaxc_s
{
	
	/*
	//reader - Only allowed to create one reader for a given convention
	// 0 - Badgerfish Reader
	// 1 - Mapped Reader
	*/

	jaxc_badgerfish_reader_t* badgerfish_reader;
	jaxc_mapped_reader_t* mapped_reader;
	int jaxc_convention;
} jaxc_t;


/* // Create JAxC */
jaxc_t* jaxc_create(void);

/* // Initialize Reader */
void jaxc_initiate_reader(jaxc_t* parser, int convention, struct json_object* obj);

void jaxc_free(jaxc_t*);

int jaxc_next(jaxc_t*);

/* //jaxc_badgerfish_reader_t* jaxc_get_reader (jaxc_t* parser); */
void* jaxc_get_reader (jaxc_t* parser); 

int jaxc_get_reader_type(jaxc_t* parser);


#endif
