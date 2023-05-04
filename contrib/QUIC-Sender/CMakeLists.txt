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
    SOURCE_FILES helper/QUIC-SendIfPossible.cc
                 helper/QUIC-StreamHandler.cc
                 helper/QUIC-LossDetection.cc
                 helper/QUIC-Stream.cc
                 helper/QUIC-Frame.cc
                 helper/QUIC-Packet.cc
                 helper/QUIC-PacketBuffer.cc
                 model/QUICSender-Receiver.cc
                 model/QUICSender-Context.cc
                 model/QUICSender-Event.cc
                 model/QUICSender-Dispatcher.cc
                 model/QUICSender-Scheduler.cc

    HEADER_FILES helper/QUIC-SendIfPossible.h
                 helper/QUIC-StreamHandler.h
                 helper/QUIC-LossDetection.h
                 helper/QUIC-Stream.h
                 helper/QUIC-Frame.h
                 helper/QUIC-Packet.h
                 helper/QUIC-PacketBuffer.h
                 model/QUICSender-Receiver.h
                 model/QUICSender-Context.h
                 model/QUICSender-Event.h
                 model/QUICSender-Dispatcher.h
                 model/QUICSender-Scheduler.h



    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC-Sender-test-suite.cc
                 ${examples_as_tests_sources}
)

