noinst_PROGRAMS = axiom
axiom_SOURCES = axiom_util_test.c
axiom_LDADD   =   \
                    $(top_builddir)/src/om/libaxis2_axiom.la \
                    $(top_builddir)/../util/src/libaxutil.la \
                    $(top_builddir)/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la

INCLUDES = -I$(top_builddir)/include \
			-I ../../../util/include \
			-I ../../../include
