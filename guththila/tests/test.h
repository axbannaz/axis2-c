#ifndef _GUTHTHILA_TESTS_
#define _GUTHTHILA_TESTS_
#include <check.h>
#include <guththila.h>
#include "guththila_defines.h"

axis2_allocator_t *allocator;
guththila_reader_t *red;
axis2_env_t *environment;
guththila_t *parser;

void
setup (void);
void
teardown (void);

Suite *
guththila_suite (void);

Suite *
guththila_attribute_suite (void);


#endif 
