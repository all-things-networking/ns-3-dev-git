check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/QUIC-Receiver-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME QUIC-Receiver
    SOURCE_FILES model/QUICReceiver-Dispatcher.cc
                 model/QUICReceiver-Context.cc
                 model/QUICReceiver-Event.cc
                 model/QUICReceiver-Scheduler.cc
                 helper/QUICReceiver-Header.cc
                 helper/QUICReceiver-Frame.cc
                 helper/QUICReceiver-Stream.cc
                 helper/QUICReceiver-ReceiveLogic.cc
                 helper/QUICReceiver-EventProcessor.cc

    HEADER_FILES model/QUICReceiver-Context.h
                 model/QUICReceiver-Dispatcher.h
                 model/QUICReceiver-Event.h
                 model/QUICReceiver-Scheduler.h
                 helper/QUICReceiver-EventProcessor.h
                 helper/QUICReceiver-Header.h
                 helper/QUICReceiver-ReceiveLogic.h
                 helper/QUICReceiver-IntermediateOutput.h
                 helper/QUICReceiver-Stream.h
                 helper/QUICReceiver-Frame.h


    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC-Receiver-test-suite.cc
                 ${examples_as_tests_sources}
)

