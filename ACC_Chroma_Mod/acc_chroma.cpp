#include "acc_chroma.h"

SMElement m_graphics;
SMElement m_physics;

INIT Init = nullptr;
UNINIT UnInit = nullptr;
CREATEEFFECT CreateEffect = nullptr;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
CREATEMOUSEEFFECT CreateMouseEffect = nullptr;
CREATEMOUSEPADEFFECT CreateMousepadEffect = nullptr;
SETEFFECT SetEffect = nullptr;
DELETEEFFECT DeleteEffect = nullptr;
QUERYDEVICE QueryDevice = nullptr;

BOOL ACC_Chroma::IsDeviceConnected(RZDEVICEID DeviceId) {
	if (QueryDevice != nullptr) {
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = QueryDevice(DeviceId, DeviceInfo);

		return DeviceInfo.Connected;
	}

	return FALSE;
}

ACC_Chroma::ACC_Chroma() :m_ChromaSDKModule(nullptr) {
}
ACC_Chroma::~ACC_Chroma() {
}

BOOL ACC_Chroma::Initialize() {
	if (m_ChromaSDKModule == nullptr) {
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == nullptr)
			return FALSE;
	}

	if (Init == nullptr) {
		auto Result = RZRESULT_INVALID;
		Init = reinterpret_cast<INIT>(GetProcAddress(m_ChromaSDKModule, "Init"));
		if (Init) {
			Result = Init();
			if (Result == RZRESULT_SUCCESS) {
				CreateEffect = reinterpret_cast<CREATEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateEffect"));
				CreateMousepadEffect = reinterpret_cast<CREATEMOUSEPADEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect"));
				CreateMouseEffect = reinterpret_cast<CREATEMOUSEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect"));
				CreateKeyboardEffect = reinterpret_cast<CREATEKEYBOARDEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect"));
				SetEffect = reinterpret_cast<SETEFFECT>(GetProcAddress(m_ChromaSDKModule, "SetEffect"));
				DeleteEffect = reinterpret_cast<DELETEEFFECT>(GetProcAddress(m_ChromaSDKModule, "DeleteEffect"));
				QueryDevice = reinterpret_cast<QUERYDEVICE>(GetProcAddress(m_ChromaSDKModule, "QueryDevice"));

				if (CreateEffect && CreateMousepadEffect && CreateMouseEffect && CreateKeyboardEffect && SetEffect && DeleteEffect && QueryDevice)
					return TRUE;
				else
					return FALSE;
			}
		}
	}

	return TRUE;
}

int ACC_Chroma::flag_mousepad_effect(int flag) {

	ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE flag_mousepad_effect = {}; // Initialize
	INIT_FLAGS

	for (int i = 0; i < ChromaSDK::Mousepad::MAX_LEDS; i++)
		flag_mousepad_effect.Color[i] = flag_colors[flag];

	return CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &flag_mousepad_effect, nullptr);
}

int ACC_Chroma::flag_mouse_effect(int flag) {

	ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 flag_effect = {}; // Initialize
	INIT_FLAGS

	for (size_t row = 0; row < ChromaSDK::Mouse::MAX_ROW; row++)
		for (size_t col = 0; col < ChromaSDK::Mouse::MAX_COLUMN; col++)
			flag_effect.Color[row][col] = flag_colors[flag];

	return CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM, &flag_effect, nullptr);
}

int ACC_Chroma::init_keyboard_effect() {
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE init_effect = {}; //Initialize

	for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
		for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
			init_effect.Color[row][col] = BLACK;

	return CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &init_effect, nullptr);
}

int ACC_Chroma::flag_keyboard_effect(int flag, int pit_limiter) {
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE flag_effect = {}; //Initialize
	INIT_FLAGS

	int start_row = pit_limiter * 2;
	
	if (flag == AC_CHECKERED_FLAG) {
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((row + col) % 3 == 0)
					flag_effect.Color[row][col] = flag_colors[flag];
	}
	else if (flag == AC_ORANGE_FLAG) {
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((col - ChromaSDK::Keyboard::MAX_COLUMN / 2) * (col - ChromaSDK::Keyboard::MAX_COLUMN / 2) + (row - ChromaSDK::Keyboard::MAX_ROW / 2) * (row - ChromaSDK::Keyboard::MAX_ROW / 2) <= 16)
					flag_effect.Color[row][col] = flag_colors[flag];
	}
	else { //all other flags
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				flag_effect.Color[row][col] = flag_colors[flag];
	}

	return CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &flag_effect, nullptr);
}

int ACC_Chroma::pit_limiter_effect(int pit_limiter, int is_pit_light_on) {
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE pit_effect = {};
	
	if (pit_limiter && !is_pit_light_on) {
		for (size_t row = 0; row < 2; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				pit_effect.Color[row][col] = PIT_BLUE;
	}
	else {
		for (size_t row = 0; row < 2; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				pit_effect.Color[row][col] = BLACK;
	}

	return CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &pit_effect, nullptr);
}