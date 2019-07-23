// MIT License

// Copyright (c) 2016 - 2017 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001/CCEXP

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Include the MVECTOR library
#include "../src/include/CCEXP.hpp"

__USE_MVECTOR_NAMESPACE__

int t00_Basics(void* mv_);
int t01_Load(void* mv_);
int t02_ExternalWriteLoad(void* mv_);
int t03_cecs_test(void* mv_);

#ifdef __CCEXP__USE_MVECTOR
	MVECTOR<char> MainMem;
#endif
int main(int argc, char **argv) {
	int RunTest = -1;
	if (argc  < 2)
		printf("Please use one of the following:\n"
			"1) CCEXP t00\n"
			"2) CCEXP t01\n"
			"3) CCEXP t02\n"
			"4) CCEXP t03\n"
		);
	for(int i = 1; i < argc; i++ ) {
				 if( !strcmp( argv[i], "t00" )) RunTest = 0;
		else if( !strcmp( argv[i], "t01" )) RunTest = 1;
		else if( !strcmp( argv[i], "t02" )) RunTest = 2;
		else if( !strcmp( argv[i], "t03" )) RunTest = 3;
		else { int ii=i-1;
			printf("Error >> Unrecognized command: [%s %s].\n"
				"Please check the commands syntax (append option: [-help]).\n"
				"Aborting...\n\n\n",argv[++ii],argv[++i]			
			);
			return -1;
		}
	}

			 if (RunTest == 0) t00_Basics((void*)&MainMem);
	else if (RunTest == 1) t01_Load((void*)&MainMem);
	else if (RunTest == 2) t02_ExternalWriteLoad((void*)&MainMem);
	else if (RunTest == 3) t03_cecs_test((void*)&MainMem);

#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: Main(): Total MVECTOR Bytes main() exits: " __ZU__ "\n",
		__FNAME__,__LINE__, MainMem.total_bytes()
	);
#endif
	return 0;
}