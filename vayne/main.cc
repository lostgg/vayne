
#include "stdafx.h"
#include "app.h"
#include <iostream>

int main(int argc, char *argv[])
{
	// »ñÈ¡ homepath Ä¿Â¼
	char *dive, *filename;
	char filepath[MAX_PATH];
	size_t size = 0;

	_dupenv_s(&dive, &size, "HOMEDRIVE");
	_dupenv_s(&filename, &size, "HOMEPATH");

	sprintf_s(filepath, "%s%s\\autodwg_offline.lic", dive, filename);

	delete filename;
	delete dive;
	/*std::size_t r = ServerApp::GetInctance()->run();
	std::cout << "Program exit unfinished events:" << r << std::endl;*/
	return 0;
}
