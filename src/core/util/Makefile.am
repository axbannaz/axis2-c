noinst_LTLIBRARIES = libaxis2_core_utils.la
#noinst_HEADERS = axis2_core_utils.h

if AXIS2_LIBJSON_C_ENABLED
LIBJSON_C_SOURCES =	jaxc_badgerfish_reader.c \
					jaxc_badgerfish_writer.c \
					jaxc_json_reader.c \
					jaxc_json_writer.c \
					jaxc_mapped_reader.c \
					jaxc_mapped_writer.c \
					jaxc_node.c \
					jaxc_stack.c \
					jaxc_stax_parser.c \
					jaxc_writer_element.c
LIBJSON_C_LIBS = -ljson-c
else
LIBJSON_C_SOURCES =
LIBJSON_C_LIBS =
endif

libaxis2_core_utils_la_SOURCES =	core_utils.c \
									${LIBJSON_C_SOURCES}

libaxis2_core_utils_la_LIBADD =	${LIBJSON_C_LIBS}

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/core/engine \
            -I$(top_builddir)/util/include \
            -I$(top_builddir)/axiom/include
