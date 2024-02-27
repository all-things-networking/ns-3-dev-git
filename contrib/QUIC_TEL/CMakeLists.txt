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
                 model/QUIC-Context.cc
                 model/QUIC-Dispatcher.cc
                 model/QUIC-Event.cc
                 model/QUIC-Scheduler.cc
                 model/QUIC-State.cc
                 helper/QUIC-Frame.cc
                 helper/QUIC-Iterm_out.cc
                 helper/QUIC-QuicHeader.cc
                 helper/QUIC-selectDataProcessor.cc
                 helper/QUIC-sendProcessor.cc
                 helper/QUIC-Stream.cc
    HEADER_FILES model/QUIC_TEL.h
                 model/QUIC-Context.h
                 model/QUIC-Dispatcher.h
                 model/QUIC-Event.h
                 model/QUIC-Scheduler.h
                 model/QUIC-State.h
                 helper/QUIC-Frame.h
                 helper/QUIC-Iterm_out.h
                 helper/QUIC-QuicHeader.h
                 helper/QUIC-selectDataProcessor.h
                 helper/QUIC-sendProcessor.h
                 helper/QUIC-Stream.h
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC_TEL-test-suite.cc
                 ${examples_as_tests_sources}
)

