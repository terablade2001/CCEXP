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

CECS_MODULE("t03-cecs")

int t03_cecs_test(void* mv_) {
	// A good tactic is to connect different objects. In this example both
	// CCEXPECS and __ECSOBJ__ do use the same static array of CECS.
	// But if there are multiple libraries that has CECS, there will be different
	// static arrays and there might be different records on different arrays!
	// The connection ensures that errors are passed to the same array.

	printf("t03:: Test Start ...\n");
	CCEXP::CCEXP DBG("./t03.ccexp", __ECSOBJ__.cecs());
	try {
		CCEXP::AddTable<int>(DBG,"Integers","int32_t");
		_CHECKRT_
		
		int vals[5]={0,2,4,6,8};
		CCEXP::AddRow<int>(DBG,"Integers",vals,5);
		CCEXP::AddRow<int>(DBG,"Integersx",vals,5);
		_CHECKRT_
	} catch(std::exception &e) {
		_ECSCLS_
		DBG.Status = CCEXPORTMAT_READY;
		std::cout << "Test capture of 1rst error (wrong table name for AddRow)... " << std::endl;
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << "  --> " << e.what() << std::endl;
	}
	
	std::cout << "-------- Continue after 1rst error capture -------------" << std::endl<< std::endl<< std::endl;
	
	CCEXP::CCEXP LD(__ECSOBJ__.cecs());
	try {
		CCEXP::StoreData(DBG);
		CCEXP::Reset(DBG);
		_CHECKRT_
	
		CCEXP::Open(LD,"./t03.ccexp");
			CCEXP::LoadTable<int>(LD,"Integersx","int32_t");
		CCEXP::Close(LD);
		_CHECKRT_
	} catch(std::exception &e) {
		_ECSCLS_
		CCEXP::Reset(LD);
		std::cout << "Test capture of 2nd error (wrong table name for loading)... " << std::endl;
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << e.what() << std::endl;
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
		std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << e.what() << std::endl;
	}

	return 0;
}