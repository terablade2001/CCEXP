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
CCEXP::CCEXP DBG;

int main(int argc, char **argv) {
	// Initialize DBG object, and set it a Filename where it will store
	// it's data when CCEXP::StoreData() fuction is called.
	CCEXP::Initialize(DBG,"DataFile.ccexp");
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
    	
	// Do a second initialization.. This will procude an error!
		CCEXP::Initialize(DBG,"DataFile.ccexp");	
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	

	// Add Tables
    CCEXP::AddTable <uint8_t>(DBG,"T_U8","uint8");
	// Add the same table twice. This also must produce an error.
		CCEXP::AddTable <float>(DBG,"T_U8","uint8");
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
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
    CCEXP::AddTableI<char>(DBG,"T_Char","char");
    
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
		
	 // Add 5 New Empty Lines to TestMaxR (ID=2).
	 // Notice that Table with ID=2 (TestMaxR) is added with maximum Rows set
	 // to 3 (by AddMatrix()). Thus errors should hit after 3 max lines.
	for (int row = 0; row < 5; row++) CCEXP::NewRow(DBG,2,1);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG); 
	
	
	size_t R0 = CCEXP::Rows(DBG, "T_U8");
	printf("\nTable T_U8 has [%zu] rows\n", R0);
	
		
	size_t SelRow = 255;
	size_t C0 = CCEXP::Cols(DBG, "T_U8", SelRow);
	printf("\nRow [%zu] of Table T_U8 has [%zu] columns\n", SelRow, (size_t)C0);
	
	// After TestMaxR table, no more errors should exist in the following test.
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	
	
	// Test Cols Error handling... (Wrong SelRow)
	// Error must occur in the following call.
		SelRow = 512; C0 = CCEXP::Cols(DBG, "T_U8", SelRow);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
		_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	// Test Rows Error handling (False Table Name)
	SelRow = 512; C0 = CCEXP::Cols(DBG, "T_UU88", SelRow);
printf("\n\n**** TEST:: Error must occur at Line [%i]! *******",__LINE__+1);
	_DBG_ERROR_STOP_OR_CONTINUE_(DBG);
	
	
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
	
	// Store all DBG data. This will clear the data of all Tables,
	// but will not remove the tables; you can re-add new data.
    CCEXP::StoreData(DBG);
	
	// Reset the DBG object. Everything is cleared. Need to Initialize
	// again in order to use it again!
	CCEXP::Reset(DBG);
	
	printf("\n... Program End ...\n");
	return 0;
}
