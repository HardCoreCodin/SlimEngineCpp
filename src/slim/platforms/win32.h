#include "./win32_base.h"
#include "../app.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

WNDCLASSA window_class;
HDC win_dc;
BITMAPINFO info;
RECT win_rect;
RAWINPUT raw_inputs;
HRAWINPUT raw_input_handle;
RAWINPUTDEVICE raw_input_device;
UINT raw_input_size;
PUINT raw_input_size_ptr = (PUINT)(&raw_input_size);
UINT raw_input_header_size = sizeof(RAWINPUTHEADER);

inline UINT getRawInput(LPVOID data) {
    return GetRawInputData(raw_input_handle, RID_INPUT, data, raw_input_size_ptr, raw_input_header_size);
}
inline bool hasRawInput() {
    return (getRawInput(0) == 0) && (raw_input_size != 0);
}
inline bool hasRawMouseInput(LPARAM lParam) {
    raw_input_handle = (HRAWINPUT)(lParam);
    return (
            (hasRawInput()) &&
            (getRawInput((LPVOID)&raw_inputs) == raw_input_size) &&
            (raw_inputs.header.dwType == RIM_TYPEMOUSE)
    );
}



SlimApp *CURRENT_APP;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    bool pressed = message == WM_SYSKEYDOWN || message == WM_KEYDOWN;
    u8 key = (u8)wParam;
    i32 x, y;
    f32 scroll_amount;

    switch (message) {
        case WM_DESTROY:
            CURRENT_APP->is_running = false;
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            GetClientRect(window_handle, &win_rect);

            info.bmiHeader.biWidth = win_rect.right - win_rect.left;
            info.bmiHeader.biHeight = win_rect.top - win_rect.bottom;
            CURRENT_APP->resize((u16)info.bmiHeader.biWidth, (u16)-info.bmiHeader.biHeight);

            break;

        case WM_PAINT:
            SetDIBitsToDevice(win_dc,
                              0, 0, window::width, window::height,
                              0, 0, 0, window::height,
                              (u32*)window::content, &info, DIB_RGB_COLORS);

            ValidateRgn(window_handle, nullptr);
            break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
            switch (key) {
                case VK_CONTROL: controls::is_pressed::ctrl   = pressed; break;
                case VK_MENU   : controls::is_pressed::alt    = pressed; break;
                case VK_SHIFT  : controls::is_pressed::shift  = pressed; break;
                case VK_SPACE  : controls::is_pressed::space  = pressed; break;
                case VK_TAB    : controls::is_pressed::tab    = pressed; break;
                case VK_ESCAPE : controls::is_pressed::escape = pressed; break;
                case VK_LEFT   : controls::is_pressed::left   = pressed; break;
                case VK_RIGHT  : controls::is_pressed::right  = pressed; break;
                case VK_UP     : controls::is_pressed::up     = pressed; break;
                case VK_DOWN   : controls::is_pressed::down   = pressed; break;
                default: break;
            }
            CURRENT_APP->OnKeyChanged(key, pressed);

            break;

        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            mouse::Button *mouse_button;
            switch (message) {
                case WM_MBUTTONUP:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONDBLCLK:
                    mouse_button = &mouse::middle_button;
                    break;
                case WM_RBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONDBLCLK:
                    mouse_button = &mouse::right_button;
                    break;
                default:
                    mouse_button = &mouse::left_button;
            }

            switch (message) {
                case WM_MBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                case WM_LBUTTONDBLCLK:
                    mouse_button->doubleClick(x, y);
                    mouse::double_clicked = true;
                    CURRENT_APP->OnMouseButtonDoubleClicked(*mouse_button);
                    break;
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                case WM_LBUTTONUP:
                    mouse_button->up(x, y);
                    CURRENT_APP->OnMouseButtonUp(*mouse_button);
                    break;
                default:
                    mouse_button->down(x, y);
                    CURRENT_APP->OnMouseButtonDown(*mouse_button);
            }

            break;

        case WM_MOUSEWHEEL:
            scroll_amount = (f32)(GET_WHEEL_DELTA_WPARAM(wParam)) / (f32)(WHEEL_DELTA);
            mouse::scroll(scroll_amount); CURRENT_APP->OnMouseWheelScrolled(scroll_amount);
            break;

        case WM_MOUSEMOVE:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            mouse::move(x, y);        CURRENT_APP->OnMouseMovementSet(x, y);
            mouse::setPosition(x, y); CURRENT_APP->OnMousePositionSet(x, y);
            break;

        case WM_INPUT:
            if ((hasRawMouseInput(lParam)) && (
                    raw_inputs.data.mouse.lLastX != 0 ||
                    raw_inputs.data.mouse.lLastY != 0)) {
                x = raw_inputs.data.mouse.lLastX;
                y = raw_inputs.data.mouse.lLastY;
                mouse::moveRaw(x, y); CURRENT_APP->OnMouseRawMovementSet(x, y);
            }

        default:
            return DefWindowProcA(hWnd, message, wParam, lParam);
    }

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
    void* window_content_and_canvas_memory = GlobalAlloc(GPTR, WINDOW_CONTENT_SIZE + (CANVAS_SIZE * CANVAS_COUNT));
    if (!window_content_and_canvas_memory)
        return -1;

    window::content = (u32*)window_content_and_canvas_memory;
    memory::canvas_memory = (u8*)window_content_and_canvas_memory + WINDOW_CONTENT_SIZE;

    controls::key_map::ctrl = VK_CONTROL;
    controls::key_map::alt = VK_MENU;
    controls::key_map::shift = VK_SHIFT;
    controls::key_map::space = VK_SPACE;
    controls::key_map::tab = VK_TAB;
    controls::key_map::escape = VK_ESCAPE;
    controls::key_map::left = VK_LEFT;
    controls::key_map::right = VK_RIGHT;
    controls::key_map::up = VK_UP;
    controls::key_map::down = VK_DOWN;

    LARGE_INTEGER performance_frequency;
    QueryPerformanceFrequency(&performance_frequency);

    timers::ticks_per_second = (u64)performance_frequency.QuadPart;
    timers::seconds_per_tick = 1.0 / (f64)(timers::ticks_per_second);
    timers::milliseconds_per_tick = 1000.0 * timers::seconds_per_tick;
    timers::microseconds_per_tick = 1000.0 * timers::milliseconds_per_tick;
    timers::nanoseconds_per_tick  = 1000.0 * timers::microseconds_per_tick;

    CURRENT_APP = createApp();
    if (!CURRENT_APP->is_running)
        return -1;

    info.bmiHeader.biSize        = sizeof(info.bmiHeader);
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biBitCount    = 32;
    info.bmiHeader.biPlanes      = 1;

    window_class.lpszClassName  = "RnDer";
    window_class.hInstance      = hInstance;
    window_class.lpfnWndProc    = WndProc;
    window_class.style          = CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    window_class.hCursor        = LoadCursorA(nullptr, MAKEINTRESOURCEA(32512));

    if (!RegisterClassA(&window_class)) return -1;

    win_rect.top = 0;
    win_rect.left = 0;
    win_rect.right  = window::width;
    win_rect.bottom = window::height;
    AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, false);

    window_handle = CreateWindowA(
            window_class.lpszClassName,
            window::title,
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT,
            CW_USEDEFAULT,
            win_rect.right - win_rect.left,
            win_rect.bottom - win_rect.top,

            nullptr,
            nullptr,
            hInstance,
            nullptr
    );
    if (!window_handle)
        return -1;

    raw_input_device.usUsagePage = 0x01;
    raw_input_device.usUsage = 0x02;
    if (!RegisterRawInputDevices(&raw_input_device, 1, sizeof(raw_input_device)))
        return -1;

    win_dc = GetDC(window_handle);

    SetICMMode(win_dc, ICM_OFF);

    ShowWindow(window_handle, nCmdShow);

    MSG message;
    while (CURRENT_APP->is_running) {
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        CURRENT_APP->OnWindowRedraw();
        mouse::resetChanges();
        InvalidateRgn(window_handle, nullptr, false);
    }

    return 0;
}