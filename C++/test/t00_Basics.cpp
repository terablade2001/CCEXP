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
	
	// Store all DBG data. This will clear the data of all Tables,
	// but will not remove the tables; you can re-add new data.
    CCEXP::StoreData(DBG);
	
	// Reset the DBG object. Everything is cleared. Need to Initialize
	// again in order to use it again!
	CCEXP::Reset(DBG);
	
	printf("\n... Program End ...\n");
	return 0;
}
