cmake_minimum_required (VERSION 2.6)
project (VierOpRij)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
elseif (CMAKE_COMPILER_IS_GNUCXX )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(MSVC)

if (UI STREQUAL "MFC")
	message("UI is MFC")
else()
	set(UI "Console")
endif()

if (UI STREQUAL "Console")
	add_definitions(-DCONSOLE)
	set(VIEROPRIJ_SOURCES_CPP
		"VierOpRijConsole.cpp"
		)
elseif (UI STREQUAL "MFC")
	set(CMAKE_MFC_FLAG 2)
	set(EXECUTABLE_TYPE WIN32)
	set(VIEROPRIJ_SOURCES_CPP
		"jstd/Threading.cpp"
		"VierOpRij.rc"
		"res/VierOpRij.ico"
		"res/VierOpRij.rc2"
		"VierOpRij.cpp"
		"VierOpRijDlg.cpp"
		"VierOpRijViewWnd.cpp"
		)
	set(VIEROPRIJ_SOURCES_H
		"VierOpRij.h"
		"VierOpRijDlg.h"
		"VierOpRijViewWnd.h"
		)
	add_subdirectory(jstd)
	set(LINK_JSTD TRUE)
endif()

add_definitions(-D_AFXDLL -DWINVER=0x600 -D_WIN32_WINNT=0x600 -DUNICODE -D_UNICODE)

set(VIEROPRIJ_SOURCES_CPP
	${VIEROPRIJ_SOURCES_CPP}
	"VierOpRijVeld.cpp"
	)

set(VIEROPRIJ_SOURCES_H
	${VIEROPRIJ_SOURCES_H}
	"stdafx.h"
	"VierOpRijVeld.h"
	)

source_group("include" FILES ${VIEROPRIJ_SOURCES_H})
source_group("src"     FILES ${VIEROPRIJ_SOURCES_CPP})

include_directories(jstd)

add_executable(VierOpRij
        ${EXECUTABLE_TYPE}
        ${VIEROPRIJ_SOURCES_CPP}
        ${VIEROPRIJ_SOURCES_H})

if (LINK_JSTD)
	target_link_libraries(VierOpRij JStd)
endif()
