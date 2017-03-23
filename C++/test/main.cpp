// MIT License

// Copyright (c) 2016 Vasileios Kon. Pothos (terablade2001)
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

int t00_Basics();
int t01_Threads();

int main(int argc, char **argv) {
	int RunTest = -1;
	if (argc  < 2)
		printf("Please use one of the following:\n"
			"1) CCEXP t00\n"
			"2) CCEXP t01\n"
		);
	for(int i = 1; i < argc; i++ ) {
				 if( !strcmp( argv[i], "t00"  )) RunTest = 0;
		// else if( !strcmp( argv[i], "t01" )) RunTest = 1;
		else { int ii=i-1;
			printf("Error >> Unrecognized command: [%s %s].\n"
				"Please check the commands syntax (append option: [-help]).\n"
				"Aborting...\n\n\n",argv[++ii],argv[++i]			
			);
			return -1;
		}
	}

			 if (RunTest == 0) t00_Basics();
	// else if (RunTest == 1) t01_Threads();

	return 0;
}