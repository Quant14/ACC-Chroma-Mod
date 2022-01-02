#pragma once
#include "include.h"
#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_


#pragma once

#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 


// Define all Colours you want
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF PIT_BLUE = RGB(0, 0, 220);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);
//You dont have to define your colors as COLORREFs, just use the RGB(xxx,xxx,xxx) function like above

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEMAT_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5

//Class of your Chroma Implementation
class ACC_Chroma
{
public:
	ACC_Chroma();
	~ACC_Chroma();
	BOOL Initialize();


	void ResetEffects(size_t DeviceType);
	//Define your methods here
	int flag_keyboard_effect(int flag, int pit_limiter);
	int pit_limiter_effect(int pit_limiter, int is_pit_light_on);
	int yellow1_effect(int yellow1, int pit_limiter);
	int yellow2_effect(int yellow2, int pit_limiter);
	int yellow3_effect(int yellow3, int pit_limiter);

	BOOL IsDeviceConnected(RZDEVICEID DeviceId);

private:
	HMODULE m_ChromaSDKModule;

};

#endif
