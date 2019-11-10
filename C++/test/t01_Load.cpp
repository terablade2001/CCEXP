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
CECS_MODULE("t01-Load")

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


int t01_Load(void* mv_) {
#ifdef __CCEXP__USE_MVECTOR
	MVECTOR<char> Mem;
#endif
	// 1. Create a file
	//{ >>>>>>>:: Create a file and store it ::<<<<<<<
        CCEXP::CCEXP DBG("DataFile.ccexp");
		// CCEXP::CCEXP DBG;
		// CCEXP::Initialize(DBG,"DataFile.ccexp");
		
		
			
		// Add Tables
		CCEXP::AddTable <uint8_t>(DBG,"Table_u8","uint8");
		CCEXP::AddTable <float>(DBG,"Table_float","single",9);
		CCEXP::AddTable <int16_t>(DBG,"Table_i16","int16");
		CCEXP::AddTable <size_t>(DBG,"Table_st","uint64");
		_DBG_ERROR_STOP_OR_CONTINUE_;
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #1 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
		for (int j=0; j < 10; j++) {
			for (int i = 0; i < 3; i++) {
				CCEXP::AddVal(DBG,"Table_u8",   (uint8_t)(j*3+i));
				CCEXP::AddVal(DBG,"Table_float",(float)(j*3+i+0.5f));
				CCEXP::AddVal(DBG,"Table_i16",  (int16_t)(j*3+i+1000));
				CCEXP::AddVal(DBG,"Table_st",   (size_t)(j*3+i+1));
			}
			CCEXP::NewRow(DBG,"Table_u8");

			// Table_float has limiting rows to 9. More than 9 rows should occur an error
			// When j == the table has already 9 rows ([0...8]).
			// I use error checking thus the program to be able to continue.
			CCEXP::NewRow(DBG,"Table_float");
			if (j >= 8) printf("\n\n**** TEST:: Error must occur at Line [%i]  *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_;  DBG.Status = CCEXPORTMAT_READY;
			
			CCEXP::NewRow(DBG,"Table_i16");
			CCEXP::NewRow(DBG,"Table_st");
		}
		_DBG_ERROR_STOP_OR_CONTINUE_;
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #2 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

		CCEXP::StoreData(DBG);
		CCEXP::Reset(DBG);
		_DBG_ERROR_STOP_OR_CONTINUE_
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #3 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	//}
	
	//{ >>>>>>>:: Part #2 LD is a loading object ::<<<<<<<	
		CCEXP::CCEXP LD;
		
		// If the CCEXP LD is not initialized, then it won't be able to
		// store the data.

		// *** All CCEXP object should be initialized first!
		CCEXP::Initialize(LD,"Output.bdat");
		// Ignoring the initialization, will yield a lot of errors!
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #4 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
		CCEXP::Open(LD,"DataFile.ccexp");
			CCEXP::LoadTable<uint8_t>(LD,"Table_u8","uint8");
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #5 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
			// Test Loading the same table twice
			CCEXP::LoadTable<uint8_t>(LD,"Table_u8","uint8");
			_DBG_ERROR_STOP_OR_CONTINUE_

			// Test Loading wrong table format
			CCEXP::LoadTable<float>(LD,"Table_i16","float");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_;  LD.Status = CCEXPORTMAT_READY;
			
			// Test Loading table that does not exist
			CCEXP::LoadTable<int16_t>(LD,"Table_i166","int16_t");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_;  LD.Status = CCEXPORTMAT_READY;
			
			CCEXP::LoadTable<int16_t>(LD,"Table_i16","int16");
			_DBG_ERROR_STOP_OR_CONTINUE_;
		CCEXP::Close(LD);
		_DBG_ERROR_STOP_OR_CONTINUE_;
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #6 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

		size_t NTables = CCEXP::NumberOfTables(LD);
		size_t rows;
		char* TableName;
		printf("\n\n[%s: %i]: Tables in LD (Should be 2) : " __ZU__ "\n",__FNAME__,__LINE__, NTables);
		for (size_t i = 0; i < NTables; i++) {
			TableName = CCEXP::getTableName(LD, i);
			rows = CCEXP::Rows(LD,TableName);
			printf("[%s: %i]: Table " __ZU__ " --> %s with (" __ZU__ ")rows\n",__FNAME__,__LINE__, i, TableName, rows);
		}
	//}
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #7 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
	
	
	// Add to LD a new Table
		CCEXP::AddTable<int>(LD,"A_New_Table","int32");
		for (int i = 0; i < 5; i++) CCEXP::AddVal(LD,"A_New_Table",i); 
		NTables = CCEXP::NumberOfTables(LD);
		printf("[%s: %i]: Tables in LD (Should be 3) : " __ZU__ "\n",__FNAME__,__LINE__, NTables);
	
	
	// Load the rest tables at LD.
	CCEXP::Open(LD,"DataFile.ccexp");
		CCEXP::LoadTable<float>(LD,"Table_float","single");
		_DBG_ERROR_STOP_OR_CONTINUE_;
		CCEXP::LoadTable<size_t>(LD,"Table_st","uint64");
		_DBG_ERROR_STOP_OR_CONTINUE_;
	CCEXP::Close(LD);
	_DBG_ERROR_STOP_OR_CONTINUE_;
	
	NTables = CCEXP::NumberOfTables(LD);
	printf("[%s: %i]: Tables in LD (Should be 5) : " __ZU__ "\n",__FNAME__,__LINE__, NTables);
	
	
	// Test if Tables are limited to new rows.
		// Increase Rows of a loaded table. This should be ok!
		for (int i = 0; i < 2; i++) {
			CCEXP::NewRow(LD,"Table_u8");
			CCEXP::AddVal(LD,"Table_u8", (uint8_t)(i));
		}
		_DBG_ERROR_STOP_OR_CONTINUE_;
	
		// Increase Rows of a loaded table which is already limited to _maxRows!
		for (int i = 0; i < 2; i++) {
			CCEXP::NewRow(LD,"Table_float");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_;  LD.Status = CCEXPORTMAT_READY;
			CCEXP::AddVal(LD,"Table_float",(float)(i+0.5f));
		}
		_DBG_ERROR_STOP_OR_CONTINUE_;

	// Test NoNewRow(): It actually cancels the NewRow() operation.
	CCEXP::NewRow(LD,"Table_st");
	CCEXP::NoNewRow(LD,"Table_st");
	CCEXP::AddVal(LD,"Table_st",999);
	_DBG_ERROR_STOP_OR_CONTINUE_;
	
	// Display the Tables now in LD.
	// * Table 'Table_u8' should have 12 rows
	// * Table 'Table_float' should have 9 as it's limited in the file.
	// * Table 'Table_st' should have 10 rows, as NoNewRow() cancelled the NewRow().
	printf("\n\n");
	for (size_t i = 0; i < NTables; i++) {
		TableName = CCEXP::getTableName(LD, i);
		rows = CCEXP::Rows(LD,TableName);
		printf("[%s: %i]: Table " __ZU__ " --> %s with (" __ZU__ ")rows\n",__FNAME__,__LINE__, i, TableName, rows);
	}
	

	
	
	
	// If the LD object has not been initialized, an on-runtime name can be
	// given to store there directly. This approach however is not recommended.
	CCEXP::StoreData(LD,"Output.ccexp");
	_DBG_ERROR_STOP_OR_CONTINUE_;
	
	
	// Test getRow(). Notice that we have 3+3+2 values:
	// 3 when adding values for row 8 (DBG object)
	// 3 when adding values for row 9. NewRow() failed due to _maxRows = 9, thus
	//   the 3 data was add at the end of row 8. (DBG object)
	// 2 data when later on at LD object I tried to add 2 new rows which also
	//   failed due to the _maxRows = 9 restriction.
	MVECTOR<float>* fv = CCEXP::getRow<float>(LD,"Table_float",8);
	_DBG_ERROR_STOP_OR_CONTINUE_;
	if (fv != NULL) {
		printf("\n\nTable_float, Row=8 values:\n >> ");
		for (size_t i = 0; i < fv->size(); i++)	printf("%2.3f ",(*fv)[i]);
		printf("\n");
	} else printf("[%s: %i]: ERROR:: fv = NULL!\n",__FNAME__,__LINE__);
	
	// Test getVal(). 4rth value will not updated (as it does not exist)
	// but an error will be occured.
	printf("\n\nTable_float, Row=0 values:\n >> ");
	for (size_t i = 0; i < 4; i++) {
		float* val = CCEXP::getVal<float>(LD,"Table_float",0, i);
		if (val != NULL) printf("%2.3f ", *val);
		else printf("val=NULL ");
	}
	printf("\n");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_;  LD.Status = CCEXPORTMAT_READY;
	
	// Test overloaded getRow(), which return pointer and columns number.
	size_t cols = 0;
	float* f = CCEXP::getRow<float>(LD,"Table_float",8, cols);
	_DBG_ERROR_STOP_OR_CONTINUE_;
	if (f != NULL) {
		printf("\n\nTable_float, Row=8 values:\n >> ");
		for (size_t i = 0; i < cols; i++)	printf("%2.3f ",f[i]);
		printf("\n");
	} else printf("[%s: %i]: ERROR:: f = NULL!\n",__FNAME__,__LINE__);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #8 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	printf("\n... t01_Load() call ending ...\n");
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: Main(): Total MVECTOR Bytes before t01_Load() returns: " __ZU__ "\n",
		__FNAME__,__LINE__, ((MVECTOR<char>*)mv_)->total_bytes()
	);
#endif

	return 0;
}
