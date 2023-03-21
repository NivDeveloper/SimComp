#include "utils.h"

namespace utils
{

    int compress(FILE* source, FILE* dest, int level)
    {
        int ret, flush;
        unsigned have;
        z_stream zstream;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        //alocating default state
        zstream.zalloc = Z_NULL;
        zstream.zfree = Z_NULL;
        zstream.opaque = Z_NULL;
        ret = deflateInit(&zstream, level);

        if (ret != Z_OK) return ret;

        do
        {
            /* code */
            zstream.avail_in = fread(in, 1, CHUNK, source);
            if (ferror(source)) {
            (void)deflateEnd(&zstream);
            return Z_ERRNO;
            }
            flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
            zstream.next_in = in;

            do
            {
                /* code */
                zstream.avail_out = CHUNK;
                zstream.next_out = out;
                ret = deflate(&zstream, flush);    /* no bad return value */
                assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                have = CHUNK - zstream.avail_out;
                if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                    (void)deflateEnd(&zstream);
                    return Z_ERRNO;
                }
            } while (zstream.avail_out == 0);
            assert(zstream.avail_in == 0);     /* all input will be used */
            
        } while (flush != Z_FINISH);
        assert(ret == Z_STREAM_END);        /* stream will be complete */

        /* clean up and return */
        (void)deflateEnd(&zstream);
        return Z_OK;


    }

    void compDiagnostics(char* inName, char* outname, size_t decomp, size_t comp, int level, std::chrono::duration<int64_t, std::micro> elapsed)
    {
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << inName << " Successfully Compressed!" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
        std::cout <<  "Compression level:\t[" << level << ((level == -1) ? "] (default compression level)" : "]") << std::endl;
        std::cout << "input size:\t\t" << decomp << " bytes" << std::endl;
        std::cout << "output size:\t\t" << comp  << " bytes"<< std::endl;
        std::cout << "Compression Ratio:\t" << std::setprecision(4) << (float(decomp)/float(comp)) << std::endl;
        std::cout << "Compression duration:\t" << elapsed.count() << " microseconds" <<  std::endl;  
    }


    int decompress(FILE *source, FILE *dest)
    {
        int ret;
        unsigned have;
        z_stream strm;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        ret = inflateInit(&strm);
        if (ret != Z_OK)
            return ret;

        /* decompress until deflate stream ends or end of file */
        do {
            strm.avail_in = fread(in, 1, CHUNK, source);
            if (ferror(source)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
            if (strm.avail_in == 0)
                break;
            strm.next_in = in;

            /* run inflate() on input until output buffer not full */
            do {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = inflate(&strm, Z_NO_FLUSH);
                assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
                }
                have = CHUNK - strm.avail_out;
                if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                    (void)inflateEnd(&strm);
                    return Z_ERRNO;
                }
            } while (strm.avail_out == 0);

            /* done when inflate() says it's done */
        } while (ret != Z_STREAM_END);

        /* clean up and return */
        (void)inflateEnd(&strm);
        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }

    void decompDiagnostics(char* inName, char* outname, size_t decomp, size_t comp, std::chrono::duration<int64_t, std::micro> elapsed)
    {
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << inName << " Successfully Decompressed!" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "input size:\t\t" << comp << " bytes" << std::endl;
        std::cout << "output size:\t\t" << decomp  << " bytes"<< std::endl;
        std::cout << "Decompression Ratio:\t" << std::setprecision(4) << ((float(decomp)/float(comp))) << std::endl;
        std::cout << "Decompression duration:\t" << elapsed.count() << " microseconds" <<  std::endl;
    }

    void zlibError(int ret)
    {
        std::cout << "zlib ERROR!: ";
        switch (ret) {
        case Z_ERRNO:
            std::cout << "i/o Error.";
            break;
        case Z_STREAM_ERROR:
            std::cout << "invalid compression level." << std::endl;
            break;
        case Z_DATA_ERROR:
            std::cout << "invalid or incomplete deflate data" << std::endl;
            break;
        case Z_MEM_ERROR:
            std::cout << "out of memory." << std::endl;
            break;
        case Z_VERSION_ERROR:
            std::cout << "zlib version does not match between included header and link file." << std::endl;
        }
    }

}