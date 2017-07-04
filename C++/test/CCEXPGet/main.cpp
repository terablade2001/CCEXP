#include "../../src/include/CCEXP.hpp"

static char FName[512]={0};
static MVECTOR<MVECTOR<char>> DBG_Tables;
static MVECTOR<MVECTOR<char>> DBG_Names;
static MVECTOR<MVECTOR<char>> DBG_Types;

CCEXP::CCEXP OBJ;
int ExtractRow() {
	printf("------- CCEXP Viewer >> [%s] ------------\n", FName);
		for (size_t i = 0; i < DBG_Tables.size(); i++) {
			printf("%s\n",DBG_Tables[i].data());
		}
	printf("-------------------------------------------------------------\n\n");
	
	int TableID;
	printf(">> Select Table by ID, or use value -1 to exit.\n");
	scanf("%i", &TableID);
	if (TableID == -1) return 0;
	if (TableID >= (int)DBG_Tables.size()) {
		printf("** TableID %i does not exist **\n", TableID);
		return 1;
	}

	char* TableName = DBG_Names[TableID].data();
	char* TypeName = DBG_Types[TableID].data();
	int TypeNameID = -1;
	
	if (strcmp(TypeName, "char") == 0) {
		CCEXP::LoadTable<char>( OBJ, TableName, TypeName ); TypeNameID = 0;		
	} else if (strcmp(TypeName, "uint8") == 0) {
		CCEXP::LoadTable<uint8_t>( OBJ,TableName, TypeName ); TypeNameID = 1;
	} else if (strcmp(TypeName, "int16") == 0) {
		CCEXP::LoadTable<int16_t>( OBJ,TableName, TypeName ); TypeNameID = 2;
	} else if (strcmp(TypeName, "uint16") == 0) {
		CCEXP::LoadTable<uint16_t>( OBJ,TableName, TypeName ); TypeNameID = 3;
	} else if (strcmp(TypeName, "int32") == 0) {
		CCEXP::LoadTable<int32_t>( OBJ,TableName, TypeName ); TypeNameID = 4;
	} else if (strcmp(TypeName, "uint32") == 0) {
		CCEXP::LoadTable<uint32_t>( OBJ,TableName, TypeName ); TypeNameID = 5;
	} else if (strcmp(TypeName, "int64") == 0) {
		CCEXP::LoadTable<int64_t>( OBJ,TableName, TypeName ); TypeNameID = 6;
	} else if (strcmp(TypeName, "uint64") == 0) {
		CCEXP::LoadTable<uint64_t>( OBJ,TableName, TypeName ); TypeNameID = 7;
	} else if (strcmp(TypeName, "single") == 0) {
		CCEXP::LoadTable<float>( OBJ,TableName, TypeName ); TypeNameID = 8;
	} else if (strcmp(TypeName, "double") == 0) {
		CCEXP::LoadTable<double>( OBJ,TableName, TypeName ); TypeNameID = 9;
	} else {
		printf("Error: [%s] is unsupported type!\n", TypeName);
	}
	__CCEXP_ERR_DISPLAY(OBJ,-1);

	size_t rows = CCEXP::Rows(OBJ,TableName);
	printf("Table %i -> [%s]::(%s) loaded. Rows of table: " __ZU__ ":\n",TableID, TableName, TypeName, rows);
	
	for (size_t i = 0; i < (size_t)rows; i++) {
		size_t cols = CCEXP::Cols(OBJ, TableName, i);
		printf("Row [" __ZU__ "] -> columns: " __ZU__ "\n", i, cols);
	}
	
	printf(">> Select Row by ID, or use value -1 to exit.\n");
	int RowID;
	scanf("%i", &RowID);
	if (RowID == -1) { CCEXP::CleanTable(OBJ, TableName); return -1; }
	if (RowID >= (int)rows) {
		printf("** Row %i does not exist **\n", RowID);
		CCEXP::CleanTable(OBJ, TableName);
		return 1;
	}
	
	char outFName[512] = {0};
	printf("Give file to write the row's data: \n");
	scanf("%510s", outFName);
	
	FILE *fp = fopen(outFName,"wb");
	if (fp == NULL) {
		printf("Failed to open [%s] file for writting...\n", outFName);
		CCEXP::CleanTable(OBJ, TableName);
		return -1;
	}
	
	size_t cols = CCEXP::Cols(OBJ, TableName, RowID);
	size_t tempcols;
	switch (TypeNameID) {
		case 0:
			fwrite(CCEXP::getRow<char>(OBJ,TableName,RowID,tempcols), sizeof(char), cols, fp);
			break;
		case 1:
			fwrite(CCEXP::getRow<uint8_t>(OBJ,TableName,RowID,tempcols), sizeof(uint8_t), cols, fp);
			break;
		case 2:
			fwrite(CCEXP::getRow<int16_t>(OBJ,TableName,RowID,tempcols), sizeof(int16_t), cols, fp);
			break;
		case 3:
			fwrite(CCEXP::getRow<uint16_t>(OBJ,TableName,RowID,tempcols), sizeof(uint16_t), cols, fp);
			break;
		case 4:
			fwrite(CCEXP::getRow<int32_t>(OBJ,TableName,RowID,tempcols), sizeof(int32_t), cols, fp);
			break;
		case 5:
			fwrite(CCEXP::getRow<uint32_t>(OBJ,TableName,RowID,tempcols), sizeof(uint32_t), cols, fp);
			break;
		case 6:
			fwrite(CCEXP::getRow<int64_t>(OBJ,TableName,RowID,tempcols), sizeof(int64_t), cols, fp);
			break;
		case 7:
			fwrite(CCEXP::getRow<uint64_t>(OBJ,TableName,RowID,tempcols), sizeof(uint64_t), cols, fp);
			break;
		case 8:
			fwrite(CCEXP::getRow<float>(OBJ,TableName,RowID,tempcols), sizeof(float), cols, fp);
			break;
		case 9:
			fwrite(CCEXP::getRow<double>(OBJ,TableName,RowID,tempcols), sizeof(double), cols, fp);
			break;
		default:
			printf("Could not store data (Type not found!)!\n");
	}
	
	fclose(fp);
	
	CCEXP::CleanTable(OBJ, TableName);
	
	return -1;
}


int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Please provide a filename\n");
		return -1;
	}

	snprintf(FName,511,"%s",argv[1]);
	
	CCEXP::Analyze(const_cast<char*>(FName), DBG_Tables);
	CCEXP::Analyze(const_cast<char*>(FName), DBG_Names, 1);
	CCEXP::Analyze(const_cast<char*>(FName), DBG_Types, 2);

	CCEXP::Open(OBJ, const_cast<char*>(FName));
		do {
			printf("\n-------------------------------------------------------------\n");
			printf("CCEXP Library (%1.3f): "
						 "https://github.com/terablade2001/CCEXP\n",
						 CCEXP_VERSION
			);
		} while (ExtractRow() != 0);
	CCEXP::Close(OBJ);

	printf("-------------------------------------------------------------\n\n");
	return 0;
}
