cmake_minimum_required (VERSION 3.0)

project (Agent-Web-Server)

option(USE_MYSQL_DB  "set ON to use mysql,  OFF is not use" OFF)
option(USE_DAMENG_DB "set ON to use dameng, OFF is not use" OFF)
option(USE_ORACLE_DB "set ON to use oracle, OFF is not use" OFF)

option(USE_STANDALONE_ASIO "set ON to use standalone Asio instead of Boost.Asio" OFF)
option(BUILD_TESTING "set ON to build library tests" OFF)

set(SOURCE_MYSQL_FILE  "")
set(SOURCE_DAMENG_FILE "")
set(SOURCE_ORACLE_FILE "")
if (USE_MYSQL_DB)
    add_definitions(-DUSE_MYSQL_DB) # 宏定义 USE_MYSQL_DB
    add_compile_options(-std=c++11 -Wall -g -Wextra -Wsign-conversion -ldl -L/usr/local/lib64/lib -lsoci_core  -L/usr/lib64/mysql -lmysqlclient -I/usr/include/mysql/ -L/usr/lib/mysql)
    set(SOURCE_MYSQL_FILE  db_util/mysql_util.cpp )
endif()
if (USE_DAMENG_DB)
    add_definitions(-DUSE_DAMENG_DB) # 宏定义 USE_DAMENG_DB
    add_compile_options(-std=c++11 -Wall -g -Wextra -Wsign-conversion -ldl -L/usr/local/lib64/lib -lsoci_core -lsoci_odbc   -I/usr/local/include/soci)
    set(SOURCE_DAMENG_FILE db_util/dameng_util.cpp )
endif()
if (USE_ORACLE_DB)
    add_definitions(-DUSE_ORACLE_DB) # 宏定义 USE_ORACLE_DB
    SET(OCCI_INCLUDE_DIR   "/home/cdd/instantclient_11_2/instantclient_11_2/sdk/include" )
    SET(OCCI_LIBRARY_PATH  "/home/cdd/instantclient_11_2/instantclient_11_2" )

    add_compile_options(-std=c++11 -Wall -g -Wextra -Wsign-conversion -ldl -L/usr/local/lib64/lib   -locci -lclntsh -I${OCCI_INCLUDE_DIR} -L${OCCI_LIBRARY_PATH})

    set(SOURCE_ORACLE_FILE db_util/oracle_util.cpp )
endif()


if(NOT MSVC)
    add_compile_options(-std=c++11 -Wall -g -Wextra -Wsign-conversion -ldl -L/usr/local/lib64/lib -L/usr/local/lib -llog4cplus -lpthread  -lsqlite3 -L/usr/local/sqlite3/lib -I/usr/local/sqlite3/include -lstdc++)
else()
    add_compile_options(/W1  -g -ldl -L/usr/local/lib64/lib -L/usr/local/lib  -llog4cplus -lpthread -lsqlite3 -L/usr/local/sqlite3/lib -I/usr/local/sqlite3/include -lstdc++)
endif()


add_library(Agent-Web-Server INTERFACE)

target_include_directories(Agent-Web-Server INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)
target_link_libraries(Agent-Web-Server INTERFACE ${CMAKE_THREAD_LIBS_INIT})

# TODO 2020 when Debian Jessie LTS ends:
# Remove Boost system, thread, regex components; use Boost::<component> aliases; remove Boost target_include_directories
if(USE_STANDALONE_ASIO)
    target_compile_definitions(Agent-Web-Server INTERFACE USE_STANDALONE_ASIO)
    include(CheckIncludeFileCXX)
    CHECK_INCLUDE_FILE_CXX(asio.hpp HAVE_ASIO)
    if(NOT HAVE_ASIO)
        message(FATAL_ERROR "Standalone Asio not found")
    endif()
else()
    find_package(Boost 1.53.0 COMPONENTS system thread REQUIRED)
    target_link_libraries(Agent-Web-Server INTERFACE ${Boost_LIBRARIES})
    target_include_directories(Agent-Web-Server INTERFACE ${Boost_INCLUDE_DIR})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        target_compile_definitions(Agent-Web-Server INTERFACE USE_BOOST_REGEX)
        find_package(Boost 1.53.0 COMPONENTS regex REQUIRED)
        target_link_libraries(Agent-Web-Server INTERFACE ${Boost_LIBRARIES})
        target_include_directories(Agent-Web-Server INTERFACE ${Boost_INCLUDE_DIR})
    endif()
endif()
if(WIN32)
    target_link_libraries(Agent-Web-Server INTERFACE ws2_32 wsock32)
endif()

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

link_directories(agent_server "/usr/lib64/mysql")

find_package(OpenSSL)
if(OPENSSL_FOUND)
    target_compile_definitions(Agent-Web-Server INTERFACE HAVE_OPENSSL)
    target_link_libraries(Agent-Web-Server INTERFACE ${OPENSSL_LIBRARIES})
    target_include_directories(Agent-Web-Server INTERFACE ${OPENSSL_INCLUDE_DIR})
endif()


set(SOURCE_FILE  handler/data_handler.cpp key_manage/key_manager.cpp
netlink/netlink_user.cpp agent_server.cpp https_server/https_server.cpp
log4cplus/my_log.cpp 3rd/inifile.cpp 3rd/crypo/aes/aes.cpp ${SOURCE_ORACLE_FILE} ${SOURCE_DAMENG_FILE} ${SOURCE_MYSQL_FILE} )

SET(LOG_INCLUDE_DIR "/usr/local/lib")
# If Agent-Web-Server is not a sub-project:
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    find_package(Boost 1.53.0 COMPONENTS system thread filesystem REQUIRED)
    if(OPENSSL_FOUND)
        add_executable(agent_server ${SOURCE_FILE} )
        target_link_libraries(agent_server Agent-Web-Server)
        target_link_libraries(agent_server ${Boost_LIBRARIES})
        target_include_directories(agent_server PRIVATE ${Boost_INCLUDE_DIR})
        target_link_libraries(agent_server liblog4cplus.a)
        target_link_libraries(agent_server /usr/local/sqlite3/lib/libsqlite3.a)
        if (USE_ORACLE_DB)
            target_link_libraries(agent_server ${OCCI_LIBRARY_PATH}/libclntsh.so)
            target_link_libraries(agent_server ${OCCI_LIBRARY_PATH}/libocci.so)
        endif()
        if (USE_MYSQL_DB)
            target_link_libraries(agent_server libmysqlclient.a)
        endif()
        if (USE_DAMENG_DB)
            target_link_libraries(agent_server libsoci_core.so)
        endif()
    endif()
    
    set(BUILD_TESTING OFF)
    
    install(FILES  server_http.hpp client_http.hpp server_https.hpp client_https.hpp crypto.hpp utility.hpp  status_code.hpp  DESTINATION include/Agent-Web-Server)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
