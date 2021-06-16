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
SMElement m_static;

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

//----
//All the code above is provided by the developer example, you can try to understand it or just use it

//Let's start with a Keyboard effect

int My_Chroma_Implementation::keyboard_effect(int flag)
{

	//Choose one of the following methods by just deleting the first and last line :)

	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Example_keyboard_effect = {}; //Initialize
	AC_FLAG_TYPE flag_colors[] = { BLACK, BLUE, YELLOW, BLACK, WHITE, BLACK, BLACK, GREEN, ORANGE };
	//The keyboard effect is initialized as a 2 dimensional matrix/array
	//e.g. the ESC-key is [0][1]
	// Source: http://developer.razerzone.com/chroma/razer-chroma-led-profiles/
	// Take the super keyboard as standard, so your programm will work with every keyboard out of the box
	for (size_t row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++)
		for (size_t col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++)
			Example_keyboard_effect.Color[row][col] = flag_colors[flag];   //Filling the whole matrix with the color orange == Setting background to orange

	//Now we apply the effect to the keyboard
	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Example_keyboard_effect, nullptr);
	//You can work with the Result as well, e.g. checking if everythings ok
	return Result_Keyboard;

	/*For key-based-effect delete this line
	Keyboard::CUSTOM_KEY_EFFECT_TYPE Example_keyboard_effect_key = {};  //Initialize key-based Effect
	//The keyboard key effect is initialized as a 2 dimensional matrix/array, but this time called by the HIBYTE and the LOBYTE of the key itself
	//e.g. the ESC-key is [HIBYTE(RZKEY_ESC)][LOBYTE(RZKEY_ESC)]
	//Use this if you want to light up specific keys
	//The whole naming scheme can be found in Keyboard::RZKEY of the RzChromaSDKTypes.h
	//Important! Don't forget the 0x01000000 | before the used COLORREF, otherwise it won't work
	Example_keyboard_effect_key.Key[HIBYTE(RZKEY_ESC)][LOBYTE(RZKEY_ESC)] = 0x01000000 | BLUE; // ESC-key will light up blue
	Example_keyboard_effect_key.Key[HIBYTE(RZKEY_1)][LOBYTE(RZKEY_1)] = 0x01000000 | RED; // 1-key will light up red
	Example_keyboard_effect_key.Key[HIBYTE(RZKEY_NUMPAD3)][LOBYTE(RZKEY_NUMPAD3)] = 0x01000000 | GREEN; // Numpad-3-key will light up green
	//Now we apply the effect to the keyboard
	RZRESULT Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
	//You can work with the Result as well, e.g. checking if everythings ok
	return Result_Keyboard;
	For key-based-effect delete this line*/

	return 0;
}

/*BOOL My_Chroma_Implementation::example_mouse() {

	ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 Example_mouse_effect = {}; // Initialize

	//The mouse effect is initialized as a 2 dimensional matrix/array
	//e.g. the scroll wheel is [2][3]
	// Source: http://developer.razerzone.com/chroma/razer-chroma-led-profiles/
	// Take the super mouse as standard, so your programm will work with every mouse out of the box 

	for (size_t row = 0; row < ChromaSDK::Mouse::MAX_ROW; row++) {
		for (size_t col = 0; col < ChromaSDK::Mouse::MAX_COLUMN; col++) {
			Example_mouse_effect.Color[row][col] = ORANGE;   //Filling the whole matrix with the color orange == Setting background to orange
		}
	}

	Example_mouse_effect.Color[2][3] = RED; //The colormatrix can be overwritten, until you finally apply the effect to the keyboard
	Example_mouse_effect.Color[2][3] = BLUE; // Only the last state of the key will be applied. So the scroll wheel will be blue, not red and not orange

	//Now we apply the effect to the keyboard
	//auto Result_Mouse = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &Example_mouse_effect, nullptr);

	//You can work with the Result as well, e.g. checking if everythings ok

	//return Result_Mouse;
	return 0;

}



BOOL My_Chroma_Implementation::example_mousemat() {

	ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE Example_mousemat_effect = {}; // Initialize

	//The mousepad effect is initialized as a 1 dimensional matrix/array
	//e.g. the Razer logo is [14]
	// Source: http://developer.razerzone.com/chroma/razer-chroma-led-profiles/

	for (size_t count = 0; count < ChromaSDK::Mousepad::MAX_LEDS; count++) {
		Example_mousemat_effect.Color[count] = ORANGE;  //Filling the whole matrix with the color orange == Setting background to orange
	}

	//a little bit advancec ;-)
	//creating a simple(!) loading animation
	RZRESULT Result_Mousemat = 0;
	for (size_t count = 0; count < ChromaSDK::Mousepad::MAX_LEDS; count++)
	{
		Example_mousemat_effect.Color[count] = BLUE;
		Sleep(500);
		//Result_Mousemat = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &Example_mousemat_effect, nullptr);

	} //if you want to work with animation, take a look at the frames that the ChromaSDK provides :)

	return Result_Mousemat;
}*/

int _tmain(int argc, _TCHAR* argv[])
{

	//initPhysics();
	initGraphics();
	//initStatic();

	cout << "Starting...\n";
	My_Chroma_Implementation impl_test; //Initialize ChromaSDK

	auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices

	if (test_for_init == TRUE)
	{
		cout << "ACC Chroma Initialized.\n";
		//wcout << "Press 1 for physics, 2 for graphics, 3 for static" << endl;
		while (true) 
		{
			SPageFileGraphic* flag_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
			auto Keyboard = impl_test.keyboard_effect(flag_data->flag);
			/*if (GetAsyncKeyState(0x31) != 0) // user pressed 1
			{
				wcout << "---------------PHYSICS INFO---------------" << endl;
				SPageFilePhysics* pf = (SPageFilePhysics*)m_physics.mapFileBuffer;
				printData("acc G", pf->accG);
				printData("brake", pf->brake);
				printData("camber rad", pf->camberRAD);
				printData("damage", pf->carDamage);
				printData("car height", pf->cgHeight);
				printData("drs", pf->drs);
				printData("tc", pf->tc);
				printData("fuel", pf->fuel);
				printData("gas", pf->gas);
				printData("gear", pf->gear);
				printData("number of tyres out", pf->numberOfTyresOut);
				printData("packet id", pf->packetId);
				printData("heading", pf->heading);
				printData("pitch", pf->pitch);
				printData("roll", pf->roll);
				printData("rpms", pf->rpms);
				printData("speed kmh", pf->speedKmh);
				printData2("contact point", pf->tyreContactPoint);
				printData2("contact normal", pf->tyreContactNormal);
				printData2("contact heading", pf->tyreContactHeading);
				printData("steer ", pf->steerAngle);
				printData("suspension travel", pf->suspensionTravel);
				printData("tyre core temp", pf->tyreCoreTemperature);
				printData("tyre dirty level", pf->tyreDirtyLevel);
				printData("tyre wear", pf->tyreWear);
				printData("velocity", pf->velocity);
				printData("wheel angular speed", pf->wheelAngularSpeed);
				printData("wheel load", pf->wheelLoad);
				printData("wheel slip", pf->wheelSlip);
				printData("wheel pressure", pf->wheelsPressure);
			}

			if (GetAsyncKeyState(0x32) != 0) // user pressed 2
			{
				wcout << "---------------GRAPHICS INFO---------------" << endl;
				SPageFileGraphic* pf = (SPageFileGraphic*)m_graphics.mapFileBuffer;
				printData("packetID ", pf->packetId);
				printData("STATUS ", pf->status);
				printData("session", pf->session);
				printData("completed laps", pf->completedLaps);
				printData("position", pf->position);
				printData("current time s", pf->currentTime);
				printData("current time", pf->iCurrentTime);
				printData("last time s", pf->lastTime);
				printData("last time ", pf->iLastTime);
				printData("best time s", pf->bestTime);
				printData("best time", pf->iBestTime);
				printData("sessionTimeLeft", pf->sessionTimeLeft);
				printData("distanceTraveled", pf->distanceTraveled);
				printData("isInPit", pf->isInPit);
				printData("currentSectorIndex", pf->currentSectorIndex);
				printData("lastSectorTime", pf->lastSectorTime);
				printData("numberOfLaps", pf->numberOfLaps);
				wcout << "TYRE COMPOUND : " << pf->tyreCompound << endl;
				printData("replayMult", pf->replayTimeMultiplier);
				printData("normalizedCarPosition", pf->normalizedCarPosition);
				printData2("carCoordinates", pf->carCoordinates);
			}

			if (GetAsyncKeyState(0x33) != 0) // user pressed 3
			{
				wcout << "---------------STATIC INFO---------------" << endl;
				SPageFileStatic* pf = (SPageFileStatic*)m_static.mapFileBuffer;
				wcout << "SM VERSION " << pf->smVersion << endl;
				wcout << "AC VERSION " << pf->acVersion << endl;

				printData("number of sessions ", pf->numberOfSessions);
				printData("numCars", pf->numCars);
				wcout << "Car model " << pf->carModel << endl;
				wcout << "Car track " << pf->track << endl;
				wcout << "Player Name " << pf->playerName << endl;
				printData("sectorCount", pf->sectorCount);

				printData("maxTorque", pf->maxTorque);
				printData("maxPower", pf->maxPower);
				printData("maxRpm", pf->maxRpm);
				printData("maxFuel", pf->maxFuel);
				printData("suspensionMaxTravel", pf->suspensionMaxTravel);
				printData("tyreRadius", pf->tyreRadius);
			}*/
			sleep_for(std::chrono::milliseconds(500));
		}
	}
	else
	{
		cout << "Unable to initialize ACC Chroma.\n";
		cin.ignore();
	}

	dismiss(m_graphics);
	dismiss(m_physics);
	dismiss(m_static);

	return 0;
}

/*#include <Windows.h>
#include "RzChromaSDK.h"

int main() {
    //CHROMA SDK
    auto CHROMA_LIB64 = LoadLibrary(TEXT("RzChromaSDK64.dll"));
    if (!CHROMA_LIB64)
        return 0;
    CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)::GetProcAddress(CHROMA_LIB64, "CreateKeyboardEffect");
    Init = (INIT)::GetProcAddress(CHROMA_LIB64, "Init");
    SetEffect = (SETEFFECT)::GetProcAddress(CHROMA_LIB64, "SetEffect");
    DeleteEffect = (DELETEEFFECT)::GetProcAddress(CHROMA_LIB64, "DeleteEffect");

    Init();

    FreeLibrary(CHROMA_LIB64);

    return 0;
}*/