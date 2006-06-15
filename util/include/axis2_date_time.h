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
  
#ifndef AXIS2_DATE_TIME_H
#define AXIS2_DATE_TIME_H

#include <axis2_utils_defines.h>
#include <axis2_env.h>
 
/**
 * @file axis2_date_time.h
 * @brief axis2-util base64 encoding holder
 */

#ifdef __cplusplus
extern "C"
{
#endif
   
/** 
 * @defgroup axis2_base64 Base64 encoding holder
 * @ingroup axis2_util
 * @{
 */    
 

typedef struct axis2_date_time_ops axis2_date_time_ops_t;   
typedef struct axis2_date_time axis2_date_time_t;

AXIS2_DECLARE_DATA struct axis2_date_time_ops
{
   /**
    * free the axis2_date_time.
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL * 
    free)(axis2_date_time_t *date_time,
            const axis2_env_t *env);
    
   /**
    * store the time value from plain text.
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @param time time as a string format HH:MM:TTZ
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    deserialize_time) (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* time_str);
    
   /**
    * store the date value from plain text.
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @param date date as a string format YYYY-MM-DD
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    deserialize_date) (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_str);
    
   /**
    * store the date value from plain text.
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @param date_time string format YYYY-MM-DDTHH:MM:SSZ
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    deserialize_date_time) (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_time_str);
    
   /**
    * store the date value from set of values
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @param year Integer -1 can be used to ignore
    * @param month Integer -1 can be used to ignore
    * @param date Integer -1 can be used to ignore
    * @param hour Integer -1 can be used to ignore
    * @param min Integer -1 can be used to ignore
    * @param second Integer -1 can be used to ignore
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL*
    set_date_time ) (axis2_date_time_t* date_time,
                        const axis2_env_t *env,
                        int year, int month, int date,
                        int hour, int min, int second );
 
   /**
    * retrive the stored time  as a string
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @return time as a string format HH:MM:SSZ
    */
    char* (AXIS2_CALL *
    serialize_time) (axis2_date_time_t *date_time,
                        const axis2_env_t *env );
  
   /**
    * retrive the stored date as a string
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @return date as a string  format YYYY-MM-DD
    */
    char* (AXIS2_CALL *
    serialize_date) (axis2_date_time_t *date_time,
                        const axis2_env_t *env );
   
   /**
    * retrive the stored date time  as a string
    * @param date_time represet the type object
    * @param env pointer to environment struct. MUST NOT be NULL
    * @return time as a string format YYYY-MM-DDTHH:MM:SSZ
    */
    char* (AXIS2_CALL *
    serialize_date_time) (axis2_date_time_t *date_time,
                        const axis2_env_t *env );
    
} ;

AXIS2_DECLARE_DATA struct axis2_date_time 
{
    axis2_date_time_ops_t *ops;
};

/**
 * Creates axis2_date_time struct with current date time
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @return pointer to newly created axis2_date_time struct
 */
AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
axis2_date_time_create(const axis2_env_t *env );

/*************************** Function macros **********************************/

#define AXIS2_DATE_TIME_FREE(date_time, env) \
        ((date_time)->ops->free (date_time, env))

#define AXIS2_DATE_TIME_DESERIALIZE_TIME(date_time, env, time ) \
        ((date_time)->ops->deserialize_time (date_time, env, time_str))

#define AXIS2_DATE_TIME_DESERIALIZE_DATE(date_time, env, date) \
        ((date_time)->ops->deserialize_date (date_time, env, date_str))

#define AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME(date_time, env, date_time_str ) \
        ((date_time)->ops->deserialize_date_time(date_time, env, date_time_str))

#define AXIS2_DATE_TIME_SET_DATE_TIME(date_time, env, \
                           year, month, date, hour, min, second ) \
        ((date_time)->ops->set_date_time (date_time, env, \
                           year, month, date, hour, min, second ) )
 
#define AXIS2_DATE_TIME_SERIALIZE_TIME(date_time, env) \
        ((date_time)->ops->serialize_time (date_time, env ) )
                              
#define AXIS2_DATE_TIME_SERIALIZE_DATE(date_time, env) \
        ((date_time)->ops->serialize_date (date_time, env ) )

#define AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(date_time, env) \
        ((date_time)->ops->serialize_date_time (date_time, env ) )

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DATE_TIME_H */
