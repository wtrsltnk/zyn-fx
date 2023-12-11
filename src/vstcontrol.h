#ifndef VSTCONTROL_H
#define VSTCONTROL_H

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include <functional>
#include <windows.h>

class VstControl
{
public:
    virtual ~VstControl();

    bool init(HWND parent, int width, int height);
    void close();
    bool getSize(short &width, short &height);
    bool isOpen() const;

    std::function<void()> renderUi;

protected:
    void setHandle(HWND hWnd) { _hWnd = hWnd; }
    int width() { return _width; }
    int height() { return _height; }

private:
    HWND _hParent = nullptr;
    HGLRC _hRC = nullptr;
    HDC _hDC = nullptr;
    HWND _hWnd = nullptr;
    int _width = 400;
    int _height = 300;

    bool CreateDeviceWGL();
    void CleanupDeviceWGL();

    friend LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LRESULT ControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // VSTCONTROL_H
