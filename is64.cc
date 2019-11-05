#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <algorithm>

std::string read_proc()
{
    FILE *file = fopen("/proc/cpuinfo", "r");

    if (file == nullptr)
    {
    	return "";
    }

    std::string ret;

    const size_t BUFF_SIZE = 1024;
    char buff[BUFF_SIZE + 1];

    while(false == feof(file))
    {
        int nread = fread(buff, 1, BUFF_SIZE, file);

        buff[nread] = '\0';

        ret += buff;   	
    }

    return ret;
}

bool is_64()
{
    std::string proc_data = read_proc();

    std::istringstream sstrm(proc_data);

    std::string line;
    while (std::getline(sstrm, line, '\n'))
    {
         size_t sep_pos = line.find(":");
	 if (sep_pos == std::string::npos)
	 {
	     continue;
	 }

	 std::string key = line.substr(0, sep_pos);
	 std::string value = line.substr(sep_pos + 1);

         auto rev_it = std::find_if(key.rbegin(), key.rend(), [](char Ch) { return !std::isspace(Ch);});

         key.erase(rev_it.base(), key.end());

	 if (value.find(" lm ") != std::string::npos)
            return true;
    }

    return false;
}

int main(int argc , char ** argv)
{

    //long word_bits = sysconf(_SC_WORD_BIT);

    //printf("%ld\n%ld\n", word_bits, sizeof(void*) * 8);
    std::cout << is_64() << std::endl;
    return 0;
}
