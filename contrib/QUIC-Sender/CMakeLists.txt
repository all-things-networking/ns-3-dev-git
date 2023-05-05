check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/QUIC-Sender-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME QUIC-Sender
    SOURCE_FILES helper/QUICSender-SendIfPossible.cc
                 helper/QUICSender-StreamHandler.cc
                 helper/QUICSender-LossDetection.cc
                 helper/QUICSender-Stream.cc
                 helper/QUICSender-Frame.cc
                 helper/QUICSender-Packet.cc
                 helper/QUICSender-PacketBuffer.cc
                 model/QUICSender-Receiver.cc
                 model/QUICSender-Context.cc
                 model/QUICSender-Event.cc
                 model/QUICSender-Dispatcher.cc
                 model/QUICSender-Scheduler.cc

    HEADER_FILES helper/QUICSender-SendIfPossible.h
                 helper/QUICSender-StreamHandler.h
                 helper/QUICSender-LossDetection.h
                 helper/QUICSender-Stream.h
                 helper/QUICSender-Frame.h
                 helper/QUICSender-Packet.h
                 helper/QUICSender-PacketBuffer.h
                 model/QUICSender-Receiver.h
                 model/QUICSender-Context.h
                 model/QUICSender-Event.h
                 model/QUICSender-Dispatcher.h
                 model/QUICSender-Scheduler.h



    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC-Sender-test-suite.cc
                 ${examples_as_tests_sources}
)

