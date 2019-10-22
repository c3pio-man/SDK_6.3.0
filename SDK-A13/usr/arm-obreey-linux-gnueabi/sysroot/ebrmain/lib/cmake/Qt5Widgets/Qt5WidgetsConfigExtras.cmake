
if (NOT TARGET Qt5::uic)
    add_executable(Qt5::uic IMPORTED)

    set(imported_location "/srv/jenkins/workspace/fw-builder-ng/U_632_6.0/SDK-B288/usr/arm-obreey-linux-gnueabi/sysroot/usr/qt5/bin/uic")
    _qt5_Widgets_check_file_exists(${imported_location})

    set_target_properties(Qt5::uic PROPERTIES
        IMPORTED_LOCATION ${imported_location}
    )
endif()

include("${CMAKE_CURRENT_LIST_DIR}/Qt5Widgets_AccessibleFactory.cmake" OPTIONAL)

set(Qt5Widgets_UIC_EXECUTABLE Qt5::uic)
