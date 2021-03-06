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

#include <rp_signed_encrypted_parts.h>

struct rp_signed_encrypted_parts_t
{
    axis2_bool_t body;
    axis2_bool_t signedparts;
    axis2_bool_t attachments;
    axutil_array_list_t *headers;
    int ref;
};

AXIS2_EXTERN rp_signed_encrypted_parts_t *AXIS2_CALL
rp_signed_encrypted_parts_create(
    const axutil_env_t * env)
{
    rp_signed_encrypted_parts_t *signed_encrypted_parts = NULL;
    signed_encrypted_parts = (rp_signed_encrypted_parts_t *)AXIS2_MALLOC(env->allocator,
        sizeof(rp_signed_encrypted_parts_t));

    if(!signed_encrypted_parts)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[neethi] Cannot create signed_encrypted_parts. Insuficient memory.");
        return NULL;
    }

    signed_encrypted_parts->headers = axutil_array_list_create(env, 0);
    if(!signed_encrypted_parts->headers)
    {
        rp_signed_encrypted_parts_free(signed_encrypted_parts, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[neethi] Cannot create signed_encrypted_parts. Headers array list creation failed.");
        return NULL;
    }

    signed_encrypted_parts->body = AXIS2_FALSE;
    signed_encrypted_parts->ref = 0;
    signed_encrypted_parts->signedparts = AXIS2_FALSE;
    signed_encrypted_parts->attachments = AXIS2_FALSE;
    return signed_encrypted_parts;
}

AXIS2_EXTERN void AXIS2_CALL
rp_signed_encrypted_parts_free(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    if(signed_encrypted_parts)
    {
        if(--(signed_encrypted_parts->ref) > 0)
        {
            return;
        }

        if(signed_encrypted_parts->headers)
        {
            int i = 0;
            for(i = 0; i < axutil_array_list_size(signed_encrypted_parts->headers, env); i++)
            {
                rp_header_t *header = NULL;
                header = (rp_header_t *)axutil_array_list_get(signed_encrypted_parts->headers, env,
                    i);
                if(header)
                {
                    rp_header_free(header, env);
                }
            }
            axutil_array_list_free(signed_encrypted_parts->headers, env);
            signed_encrypted_parts->headers = NULL;
        }
        AXIS2_FREE(env->allocator, signed_encrypted_parts);
        signed_encrypted_parts = NULL;
    }
}

/* Implementations */

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_signed_encrypted_parts_get_body(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    return signed_encrypted_parts->body;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_set_body(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env,
    axis2_bool_t body)
{
    AXIS2_PARAM_CHECK(env->error, body, AXIS2_FAILURE);
    signed_encrypted_parts->body = body;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_signed_encrypted_parts_get_signedparts(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    return signed_encrypted_parts->signedparts;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_set_signedparts(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env,
    axis2_bool_t signedparts)
{
    signed_encrypted_parts->signedparts = signedparts;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
rp_signed_encrypted_parts_get_headers(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    return signed_encrypted_parts->headers;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_add_header(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env,
    rp_header_t * header)
{
    AXIS2_PARAM_CHECK(env->error, header, AXIS2_FAILURE);
    axutil_array_list_add(signed_encrypted_parts->headers, env, header);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_increment_ref(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    signed_encrypted_parts->ref++;
    return AXIS2_SUCCESS;
}
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_signed_encrypted_parts_get_attachments(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env)
{
    return signed_encrypted_parts->attachments;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_set_attachments(
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t * env,
    axis2_bool_t attachments)
{
    signed_encrypted_parts->attachments = attachments;
    return AXIS2_SUCCESS;
}
