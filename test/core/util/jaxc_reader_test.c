///*
// * Licensed to the Apache Software Foundation (ASF) under one or more
// * contributor license agreements.  See the NOTICE file distributed with
// * this work for additional information regarding copyright ownership.
// * The ASF licenses this file to You under the Apache License, Version 2.0
// * (the "License"); you may not use this file except in compliance with
// * the License.  You may obtain a copy of the License at
// *
// *      http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include <axutil_utils.h>
//
//#include <json.h>
//#include <jaxc.h>
//#include <jaxc_badgerfish_reader.h>
//#include <jaxc_node.h>
//#include <jaxc_json_reader.h>
//
//
//
//int main(int argc, char **argv)
//{
//	jaxc_json_reader_t* json_rdr;
//	axutil_env_t * env = axutil_env_create_all("kas.log",4);
//	
//	printf ("---------------Test---------------------\n");
//
//	//create (<json_string>, <convention>, <axutil_env>) --->> Follows
//	json_rdr = json_reader_create("{ \"book\":{\"title\":{ \"$\": \"Compilers\", \"@subtitle\":\"Principles & Tools\"},\"author\":[{\"$\":\"Jeffrey Ullmann\"}, {\"$\":\"Ravi Sethi\"}, {\"$\":\"Abraham Silberchartz\"}], \"media\": { \"$\":\"CD\"},  \"language\": { \"$\":\"English\"} ,  \"traslations\": { \"$\":\"French\"}, \"@isbn\": \"147LKRR\",\"@source\":\"Wiki\", \"@journalinfo\":\"The Scientist\"}}", 0, env);
//	//json_rdr = json_reader_create("{\"book\":{\"@lowprice\":\"SubContinent\", \"autor\":[\"Ravi Sethi\",\"Alfred Aho\"]}}", 1, env); 
//
//	//tmp_obj = json_object_new_string("{\"book\":{\"@lowprice\":\"SubContinent\", \"autor\":[\"Ravi Sethi\",\"Alfred Aho\"]}}");
//	
//	json_reader_read(json_rdr, env);
//
//	//printf ("{ \"book\":{\"title\":{ \"$\": \"Compilers\", \"@subtitle\":\"Principles & Tools\"},\"author\":[{\"$\":\"Jeffrey Ullmann\"}, {\"$\":\"Ravi Sethi\"}, {\"$\":\"Abraham Silberchartz\"}], \"media\": { \"$\":\"CD\"},  \"language\": { \"$\":\"English\"} ,  \"traslations\": { \"$\":\"French\"}, \"@isbn\": \"147LKRR\",\"@source\":\"Wiki\", \"@journalinfo\":\"The Scientist\"}}\n");
//	printf("\n\nConverted XML String : \n%s\n", json_reader_get_converted_xml_str(json_rdr, env)); 
//	
//	free (json_rdr);
//	//free(env);
//	printf ("---------------END Test---------------------\n");
//	return 0;
//	
//}