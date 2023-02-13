#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

#define STR(x) #x
#define XSTR(x) STR(x)

#define DEFAULT_COUNT 100
#define DEFAULT_NAME "out"
#define DEFAULT_MIN 0
#define DEFAULT_MAX 100
#define DEFAULT_WRITEC true

static const char* help_message = 
"\nrngen - creates a file containing numbers generated using the Mersenne Twister 19937 generator.\n"
"\n"
"Usage:\n"
"\trngen [options] <name>\n"
"\n"
"Options:\n"
"\t-h, -?\t\tprint this message and return.\n"
"\t-c <number>\tsets number count (default value is " XSTR(DEFAULT_COUNT) ").\n"
"\t-m <number>\tsets number lower bound (default value is " XSTR(DEFAULT_MIN) ").\n"
"\t-M <number>\tsets number higher bound (default value is " XSTR(DEFAULT_MAX) ").\n"
"\t-s <number>\tsets the rng seed (default value is set by time).\n"
"\t-n\t\twrites the number count on the first row (default value is " XSTR(DEFAULT_WRITEC) ").\n";

int main(int argc, char **argv)
{
	size_t count = DEFAULT_COUNT;
	uint_fast32_t seed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	).count();
	std::string file_name = DEFAULT_NAME;
	int min = DEFAULT_MIN;
	int max = DEFAULT_MAX;
	bool writec = false;
	
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (argv[i][2] != 0) goto help;
			switch(argv[i][1]) {
				case 'c':
					if (argc < i+1) goto help;
					count = std::stoul(argv[++i]);
					break;
				case 'm':
					if (argc < i+1) goto help;
					min = std::stoul(argv[++i]);
					break;
				case 'M':
					if (argc < i+1) goto help;
					max = std::stoul(argv[++i]);
					break;
				case 'n':
					writec = true;
					break;
				case 's':
					if (argc < i+1) goto help;
					seed = std::stoul(argv[++i]);
					break;
				default:
					goto help;
			}
		}
		else file_name = argv[i];
	}

	{
		std::ofstream fout(file_name);
		if (!fout.is_open()) {
			std::cerr << "ERROR: could not open file\n";
			return -1;
		}

		std::uniform_int_distribution<int> distr(min, max);
		std::mt19937 gen(seed);

		if (writec) fout << count << '\n';
		for(uint_fast32_t i = 0; i < count; ++i)
        		fout << distr(gen) << ' ';

		fout.close();
	}

	return 0;

	help:
	std::cout << help_message;
	return 0;
}