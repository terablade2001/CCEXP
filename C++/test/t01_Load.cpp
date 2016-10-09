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


int main(int argc, char **argv) {
	// 1. Create a file
	//{ >>>>>>>:: Create a file and store it ::<<<<<<<
		CCEXP::CCEXP DBG;
		CCEXP::Initialize(DBG,"DataFile.ccexp");
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
		
			
		// Add Tables
		CCEXP::AddTable <uint8_t>(DBG,"Table_u8","uint8");
		CCEXP::AddTable <float>(DBG,"Table_float","single");
		CCEXP::AddTable <int16_t>(DBG,"Table_i16","int16");
		CCEXP::AddTable <size_t>(DBG,"Table_st","uint64");
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
		
		for (int j=0; j < 10; j ++) {
			for (int i = 0; i < 3; i++) {
				CCEXP::AddVal(DBG,"Table_u8",   (uint8_t)(j*3+i));
				CCEXP::AddVal(DBG,"Table_float",(float)(j*3+i+0.5f));
				CCEXP::AddVal(DBG,"Table_i16",  (int16_t)(j*3+i+1000));
				CCEXP::AddVal(DBG,"Table_st",   (size_t)(j*3+i+1));
			}
			CCEXP::NewLine(DBG,"Table_u8");
			CCEXP::NewLine(DBG,"Table_float");
			CCEXP::NewLine(DBG,"Table_i16");
			CCEXP::NewLine(DBG,"Table_st");
		}
				
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
			CCEXP::getTableName(LD, i, TableName);
			CCEXP::Rows(LD,TableName,rows);
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
	
	// Store the LD object. This would produce error if LD is not initialized!
	CCEXP::StoreData(LD);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	// If the LD object has not been initialized, an on-runtime name can be
	// given to store there directly. This approach however is not recommended.
	CCEXP::StoreData(LD,"Output.ccexp");
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	
	
	printf("\n... Program End ...\n");
	return 0;
}
