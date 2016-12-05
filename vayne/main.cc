
#include "stdafx.h"
#include "app.h"
#include "base/package.hpp"
#include "protocol/app/connect_proto_.hpp"
 

int main(int argc, char *argv[])
{
	/*lfcd::Package pk;
	int b = 281487;
	pk.set(b);
	auto t = pk.get<int>();
	std::cout << std::get<0>(t) << std::endl;*/
	std::size_t r = ServerApp::GetInctance()->run();
	std::cout << "Program exit unfinished events:" << r << std::endl;
	return 0;
}
