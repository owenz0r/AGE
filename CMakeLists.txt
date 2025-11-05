cmake_minimum_required(VERSION "3.19.2")

#add_subdirectory("contrib/SDL")
#add_subdirectory("contrib/SDL_ttf")
#add_subdirectory("contrib/SDL_image")
#add_subdirectory("contrib/SDL_mixer")

project("AngryGooseEngine")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*/*.cpp)

add_library(AGE STATIC ${MY_SOURCES})

target_include_directories(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(AGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL/include")
target_include_directories(AGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL_ttf")
target_include_directories(AGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL_image/include")
target_include_directories(AGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL_mixer/include")

target_sources(AGE PRIVATE "${MY_SOURCES}")

target_link_libraries(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/mac/libSDL2main.a)
target_link_libraries(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/mac/libSDL2.dylib)
target_link_libraries(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/mac/libSDL2_ttf.dylib)
target_link_libraries(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/mac/libSDL2_image.dylib)
target_link_libraries(AGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/mac/libSDL2_mixer.dylib)

#function(copy_per_config_dlls TARGET DLL_MAPPINGS)
#	foreach(MAPPING ${DLL_MAPPINGS})
#		string(REPLACE "|" ";" PARTS "${MAPPING}")
#		list(GET PARTS 0 RELEASE_NAME)
#		list(GET PARTS 1 DEBUG_NAME)
#		list(GET PARTS 2 DEBUG_DIR)
#		list(GET PARTS 3 RELEASE_DIR)
#
#		set(DLL_RELEASE "${RELEASE_DIR}/${RELEASE_NAME}")
#		set(DLL_DEBUG "${DEBUG_DIR}/${DEBUG_NAME}")
#
#		add_custom_command(TARGET ${TARGET} POST_BUILD
#			COMMAND ${CMAKE_COMMAND} -E echo "Target location - $<TARGET_FILE:SDL2_image>"
#			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET}>/bin/"
#			COMMAND ${CMAKE_COMMAND} -E copy_if_different
#				"$<$<CONFIG:Debug>:${DLL_DEBUG}>"
#				"$<$<CONFIG:Release>:${DLL_RELEASE}>"
#				"$<TARGET_FILE_DIR:${TARGET}>/bin/"
#		)
#	endforeach()
#endfunction()

#set(DLL_MAPPINGS
#	"SDL2.dll|SDL2d.dll|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Debug/contrib/SDL|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Release/contrib/SDL"
#	"SDL2_ttf.dll|SDL2_ttfd.dll|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Debug/contrib/SDL_ttf|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Release/contrib/SDL_ttf"
#	"SDL2_image.dll|SDL2_imaged.dll|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Debug/contrib/SDL_image|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Release/contrib/SDL_image"
#	"SDL2_mixer.dll|SDL2_mixerd.dll|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Debug/contrib/SDL_mixer|${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Release/contrib/SDL_mixer"
#)

#copy_per_config_dlls(libAGE "${DLL_MAPPINGS}")

#add_custom_command(TARGET libAGE POST_BUILD
#			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:libAGE>/bin/"
#			COMMAND ${CMAKE_COMMAND} -E copy_if_different
#				"$<TARGET_FILE:SDL2>"
#				"$<TARGET_FILE_DIR:libAGE>/bin/"
#			COMMAND ${CMAKE_COMMAND} -E copy_if_different
#				"$<TARGET_FILE:SDL2_ttf>"
#				"$<TARGET_FILE_DIR:libAGE>/bin/"
#			COMMAND ${CMAKE_COMMAND} -E copy_if_different
#				"$<TARGET_FILE:SDL2_image>"
#				"$<TARGET_FILE_DIR:libAGE>/bin/"
#			COMMAND ${CMAKE_COMMAND} -E copy_if_different
#				"$<TARGET_FILE:SDL2_mixer>"
#				"$<TARGET_FILE_DIR:libAGE>/bin/"
#)

add_executable(AGE_test ${CMAKE_CURRENT_SOURCE_DIR}/source/libAGE_test.cpp)
target_link_libraries(AGE_test PRIVATE AGE)
