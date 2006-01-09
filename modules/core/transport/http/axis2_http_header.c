/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_http_header.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>


/** 
 * @brief HTTP Header struct impl
 *	Axis2 HTTP Header impl  
 */
typedef struct axis2_http_header_impl axis2_http_header_impl_t;  
  
struct axis2_http_header_impl
{
	axis2_http_header_t http_header;
	axis2_char_t *name;
    axis2_char_t *value;
};

#define AXIS2_INTF_TO_IMPL(http_header) \
                ((axis2_http_header_impl_t *)(http_header))

/***************************** Function headers *******************************/
axis2_char_t* AXIS2_CALL 
axis2_http_header_to_external_form (axis2_http_header_t *header, 
                axis2_env_t **env);
    
axis2_char_t* AXIS2_CALL 
axis2_http_header_get_name (axis2_http_header_t *header, axis2_env_t **env);
    
axis2_char_t* AXIS2_CALL 
axis2_http_header_get_value (axis2_http_header_t *header, axis2_env_t **env);
    
axis2_status_t AXIS2_CALL 
axis2_http_header_free (axis2_http_header_t *header, axis2_env_t **env);								

/***************************** End of function headers ************************/

AXIS2_DECLARE(axis2_http_header_t *) AXIS2_CALL
axis2_http_header_create (axis2_env_t **env, axis2_char_t *name, 
                            axis2_char_t *value)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK(name, env, NULL);
    AXIS2_FUNC_PARAM_CHECK(value, env, NULL);
        
    axis2_http_header_impl_t *http_header_impl = 
                        (axis2_http_header_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_header_impl_t));
	
    if(NULL == http_header_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    http_header_impl->name = (axis2_char_t *)AXIS2_STRDUP(name, env);
    http_header_impl->value = (axis2_char_t *)AXIS2_STRDUP(value, env);
     
    http_header_impl->http_header.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_header_ops_t));
    if(NULL == http_header_impl->http_header.ops)
	{
		axis2_http_header_free((axis2_http_header_t*)
                         http_header_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    http_header_impl->http_header.ops->to_external_form = 
                        axis2_http_header_to_external_form;
    http_header_impl->http_header.ops->get_name = axis2_http_header_get_name;
    http_header_impl->http_header.ops->get_value = axis2_http_header_get_value;
    http_header_impl->http_header.ops->free = axis2_http_header_free;
                        
	return &(http_header_impl->http_header);
}

AXIS2_DECLARE(axis2_http_header_t *) AXIS2_CALL
axis2_http_header_create_by_str (axis2_env_t **env, axis2_char_t *str)
{
	axis2_char_t *tmp_str = NULL;
	axis2_char_t *ch = NULL;
	axis2_char_t *ch2 = NULL;
	axis2_http_header_t *ret = NULL;
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK(str, env, NULL);
	
	tmp_str = AXIS2_STRDUP(str, env);
	if(NULL == tmp_str)
	{
		return NULL;
	}
	ch = strchr(tmp_str, ':');
	if(NULL == ch)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_HEADER, 
						AXIS2_FAILURE);
		AXIS2_FREE((*env)->allocator, tmp_str);
		return NULL;
	}
	ch2 = ch + sizeof(axis2_char_t);
	*ch = '\0';
	ret = axis2_http_header_create(env, tmp_str, ch2);
	AXIS2_FREE((*env)->allocator, tmp_str);
	return ret;	
}

axis2_status_t AXIS2_CALL 
axis2_http_header_free (axis2_http_header_t *header, axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(header, env, AXIS2_FAILURE);
    axis2_http_header_impl_t *http_header_impl =
                        AXIS2_INTF_TO_IMPL(header);
    if(NULL != http_header_impl->name)
    {
        AXIS2_FREE((*env)->allocator, http_header_impl->name);
        http_header_impl->name = NULL;
    }
    if(NULL != http_header_impl->value)
    {
        AXIS2_FREE((*env)->allocator, http_header_impl->value);
        http_header_impl->value = NULL;
    }
    if(NULL != header->ops)
        AXIS2_FREE((*env)->allocator, header->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(header));
	return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL 
axis2_http_header_to_external_form (axis2_http_header_t *header, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header, env, NULL);
    axis2_http_header_impl_t *http_header_impl = AXIS2_INTF_TO_IMPL(header);
    axis2_ssize_t len = AXIS2_STRLEN(http_header_impl->name) + 
                AXIS2_STRLEN(http_header_impl->value) + 4;
    axis2_char_t *external_form = (axis2_char_t*) AXIS2_MALLOC((*env)->allocator,
                len);
    sprintf(external_form, "%s:%s%s", http_header_impl->name, 
                http_header_impl->value, AXIS2_HTTP_CRLF);
    return external_form;
}


axis2_char_t* AXIS2_CALL 
axis2_http_header_get_name (axis2_http_header_t *header, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header, env, NULL);
    return AXIS2_INTF_TO_IMPL(header)->name;
}


axis2_char_t* AXIS2_CALL 
axis2_http_header_get_value (axis2_http_header_t *header, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header, env, NULL);
    return AXIS2_INTF_TO_IMPL(header)->value;
}
