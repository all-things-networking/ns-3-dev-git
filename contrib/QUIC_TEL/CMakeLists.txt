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
                 model/QUIC_TEL-Context.cc
                 model/QUIC_TEL-Dispatcher.cc
                 model/QUIC_TEL-Event.cc
                 model/QUIC_TEL-Scheduler.cc
                 model/QUIC_TEL-State.cc
                 #model/QUIC_TEL-ReceiveLogic.cc
                 helper/QUIC_TEL-Frame.cc
                 helper/QUIC_TEL-Iterm_out.cc
                 helper/QUIC_TEL-PacketInfo.cc
                 helper/QUIC_TEL-QuicHeader.cc
                 #helper/QUIC_TEL-ackProcessor.cc
                 helper/QUIC_TEL-selectDataProcessor.cc
                 helper/QUIC_TEL-sendProcessor.cc
                 helper/QUIC_TEL-Stream.cc
    HEADER_FILES model/QUIC_TEL.h
                 model/QUIC_TEL-Context.h
                 model/QUIC_TEL-Dispatcher.h
                 model/QUIC_TEL-Event.h
                 model/QUIC_TEL-Scheduler.h
                 model/QUIC_TEL-State.h
                 #model/QUIC_TEL-ReceiveLogic.h
                 helper/QUIC_TEL-Frame.h
                 helper/QUIC_TEL-Iterm_out.h
                 helper/QUIC_TEL-PacketInfo.h
                 helper/QUIC_TEL-QuicHeader.h
                 #helper/QUIC_TEL-ackProcessor.h
                 helper/QUIC_TEL-selectDataProcessor.h
                 helper/QUIC_TEL-sendProcessor.h
                 helper/QUIC_TEL-Stream.h
    LIBRARIES_TO_LINK ${libcore} ${libinternet} ${ns3-contrib-libs}
    TEST_SOURCES test/QUIC_TEL-test-suite.cc
                 ${examples_as_tests_sources}
)

