// TestLogger.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"


int main()
{
	Log.start();
	for (int i = 0; i < 100000; i++) {
		Log(MessageTypes::DEBUG) << LoggerFormat::format("Hello %", i);
		Log(MessageTypes::ERR) << LoggerFormat::format("Hello %", i);
	}
    return 0;
}

