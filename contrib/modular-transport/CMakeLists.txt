check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/modular-transport-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME modular-transport
    SOURCE_FILES model/modular-transport.cc
                 helper/modular-transport-helper.cc
                 model/dispatcher.cc
                 model/eventprocessor.cc
                 model/mt-scheduler.cc
                 model/mt-header.cc
                 model/mt-state.cc
                 model/TCP-header.cc
                 model/TCP-scheduler.cc

    HEADER_FILES model/modular-transport.h
                 helper/modular-transport-helper.h
                 model/dispatcher.h
                 model/mt-event.h
                 model/eventprocessor.h
                 model/mt-scheduler.h
                 model/mt-header.h
                 model/mt-state.h
                 model/TCP-header.h
                 model/TCP-scheduler.h
    LIBRARIES_TO_LINK ${libinternet}
    TEST_SOURCES test/modular-transport-test-suite.cc
                 ${examples_as_tests_sources}
)

