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

#include "include/CCEXP.hpp"

using namespace std;
__USE_MVECTOR_NAMESPACE__

namespace CCEXP {

CCEXP::CCEXP() :
	fp(NULL),
	Status(CCEXPORTMAT_INIT),
	STCounter(0),
	isActive(true),
	ErrorId(0),
	lfp(NULL),
	LoadTotalTables(0),
	LoadTableIndex(0)
{ 
	SavingFile[0]=0;
	SavingFile[1]=0;
	__CCEXP_VECTOR_STEPS(errorchar,1,1);
	__CCEXP_VECTOR_CLEAR(Errors);
	errorchar.clear();
}

CCEXP::CCEXP(const char* fname):
	fp(NULL),
	Status(CCEXPORTMAT_INIT),
	STCounter(0),
	isActive(true),
	ErrorId(0),
	lfp(NULL),
	LoadTotalTables(0),
	LoadTableIndex(0)
{
	SavingFile[0]=0;
	SavingFile[1]=0;
	__CCEXP_VECTOR_CLEAR(Errors);

	Initialize(*this, fname);
}

CCEXP::~CCEXP() {
	if (lfp != NULL) fclose(lfp);
	Status = CCEXPORTMAT_DIED;
}

size_t CCEXP::getTableIndexByName(const char* Name) {
	size_t sel = MAXSIZE_T;
	for (size_t i = 0; i < M.size(); i++)
		if ((M[i])->CompareName(Name) == 0) { sel=i; break; }
	return sel;	
}

size_t CCEXP::checkDuplicatedNames(const char* Name) {
	size_t cnt = 0;
	for (size_t i = 0; i < M.size(); i++)
		if ((M[i])->CompareName(Name) == 0) { cnt++; }
	return cnt; // return how many times a Table name was found in the list.
}

void CCEXP::setStatus(int st) { Status = st; }
	


//@#: ############### CCEXP API Functions ###############
void Initialize(CCEXP &obj, const char* fname, const char* Path, bool isactive) {
	__CCEXP_VECTOR_CLEAR(obj.Errors);
	obj.ErrorId = 0;
	obj.isActive = isactive;
	if (!obj.isActive) return; // This is normal return, if the object is non-active. It's not an error.
	int prevStatus = obj.Status;
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (prevStatus != CCEXPORTMAT_INIT) CCEXP_ERR_V(obj , ERROR::StatusNotInit , "Initialize():: CCEXP object with name [%s] has wrong status. Confirm you have reset it first." , fname );
	if (fname == NULL) CCEXP_ERR_V(obj, ERROR::IO_Error , "Initialize():: Export Filename is NULL! (!%u!)", 0);
	if (Path == NULL) snprintf(obj.SavingFile,256,"%s",fname);
	else snprintf(obj.SavingFile,256,"%s/%s", Path, fname);
	obj.Status = CCEXPORTMAT_READY;
}



void StoreData(CCEXP &obj, const char* FName) {
	obj.ErrorId = 0;
	const char* SelName = (FName == NULL)? const_cast<char*>(obj.SavingFile) : FName ;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "StoreData():: CCEXP object with filename [%s] has wrong status." , SelName );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (strcmp(SelName,"") == 0) CCEXP_ERR_V(obj , ERROR::StatusNotInit , "StoreData():: CCEXP object has no filename to store the data! (!%u!)", 0);
	FILE *fp;
	// fp = fopen(SelName,"wb");
	__CCEXP_FOPEN__(fp, SelName, "wb");
	
	if (fp == NULL) {
		obj.Status = CCEXPORTMAT_READY;
		CCEXP_ERR_V(obj , ERROR::IO_Error , "StoreData():: Failed to open for writting the file [%s]." , SelName );
	}

	uint64_t MatricesToSave = 0;
	const uint64_t N = (uint64_t)obj.M.size();
	for (uint64_t i=0; i < N; i++) {
		if (! (obj.M[i])->getIgnoreStatus()) { MatricesToSave++; }
	}
	
	if (fp != NULL) {
		uint32_t STBytes = sizeof(uint64_t);
		fwrite(&STBytes, sizeof(uint32_t), 1, fp); /// Store the size of (uint64_t)
	
		fwrite(&MatricesToSave,sizeof(uint64_t),1,fp);
		for (uint64_t i=0; i < N; i++) {
			if (! (obj.M[i])->getIgnoreStatus()) {
				(obj.M[i])->StoreData(fp);
			}
		}
		fclose(fp);
	}
	obj.Status = CCEXPORTMAT_READY;
}



size_t StoreIData(CCEXP &obj) {
	obj.ErrorId = 0;
	const size_t RetV = obj.STCounter;
	if (!obj.isActive) return RetV;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, RetV, ERROR::StatusNotReady , "StoreIData():: CCEXP object with filename [%s] has wrong status. Confirm you have reset it first." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	char fname[257]={0};
	snprintf(fname, 256, "%s.%4.4i", obj.SavingFile, obj.STCounter++);
	StoreData(obj, fname);
	if (obj.ErrorId != 0) {
		CCEXP_ERR_T(obj, RetV, ERROR::Other, "StoreIData():: Call to StoreData() failed... (!%u!)", 0);
		obj.Status = CCEXPORTMAT_READY;
	}
	const uint64_t N = (uint64_t)obj.M.size();
	for (uint64_t i=0; i < N; i++) { (obj.M[i])->Reset(); }
	obj.Status = CCEXPORTMAT_READY;
	return RetV;
}



void NewRow(CCEXP &obj, size_t sel, int empty) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "NewRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->NewRow(empty);
		if (ret != 0) CCEXP_ERR_V(obj, ret, "NewRow():: Table with ID = " __ZU__ " return error during call to NewRow()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "NewRow():: Table with ID = " __ZU__ " was not found!...", sel);
}
void NewRow(CCEXP &obj, const char *matname, int empty) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "NewRow():: Failed to find table with name [%s]!", matname);
	NewRow(obj, sel, empty);
}


void NoNewRow(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "NoNewRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->NoNewRow();
		if (ret != 0) CCEXP_ERR_V(obj, ret, "NoNewRow():: Table with ID = " __ZU__ " return error during call to NoNewRow()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "NoNewRow():: Table with ID = " __ZU__ " was not found!...", sel);
}
void NoNewRow(CCEXP &obj, const char *matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "NoNewRow():: Failed to find table with name [%s]!", matname);
	NoNewRow(obj, sel);
}


size_t Rows(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, 0, ERROR::StatusNotReady, "Rows():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		size_t rows;
		int ret = (obj.M[sel])->Rows(rows);
		if (ret != 0) CCEXP_ERR_T(obj, 0, ret, "Rows():: Table with ID = " __ZU__ " return error during call to Rows()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return rows;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Rows():: Table with ID = " __ZU__ " was not found!...", sel);
}
size_t Rows(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Rows():: Failed to find table with name [%s]!", matname);
	return Rows(obj, sel); 
}



size_t Cols(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, 0, ERROR::StatusNotReady , "Cols():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		size_t cols;
		int ret = (obj.M[sel])->Cols(row, cols);
		if (ret != 0) CCEXP_ERR_T(obj, 0, ret, "Cols():: Table with ID = " __ZU__ " return error during call to Cols()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return cols;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Cols():: Table with ID = " __ZU__ " was not found!...", sel);
}
size_t Cols(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Cols():: Failed to find table with name [%s]!", matname);
	return Cols(obj, sel, row); 
}


bool Ignored(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, 0, ERROR::StatusNotReady, "Ignored():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		bool ret = (obj.M[sel])->getIgnoreStatus();
		obj.Status = CCEXPORTMAT_READY;
		return ret;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Ignored():: Table with ID = " __ZU__ " was not found!...", sel);
}
bool Ignored(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_T(obj, 0, ERROR::TableNotFound, "Ignored():: Failed to find table with name [%s]!", matname);
	return Ignored(obj, sel); 
}



void DeleteLastRow(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "DeleteLastRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastRow();
		if (ret != 0) CCEXP_ERR_V(obj, ret, "DeleteLastRow():: Table with ID = " __ZU__ " return error during call to DeleteLastRow()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteLastRow():: Table with ID = " __ZU__ " was not found!...", sel);
}
void DeleteLastRow(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteLastRow():: Failed to find table with name [%s]!", matname);
	DeleteLastRow(obj, sel);
}

void DeleteRow(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "DeleteRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteRow(row);
		if (ret != 0) CCEXP_ERR_V(obj, ret, "DeleteRow():: Table with ID = " __ZU__ " return error during call to DeleteRow()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteRow():: Table with ID = " __ZU__ " was not found!...", sel);
}
void DeleteRow(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteRow():: Failed to find table with name [%s]!", matname);
	DeleteRow(obj, sel, row); 
}


void DeleteLastElement(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "DeleteLastElement():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastElement(row);
		if (ret != 0) CCEXP_ERR_V(obj, ret, "DeleteLastElement():: Table with ID = " __ZU__ " return error during call to DeleteLastElement()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteLastElement():: Table with ID = " __ZU__ " was not found!...", sel);
}
void DeleteLastElement(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "DeleteLastElement():: Failed to find table with name [%s]!", matname);
	DeleteLastElement(obj, sel, row); 
}


size_t getTableID(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t sel = obj.getTableIndexByName(matname);
	obj.Status = CCEXPORTMAT_READY;
	if (sel == MAXSIZE_T) CCEXP_ERR_T(obj, MAXSIZE_T, ERROR::TableNotFound, "getTableID():: Failed to find table with name [%s]!", matname);
	return sel;
}

char* getTableName(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	obj.Status = CCEXPORTMAT_ACTIVE;
	const size_t N = obj.M.size();
	if (sel >= N) CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getTableName():: Table with ID = " __ZU__ " was not found!... (Table ID >= CCEXP Tables)", sel);
	char* matname = obj.M[sel]->getName();
	obj.Status = CCEXPORTMAT_READY;
	return matname;
}



void CleanTable(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "CleanTable():: CCEXP Table with ID [" __ZU__ "] has wrong status." , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->Reset();
		if (ret != 0) CCEXP_ERR_V(obj, ret, "CleanTable():: Table with ID = " __ZU__ " return error during call to CleanTable()...", sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "CleanTable():: Table with ID = " __ZU__ " was not found!...", sel);
}
void CleanTable(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "CleanTable():: Failed to find table with name [%s]!", matname);
	CleanTable(obj, sel); 
}


void Reset(CCEXP &obj) {
	obj.ErrorId = 0;
	obj.Status = CCEXPORTMAT_ACTIVE;
	__CCEXP_VECTOR_CLEAR(obj.M);
	if (obj.fp  != NULL) { fclose(obj.fp);  obj.fp  = NULL; }
	if (obj.lfp != NULL) { fclose(obj.lfp); obj.lfp = NULL; }
	obj.STCounter       = 0;
	obj.isActive        = true;
	obj.LoadTotalTables = 0;
	obj.LoadTableIndex  = 0 ;
	obj.Status = CCEXPORTMAT_INIT;
	obj.errorchar.clear();
	return;
}

size_t GetErrors(
	CCEXP &obj,
	MVECTOR<string>* &ptrError
) {
	// This function should not change CCEXP object status. Thus if a critical
	// error occurs, then all the CCEXP object should be disabled (status errors
	// for every call) denoting to the client that he must fix the error.
	// Just for example-debugging reasons the function:
	// CCEXP::DBG_SetStatus() can be used to change CCEXP status via force.
	size_t NumberOfErrors = obj.Errors.size();
	ptrError = &(obj.Errors);
	return NumberOfErrors;
}

char*	GetErrors(CCEXP &obj, size_t &rows) {
	size_t NumberOfErrors = obj.Errors.size();
	rows = NumberOfErrors;
	obj.errorchar.resize(CCEXP_MAX_ERRORCHAR_SIZE+1);
	obj.errorchar[0] = 0; obj.errorchar[1] = 0;
	if (NumberOfErrors == 0) {
		sprintf(obj.errorchar.data(),"CCEXP 0 Errors");
	} else {
		sprintf(obj.errorchar.data(),"------- CCEXP " __ZU__ " Errors -------",
			NumberOfErrors
		);
		for (size_t i = 0; i < NumberOfErrors; i++) {
			snprintf(
				obj.errorchar.data(), CCEXP_MAX_ERRORCHAR_SIZE, "%s\n%s",
				obj.errorchar.data(), obj.Errors[i].c_str()
			);
		}
	}
	return obj.errorchar.data();
}

size_t GetNErrors(CCEXP &obj) {
	return obj.Errors.size();
}

size_t NumberOfTables(CCEXP &obj) {
	return obj.M.size();
}

void DBG_SetStatus(CCEXP &obj, int status) {
	obj.setStatus(status);
}

void Open(CCEXP &obj, const char* filename) {
	obj.ErrorId = 0;
	if (!obj.isActive) obj.isActive = true;	// An Open() call always enable a CCEXP object.
	if (filename == NULL) CCEXP_ERR_V(obj, ERROR::IO_Error , "Load():: Loading Filename is NULL! (!%u!)", 0);
	if ((obj.Status != CCEXPORTMAT_INIT) && (obj.Status != CCEXPORTMAT_READY))
		CCEXP_ERR_V(obj , ERROR::StatusIsWrong , "Open():: CCEXP object has wrong status! (!%u!)", 0);
	if ((obj.SavingFile[0] == 0) && (obj.SavingFile[1] == 0)) Initialize(obj, filename);
	
	obj.Status = CCEXPORTMAT_ACTIVE;
		if (obj.lfp != NULL) fclose(obj.lfp);
		
		// Open the file but do not close it. CCEXP::Close() is for this job.
		// obj.lfp = fopen(filename,"rb");
		__CCEXP_FOPEN__(obj.lfp, filename, "rb");
		FILE* lfp = obj.lfp;
		if (lfp == NULL) CCEXP_ERR_V(obj , ERROR::IO_Error , "Open():: Failed to open for reading the file [%s]." , filename );		

		uint32_t LoadSTBytes;
		fread(&LoadSTBytes, sizeof(uint32_t), 1, lfp);
		if (LoadSTBytes != sizeof(uint64_t)) 
			CCEXP_ERR_V(obj , ERROR::IO_Error , "Open():: sizeof(uint64_t) is different than the one stored at the file [%s]." , filename );

		uint64_t LoadTotalTables;
		fread(&LoadTotalTables,sizeof(uint64_t),1,lfp);
		obj.LoadTotalTables = (size_t)LoadTotalTables;

		obj.LoadTableIndex = 0;
	obj.Status = CCEXPORTMAT_READY;
	return;
}



void Close(CCEXP &obj) {
	obj.ErrorId = 0;
	if (obj.lfp == NULL) CCEXP_ERR_V(obj , ERROR::CCEXP_AlreadyClosed , "Close():: Required file is alread closed (!%u!)", 0 );
	fclose(obj.lfp); obj.lfp = NULL;
	return;
}

int Analyze(CCEXP &obj, MVECTOR<MVECTOR<char>> &v) {
	__CCEXP_VECTOR_CLEAR(v);
	__CCEXP_VECTOR_STEPS(v,1,1);
	size_t NTables = obj.M.size();
	v.cresize(NTables);
	for (size_t i = 0; i < NTables; i++) {
		__CCEXP_VECTOR_STEPS(v[i],1,1);
		v[i].cresize(260,0);
		size_t rows = Rows(obj, i);
		if (rows == 1) {
			size_t cols = Cols(obj, i, 0);
			snprintf(v[i].data(), 256,
				"[%3.1" __ZUn_ "]> [1x %5.0" __ZUn_ " ] [%i] :: (%s): %s",
				i, cols, 1-(int)(obj.M[i])->getIgnoreStatus(),
				(obj.M[i])->getType(), obj.M[i]->getName()
			);
		} else {
			snprintf(v[i].data(), 256,
				"[%3.1" __ZUn_ "]> [%5.0" __ZUn_ " {} ] [%i] :: (%s): %s",
				i, rows, 1-(int)(obj.M[i])->getIgnoreStatus(),
				(obj.M[i])->getType(), obj.M[i]->getName()
			);
		}
	}
	return 0;
}

int Analyze(const char* filename, MVECTOR<MVECTOR<char>> &v, int flag) {
	if (filename == NULL) { return -1; }

	FILE* lfp;
	__CCEXP_FOPEN__(lfp, filename, "rb");
	if (lfp == NULL) { return -1; }

	uint32_t LoadSTBytes;
	fread(&LoadSTBytes, sizeof(uint32_t), 1, lfp);
	if (LoadSTBytes != sizeof(uint64_t)) { fclose(lfp); return -1; }

	uint64_t LoadTotalTables;
	fread(&LoadTotalTables, sizeof(uint64_t), 1, lfp);

	char loadName[65]={0};
	char loadType[65]={0};
	uint64_t typeSize=0;
	uint64_t N=0;
	uint64_t MaxRows=0;

	__CCEXP_VECTOR_CLEAR(v);
	__CCEXP_VECTOR_STEPS(v,1,1);
	v.cresize(LoadTotalTables);
	MVECTOR<uint64_t> DPL;
	__CCEXP_VECTOR_STEPS(DPL,1,1);
	for (uint64_t i = 0; i < (uint64_t)LoadTotalTables; i++) {
		v[i].cresize(260,0);

		fread(loadName, sizeof(char), 64, lfp);
		fread(loadType, sizeof(char), 64, lfp);
		fread(&typeSize, sizeof(uint64_t), 1, lfp);
		fread(&N, sizeof(uint64_t), 1, lfp);
		fread(&MaxRows, sizeof(uint64_t), 1, lfp);
		if (N > MaxRows) N = MaxRows;
		DPL.resize(N);
		fread(DPL.data(), sizeof(uint64_t), N, lfp);
		uint64_t TableBytes = 0;
		for (uint64_t c = 0; c < N; c++)
			TableBytes += DPL[c]*typeSize;
		fseek(lfp, TableBytes, SEEK_CUR);

		if (flag == 0) {
			if (N > 1) {
				snprintf(v[i].data(), 256,
					"[%3.1" __ZUn_ "]> [%5.0" __ZUn_ " {} ] [%i] :: (%s): %s",
					(size_t)i, (size_t)N, 1,
					loadType, loadName
				);
			} else {
				if (N > 0) N = DPL[0];
				snprintf(v[i].data(), 256,
					"[%3.1" __ZUn_ "]> [1x %5.0" __ZUn_ " ] [%i] :: (%s): %s",
					(size_t)i, (size_t)N, 1,
					loadType, loadName
				);
			}
		} else if (flag == 1) {
			snprintf(v[i].data(), 256, "%s", loadName);
		} else if (flag == 2) {
			snprintf(v[i].data(), 256, "%s", loadType);
		}
	}
	fclose(lfp);
	return 0;
}

}; // namespace CCEXP;
