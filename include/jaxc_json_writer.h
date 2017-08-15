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

#ifndef JAXC_JSON_WRITER
#define JAXC_JSON_WRITER

#include <stdio.h>
#include <stdlib.h>
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

#include <string.h>
#include <axutil_utils.h>
#include <axiom.h>

#include <jaxc_badgerfish_writer.h>
#include <jaxc_mapped_writer.h>

#ifdef WIN32
#define AXIS2_EXTERN __declspec(dllexport)
#endif

typedef struct jaxc_json_writer_s
{
	jaxc_badgerfish_writer_t* badgerfish_writer;/* Badgerfish Writer */
	jaxc_mapped_writer_t* mapped_writer;
	int current_convention;
}jaxc_json_writer_t;

/* Init param -> convention | i.e. 0 - Badgerfish, 1 - Mapped */

AXIS2_EXTERN jaxc_json_writer_t* AXIS2_CALL json_writer_create(int init_param, const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL json_writer_free(jaxc_json_writer_t* writer, const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL json_writer_write(jaxc_json_writer_t* writer, const axiom_node_t* root_node, const axutil_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL json_writer_get_converted_xml_str (jaxc_json_writer_t* writer, const axutil_env_t *env);


#endif
