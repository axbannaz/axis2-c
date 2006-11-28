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

#ifndef AXIS2_CALLBACK_RECV_H
#define AXIS2_CALLBACK_RECV_H

/**
 * @defgroup axis2_callback_recv callback message receiver
 * @ingroup axis2_client_api
 * callback message receiver, that is used as the message receiver in the
 * operation in case of asynchronous invocation for receiving the result.
 */

/**
  * @file axis2_axis2_callback_recv.h
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_msg_recv.h>
#include <axis2_callback.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Callback receiver service name */
#define AXIS2_CALLBACK_RECV_SVC_NAME "ClientService"

    /** Type name for struct axis2_callback_recv_ops */
    typedef struct axis2_callback_recv_ops axis2_callback_recv_ops_t;
    /** Type name for struct axis2_callback_recv */
    typedef struct axis2_callback_recv axis2_callback_recv_t;


    /**
     * callback receiver ops struct.
     * Encapsulator struct for ops of axis2_callback_recv.
     */
    struct axis2_callback_recv_ops
    {
        /**
         * Gets the base struct which is of type message receiver.
         * @param callback_recv pointer to callback receiver struct
         * @param env pointer to environment struct
         * @return pointer to base message receiver struct
         */
        axis2_msg_recv_t *(AXIS2_CALL *
                get_base)(
                    const axis2_callback_recv_t *callback_recv,
                    const axis2_env_t *env);

        /**
         * Frees the callback receiver struct.
         * @param callback_recv pointer to callback receiver struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_callback_recv_t *callback_recv,
                    const axis2_env_t *env);

        /**
         * Adds a callback corresponding to given WSA message ID to message
         * receiver.
         * @param callback_recv pointer to callback receiver struct
         * @param env pointer to environment struct
         * @param msg_id message ID indicating which message the callback is
         * supposed to deal with
         * @param callback callback to be added. callback receiver assumes 
         * ownership of the callback
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_callback)(
                    struct axis2_callback_recv *callback_recv,
                    const axis2_env_t *env,
                    const axis2_char_t *msg_id,
                    axis2_callback_t *callback);
    };

    /**
     * callback receiver struct.
     */
    struct axis2_callback_recv
    {
        /** operations of callback receiver */
        axis2_callback_recv_ops_t *ops;
    };

    /**
     * Creates a callback receiver struct.
     * @param env pointer to environment struct
     * @return a pointer to newly created callback receiver struct,
     *         or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_callback_recv_t *AXIS2_CALL
    axis2_callback_recv_create(
        const axis2_env_t *env);

/** Gets the base message receiver. 
    @sa axis2_callback_recv_ops#get_base */
#define AXIS2_CALLBACK_RECV_GET_BASE(callback_recv, env) \
        ((callback_recv)->ops->get_base(callback_recv, env))

/** Frees callback message receiver. 
    @sa axis2_callback_recv_ops#free */
#define AXIS2_CALLBACK_RECV_FREE(callback_recv, env) \
        ((callback_recv)->ops->free(callback_recv, env))

/** Adds callback to callback message receiver. 
    @sa axis2_callback_recv_ops#free */
#define AXIS2_CALLBACK_RECV_ADD_CALLBACK(callback_recv, env, msg_id, callback)\
        ((callback_recv)->ops->add_callback(callback_recv, env, msg_id, callback))


    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_RECV_H */