#include "vstcontrol.h"

#include <glad/glad_wgl.h>
#include <iostream>

VstControl::~VstControl() = default;

// Forward declarations of helper functions
HDC CleanupDeviceWGL(HWND hWnd);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define IDT_TIMER1 4835

bool VstControl::init(
    HWND parent,
    int width,
    int height)
{
    _hParent = parent;
    _width = width;
    _height = height;

    auto hInst = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {
        /*.cbSize =*/sizeof(wc),
        /*.style =*/CS_OWNDC,
        /*.lpfnWndProc =*/WndProc,
        /*.cbClsExtra =*/0L,
        /*.cbWndExtra =*/0L,
        /*.hInstance =*/hInst,
        /*.hIcon =*/nullptr,
        /*.hCursor =*/nullptr,
        /*.hbrBackground =*/nullptr,
        /*.lpszMenuName =*/nullptr,
        /*.lpszClassName =*/"VstControl",
        /*.hIconSm =*/nullptr,
    };

    WNDCLASSEX wincl;
    if (!GetClassInfoEx(hInst, "VstControl", &wincl))
    {
        if (!RegisterClassEx(&wc))
        {
            std::cerr << "Failed to register window class: " << GetLastError() << std::endl;
        }
    }

    _hWnd = CreateWindowEx(
        0, "VstControl", "VstControl",
        WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
        0, 0, width, height,
        _hParent,
        nullptr, nullptr,
        static_cast<void *>(this));

    if (_hWnd == nullptr)
    {
        std::cerr << "Failed to create window: " << GetLastError() << std::endl;
        return false;
    }

    // Initialize OpenGL
    if (!CreateDeviceWGL())
    {
        CleanupDeviceWGL();
        DestroyWindow(_hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    wglMakeCurrent(_hDC, _hRC);

    gladLoadGL();
    gladLoadWGL(_hDC);

    ShowWindow(_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(_hWnd);

    auto _createContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (_createContextAttribs != nullptr)
    {
        int attribs[] =
            {
                // Major version
                WGL_CONTEXT_MAJOR_VERSION_ARB,
                4,
                // Minor version
                WGL_CONTEXT_MINOR_VERSION_ARB,
                6,
                // Additional falgs
                WGL_CONTEXT_FLAGS_ARB,
                0,
                0,
            };

        auto tempContext = _createContextAttribs(_hDC, 0, attribs);

        if (tempContext != nullptr)
        {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(_hRC);

            _hRC = tempContext;
            wglMakeCurrent(_hDC, _hRC);
        }
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    ImGuiStyle &style = ImGui::GetStyle();
    // style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
    // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
    // // style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.68f, 0.68f, 0.68f, 0.00f);
    // style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    // style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
    // style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
    // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
    // style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
    // style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
    // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
    // style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
    // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
    // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
    // // style.Colors[ImGuiCol_ComboBg] = ImVec4(0.89f, 0.98f, 1.00f, 0.99f);
    // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
    // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
    // style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
    // style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
    // style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
    // style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
    // style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
    // style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
    // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
    // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    // // style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
    // // style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
    // // style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
    // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
    // // style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);
    // style.Alpha = 1.0f;
    // // style.WindowFillAlphaDefault = 1.0f;
    // style.FrameRounding = 4;
    // style.IndentSpacing = 12.0f;

    ImGui::StyleColorsDark();

//    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);

    io.Fonts->Clear();
    ImFont *font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    if (font != nullptr)
    {
        io.FontDefault = font;
    }
    else
    {
        io.Fonts->AddFontDefault();
    }

    io.Fonts->Build();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(_hWnd);
    ImGui_ImplOpenGL3_Init();

    SetTimer(_hWnd, IDT_TIMER1, 10, (TIMERPROC)NULL);

    return true;
}

void VstControl::close()
{
    DestroyWindow(_hWnd);
    _hWnd = nullptr;
    _hDC = nullptr;
    _hRC = nullptr;
}

bool VstControl::getSize(short &width, short &height)
{
    RECT rect;

    if (GetWindowRect(_hWnd, &rect) == FALSE)
    {
        return false;
    }

    width = static_cast<short>(rect.right - rect.left);
    height = static_cast<short>(rect.bottom - rect.top);

    return true;
}

bool VstControl::isOpen() const
{
    return _hWnd != nullptr;
}

// Helper functions
bool VstControl::CreateDeviceWGL()
{
    _hDC = ::GetDC(_hWnd);

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ChoosePixelFormat(_hDC, &pfd);
    if (pf == 0)
    {
        return false;
    }

    if (SetPixelFormat(_hDC, pf, &pfd) == FALSE)
    {
        return false;
    }

    ReleaseDC(_hWnd, _hDC);

    _hDC = GetDC(_hWnd);
    if (!_hRC)
    {
        _hRC = wglCreateContext(_hDC);
    }

    return true;
}

void VstControl::CleanupDeviceWGL()
{
    wglMakeCurrent(nullptr, nullptr);

    ReleaseDC(_hWnd, _hDC);
    _hDC = nullptr;
}

LRESULT VstControl::ControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    switch (msg)
    {
        case WM_TIMER:
        {
            switch (wParam)
            {
                case IDT_TIMER1:
                {
                    InvalidateRect(_hWnd, NULL, FALSE);
                    return 0;
                }
            }

            return 0;
        }
        case WM_PAINT:
        {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            if (renderUi)
            {
                renderUi();
            }

            ImGui::Render();
            glViewport(0, 0, this->_width, this->_height);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SwapBuffers(_hDC);

            return 0;
        }
        case WM_SIZE:
        {
            if (wParam != SIZE_MINIMIZED)
            {
                this->_width = LOWORD(lParam);
                this->_height = HIWORD(lParam);
            }
            return 0;
        }
        case WM_DESTROY:
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();

            CleanupDeviceWGL();
            wglDeleteContext(_hRC);
            _hRC = nullptr;

            UnregisterClass("VstControl", GetModuleHandle(nullptr));
            _hWnd = nullptr;

            return 0;
        }
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    auto control = reinterpret_cast<VstControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (msg)
    {
        case WM_NCCREATE:
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
            break;
        }

        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            {
                return 0;
            }

            break;
        }
    }

    return control->ControlProc(hWnd, msg, wParam, lParam);
}
