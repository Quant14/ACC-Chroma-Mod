#pragma once
#include "acc_chroma.h"
#include "acc_app.h"

class Chroma_Handler {
public:
	ACC_Chroma chroma;
	bool exit;

	Chroma_Handler();
	void dismissSharedMemory();
};

void exitHandler(int a);