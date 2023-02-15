#include "main.h"

Chroma_Handler::Chroma_Handler() {
	exit = false;
};

void Chroma_Handler::dismissSharedMemory() {
	dismiss(m_graphics);
	dismiss(m_physics);
}

Chroma_Handler* handler = new Chroma_Handler();

void exitHandler(int a) {
	handler->exit = true;
	//handler->dismissSharedMemory();
}

int _tmain(int argc, _TCHAR* argv[]) {
	signal(SIGBREAK, &exitHandler);

	initPhysics();
	initGraphics();

	cout << "Starting...\n";
	//ACC_Chroma impl_test; //Initialize ChromaSDK

	//auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices

	if (handler->chroma.Initialize()) {
		cout << "ACC Chroma Initialized." << endl
			<< "Feel free to contact me through RaceDepartment." << endl;

		SPageFileGraphic* graphic_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		SPageFilePhysics* physics_data = (SPageFilePhysics*)m_physics.mapFileBuffer;

		int old_pit_limiter = 0;
		int is_pit_light_on = 0;
		int old_flag = 0;

		auto Keyboard = handler->chroma.init_keyboard_effect();

		while (true) {
			graphic_data = (SPageFileGraphic*)m_graphics.mapFileBuffer;
			physics_data = (SPageFilePhysics*)m_physics.mapFileBuffer;

			if (graphic_data->flag != old_flag) {
				auto Keyboard = handler->chroma.flag_keyboard_effect(graphic_data->flag, physics_data->pitLimiterOn);

				if (graphic_data->flag != AC_CHECKERED_FLAG && graphic_data->flag != AC_ORANGE_FLAG) {
					auto Mouse = handler->chroma.flag_mouse_effect(graphic_data->flag);
					auto Mousepad = handler->chroma.flag_mousepad_effect(graphic_data->flag);
				}
				old_flag = graphic_data->flag;
			}

			if (physics_data->pitLimiterOn || old_pit_limiter) {
				auto Keyboard = handler->chroma.pit_limiter_effect(physics_data->pitLimiterOn, is_pit_light_on);

				if (physics_data->pitLimiterOn && !is_pit_light_on)
					is_pit_light_on = 1;
				else
					is_pit_light_on = 0;

				old_pit_limiter = physics_data->pitLimiterOn;
			}
			if (handler->exit) {
				handler->dismissSharedMemory();
				break;
			}
			sleep_for(std::chrono::milliseconds(300));
		}
	}
	else {
		cout << "Failed to initialize ACC Chroma." << endl
			<< "Make sure Razer Synapse and Chroma Connect are running." << endl
			<< "For further questions feel free to contact me on RaceDepartment.";
		cin.ignore();
		handler->dismissSharedMemory();
	}

	return 0;
}