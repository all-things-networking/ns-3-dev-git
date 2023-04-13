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
                 model/mt-context.cc
                 model/mt-eventprocessor.cc
                 model/mt-event.cc
                 model/mt-header.cc
                 model/mt-receiver.cc
                 model/mt-state.cc
                 model/TCP-header.cc
                 model/QUIC-Receiver.cc
                 model/QUIC-Context.cc
                 model/QUIC-Event.cc
                 model/QUIC-Dispatcher.cc
                 model/QUIC-Scheduler.cc
                 model/QUIC-sender/QUIC-SendIfPossible.cc
                 model/QUIC-sender/QUIC-StreamHandler.cc
                 model/QUIC-sender/QUIC-LossDetection.cc
                 model/QUIC-sender/QUIC-Stream.cc
                 model/QUIC-sender/QUIC-Frame.cc
                 model/QUIC-sender/QUIC-Packet.cc
                 model/QUIC-sender/QUIC-PacketBuffer.cc

    HEADER_FILES model/modular-transport.h
                 helper/modular-transport-helper.h
                 model/mt-context.h
                 model/mt-dispatcher.h
                 model/mt-event.h
                 model/mt-eventprocessor.h
                 model/mt-receiver.h
                 model/mt-scheduler.h
                 model/mt-header.h
                 model/mt-state.h
                 model/TCP-header.h
                 model/QUIC-Receiver.h
                 model/QUIC-Context.h
                 model/QUIC-Event.h
                 model/QUIC-Dispatcher.h
                 model/QUIC-Scheduler.h
                 model/QUIC-sender/QUIC-SendIfPossible.h
                 model/QUIC-sender/QUIC-StreamHandler.h
                 model/QUIC-sender/QUIC-LossDetection.h
                 model/QUIC-sender/QUIC-Stream.h
                 model/QUIC-sender/QUIC-Frame.h
                 model/QUIC-sender/QUIC-Packet.h
                 model/QUIC-sender/QUIC-PacketBuffer.h
                 
    LIBRARIES_TO_LINK ${libinternet}
    TEST_SOURCES test/modular-transport-test-suite.cc
                 ${examples_as_tests_sources}
)

