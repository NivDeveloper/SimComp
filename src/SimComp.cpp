/**
 * @file SimComp.cpp
 * @author Nivan Poken (PKNNIV001@myuct.ac.za)
 * @brief A simple compression/decompression program
 * @version 0.1
 * @date 2023-03-19
 */

#include <cstring>
#include <cstdio>
#include <iostream>
#include <chrono>
#include "utils.h"
#include "zlib.h"

// taken from https://zlib.net/zlib_how.html Usage example ///////////////
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384
///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

    // if user uses compression flag
    if (argc >= 5 && strcmp(argv[1], "-c") == 0 && strcmp(argv[3], "-o") == 0)
    {
        std::cout << "compressing file..." << std::endl;
        FILE* in = fopen(argv[2], "r+");
        FILE* out = fopen(argv[4], "w");
        int level = Z_DEFAULT_COMPRESSION;  //set default compression level

        if (argc == 7 && strcmp(argv[5], "-l") == 0) level = std::stoi(argv[6]);
        
        //measure time taken for compression
        auto start = std::chrono::steady_clock::now();
        int comp = utils::compress(in, out, level);
        auto stop = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (comp != Z_OK)   //check if compression successful
        {
            utils::zlibError(comp);
            return -1;
        }

        //calculate size of decompressed and compressed files
        fseek(in, 0L, SEEK_END);
        int insize = ftell(in);
        fseek(out, 0L, SEEK_END);
        int outsize = ftell(out);

        fclose(in);
        fclose(out);

        //print diagnostics
        utils::compDiagnostics(argv[2], argv[4], insize, outsize, level, elapsed);
    }
    else if (argc == 5 && strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-o") == 0) // if user selects decompression flag
    {
        //call decompression function
        std::cout << "decompressing file..." << std::endl;
        FILE* in = fopen(argv[2], "r+");
        FILE* out = fopen(argv[4], "w");

        //measure time taken for decompression
        auto start = std::chrono::steady_clock::now();
        int comp = utils::decompress(in, out);
        auto stop = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (comp != Z_OK) //check if decompression was successfull
        {
            utils::zlibError(comp);
            return -1;
        }

        //measure size of files
        fseek(in, 0L, SEEK_END);
        int insize = ftell(in);
        fseek(out, 0L, SEEK_END);
        int outsize = ftell(out);

        fclose(in);
        fclose(out);

        //print diagnostics
        utils::decompDiagnostics(argv[2], argv[4], insize, outsize, elapsed);

    } // else print usage prompt
    else printf("SimComp USAGE: SimComp [-d] [-c] <source> [-o] <dest> [-l] <level>\n");

    return 0;
}