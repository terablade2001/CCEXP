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

// Include the CCEXP library
#include "../src/include/CCEXP.hpp"

CECS_MODULE("t02-ExtWriteLoad")

// #define ERROR_STOP
#ifdef ERROR_STOP
	#define _DBG_ERROR_STOP_OR_CONTINUE_ {\
		std::cout << endl << "_DBG_ERROR_STOP_OR_CONTINUE_: ["<< __FNAME__ <<", "<< __LINE__ << "] -->" << std::endl;\
		_CERRI("Error detected. Aborting!");\
	}
#else
	#define _DBG_ERROR_STOP_OR_CONTINUE_ \
		std::cout << endl << "_DBG_ERROR_STOP_OR_CONTINUE_: ["<< __FNAME__ <<", "<< __LINE__ << "] -->" << std::endl;\
		if (0!=_NERR_) {\
			std::cout << __ECSOBJ__.str() << std::endl;\
			_ECSCLS_\
		}
#endif

int t02_ExternalWriteLoad(void* mv_) {

#ifdef __CCEXP__USE_MVECTOR
	MVECTOR<char> Mem;
#endif
	// Load a Binary file which created with external software (i.e. Python, MatLab(Octave) etc)
	CCEXP::CCEXP LD;
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #1 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	CCEXP::Open(LD,"SharedData.ccexp");
		CCEXP::LoadTable<float>(LD,"Table_1","single");
		CCEXP::LoadTable<uint8_t>(LD,"Table_2","uint8");
	CCEXP::Close(LD);
	_CERRI("Generate SharedData.ccexp using [t02_ExternalWriteLoad.m] file.")
	_DBG_ERROR_STOP_OR_CONTINUE_;
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #2 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	size_t Rows_1 = CCEXP::Rows(LD, "Table_1");
	size_t Rows_2 = CCEXP::Rows(LD, "Table_2");
	_DBG_ERROR_STOP_OR_CONTINUE_;
	
	cout << endl << "Table_1 has [" << Rows_1 <<
	  "] rows, while Table_2 has [" << Rows_2 <<
	  "] rows." << endl << endl;
	
	// Print Table_1 data ...
	for (size_t r = 0; r < Rows_1; r++) {
		size_t col = CCEXP::Cols(LD, "Table_1", r);
		cout << "Table_1, row(" << r << ") = [";
		for (size_t c = 0; c < col; c++) {
			float* v1 = CCEXP::getVal<float>(LD, "Table_1", r, c);
            if (v1 != NULL) cout << *v1 << " ";
            else cout << "v1=NULL" << " ";
		}
		cout << "]" << endl;
	}
	
	cout << endl;
	
	// Print Table_2 data ...
	for (size_t r = 0; r < Rows_2; r++) {
		size_t col = CCEXP::Cols(LD, "Table_2", r);
		cout << "Table_2, row(" << r << ") = [";
		for (size_t c = 0; c < col; c++) {
			uint8_t* v1 = CCEXP::getVal<uint8_t>(LD, "Table_2", r, c);
			if (v1 != NULL) printf("%u ", *v1);
            else printf("v1=NULL ");
		}
		cout << "]" << endl;
	}

	_DBG_ERROR_STOP_OR_CONTINUE_;

	printf("\n... t02_ExternalWriteLoad() call ending ...\n");
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: Main(): Total MVECTOR Bytes before t02_ExternalWriteLoad() returns: " __ZU__ "\n",
		__FNAME__,__LINE__, ((MVECTOR<char>*)mv_)->total_bytes()
	);
#endif
	return 0;
}
