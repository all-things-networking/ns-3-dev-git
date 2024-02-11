check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/QUIC_TEL-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME QUIC_TEL
    SOURCE_FILES model/QUIC_TEL.cc
                 helper/QUIC_TEL-helper.cc
    HEADER_FILES model/QUIC_TEL.h
                 helper/QUIC_TEL-helper.h
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC_TEL-test-suite.cc
                 ${examples_as_tests_sources}
)

