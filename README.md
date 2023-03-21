# SimComp
a simple compression/decompression command line application using the zlib compression library and prints out some useful diagnostic information about the process.
## USAGE
- this program already has a pre-built binary in the `build` folder named `SimComp.exe`
- alternate build versions can be created by altering the `CMakeLists.txt` file in the root directory of this project.
- to use the project built as is, place a file which is to be compressed/decompressed in the root directory of this project and navigate to this root directory in the termianal.
- there are 4 flags which can be set and passed parameters whilst using this application:
`-c` [REQUIRED] signifies that the following arguement is a file to be compressed
`-l` [OPTIONAL] the following arguement is the level of compression to be done [between 1 and 9]
`-d` [REQUIRED] signifies that the following arguement is a file to be decompressed
`-o` [REQUIRED] the following arguement is the output file which the output of compression/decompression will be written to.

An example showing the compression of the `example.txt` text file in the root directory is as follows:
`./build/SimComp.exe -c example.txt -o compressed.bin -l 8`
this compresses the text file to a binary file and prints out some useful diagnostic information.

An example of decompression using the above compressed file is as follows:
`./build/SimComp.exe -d compressed.bin -o decomp.txt`
this will produce a text file called `decomp.txt` that will contain the same contents as in the original example text file