check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/QUIC-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME QUIC
    SOURCE_FILES helper/QUIC-SendIfPossible.cc
                 helper/QUIC-StreamHandler.cc
                 helper/QUIC-LossDetection.cc
                 helper/QUIC-Stream.cc
                 helper/QUIC-Frame.cc
                 helper/QUIC-Packet.cc
                 helper/QUIC-PacketBuffer.cc
                 helper/QUIC-Header.cc
                 helper/QUIC-ReceiveLogic.cc
                 model/QUICSender-Receiver.cc
                 model/QUIC-Context.cc
                 model/QUIC-Event.cc
                 model/QUIC-Dispatcher.cc
                 model/QUIC-Scheduler.cc

    HEADER_FILES helper/QUIC-SendIfPossible.h
                 helper/QUIC-StreamHandler.h
                 helper/QUIC-LossDetection.h
                 helper/QUIC-Stream.h
                 helper/QUIC-Frame.h
                 helper/QUIC-Packet.h
                 helper/QUIC-PacketBuffer.h
                 helper/QUIC-Header.h
                 helper/QUIC-ReceiveLogic.h
                 model/QUICSender-Receiver.h
                 model/QUIC-Context.h
                 model/QUIC-Event.h
                 model/QUIC-Dispatcher.h
                 model/QUIC-Scheduler.h

    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC-test-suite.cc
                 ${examples_as_tests_sources}
)

