#include "acc_chroma.h"
#include "acc_app.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif
using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace std;
using std::this_thread::sleep_for;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

SMElement m_graphics;
SMElement m_physics;
//SMElement m_static;

INIT Init = nullptr;
UNINIT UnInit = nullptr;
CREATEEFFECT CreateEffect = nullptr;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
SETEFFECT SetEffect = nullptr;
DELETEEFFECT DeleteEffect = nullptr;
QUERYDEVICE QueryDevice = nullptr;

BOOL ACC_Chroma::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != nullptr)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = QueryDevice(DeviceId, DeviceInfo);

		return DeviceInfo.Connected;
	}

	return FALSE;
}

ACC_Chroma::ACC_Chroma() :m_ChromaSDKModule(nullptr)
{
}
ACC_Chroma::~ACC_Chroma()
{
}

BOOL ACC_Chroma::Initialize()
{
	if (m_ChromaSDKModule == nullptr)
	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == nullptr)
			return FALSE;
	}

	if (Init == nullptr)
	{
		auto Result = RZRESULT_INVALID;
		Init = reinterpret_cast<INIT>(GetProcAddress(m_ChromaSDKModule, "Init"));
		if (Init)
		{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)
			{
				CreateEffect = reinterpret_cast<CREATEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateEffect"));
				CreateKeyboardEffect = reinterpret_cast<CREATEKEYBOARDEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect"));
				SetEffect = reinterpret_cast<SETEFFECT>(GetProcAddress(m_ChromaSDKModule, "SetEffect"));
				DeleteEffect = reinterpret_cast<DELETEEFFECT>(GetProcAddress(m_ChromaSDKModule, "DeleteEffect"));
				QueryDevice = reinterpret_cast<QUERYDEVICE>(GetProcAddress(m_ChromaSDKModule, "QueryDevice"));

				if (CreateEffect && CreateKeyboardEffect && SetEffect && DeleteEffect && QueryDevice)
					return TRUE;
				else
					return FALSE;
			}
		}
	}

	return TRUE;
}

void ACC_Chroma::ResetEffects(size_t DeviceType)
{
	switch (DeviceType)
	{
	case 0:
		if (CreateKeyboardEffect)
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		break;
	case 1:
		if (CreateKeyboardEffect)
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		break;
	}
}

int ACC_Chroma::flag_keyboard_effect(int flag, int pit_limiter)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE flag_effect = {}; //Initialize
	AC_FLAG_TYPE flag_colors[] = { BLACK, BLUE, YELLOW, BLACK, WHITE, WHITE, WHITE, GREEN, ORANGE };
	int start_row = 0;
	if (pit_limiter)
		start_row = 2;
	
	if (flag == 5)
	{
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((row + col) % 3 == 0)
					flag_effect.Color[row][col] = flag_colors[flag];
	}
	else if (flag == 6)
	{
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if (col > (ChromaSDK::Keyboard::MAX_ROW - row) * 5 - 5)
					flag_effect.Color[row][col] = flag_colors[flag];
	}
	else if (flag == 8)
	{
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((col - ChromaSDK::Keyboard::MAX_COLUMN / 2) * (col - ChromaSDK::Keyboard::MAX_COLUMN / 2) + (row - ChromaSDK::Keyboard::MAX_ROW / 2) * (row - ChromaSDK::Keyboard::MAX_ROW / 2) <= 16)
					flag_effect.Color[row][col] = flag_colors[flag];
	}
	else
	{
		for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				flag_effect.Color[row][col] = flag_colors[flag];
	}

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &flag_effect, nullptr);
	return Result_Keyboard;
}

int ACC_Chroma::pit_limiter_effect(int pit_limiter, int is_pit_light_on)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE pit_effect = {};
	
	if (pit_limiter && !is_pit_light_on)
	{
		for (size_t row = 0; row < 2; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				pit_effect.Color[row][col] = PIT_BLUE;
	}
	else
	{
		for (size_t row = 0; row < 2; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				pit_effect.Color[row][col] = BLACK;
	}
	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &pit_effect, nullptr);
	return Result_Keyboard;
}

int ACC_Chroma::yellow1_effect(int yellow1, int pit_limiter)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE yellow1_effect = {};
	int start_row = 0;
	if (pit_limiter)
		start_row = 2;

	for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
		for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN / 3; col++)
			yellow1_effect.Color[row][col] = YELLOW;

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &yellow1_effect, nullptr);
	return Result_Keyboard;
}

int ACC_Chroma::yellow2_effect(int yellow2, int pit_limiter)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE yellow2_effect = {};
	int start_row = 0;
	if (pit_limiter)
		start_row = 2;

	for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
		for (size_t col = ChromaSDK::Keyboard::MAX_COLUMN / 3; col < ChromaSDK::Keyboard::MAX_COLUMN / 1.5; col++)
			yellow2_effect.Color[row][col] = YELLOW;

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &yellow2_effect, nullptr);
	return Result_Keyboard;
}

int ACC_Chroma::yellow3_effect(int yellow3, int pit_limiter)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE yellow3_effect = {};
	int start_row = 0;
	if (pit_limiter)
		start_row = 2;

	for (size_t row = start_row; row < ChromaSDK::Keyboard::MAX_ROW; row++)
		for (size_t col = ChromaSDK::Keyboard::MAX_COLUMN / 1.5; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
			yellow3_effect.Color[row][col] = YELLOW;

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &yellow3_effect, nullptr);
	return Result_Keyboard;
}

void exitHandler(int sig)
{
	signal(sig, SIG_IGN);
	dismiss(m_graphics);
	exit(0);
}

int _tmain(int argc, _TCHAR* argv[])
{

	initPhysics();
	initGraphics();
	//initStatic();

	cout << "Starting...\n";
	signal(SIGINT, exitHandler);
	ACC_Chroma impl_test; //Initialize ChromaSDK

	auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices

	if (test_for_init == TRUE)
	{
		cout << "ACC Chroma Initialized.\n";
		SPageFileGraphic* graphic_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		int old_flag = 0;
		SPageFilePhysics* physics_data = (SPageFilePhysics*)m_physics.mapFileBuffer;
		int old_yellow1 = 0;
		int old_yellow2 = 0;
		int old_yellow3 = 0;
		int old_pit_limiter = 0;
		int is_pit_light_on = 0;
		while (true) 
		{
			graphic_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
			physics_data = (SPageFilePhysics*)m_physics.mapFileBuffer;

			if (graphic_data->flag != old_flag)
			{
				auto Keyboard = impl_test.flag_keyboard_effect(graphic_data->flag, physics_data->pitLimiterOn);
				old_flag = graphic_data->flag;
			}

			if (graphic_data->GlobalYellow1 != old_yellow1 && graphic_data->flag != 2)
			{
				if (graphic_data->GlobalYellow1)
					auto Keyboard = impl_test.yellow1_effect(graphic_data->GlobalYellow1, physics_data->pitLimiterOn);
				else
					auto Keyboard = impl_test.flag_keyboard_effect(0, physics_data->pitLimiterOn);
				old_yellow1 = graphic_data->GlobalYellow1;
			}

			if (graphic_data->GlobalYellow2 != old_yellow2 && graphic_data->flag != 2)
			{
				if (graphic_data->GlobalYellow2)
					auto Keyboard = impl_test.yellow2_effect(graphic_data->GlobalYellow2, physics_data->pitLimiterOn);
				else
					auto Keyboard = impl_test.flag_keyboard_effect(0, physics_data->pitLimiterOn);
				old_yellow2 = graphic_data->GlobalYellow2;
			}

			if (graphic_data->GlobalYellow3 != old_yellow3 && graphic_data->flag != 2)
			{
				if (graphic_data->GlobalYellow3)
					auto Keyboard = impl_test.yellow3_effect(graphic_data->GlobalYellow3, physics_data->pitLimiterOn);
				else
					auto Keyboard = impl_test.flag_keyboard_effect(0, physics_data->pitLimiterOn);
				old_yellow3 = graphic_data->GlobalYellow3;
			}

			if (physics_data->pitLimiterOn || old_pit_limiter)
			{
				auto Keyboard = impl_test.pit_limiter_effect(physics_data->pitLimiterOn, is_pit_light_on);
				if (physics_data->pitLimiterOn && !is_pit_light_on)
					is_pit_light_on = 1;
				else
					is_pit_light_on = 0;
				old_pit_limiter = physics_data->pitLimiterOn;
			}

			sleep_for(std::chrono::milliseconds(300));
		}
	}
	else
	{
		cout << "Unable to initialize ACC Chroma.\n";
		cin.ignore();
	}

	return 0;
}
