/*
 * checkTable.cpp
 *
 *  Created on: 2012/10/07
 *      Author: ryosuke
 */

#include <iostream>

#include "Develop/checkTableGenerator.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "ERROR: require an argument.\n";
	}
	Develop::CheckTableGenerator codeGen;
	codeGen.write(argv[1]);
}
