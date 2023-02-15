#pragma once
#include "include.h"
#include "acc_app.h"

#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 

#define INIT_FLAGS AC_FLAG_TYPE flag_colors[] = { BLACK, BLUE, YELLOW, BLACK, WHITE, WHITE, WHITE, GREEN, ORANGE };

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEPAD_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF PIT_BLUE = RGB(0, 0, 220);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF ORANGE = RGB(255, 165, 00);

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace std;
using std::this_thread::sleep_for;

class ACC_Chroma {
public:
	ACC_Chroma();
	~ACC_Chroma();
	BOOL Initialize();

	int flag_mousepad_effect(int flag);
	int flag_mouse_effect(int flag);
	int init_keyboard_effect();
	int flag_keyboard_effect(int flag, int pit_limiter);
	int pit_limiter_effect(int pit_limiter, int is_pit_light_on);

	BOOL IsDeviceConnected(RZDEVICEID DeviceId);

private:
	HMODULE m_ChromaSDKModule;

};