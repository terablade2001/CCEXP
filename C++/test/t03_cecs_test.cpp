// MIT License

// Copyright (c) 2016 - 2019 Vasileios Kon. Pothos (terablade2001)
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

int t03_cecs_test(void* mv_) {
	#ifndef __CCEXP__USE_CECS
		printf("__CCEXP__USE_CECS is not defined. This test can't run!");
		return -1;
	#endif
	printf("t03:: Test Start ...\n");
	CCEXP::CCEXP DBG("./t03.ccexp");
	try {
		CCEXP::AddTable<int>(DBG,"Integers","int32_t");
		_CHECKRT_
		
		int vals[5]={0,2,4,6,8};
		CCEXP::AddRow<int>(DBG,"Integers",vals,5);
		CCEXP::AddRow<int>(DBG,"Integersx",vals,5);
		_CHECKRT_
	} catch(std::exception &e) {
		__ECSOBJ__.clear();
		std::cout << "Test capture of 1rst error (wrong table name for AddRow)... " << std::endl;
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << "  --> " << e.what() << std::endl;
	}
	
	std::cout << "-------- Continue after 1rst error capture -------------" << std::endl<< std::endl<< std::endl;
	
	CCEXP::CCEXP LD;
	try {
		CCEXP::StoreData(DBG);
		CCEXP::Reset(DBG);
		_CHECKRT_
	
		CCEXP::Open(LD,"./t03.ccexp");
			CCEXP::LoadTable<int>(LD,"Integersx","int32_t");
		CCEXP::Close(LD);
		_CHECKRT_
	} catch(std::exception &e) {
		__ECSOBJ__.clear();
		CCEXP::Reset(LD);
		std::cout << "Test capture of 2nd error (wrong table name for loading)... " << std::endl;
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << "  --> " << e.what() << std::endl;
	}
	
	std::cout << "-------- Continue after 2nd error capture -------------" << std::endl<< std::endl<< std::endl;
	
	try {
		CCEXP::Open(LD,"./t03.ccexp");
			CCEXP::LoadTable<int>(LD,"Integers","int32_t");
		CCEXP::Close(LD);
		_CHECKRT_
	
		size_t length = 0;
		int* v = CCEXP::getRow<int>(LD,"Integers", 0, length);
		
		for (size_t i=0; i < length; i++)
			printf("%i -> %i\n",(int)i, v[i]);
		_CHECKRT_
		
		CCEXP::Reset(LD);
		_CHECKRT_
		
		printf("t03:: Test Completed!\n");
	} catch(std::exception &e) {
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << "  --> " << e.what() << std::endl;
	}
	return 0;
}
