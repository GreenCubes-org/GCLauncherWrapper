//
//    GreenCubes launcher wrapper for POSIX (*nix) OSs.
//    Currently supports only x86-64 java version.
//    
//    Compile with g++:
//    g++ -m32 greencubes.cpp -o greencubes
//

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <errno.h>

std::string java64 = "./jre/64/bin/java";
std::string java32 = "./jre/32/bin/java";

#define PARAMS_COUNT 4

std::string launchParams[PARAMS_COUNT] = {"-Xms64M", "-Xmx128M", "-jar", "greencubes.jar"}; // Java arguments, argv will be passed to java program

int main(int argc, char** argv)
{
	std::string javaExe = java64; // We do not support x86-32 version

	char* arguments[PARAMS_COUNT + (argc - 1) + 1 + 1]; // launchParams length + program name + argc + NULL
	
	arguments[0] = (char *)(javaExe.c_str());

	for(int i = 0; i < PARAMS_COUNT; ++i) {
		arguments[i + 1] = (char *)(launchParams[i].c_str());
	}
	for(int i = 1; i < argc; ++i) {
		arguments[i - 1 + 1 + PARAMS_COUNT] = argv[i];
	}

	arguments[PARAMS_COUNT + argc - 1 + 1 - 1 + 1] = NULL;
	if(execv(arguments[0], arguments) == -1) {
		printf("Error: %d\n", errno);
	}
	return 0;
}
