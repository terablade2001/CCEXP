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

// Demo/Debugging tool. Enable or Comment ERROR_STOP, to stop on Errors or
// to force debugging behaviour and continue.
// * For this UnitsTests Demo I have not set ERROR_STOP thus every error
// just shows an error message and moves on. 

// #define ERROR_STOP
#ifdef ERROR_STOP
	#define _DBG_ERROR_STOP_OR_CONTINUE_(x) \
		__CCEXP_ERR_DISPLAY((x),-1); if ((x).Status != 1) return (x).Status;
#else	
	#define _DBG_ERROR_STOP_OR_CONTINUE_(x) \
		__CCEXP_ERR_DISPLAY((x),-1); CCEXP::DBG_SetStatus((x), 1);
#endif

// Define a CCEXP object (DBG)
static CCEXP::CCEXP DBG;

int t00_Basics(void* mv_) {
#ifdef __CCEXP__USE_MVECTOR
	MVECTOR<char> Mem;
#endif
	// Initialize DBG object, and set it a Filename where it will store
	// it's data when CCEXP::StoreData() fuction is called.
	CCEXP::Initialize(DBG,"DataFile.ccexp");
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #1 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
  
	// Do a second initialization.. This will procude an error!
		CCEXP::Initialize(DBG,"DataFile.ccexp");	
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #2 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	// Add Tables
    CCEXP::AddTable <uint8_t>(DBG,"T_U8","uint8");
	// Add the same table twice. This also must produce an error.
		CCEXP::AddTable <float>(DBG,"T_U8","uint8");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #3 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	CCEXP::AddTable <float>(DBG,"T_F32","single");
	CCEXP::AddTable <uint8_t>(DBG,"TestMaxR","uint8",3);
	CCEXP::AddTable <int>(DBG,"AddVal","int32");
	CCEXP::AddTable <int>(DBG,"DeleteRow","int32");
	CCEXP::AddTable <int>(DBG,"DeleteLastElement","int32");
	CCEXP::AddTable <int>(DBG,"AppendRow","int32");
	CCEXP::AddTable <int>(DBG,"ReplaceRow","int32");
	CCEXP::AddTable <int>(DBG,"SetVal","int32");
	CCEXP::AddTable <int>(DBG,"InitRowByScalar","int32");
	CCEXP::AddTable <size_t>(DBG,"getTableID","uint64");
	
	// Add a third table, but for this 'debug-session' ignore it completely.
	// If need to re-enable it, just remove letter I
  CCEXP::AddTable <char>(DBG,"T_Char","char");

	// Add a string to the T_Char Table. If need to store that table too,
	// then change the corresponded called function AddTableI() to AddTable()!
	char TestString[256] = "Hello World!";
	CCEXP::AddRow<char>(DBG,"T_Char",TestString,256);

	uint8_t* pU8 = new uint8_t[256];
	float* pF32 = new float[256];

	// Create 255 rows of uint8_t and float data.
	for (int row = 0; row < 256; row++) {
		for (int col = 0; col < 256; col++) {
			pU8[col] = col;
			pF32[col] = float(col)+0.125f;
		}
		
		// Store every row of uint8_t data
		CCEXP::AddRow<uint8_t>(DBG,"T_U8",pU8,row);

		// Add 127 empty rows for float data, and then add the rest data normally.
		if (row > 127) CCEXP::AddRow<float>(DBG,"T_F32",pF32,row);
		else CCEXP::NewRow(DBG,"T_F32",1);
	}

	// Until the below line, there should not be any error.
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("\n");
	printf("[%s: %i]: #4 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	 // Add 5 New Empty Lines to TestMaxR (ID=2).
	 // Notice that Table with ID=2 (TestMaxR) is added with maximum Rows set
	 // to 3 (by AddMatrix()). Thus errors should hit after 3 max lines.
	for (int row = 0; row < 5; row++)
		CCEXP::NewRow(DBG, 2, 1);
size_t n_errors;
printf("%s\n", CCEXP::GetErrors(DBG, n_errors)); // Display the error as simple char sequence.
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG); 
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #5 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
	size_t R0 = CCEXP::Rows(DBG, "T_U8");
	printf("\nTable T_U8 has [" __ZU__ "] rows\n", R0);
	

	size_t SelRow = 255;
	size_t C0 = CCEXP::Cols(DBG, "T_U8", SelRow);
	printf("\nRow [" __ZU__ "] of Table T_U8 has [" __ZU__ "] columns\n", SelRow, (size_t)C0);
	
	// After TestMaxR table, no more errors should exist in the following test.
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #6 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
	
	// Test Cols Error handling... (Wrong SelRow)
	// Error must occur in the following call.
		SelRow = 512; C0 = CCEXP::Cols(DBG, "T_U8", SelRow);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test Rows Error handling (False Table Name)
	SelRow = 512; C0 = CCEXP::Cols(DBG, "T_UU88", SelRow);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
#ifdef __CCEXP__USE_MVECTOR
	printf("\n");
	printf("[%s: %i]: #7 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
	delete[] pU8;
	delete[] pF32;
	
	// Test "AddVal" & "DeleteLastRow"
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 4; i++)
			CCEXP::AddVal(DBG,"AddVal",j*4+i);
	CCEXP::NewRow(DBG,"AddVal");
	for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"AddVal",i);
	
	CCEXP::NewRow(DBG,"AddVal");
	for (int i=0; i < 3; i++) CCEXP::AddVal(DBG,"AddVal",i);
	CCEXP::DeleteLastRow(DBG,"AddVal");
	for (int i=5; i < 8; i++) CCEXP::AddVal(DBG,"AddVal",i);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #8 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif

	// Test "DeleteRow"
	for (int i=0; i < 10; i++) {
		CCEXP::AddVal(DBG,"DeleteRow",i); CCEXP::NewRow(DBG,"DeleteRow");
	}
	CCEXP::DeleteRow(DBG,"DeleteRow",3);
	CCEXP::DeleteRow(DBG,"DeleteRow",8);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test "DeleteLastElement"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"DeleteLastElement",i);
		CCEXP::NewRow(DBG,"DeleteLastElement");
	}
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #9 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	
	// Test "AppendRow"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"AppendRow",i);
		CCEXP::NewRow(DBG,"AppendRow");
	}
	int AppendRowData[3] = {21, 22, 23};
	CCEXP::AppendRow<int>(DBG,"AppendRow",1,AppendRowData,3);
	
	// Test "ReplaceRow"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"ReplaceRow",i);
		CCEXP::NewRow(DBG,"ReplaceRow");
	}
	int ReplaceRowData[3] = {31, 32, 33};
	CCEXP::ReplaceRow<int>(DBG,"ReplaceRow",1,ReplaceRowData,3);
	
	// Test "SetVal"
	for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"SetVal",i);
	CCEXP::SetVal<int>(DBG,"SetVal",0,2,-11);
	CCEXP::SetVal<int>(DBG,"SetVal",0,4,-12);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test "InitRowByScalar"
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,7,3);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,7,4);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",0,3,2);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,0,0);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test "GetTableID"
	size_t TableID;
	TableID = CCEXP::getTableID(DBG,"AddVal");            CCEXP::AddVal(DBG,"getTableID",TableID);
	TableID = CCEXP::getTableID(DBG,"DeleteLastElement"); CCEXP::AddVal(DBG,"getTableID",TableID);
	TableID = CCEXP::getTableID(DBG,"SetVal");            CCEXP::AddVal(DBG,"getTableID",TableID);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test "getTableName"
	printf("\n\n");
	char* TableName = NULL;
	TableName = CCEXP::getTableName(DBG,3);
	printf("Table Name of Table with ID (%i) is: %s\n", 3, TableName);
	TableName = CCEXP::getTableName(DBG,5);
	printf("Table Name of Table with ID (%i) is: %s\n", 5, TableName);
	TableName = CCEXP::getTableName(DBG,8);
	printf("Table Name of Table with ID (%i) is: %s\n", 8, TableName);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Display DBG tables
	printf("\n---------- Display DBG object's tables: ------------------------\n");
	__CCEXP_PRINT_TABLES__(DBG)
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	printf("----------------------------------------------------------------\n\n");

#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: #10 : Total Bytes: " __ZU__ "\n",__FNAME__,__LINE__, Mem.total_bytes() );
#endif
	// Store all DBG data. This will clear the data of all Tables,
	// but will not remove the tables; you can re-add new data.
    CCEXP::StoreData(DBG);

#ifdef __CCEXP__USE_MVECTOR
	printf("\n\n\n");
	printf("--- MVECTOR Size before Reset: " __ZU__ " bytes. \n", Mem.total_bytes());
#endif
	// Reset the DBG object. Everything is cleared. Need to Initialize
	// again in order to use it again!
	CCEXP::Reset(DBG);

#ifdef __CCEXP__USE_MVECTOR
	printf("--- MVECTOR Size after Reset: " __ZU__ " bytes. \n", Mem.total_bytes());
#endif
	
	printf("\n... t00_Basics() call ending ...\n");
#ifdef __CCEXP__USE_MVECTOR
	printf("[%s: %i]: Main(): Total MVECTOR Bytes before t00_Basics() returns: " __ZU__ "\n",
		__FNAME__,__LINE__, ((MVECTOR<char>*)mv_)->total_bytes()
	);
#endif

	// Display DBG tables
	printf("\n---------- Display DataFile.ccexp tables: ----------------------\n");
	__CCEXP_PRINT_TABLES__("DataFile.ccexp")
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	printf("----------------------------------------------------------------\n\n");

	return 0;
}
