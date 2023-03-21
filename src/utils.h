#pragma once
#include "zlib.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

#define CHUNK 16384

namespace utils
{
    int compress(FILE* source, FILE* dest, int level);
    void compDiagnostics(char* inName, char* outname, size_t decomp, size_t comp, int level, std::chrono::duration<int64_t, std::micro> elapsed);
    int decompress(FILE* source, FILE* dest);
    void decompDiagnostics(char* inName, char* outname, size_t decomp, size_t comp, std::chrono::duration<int64_t, std::micro> elapsed);
    void zlibError(int ret);


}