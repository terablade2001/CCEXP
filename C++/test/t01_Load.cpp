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


int main(int argc, char **argv) {
	// 1. Create a file
	//{ >>>>>>>:: Create a file and store it ::<<<<<<<
		CCEXP::CCEXP DBG;
		CCEXP::Initialize(DBG,"DataFile.ccexp");
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
		
			
		// Add Tables
		CCEXP::AddTable <uint8_t>(DBG,"Table_u8","uint8");
		CCEXP::AddTable <float>(DBG,"Table_float","single",9);
		CCEXP::AddTable <int16_t>(DBG,"Table_i16","int16");
		CCEXP::AddTable <size_t>(DBG,"Table_st","uint64");
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
		
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
			_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
			
			CCEXP::NewRow(DBG,"Table_i16");
			CCEXP::NewRow(DBG,"Table_st");
		}
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);

		CCEXP::StoreData(DBG);
		CCEXP::Reset(DBG);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	//}
	
	//{ >>>>>>>:: Part #2 LD is a loading object ::<<<<<<<	
		CCEXP::CCEXP LD;
		
		// If the CCEXP LD is not initialized, then it won't be able to
		// store the data.
		// *** All CCEXP object should be initialized first!
		// CCEXP::Initialize(LD,"Output.bdat");
		
		CCEXP::Open(LD,"DataFile.ccexp");
			CCEXP::LoadTable<uint8_t>(LD,"Table_u8","uint8");
			
			// Test Loading the same table twice
			CCEXP::LoadTable<uint8_t>(LD,"Table_u8","uint8");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_(LD);

			// Test Loading wrong table format
			CCEXP::LoadTable<float>(LD,"Table_i16","float");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_(LD);
			
			// Test Loading table that does not exist
			CCEXP::LoadTable<int16_t>(LD,"Table_i166","int16_t");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_(LD);
			
			CCEXP::LoadTable<int16_t>(LD,"Table_i16","int16");
			_DBG_ERROR_STOP_OR_CONTINUE_(LD);
		CCEXP::Close(LD);
		_DBG_ERROR_STOP_OR_CONTINUE_(LD);
		
		size_t NTables = CCEXP::NumberOfTables(LD);
		size_t cols, rows;
		char* TableName;
		printf("\n\n[%s: %i]: Tables in LD (Should be 2) : %lu\n",__FNAME__,__LINE__, NTables);
		for (size_t i = 0; i < NTables; i++) {
			TableName = CCEXP::getTableName(LD, i);
			rows = CCEXP::Rows(LD,TableName);
			printf("[%s: %i]: Table %lu --> %s with (%lu)rows\n",__FNAME__,__LINE__, (uint64_t)i, TableName, (uint64_t)rows);
		}
	//}
	
	
	
	
	// Add to LD a new Table
		CCEXP::AddTable<int>(LD,"A_New_Table","int32");
		for (int i = 0; i < 5; i++) CCEXP::AddVal(LD,"A_New_Table",i); 
		NTables = CCEXP::NumberOfTables(LD);
		printf("[%s: %i]: Tables in LD (Should be 3) : %lu\n",__FNAME__,__LINE__, NTables);
	
	
	// Load the rest tables at LD.
	CCEXP::Open(LD,"DataFile.ccexp");
		CCEXP::LoadTable<float>(LD,"Table_float","single");
		_DBG_ERROR_STOP_OR_CONTINUE_(LD);
		CCEXP::LoadTable<size_t>(LD,"Table_st","uint64");
		_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	CCEXP::Close(LD);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	NTables = CCEXP::NumberOfTables(LD);
	printf("[%s: %i]: Tables in LD (Should be 5) : %lu\n",__FNAME__,__LINE__, NTables);
	
	
	// Test if Tables are limited to new rows.
		// Increase Rows of a loaded table. This should be ok!
		for (int i = 0; i < 2; i++) {
			CCEXP::NewRow(LD,"Table_u8");
			CCEXP::AddVal(LD,"Table_u8", (uint8_t)(i));
		}
		_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
		// Increase Rows of a loaded table which is already limited to _maxRows!
		for (int i = 0; i < 2; i++) {
			CCEXP::NewRow(LD,"Table_float");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
			_DBG_ERROR_STOP_OR_CONTINUE_(LD);
			CCEXP::AddVal(LD,"Table_float",(float)(i+0.5f));
		}
		_DBG_ERROR_STOP_OR_CONTINUE_(LD);

	// Test NoNewRow(): It actually cancels the NewRow() operation.
	CCEXP::NewRow(LD,"Table_st");
	CCEXP::NoNewRow(LD,"Table_st");
	CCEXP::AddVal(LD,"Table_st",999);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	// Display the Tables now in LD.
	// * Table 'Table_u8' should have 12 rows
	// * Table 'Table_float' should have 9 as it's limited in the file.
	// * Table 'Table_st' should have 10 rows, as NoNewRow() cancelled the NewRow().
	printf("\n\n");
	for (size_t i = 0; i < NTables; i++) {
		TableName = CCEXP::getTableName(LD, i);
		rows = CCEXP::Rows(LD,TableName);
		printf("[%s: %i]: Table %lu --> %s with (%lu)rows\n",__FNAME__,__LINE__, (uint64_t)i, TableName, (uint64_t)rows);
	}
	

	
	
	
	// Store the LD object. This would produce error if LD is not initialized!
	printf("[%s: %i]: ErrorId should be 0: %i\n",__FNAME__,__LINE__, LD.ErrorId);
	CCEXP::StoreData(LD);
	printf("[%s: %i]: ErrorId should be NOT 0: %i\n",__FNAME__,__LINE__, LD.ErrorId);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	// If the LD object has not been initialized, an on-runtime name can be
	// given to store there directly. This approach however is not recommended.
	CCEXP::StoreData(LD,"Output.ccexp");
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	
	// Test getRow(). Notice that we have 3+3+2 values:
	// 3 when adding values for row 8 (DBG object)
	// 3 when adding values for row 9. NewRow() failed due to _maxRows = 9, thus
	//   the 3 data was add at the end of row 8. (DBG object)
	// 2 data when later on at LD object I tried to add 2 new rows which also
	//   failed due to the _maxRows = 9 restriction.
	vector<float>& fv = CCEXP::getRow<float>(LD,"Table_float",8);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	printf("\n\nTable_float, Row=8 values:\n >> ");
	for (int i = 0; i < fv.size(); i++)	printf("%2.3f ",fv[i]);
	printf("\n");
	
	// Test getVal(). 4rth value will not updated (as it does not exist)
	// but an error will be occured.
	float val=0.0f;
	printf("\n\nTable_float, Row=0 values:\n >> ");
	for (int i = 0; i < 4; i++) {
		float& val = CCEXP::getVal<float>(LD,"Table_float",0,(size_t)i);
		printf("%2.3f ", val);
	}
	printf("\n");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	
	printf("\n... Program End ...\n");
	return 0;
}
