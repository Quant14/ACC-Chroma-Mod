
#ifndef _RZCHROMASDK_H_
#define _RZCHROMASDK_H_

#pragma once

#include "RzErrors.h"
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Headset;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::ChromaLink;

// Exported functions
/*
//not usable with the sample application afaik
#ifdef __cplusplus
extern "C"
{
#endif

    RZRESULT Init(void);

    RZRESULT UnInit(void);

    RZRESULT CreateEffect(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateKeyboardEffect(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateMouseEffect(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateHeadsetEffect(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateMousepadEffect(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateKeypadEffect(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT CreateChromaLinkEffect(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

    RZRESULT DeleteEffect(RZEFFECTID EffectId);

    RZRESULT SetEffect(RZEFFECTID EffectId);

    RZRESULT RegisterEventNotification(HWND hWnd);

    RZRESULT UnregisterEventNotification();

    RZRESULT QueryDevice(RZDEVICEID DeviceId, DEVICE_INFO_TYPE& DeviceInfo);

#ifdef __cplusplus
}
#endif

//*/

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATECHROMALINKEFFECT)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
//typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);
INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
CREATEMOUSEEFFECT CreateMouseEffect = NULL;
CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
CREATEMOUSEPADEFFECT CreateMousematEffect = NULL;
CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
CREATECHROMALINKEFFECT CreateChromaLinkEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;
#endif