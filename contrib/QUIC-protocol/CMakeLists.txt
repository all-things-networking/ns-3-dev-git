check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/QUIC-protocol-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME QUIC-protocol
    SOURCE_FILES model/QUIC-protocol.cc
                 helper/QUIC-protocol-helper.cc
                 helper/QUIC-SendIfPossible.cc
                 helper/QUIC-StreamHandler.cc
                 helper/QUIC-LossDetection.cc
                 helper/QUIC-Stream.cc
                 helper/QUIC-Frame.cc
                 helper/QUIC-Packet.cc
                 helper/QUIC-PacketBuffer.cc
                 helper/QUIC-Header.cc
                 helper/QUIC-ReceiveProcessor.cc
                 helper/QUIC-PacketDemultiplexer.cc
                 helper/QUIC-BufferManagement.cc
                 helper/QUIC-ReceiverAckHandler.cc
                 helper/QUIC-SendPacket.cc
                 helper/QUIC-AddStreamData.cc
                 helper/QUIC-HandleReceiveACK.cc
                 model/QUIC-ReceiveLogic.cc
                 model/QUIC-Context.cc
                 model/QUIC-Event.cc
                 model/QUIC-Dispatcher.cc
                 model/QUIC-Scheduler.cc

    HEADER_FILES model/QUIC-protocol.h
                 helper/QUIC-protocol-helper.h
                 helper/QUIC-SendIfPossible.h
                 helper/QUIC-StreamHandler.h
                 helper/QUIC-LossDetection.h
                 helper/QUIC-Stream.h
                 helper/QUIC-Frame.h
                 helper/QUIC-Packet.h
                 helper/QUIC-PacketBuffer.h
                 helper/QUIC-Header.h
                 helper/QUIC-ReceiveProcessor.h
                 helper/QUIC-PacketDemultiplexer.h
                 helper/QUIC-BufferManagement.h
                 helper/QUIC-ReceiverAckHandler.h
                 helper/QUIC-SendPacket.h
                 helper/QUIC-AddStreamData.h
                 helper/QUIC-HandleReceiveACK.h
                 model/QUIC-ReceiveLogic.h
                 model/QUIC-Context.h
                 model/QUIC-Event.h
                 model/QUIC-Dispatcher.h
                 model/QUIC-Scheduler.h

    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/QUIC-protocol-test-suite.cc
                 ${examples_as_tests_sources}
)

