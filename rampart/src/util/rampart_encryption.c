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

#include <stdio.h>
#include <axis2_utils.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_utility.h>
#include <rampart_encryption.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_data_reference.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>
#include <oxs_key.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <rampart_handler_util.h>
#include <oxs_token_reference_list.h>
#include <axis2_array_list.h>
#include <oxs_axiom.h>
#include <oxs_asym_ctx.h>
#include <oxs_xml_encryption.h>
#include <axis2_key_type.h>

/*Private functions*/
/*
 * Get encryption parts. Get node list to be encrypted. If nothing is found use the body.
 * If body is specified in the encryption parts encrypt the immediate child of <body>
 **/

/*Private function implementations*/
/*
static axis2_status_t  
rampart_enc_get_nodes_to_encrypt(const axis2_env_t *env,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope,
    axis2_array_list_t *nodes_to_encrypt)
{
    axis2_char_t *encryption_parts = NULL;

    encryption_parts =  AXIS2_STRDUP(RAMPART_ACTIONS_GET_ENCRYPTION_PARTS(actions, env), env);
    if((!encryption_parts) || (0 == AXIS2_STRCMP(encryption_parts, "") || (0 == AXIS2_STRCMP(encryption_parts, "Body")))){
        axiom_soap_body_t *body = NULL;
        axiom_node_t *body_node = NULL;
        axiom_node_t *body_child_node = NULL;

        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] No encryption parts specified. Using the body as default");
        body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
        body_child_node = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
        axis2_array_list_add(nodes_to_encrypt, env, body_child_node);
        return AXIS2_SUCCESS;
    }else if(0 == AXIS2_STRCMP(encryption_parts, "Header")){
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] We do not encrypt SOAP headers");
        return AXIS2_SUCCESS;
    }else{
        axis2_array_list_t *str_list =  NULL;
        axiom_node_t *envelope_node = NULL;
        axiom_node_t *node = NULL;
        axis2_char_t *local_name = NULL;
        int size, i = 0;

        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] EncryptionParts specified = %s", encryption_parts);
        envelope_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);
        str_list = axis2_tokenize(env, encryption_parts, ' ');
        size = axis2_array_list_size(str_list, env);
        for(i=0 ; i < size ; i++ ){
            local_name = axis2_array_list_get(str_list, env, i);
            if(0 == AXIS2_STRCMP(local_name, "Security")){
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] We do not encrypt %s", local_name);
                continue;
            }
            node = oxs_axiom_get_node_by_local_name(env, envelope_node, local_name);
            axis2_array_list_add(nodes_to_encrypt, env, node);
        }
        return AXIS2_SUCCESS;
    }

    return AXIS2_SUCCESS;
}
*/



/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_enc_encrypt_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node)
{
    axis2_array_list_t *nodes_to_encrypt = NULL;
    axis2_array_list_t *id_list = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *enc_sym_algo = NULL;
    axis2_char_t *enc_asym_algo = NULL;
    axis2_char_t *eki = NULL;
    axis2_char_t *certificate_file = NULL;
    axis2_char_t *password = NULL;
    oxs_key_t *session_key = NULL;
    oxs_asym_ctx_t *asym_ctx = NULL;
    axis2_bool_t server_side = AXIS2_FALSE;
    int token_type = 0;   
    rp_property_t *token = NULL;
    axis2_char_t *enc_user = NULL;
    rampart_callback_t *password_callback = NULL;
    password_callback_fn password_function = NULL;
    void *param = NULL;
    void *key_buf = NULL;
    int i = 0;
    /*Get nodes to be encrypted*/
    server_side = axis2_msg_ctx_get_server_side(msg_ctx,env);
    nodes_to_encrypt = axis2_array_list_create(env,0);

    status = rampart_context_get_nodes_to_encrypt(rampart_context,env,soap_envelope,nodes_to_encrypt);

    if((status!=AXIS2_SUCCESS)||(axis2_array_list_size(nodes_to_encrypt,env)==0))
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] No parts specified or specified parts can't be found for encryprion.");
        return AXIS2_SUCCESS;
    }                            
    /*Now we have to check whether a token is specified.*/
    token = rampart_context_get_token(rampart_context,env,AXIS2_TRUE,server_side);
    if(!token)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_encryption] Encryption Token is not specified");
        return AXIS2_SUCCESS;
    }   
    token_type = rp_property_get_type(token,env);
    if(token_type != RP_TOKEN_X509)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_encryption] We only support X509 tokens");
        return AXIS2_SUCCESS;
    }
    if(rampart_context_check_is_derived_keys(env,token))
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_encryption] We still do not support derived keys");
        return AXIS2_FAILURE;
    }        
    /*Get the symmetric encryption algorithm*/
    enc_sym_algo = rampart_context_get_enc_sym_algo(rampart_context,env);
    /*If not specified set the default*/
    if(!enc_sym_algo ||  (0 == AXIS2_STRCMP(enc_sym_algo, ""))){
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] No symmetric algorithm is specified for encryption. Using the default");
        enc_sym_algo = OXS_DEFAULT_SYM_ALGO;    
    }
    /*Generate the  session key*/
    session_key = oxs_key_create(env);
    status = OXS_KEY_FOR_ALGO(session_key, env, enc_sym_algo); 
    if(AXIS2_FAILURE == status){
        return AXIS2_FAILURE;
    }

    /*Create a list to store EncDataIds. This will be used in building the ReferenceList*/
    id_list = axis2_array_list_create(env, 5);

    /*Repeat until all encryption parts are encrypted*/
    for(i=0 ; i < axis2_array_list_size(nodes_to_encrypt, env); i++){
        axiom_node_t *node_to_enc = NULL;
        axiom_node_t *parent_of_node_to_enc = NULL;
        axiom_node_t *enc_data_node = NULL;
        oxs_ctx_t *enc_ctx = NULL;
        axis2_char_t *id = NULL;
        axis2_status_t enc_status = AXIS2_FAILURE;
        
        /*Get the node to be encrypted*/
        node_to_enc = (axiom_node_t *)axis2_array_list_get(nodes_to_encrypt, env, i);
        if(!node_to_enc){
            return AXIS2_FAILURE;
        }
        /*Create the encryption context for OMXMLSEC*/
        enc_ctx = oxs_ctx_create(env);
        /*Set the key*/
        OXS_CTX_SET_KEY(enc_ctx, env, session_key);
        /*Set the algorithm*/
        OXS_CTX_SET_ENC_MTD_ALGORITHM(enc_ctx, env, enc_sym_algo);
        /*Create an empty EncryptedDataNode*/
        parent_of_node_to_enc = AXIOM_NODE_GET_PARENT(node_to_enc, env);
        id = oxs_util_generate_id(env,(axis2_char_t*)OXS_ENCDATA_ID);
        enc_data_node = oxs_token_build_encrypted_data_element(env, parent_of_node_to_enc, OXS_TYPE_ENC_ELEMENT, id );
        enc_status = oxs_xml_enc_encrypt_node(env, enc_ctx, node_to_enc, &enc_data_node);
        axis2_array_list_add(id_list, env, id);
        if(AXIS2_FAILURE == enc_status){
            return AXIS2_FAILURE;
        }
    }
    /*Get the asymmetric key encryption algorithm*/
    enc_asym_algo = rampart_context_get_enc_asym_algo(rampart_context,env);
    
    /*Get encryption key identifier*/
    eki = rampart_context_get_enc_key_identifier(rampart_context,token,server_side,env);
    if(!eki)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] The token is not needed for inclusion.");
        return AXIS2_SUCCESS;
    }

    /*Create asymmetric encryption context*/
    asym_ctx = oxs_asym_ctx_create(env);
    oxs_asym_ctx_set_algorithm(asym_ctx, env, enc_asym_algo);

    /*First check whether the public key is set*/
    key_buf = rampart_context_get_receiver_certificate(rampart_context,env);
    if(key_buf)
    {
        axis2_key_type_t type = 0;
        type = rampart_context_get_receiver_certificate_type(rampart_context,env);
        if(type == AXIS2_KEY_TYPE_PEM)
        {
            oxs_asym_ctx_set_format(asym_ctx, env, OXS_ASYM_CTX_FORMAT_PEM);
            oxs_asym_ctx_set_pem_buf(asym_ctx, env, (axis2_char_t *)key_buf);            
        }            
    }        
    
    /*Buffer is null load from the file*/
    else
    {
        certificate_file = rampart_context_get_receiver_certificate_file(rampart_context,env);        
        oxs_asym_ctx_set_file_name(asym_ctx, env, certificate_file);
        oxs_asym_ctx_set_format(asym_ctx, env, oxs_util_get_format_by_file_extension(env, certificate_file));

        /*Get the password to retrieve the key from key store*/
        password = rampart_context_get_prv_key_password(rampart_context,env);

        if(!password)
        {
            enc_user = rampart_context_get_encryption_user(rampart_context,env);

            if(!enc_user)
                enc_user = rampart_context_get_user(rampart_context,env);

            if(enc_user)
            {
                password_function = rampart_context_get_pwcb_function(rampart_context,env);
                if(password_function)
                    password = (*password_function)(env,enc_user,param);

                else
                {
                    password_callback = rampart_context_get_password_callback(rampart_context,env);
                    if(!password_callback)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] Password call back module is not loaded.");
                        return AXIS2_FAILURE;
                    }
                    password = rampart_callback_password(env, password_callback, enc_user);
                    if(password)
                        oxs_asym_ctx_set_password(asym_ctx, env, password);
                }
            }
        }
    }
    
    oxs_asym_ctx_set_operation(asym_ctx, env, OXS_ASYM_CTX_OPERATION_PUB_ENCRYPT);
    oxs_asym_ctx_set_st_ref_pattern(asym_ctx, env, eki);
    /*Encrypt the session key*/
    status = oxs_xml_enc_encrypt_key(env, asym_ctx, sec_node, session_key, id_list);    
    if(AXIS2_FAILURE == status){
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}




