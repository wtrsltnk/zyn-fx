
CPMAddPackage(
    NAME imgui
    GIT_TAG v1.90
    GITHUB_REPOSITORY ocornut/imgui
    DOWNLOAD_ONLY True
)

if (imgui_ADDED)
    add_library(imgui
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_demo.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    )

    target_include_directories(imgui
        PUBLIC
            "include"
            "${imgui_SOURCE_DIR}/"
            "${imgui_SOURCE_DIR}/backends/"
    )

    target_link_libraries(imgui
        PRIVATE
            glad
            ${OPENGL_LIBRARIES}
    )
else()
    message(FATAL_ERROR "IMGUI not found")
endif()

