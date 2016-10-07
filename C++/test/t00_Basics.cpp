// Include the CCEXP library
#include "../src/include/CCEXP.hpp"

// Define a CCEXP object (DBG)
CCEXP::CCEXP DBG;

int main(int argc, char **argv) {
	// Initialize DBG object, and set it a Filename where it will store
	// it's data when CCEXP::StoreData() fuction is called.
	CCEXP::Initialize(DBG,"DataFile.ccexp");
	__CCEXP_ERR_DISPLAY(DBG,-1);
    
	
	// Do a second initialization.. This will procude an error!
    CCEXP::Initialize(DBG,"DataFile.ccexp");
	__CCEXP_ERR_DISPLAY(DBG,-1); // Display the error and continue...
	

	// Add two tables for uint8_t and float data
    CCEXP::AddTable <uint8_t>(DBG,"T_U8","uint8");
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
		else CCEXP::NewLine(DBG,"T_F32",1);		
	}
	
	 // Add 5 New Empty Lines to TestMaxR (ID=2). Errors should hit after 3 max lines
	for (int row = 0; row < 5; row++) CCEXP::NewLine(DBG,2,1);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	
	size_t R0; CCEXP::Rows(DBG, "T_U8", R0);
	printf("\nTable T_U8 has [%lu] rows\n", (uint64_t)R0);
	
	
	size_t SelRow = 255;
	size_t C0; CCEXP::Cols(DBG, "T_U8", SelRow, C0);
	printf("\nRow [%lu] of Table T_U8 has [%lu] columns\n", SelRow, (size_t)C0);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	
	
	// Test Cols Error handling... (Wrong SelRow)
	SelRow = 512; CCEXP::Cols(DBG, "T_U8", SelRow, C0);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	// Test Rows Error handling (False Table Name)
	SelRow = 512; CCEXP::Cols(DBG, "T_UU88", SelRow, C0);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	
	
	delete[] pU8;
	delete[] pF32;
	
	// Test "AddVal" & "DeleteLastRow"
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 4; i++)
			CCEXP::AddVal(DBG,"AddVal",j*4+i);
	CCEXP::NewLine(DBG,"AddVal");
	for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"AddVal",i);
	
	CCEXP::NewLine(DBG,"AddVal");
	for (int i=0; i < 3; i++) CCEXP::AddVal(DBG,"AddVal",i);
	CCEXP::DeleteLastRow(DBG,"AddVal");
	for (int i=5; i < 8; i++) CCEXP::AddVal(DBG,"AddVal",i);


	// Test "DeleteRow"
	for (int i=0; i < 10; i++) {
		CCEXP::AddVal(DBG,"DeleteRow",i); CCEXP::NewLine(DBG,"DeleteRow");
	}
	CCEXP::DeleteRow(DBG,"DeleteRow",3);
	CCEXP::DeleteRow(DBG,"DeleteRow",8);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	// Test "DeleteLastElement"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"DeleteLastElement",i);
		CCEXP::NewLine(DBG,"DeleteLastElement");
	}
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
	CCEXP::DeleteLastElement(DBG,"DeleteLastElement",1);
		
	
	// Test "AppendRow"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"AppendRow",i);
		CCEXP::NewLine(DBG,"AppendRow");
	}
	int AppendRowData[3] = {21, 22, 23};
	CCEXP::AppendRow<int>(DBG,"AppendRow",1,AppendRowData,3);
	
	// Test "ReplaceRow"
	for (int j=0; j < 3; j++) {
		for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"ReplaceRow",i);
		CCEXP::NewLine(DBG,"ReplaceRow");
	}
	int ReplaceRowData[3] = {31, 32, 33};
	CCEXP::ReplaceRow<int>(DBG,"ReplaceRow",1,ReplaceRowData,3);
	
	// Test "SetVal"
	for (int i=0; i < 5; i++) CCEXP::AddVal(DBG,"SetVal",i);
	CCEXP::SetVal<int>(DBG,"SetVal",0,2,-11);
	CCEXP::SetVal<int>(DBG,"SetVal",0,4,-12);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	// Test "InitRowByScalar"
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,7,3);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,7,4);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",0,3,2);
	CCEXP::InitRowByScalar(DBG,"InitRowByScalar",-1,0,0);
	__CCEXP_ERR_DISPLAY(DBG,-1);
	
	// Test "GetTableID"
	size_t TableID;
	CCEXP::getTableID(DBG,"AddVal", TableID);            CCEXP::AddVal(DBG,"getTableID",TableID);
	CCEXP::getTableID(DBG,"DeleteLastElement", TableID); CCEXP::AddVal(DBG,"getTableID",TableID);
	CCEXP::getTableID(DBG,"SetVal", TableID);            CCEXP::AddVal(DBG,"getTableID",TableID);
	__CCEXP_ERR_DISPLAY(DBG,-1);

	
	// Test "getTableName"
	printf("\n\n");
	char* TableName = NULL;
	CCEXP::getTableName(DBG,3,TableName);
	printf("Table Name of Table with ID (%lu) is: %s\n", (uint64_t)3, TableName);
	CCEXP::getTableName(DBG,5,TableName);
	printf("Table Name of Table with ID (%lu) is: %s\n", (uint64_t)5, TableName);
	CCEXP::getTableName(DBG,8,TableName);
	printf("Table Name of Table with ID (%lu) is: %s\n", (uint64_t)8, TableName);
	
	
	
	 __CCEXP_ERR_DISPLAY(DBG,-1);
	
	// Store all DBG data. This will clear the data of all Tables,
	// but will not remove the tables; you can re-add new data.
    CCEXP::StoreData(DBG);
	
	// Reset the DBG object. Everything is cleared. Need to Initialize
	// again in order to use it again!
	CCEXP::Reset(DBG);
	
	printf("\n... Program End ...\n");
	return 0;
}
