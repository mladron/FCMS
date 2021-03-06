﻿//
// Count-Min Sketch with Fingerprint test  
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "CMSF.hpp"


/*
 * test_setup stores the command line arguments
 */

struct test_setup {
	unsigned int width;
	short int depth;
	short int fingerprint_bits;
	short int conservative_update;
	short int fingerprint_limit;
	std::string inputKeys;
	std::string inputUniqueKeys;
	std::string frequency;
};


/*
 * sets the test setup with the command line arguments
 */

test_setup setup_CMSF_test(int argc, char* argv[]) {
	test_setup setup;

	setup.width = 0;
	setup.depth = -1;
	setup.fingerprint_bits = -1;
	setup.conservative_update = -1;
	setup.fingerprint_limit = -1;
	setup.inputKeys = "";
	setup.inputUniqueKeys = "";

	for (int i = 1; i < argc; i++) {
		std::stringstream str;
		str << argv[i];

		std::string argument = str.str();

		// argument 'w' is used for table width, default value is w=1048576 

		if (argument[0] == 'w' || argument[0] == 'W') {
			int len = argument.size();
			setup.width = std::stof(argument.substr(2, len - 1).c_str());
		}

		// argument 'd' is used for table depth, default value is d=2

		if (argument[0] == 'd' || argument[0] == 'D') {
			int len = argument.size();
			setup.depth = std::stof(argument.substr(2, len - 1).c_str());
		}

		// argument 'b' is used for fingerprint bits, default value is b=0, valid values are {4, 5, 6, 7, 8}

		if (argument[0] == 'b' || argument[0] == 'B') {
			int len = argument.size();
			setup.fingerprint_bits = std::stof(argument.substr(2, len - 1).c_str());
		}

		// argument 'c' is used for the conservative update, default value is c=0

		if (argument[0] == 'c' || argument[0] == 'C') {
			int len = argument.size();
			setup.conservative_update = std::stof(argument.substr(2, len - 1).c_str());
		}

		// argument 'u' is used for the fingerprint limit, default value is u=0, max value is 32767

		if (argument[0] == 'u' || argument[0] == 'U') {
			int len = argument.size();
			setup.fingerprint_limit = std::stof(argument.substr(2, len - 1).c_str());
		}

		// argument 'f' is used for the input file name

		if (argument[0] == 'f' || argument[0] == 'F') {
			int len = argument.size();
			setup.inputKeys = argument.substr(2, len - 1).c_str();
		}
	}

	// set default values for the arguments

	if (setup.width == 0)
		setup.width = 1048576;

	if (setup.depth == -1)
		setup.depth = 2;

	if (setup.fingerprint_bits == -1 || setup.fingerprint_bits == 0) {
		setup.fingerprint_bits = 0;
		setup.conservative_update = 0;
		setup.fingerprint_limit = 0;
	}
	else {
		// setup.fingerprint_bits valid values are {4, 5, 6, 7, 8}

		if (setup.fingerprint_bits < 4 || setup.fingerprint_bits > 8)
			setup.fingerprint_bits = 4;

		if (setup.conservative_update == -1)
			setup.conservative_update = 0;

		if (setup.fingerprint_limit == -1)
			setup.fingerprint_limit = 0;
	}

	if (setup.inputKeys.compare("") == 0) {
		setup.inputKeys = "1000_ips.txt";
		setup.inputUniqueKeys = "1000_ips_sorted_unique.txt";
		setup.frequency = "1000_ips_sorted_frequency.txt";
	}
	else {
		setup.inputUniqueKeys = setup.inputKeys.substr(0, setup.inputKeys.length() - 4) + "_sorted_unique.txt";
		setup.frequency = setup.inputKeys.substr(0, setup.inputKeys.length() - 4) + "_sorted_frequency.txt";
	}

	return setup;
}


/*
 * shows the test setup on the console
 */

void display_CMSF_test_parameters(test_setup setup) {
	std::cout << "CMSF - Count-Min Sketch with Fingerprint\n\n";

	std::cout << "Table width         w = " << setup.width << "\n";
	std::cout << "Table depth         d = " << setup.depth << "\n";
	std::cout << "Fingerprint bits    b = " << setup.fingerprint_bits << "\n";
	std::cout << "Conservative update c = " << setup.conservative_update << "\n";
	std::cout << "Fingerprint limit   u = " << setup.fingerprint_limit << "\n";
	std::cout << "Input keys          f = '" << setup.inputKeys << "', '" << setup.inputUniqueKeys << "', " << setup.frequency << "\n";
	std::cout << "\n";
}


/*
 * Count-Min Sketch with fingerprint test
 */

void CMSF_test(unsigned int width, short int depth, short int fingerprint_bits, short int conservative, short int fp_limit, std::string keys, std::string uniqueKeys, std::string frequency) {
	CMSF *cms = new CMSF(width, depth, 0, conservative, fp_limit);
	CMSF *cmf = new CMSF(width, depth, fingerprint_bits, conservative, fp_limit);

	// initialize the sketch using keys from input file

	std::ifstream inputFile;
	char ip[60];

	inputFile.open(keys);

	if (inputFile.is_open()) {
		while (!inputFile.eof()) {
			inputFile.getline(ip, 60);

			cms->update(ip);
			cmf->update(ip);
		}

		inputFile.close();
	}
	else
		std::cout << "Input file '" << keys << "' not found!\n";

	// get the sketch estimate for unique keys

	std::ofstream outputFile_CMS, outputFile_CMSF;

	std::stringstream ss;
	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_b" << fingerprint_bits << "_c" << conservative << "_u" << fp_limit << "_cms_estimates.txt";
	std::string cms_outputfile = ss.str();

	outputFile_CMS.open(cms_outputfile);

	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_b" << fingerprint_bits << "_c" << conservative << "_u" << fp_limit << "_cmsf_estimates.txt";
	std::string cmsf_outputfile = ss.str();

	outputFile_CMSF.open(cmsf_outputfile);

	double estimate;

	std::ifstream frequencyFile;
	int key_frequency;

	inputFile.open(uniqueKeys);
	frequencyFile.open(frequency);

	if (inputFile.is_open()) {
		while (!inputFile.eof()) {
			inputFile.getline(ip, 60);
			frequencyFile >> key_frequency;

			estimate = cms->estimate(ip);

			outputFile_CMS << estimate << "\t" << key_frequency << "\n";

			estimate = cmf->estimate(ip);

			outputFile_CMSF << estimate << "\t" << key_frequency << "\n";
		}

		inputFile.close();
		frequencyFile.close();
	}
	else
		std::cout << "Input file '" << uniqueKeys << "' not found!\n";

	outputFile_CMS.close();
	outputFile_CMSF.close();

	delete cms;
	delete cmf;
}


/*
 * validates the files used bu the test
 */

bool validateInputFiles(std::string keys, std::string uniqueKeys, std::string frequency) {
	std::ifstream inputFile;

	inputFile.open(keys);

	if (inputFile.is_open())
		inputFile.close();
	else {
		std::cout << "Input file '" << keys << "' not found!\n";

		return false;
	}

	inputFile.open(uniqueKeys);

	if (inputFile.is_open())
		inputFile.close();
	else {
		std::cout << "Input file '" << uniqueKeys << "' not found!\n";

		return false;
	}

	inputFile.open(frequency);

	if (inputFile.is_open())
		inputFile.close();
	else {
		std::cout << "Input file '" << frequency << "' not found!\n";

		return false;
	}

	return true;
}


int main(int argc, char **argv) {
	test_setup setup;

	// command line arguments for Count-Min Sketch with Fingerprint test program
	//
	// w: table width, default value is w=1048576
	// d: table depth or number of counters checked, default value is d=2
	// b: fingerprint bits, default value is 0 for standard Count-Min Sketch, when bits is not zero it
	//    indicates the bits used for the fingerprint related to the counter
	// c: conservative update {0: normal update, 1: conservative update}, default value is c=0
	// u: the fingerprint is used only if the counter is less than the given value, default value is u=0
	// f: input keys, default value is 1000_ips.txt
	//
	// example: cmsf.exe w=1048576 d=2, b=2, f=1000_ips.txt

	setup = setup_CMSF_test(argc, argv);

	// show Count-Min Sketch test parameters

	display_CMSF_test_parameters(setup);

	if (validateInputFiles(setup.inputKeys, setup.inputUniqueKeys, setup.frequency))
		CMSF_test(setup.width, setup.depth, setup.fingerprint_bits, setup.conservative_update, setup.fingerprint_limit, setup.inputKeys, setup.inputUniqueKeys, setup.frequency);

	return 0;
}
