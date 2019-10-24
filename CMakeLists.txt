cmake_minimum_required (VERSION 2.8.7)
project (OpenMAMA-zmq)

if(UNIX)
    if(MAMA_ROOT)
        set(DEFAULT_INSTALL_PREFIX $MAMA_ROOT/lib)
    endif()
    set(DEFAULT_MAMA_ROOT "/opt/openmama")
else()
    if(CMAKE_CL_64)
        set(DEFAULT_INSTALL_PREFIX $ENV{ProgramW6432})
    else()
        set(DEFAULT_INSTALL_PREFIX $ENV{PROGRAMFILES})
    endif()

    # Note path is relative to where it would be used
    set(DEFAULT_MAMA_ROOT "${DEFAULT_INSTALL_PREFIX}/OpenMAMA")
    set(DEFAULT_ZMQ_ROOT "${DEFAULT_INSTALL_PREFIX}/ZeroMQ 4.0.4")
    set(DEFAULT_EVENT_ROOT "${DEFAULT_INSTALL_PREFIX}/libevent")
endif()

if(NOT MAMA_ROOT)
    set(MAMA_ROOT ${DEFAULT_MAMA_ROOT})
endif()

if(NOT ZMQ_ROOT)
    set(ZMQ_ROOT ${DEFAULT_ZMQ_ROOT})
endif()

if(NOT EVENT_ROOT)
    set(EVENT_ROOT ${DEFAULT_EVENT_ROOT})
endif()

add_subdirectory(src)

if(OPENMAMA_SRC)
    include_directories(${MAMA_ROOT}/include)
    include_directories(${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c)
    include_directories(${OPENMAMA_SRC}/mama/c_cpp/src/c)
    link_directories(${MAMA_ROOT}/lib)
    link_directories(${GTEST_ROOT}/lib)

    add_executable(UnitTestMamaC
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/MainUnitTestC.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/inboxtest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/iotest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/mamainternaltest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/msgutils.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/openclosetest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/publishertest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/queuetest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/dictionarytest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/subscriptiontest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/timertest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/transporttest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcachevectortest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcachelisttest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcachemaptest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcachefieldtest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcacheiteratortest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcacherecordtest.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/fieldcache/fieldcachetest.cpp
    )
    target_link_libraries(UnitTestMamaC mama gtest pthread wombatcommon)
    install(TARGETS UnitTestMamaC DESTINATION bin)

    add_executable(UnitTestMamaMiddlewareC
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/MainUnitTestC.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareGeneralTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareInboxTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareIoTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareMsgTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewarePublisherTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareQueueTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareSubscriptionTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareTimerTests.cpp
        ${OPENMAMA_SRC}/mama/c_cpp/src/gunittest/c/middleware/middlewareTransportTests.cpp
    )
    target_link_libraries(UnitTestMamaMiddlewareC mama gtest pthread wombatcommon)
    install(TARGETS UnitTestMamaMiddlewareC DESTINATION bin)
else()
    message(WARNING "If you want full unit tests, you should provide -DOPENMAMA_SRC=")
endif()
