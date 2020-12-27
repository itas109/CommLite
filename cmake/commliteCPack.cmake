cmake_minimum_required(VERSION 3.11)

# set version
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

# package version
set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

# package name 
set(CPACK_PACKAGE_NAME "com.itas109.commlite")
# package description
set(CPACK_PACKAGE_DESCRIPTION "textual user interface serial communication tool")
# package contact
set(CPACK_PACKAGE_CONTACT "itas109 <itas109@qq.com>")
# package file name
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

##############debian/control###############
# package 
set(CPACK_GENERATOR "DEB") # TGZ;ZIP;DEB;RPM
# debian package name
set(CPACK_DEBIAN_PACKAGE_NAME "com.itas109.commlite")
# cpu arch
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "arm64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "mips64el")
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "mips64el")
else()
	# set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
endif()
# debian package depends
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6)")
# debian section
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
# debian priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "standard")
include(CPack)

# do not make the install target depend on the all target
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
# install application
install(FILES ${PROJECT_BINARY_DIR}/commlite DESTINATION /opt/apps/com.itas109.commlite/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# install desktop
install(FILES ${PROJECT_SOURCE_DIR}/packages/deb/debian/usr/share/applications/com.itas109.commlite.desktop DESTINATION /usr/share/applications)
install(FILES ${PROJECT_SOURCE_DIR}/packages/deb/debian/usr/share/icons/hicolor/scalable/apps/com.itas109.commlite.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)
# install other files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/packages/deb/debian/opt/apps/com.itas109.commlite/entries DESTINATION /opt/apps/com.itas109.commlite)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/packages/deb/debian/opt/apps/com.itas109.commlite/files DESTINATION /opt/apps/com.itas109.commlite)
install(FILES ${PROJECT_SOURCE_DIR}/packages/deb/debian/opt/apps/com.itas109.commlite/info DESTINATION /opt/apps/com.itas109.commlite)