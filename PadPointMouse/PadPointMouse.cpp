
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <Xinput.h>
#include <shellapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>


#include <stdio.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>




#pragma comment(lib, "Winmm.lib")

// What even is this black magic 
#include <initguid.h>
#include <Objbase.h>
#pragma hdrstop


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        TextOut(hdc, 10, 10, L"This is a notification message!", strlen("This is a notification message!"));
        EndPaint(hwnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// 4ce576fa-83dc-4F88-951c-9d0782b4e376
DEFINE_GUID(CLSID_UIHostNoLaunch,
    0x4CE576FA, 0x83DC, 0x4f88, 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76);

// 37c994e7_432b_4834_a2f7_dce1f13b834b
DEFINE_GUID(IID_ITipInvocation,
    0x37c994e7, 0x432b, 0x4834, 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b);

// What even is this black magic 

void printHelp() {


    std::cout << "=============PadPointMouse===============" << std::endl;

    std::cout << "=========================================" << std::endl;

    std::cout << "              How to use                 \n\n" << std::endl;
    
    std::cout << "A: Left click" << std::endl;

    std::cout << "B: Right click" << std::endl;

    std::cout << "X: Touch Keyboard" << std::endl;

    std::cout << "Y: Switch between PC/controller mode" << std::endl;

    std::cout << "Right Trigger: Left click" << std::endl;

    std::cout << "Left Trigger: Right click" << std::endl;

    std::cout << "Left Bumper: Browser backward" << std::endl;

    std::cout << "Right Bumper: Browser forward" << std::endl;

    std::cout << "Select: End program" << std::endl;

    std::cout << "Start: A nice surprise" << std::endl;

    std::cout << "D-Pad-Left: Windows Explorer" << std::endl;

    std::cout << "D-Pad-Right: Windows Explorer" << std::endl;

    std::cout << "D-Pad-Up: Volume up" << std::endl;

    std::cout << "D-Pad-Left: Volume down" << std::endl;

    std::cout << "Both sticks in: screenshot\n\n" << std::endl;

    std::cout << "=============PadPointMouse===============" << std::endl;

    std::cout << "=========================================" << std::endl;
}


bool GetVolumeLevel(float* system_volume) {
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pAudioEndpointVolume = nullptr;

    try {
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
            __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        if (FAILED(hr))
            throw "CoCreateInstance";

        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        if (FAILED(hr))
            throw "GetDefaultAudioEndpoint";

        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr,
            (void**)&pAudioEndpointVolume);
        if (FAILED(hr))
            throw "pDevice->Activate";

        float fVolume;
        hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);

        if (FAILED(hr))
            throw "GetMasterVolumeLevelScalar";

        *system_volume = fVolume;

        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();
        return true;
    }
    catch (...) {
        if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if (pDevice) pDevice->Release();
        if (pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}

bool ChangeVolume(double nVolume, bool bScalar) {
    HRESULT hr = CoInitialize(NULL);
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pAudioEndpointVolume = NULL;

    // Initialize COM and get the default audio endpoint
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);

    // Set the volume (0.0 to 1.0)
    hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(static_cast<float>(nVolume), NULL);

    // Clean up
    pAudioEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();

    return SUCCEEDED(hr);
}

struct ITipInvocation : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Toggle(HWND wnd) = 0;
};

int main()
{
    printHelp();


    float volume;

    POINT point;
   
    GetCursorPos(&point);


    CoInitialize(nullptr);
    try {
        GetVolumeLevel(&volume);
    }
    catch (...) {
        volume = 0.f;
    }

    CoUninitialize();

    float x_pos = (float)point.x;
    float y_pos = (float)point.y;

    int mouseVelocity = 4;

    float moveBy = 0.00001;
    float volumeChange = 0.1;

    INPUT inputs[50] = {};
    bool buttonDown[50] = {};
    

    ZeroMemory(inputs, sizeof(inputs));

    bool controllerUse = true;

    while (true)
    {
        for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
        {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            DWORD result = XInputGetState(i, &state);

            

            if (result == ERROR_SUCCESS)
            {

                if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y && buttonDown[15] != 1) {
                    controllerUse = !controllerUse;

                }

                if (controllerUse) {

                    x_pos += state.Gamepad.sThumbLX * moveBy;
                    y_pos += -state.Gamepad.sThumbLY * moveBy;
                  

                    if (state.Gamepad.bRightTrigger > 0 && buttonDown[0] != 1) {

                        inputs[0].type = INPUT_MOUSE;
                        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                        inputs[1].type = INPUT_MOUSE;
                        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                       
                    }
               
                    if (state.Gamepad.bLeftTrigger > 0 && buttonDown[1] != 1) {

                        inputs[2].type = INPUT_MOUSE;
                        inputs[2].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                        inputs[3].type = INPUT_MOUSE;
                        inputs[3].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

                    }

                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && buttonDown[2] != 1) {
                        inputs[0].type = INPUT_MOUSE;
                        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                        inputs[1].type = INPUT_MOUSE;
                        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && buttonDown[3] != 1) {
                        inputs[2].type = INPUT_MOUSE;
                        inputs[2].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                        inputs[3].type = INPUT_MOUSE;
                        inputs[3].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X && buttonDown[4] != 1) {

                        HRESULT hr;
                        hr = CoInitialize(0);

                        ITipInvocation* tip;
                        hr = CoCreateInstance(CLSID_UIHostNoLaunch, 0, CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, IID_ITipInvocation, (void**)&tip);
                        tip->Toggle(GetDesktopWindow());
                        tip->Release();


                    }

                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && buttonDown[5] != 1) {
                        volume -= volumeChange;
                        ChangeVolume(volume, 1);


                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && buttonDown[6] != 1) {

                        system("explorer.exe");
                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && buttonDown[7] != 1) {

                        inputs[4].type = INPUT_KEYBOARD;
                        inputs[4].ki.dwFlags = XINPUT_KEYSTROKE_KEYDOWN;
                        keybd_event(VK_LWIN, 0x5B, inputs[4].ki.dwFlags, inputs[4].ki.dwExtraInfo);
                        inputs[4].type = INPUT_KEYBOARD;
                        inputs[4].ki.dwFlags = XINPUT_KEYSTROKE_KEYUP;
                        keybd_event(VK_LWIN, 0x5B, inputs[4].ki.dwFlags, inputs[4].ki.dwExtraInfo);
                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && buttonDown[8] != 1) {
                        volume += volumeChange;
                        ChangeVolume(volume, 1);

                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && buttonDown[11] != 1) {

                        // VK_BROWSER_BACK

                        inputs[10].type = INPUT_KEYBOARD;
                        inputs[10].ki.dwFlags = XINPUT_KEYSTROKE_KEYDOWN;

                        keybd_event(VK_BROWSER_BACK, 0xA6, inputs[10].ki.dwFlags, inputs[10].ki.dwExtraInfo);

                        inputs[10].type = INPUT_KEYBOARD;
                        inputs[10].ki.dwFlags = XINPUT_KEYSTROKE_KEYUP;

                        keybd_event(VK_BROWSER_BACK, 0xA6, inputs[10].ki.dwFlags, inputs[10].ki.dwExtraInfo);

                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && buttonDown[12] != 1) {
                        
                        // VK_BROWSER_FORWARD
                        inputs[9].type = INPUT_KEYBOARD;
                        inputs[9].ki.dwFlags = XINPUT_KEYSTROKE_KEYDOWN;
                        
                        keybd_event(VK_BROWSER_FORWARD, 0xA7, inputs[9].ki.dwFlags, inputs[9].ki.dwExtraInfo);
                        
                        inputs[9].type = INPUT_KEYBOARD;
                        inputs[0].ki.dwFlags = XINPUT_KEYSTROKE_KEYUP;
                        
                        keybd_event(VK_BROWSER_FORWARD, 0xA7, inputs[9].ki.dwFlags, inputs[9].ki.dwExtraInfo);



                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && buttonDown[13] != 1) {

                        system("start https://cataas.com/cat");
                    }
                    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK && buttonDown[14] != 1) {
                        goto exit_while;
                    }
                    if (std::abs(state.Gamepad.sThumbRY) > 0) {
                    
                        short actualValue = state.Gamepad.sThumbRY;
                        inputs[16].type = INPUT_MOUSE;
                        inputs[16].mi.dwFlags = MOUSEEVENTF_WHEEL;
                        mouse_event(inputs[16].mi.dwFlags, inputs[16].mi.dx, inputs[16].mi.dy, state.Gamepad.sThumbRY * 0.001, inputs[16].mi.dwExtraInfo);
                    }

                    SetCursorPos(x_pos, y_pos);

                }


                UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
                if (uSent != ARRAYSIZE(inputs))
                {
                    std::cout << "Error ya mum" << std::endl;
                }

                if (
                    state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && buttonDown[10] != 1
                    &&
                    state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB && buttonDown[9] != 1
                    ) {


                    inputs[7].type = INPUT_KEYBOARD;
                    inputs[7].ki.dwFlags = XINPUT_KEYSTROKE_KEYDOWN;
                    keybd_event(VK_LWIN, 0x5B, inputs[7].ki.dwFlags, inputs[7].ki.dwExtraInfo);

                    inputs[5].type = INPUT_KEYBOARD;
                    inputs[5].ki.dwFlags = XINPUT_KEYSTROKE_KEYDOWN;
                    keybd_event(VK_SNAPSHOT, 0x2C, inputs[5].ki.dwFlags, inputs[5].ki.dwExtraInfo);

                    inputs[8].type = INPUT_KEYBOARD;
                    inputs[8].ki.dwFlags = XINPUT_KEYSTROKE_KEYUP;
                    keybd_event(VK_LWIN, 0x5B, inputs[8].ki.dwFlags, inputs[8].ki.dwExtraInfo);
                    inputs[6].type = INPUT_KEYBOARD;
                    inputs[6].ki.dwFlags = XINPUT_KEYSTROKE_KEYUP;
                    keybd_event(VK_SNAPSHOT, 0x2C, inputs[6].ki.dwFlags, inputs[6].ki.dwExtraInfo);


                    system("powershell -c (New-Object Media.SoundPlayer \"C:\\Windows\\Media\\tada.wav\").PlaySync();");
                    
                }


                buttonDown[0] = (state.Gamepad.bRightTrigger > 0) ? 1: 0;
                buttonDown[1] = (state.Gamepad.bLeftTrigger > 0) ? 1 : 0;
                buttonDown[2] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 1 : 0;
                buttonDown[3] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 1 : 0;
                buttonDown[4] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 1 : 0;
                buttonDown[5] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;
                buttonDown[6] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
                buttonDown[7] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
                buttonDown[8] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
                buttonDown[9] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
                buttonDown[10] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;
                buttonDown[11] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
                buttonDown[12] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;
                buttonDown[13] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 1 : 0;
                buttonDown[14] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0;
                buttonDown[15] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0;
                buttonDown[16] = (std::abs(state.Gamepad.sThumbRY) > 0) ? 1 : 0;


  
                ZeroMemory(inputs, sizeof(inputs));
            }

        }
    }

    exit_while:

    return 0;
}