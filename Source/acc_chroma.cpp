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
//SMElement m_physics;
//SMElement m_static;

INIT Init = nullptr;
UNINIT UnInit = nullptr;
CREATEEFFECT CreateEffect = nullptr;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
SETEFFECT SetEffect = nullptr;
DELETEEFFECT DeleteEffect = nullptr;
QUERYDEVICE QueryDevice = nullptr;

BOOL My_Chroma_Implementation::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != nullptr)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = QueryDevice(DeviceId, DeviceInfo);

		return DeviceInfo.Connected;
	}

	return FALSE;
}

My_Chroma_Implementation::My_Chroma_Implementation() :m_ChromaSDKModule(nullptr)
{
}
My_Chroma_Implementation::~My_Chroma_Implementation()
{
}

BOOL My_Chroma_Implementation::Initialize()
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

void My_Chroma_Implementation::ResetEffects(size_t DeviceType)
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

int My_Chroma_Implementation::keyboard_effect(int flag)
{
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Example_keyboard_effect = {}; //Initialize
	AC_FLAG_TYPE flag_colors[] = { BLACK, BLUE, YELLOW, BLACK, WHITE, WHITE, WHITE, GREEN, ORANGE };
	
	if (flag == 5)
	{
		for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((row + col) % 3 == 0)
					Example_keyboard_effect.Color[row][col] = flag_colors[flag];
	}
	else if (flag == 6)
	{
		for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if (col > (ChromaSDK::Keyboard::MAX_ROW - row) * 5 - 5)
					Example_keyboard_effect.Color[row][col] = flag_colors[flag];
	}
	else if (flag == 8)
	{
		for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				if ((col - ChromaSDK::Keyboard::MAX_COLUMN / 2) * (col - ChromaSDK::Keyboard::MAX_COLUMN / 2) + (row - ChromaSDK::Keyboard::MAX_ROW / 2) * (row - ChromaSDK::Keyboard::MAX_ROW / 2) <= 16)
					Example_keyboard_effect.Color[row][col] = flag_colors[flag];
	}
	else
	{
		for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
			for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
				Example_keyboard_effect.Color[row][col] = flag_colors[flag];
	}

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Example_keyboard_effect, nullptr);
	return Result_Keyboard;
	
	return 0;
}

void exitHandler(int sig)
{
	signal(sig, SIG_IGN);
	dismiss(m_graphics);
	exit(0);
}

int _tmain(int argc, _TCHAR* argv[])
{

	//everything that is commented, you do not need. I have left it in for my own ease.

	//initPhysics();
	initGraphics();
	//initStatic();

	cout << "Starting...\n";
	signal(SIGINT, exitHandler);
	My_Chroma_Implementation impl_test; //Initialize ChromaSDK

	auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices

	if (test_for_init == TRUE)
	{
		cout << "ACC Chroma Initialized.\n";
		int old_flag = 0;
		while (true) 
		{
			SPageFileGraphic* flag_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
			if (flag_data->flag != old_flag)
			{
				auto Keyboard = impl_test.keyboard_effect(flag_data->flag);
				old_flag = flag_data->flag;
			}
			
			sleep_for(std::chrono::milliseconds(200));
		}
	}
	else
	{
		cout << "Unable to initialize ACC Chroma.\n";
		cin.ignore();
	}

	return 0;
}
