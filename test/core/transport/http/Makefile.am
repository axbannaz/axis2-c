TESTS = test_http_transport
check_PROGRAMS = test_http_transport
noinst_PROGRAMS = test_http_transport
SUBDIRS =
AM_CFLAGS = -g -pthread
test_http_transport_SOURCES = test_http_transport.c

test_http_transport_LDADD   =  \
                                $(LDFLAGS) \
		                    ../../../../util/src/libaxutil.la \
							$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la \
							$(top_builddir)/src/core/util/libaxis2_core_utils.la \
						    $(top_builddir)/neethi/src/libneethi.la \
		                    ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
							$(top_builddir)/src/core/engine/libaxis2_engine.la \
							$(top_builddir)/src/core/transport/http/common/.libs/libaxis2_http_common.la \
							$(top_builddir)/src/core/context/libaxis2_context.la \
       						../../../../axiom/src/om/libaxis2_axiom.la \
       						../../../../axiom/src/soap/libaxis2_soap.la \
							$(top_builddir)/src/core/description/libaxis2_description.la

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I ../../../../util/include \
            -I ../../../../axiom/include

