#pragma once
#include "include.h"
#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_


#pragma once

#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 


const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF PIT_BLUE = RGB(0, 0, 220);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF ORANGE = RGB(255, 165, 00);

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEPAD_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5

class ACC_Chroma
{
public:
	ACC_Chroma();
	~ACC_Chroma();
	BOOL Initialize();

	int flag_mousepad_effect(int flag);
	int flag_mouse_effect(int flag);
	int flag_keyboard_effect(int flag, int pit_limiter);
	int pit_limiter_effect(int pit_limiter, int is_pit_light_on);

	BOOL IsDeviceConnected(RZDEVICEID DeviceId);

private:
	HMODULE m_ChromaSDKModule;

};

#endif
