// MIT License

// Copyright (c) 2016 - 2020 Vasileios Kon. Pothos (terablade2001)
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

#ifndef __ECSOBJ__
	CECS CCEXPECS("CCEXP","CECS::CCEXP");
#else
	CECS CCEXPECS("CCEXP");
#endif




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
}

CCEXP::CCEXP(void* pCECS) :
	fp(NULL),
	Status(CCEXPORTMAT_INIT),
	STCounter(0),
	isActive(true),
	ErrorId(0),
	lfp(NULL),
	LoadTotalTables(0),
	LoadTableIndex(0)
{ 
	CCEXPECS.ConnectTo(pCECS);
	SavingFile[0]=0;
	SavingFile[1]=0;
}


CCEXP::CCEXP(const char* fname, void* pCECS):
	fp(NULL),
	Status(CCEXPORTMAT_INIT),
	STCounter(0),
	isActive(true),
	ErrorId(0),
	lfp(NULL),
	LoadTotalTables(0),
	LoadTableIndex(0)
{
	CCEXPECS.ConnectTo(pCECS);
	SavingFile[0]=0;
	SavingFile[1]=0;

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
#define f(i,a,b) for(size_t i=a; i<b; i++)

size_t CCEXP::checkDuplicatedNames(const char* Name) {
	size_t cnt = 0;
	f(i,0,M.size())
// 	for (size_t i = 0; i < M.size(); i++)
		if ((M[i])->CompareName(Name) == 0) { cnt++; }
	return cnt; // return how many times a Table name was found in the list.
}

void CCEXP::setStatus(int st) { Status = st; }
	


//@#: ############### CCEXP API Functions ###############
void Initialize(CCEXP &obj, const char* fname, const char* Path, bool isactive) {
	obj.ErrorId = 0;
	obj.isActive = isactive;
	if (!obj.isActive) return; // This is normal return, if the object is non-active. It's not an error.
	int prevStatus = obj.Status;
	obj.Status = CCEXPORTMAT_ACTIVE;
	CECS_ERR(CCEXPECS,fname==NULL,"Initialize():: Export Filename is NULL!");
	if (Path == NULL) ccexp_snprintf(obj.SavingFile,256,"%s",fname)
	else ccexp_snprintf(obj.SavingFile,256,"%s/%s", Path, fname)
	CECS_ERR(CCEXPECS,prevStatus != CCEXPORTMAT_INIT, "[%s]: Initialize():: Wrong status detected!", fname)
	obj.Status = CCEXPORTMAT_READY;
}



void StoreData(CCEXP &obj, const char* FName) {
	obj.ErrorId = 0;
	const char* SelName = (FName == NULL)? const_cast<char*>(obj.SavingFile) : FName ;
	if (!obj.isActive) return;
	CECS_ERR(CCEXPECS,0==strcmp(SelName,""),"StoreData():: Empty filename [] for storing!")
	CECS_ERR(CCEXPECS,obj.Status!=CCEXPORTMAT_READY,"[%s]: StoreData():: Wrong status (%i) detected!", SelName, obj.Status)
	obj.Status = CCEXPORTMAT_ACTIVE;
	FILE *fp;
	__CCEXP_FOPEN__(fp, SelName, "wb");
	
	if (fp == NULL) {
		obj.Status = CCEXPORTMAT_READY;
		CECS_ERR(CCEXPECS,1,"[%s]: StoreData():: Failing opening file for writting...", SelName)
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
	CECS_ERRI(CCEXPECS,obj.Status!=CCEXPORTMAT_READY,"[%s]: StoreIData():: Wrong status (%i) detected!" , obj.SavingFile, obj.Status );
	obj.Status = CCEXPORTMAT_ACTIVE;
	char fname[385]={0};
	ccexp_snprintf(fname, 384, "%s.%4.4i", obj.SavingFile, obj.STCounter++);
	StoreData(obj, fname);
	if (obj.ErrorId != 0) {
		CECS_ERRI(CCEXPECS,1,"[%s]: StoreIData():: Call to StoreData() failed...", obj.SavingFile);
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
	CECS_ERR(CCEXPECS,obj.Status!=CCEXPORTMAT_READY, "[%s]: NewRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , obj.SavingFile, sel)
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->NewRow(empty);
		CECS_ERR(CCEXPECS,0!=ret, "[%s]: NewRow():: Table with ID = " __ZU__ " return error during call to NewRow()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1, "[%s]: NewRow():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
void NewRow(CCEXP &obj, const char *matname, int empty) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: NewRow():: Failed to find table with name [%s]!", obj.SavingFile, matname)
	NewRow(obj, sel, empty);
}


void NoNewRow(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	CECS_ERR(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: NoNewRow():: CCEXP Table with ID [" __ZU__ "] has wrong status." , obj.SavingFile, sel )
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->NoNewRow();
		CECS_ERR(CCEXPECS,ret != 0, "[%s]: NoNewRow():: Table with ID = " __ZU__ " return error during call to NoNewRow()...", obj.SavingFile, sel)
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1, "[%s]: NoNewRow():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel)
}
void NoNewRow(CCEXP &obj, const char *matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: NoNewRow():: Failed to find table with name [%s]!", obj.SavingFile, matname)
	NoNewRow(obj, sel);
}


size_t Rows(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	CECS_ERRI(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: Rows():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile, sel )
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		size_t rows;
		int ret = (obj.M[sel])->Rows(rows);
		CECS_ERRI(CCEXPECS,ret != 0, "[%s]: Rows():: Table with ID = " __ZU__ " return error during call to Rows()...", obj.SavingFile, sel)
		obj.Status = CCEXPORTMAT_READY;
		return rows;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERRI(CCEXPECS,1, "[%s]: Rows():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
size_t Rows(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERRI(CCEXPECS,sel == MAXSIZE_T, "[%s]: Rows():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	return Rows(obj, sel); 
}



size_t Cols(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	CECS_ERRI(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: Cols():: CCEXP Table with ID [" __ZU__ "] has wrong status." , obj.SavingFile, sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		size_t cols;
		int ret = (obj.M[sel])->Cols(row, cols);
		CECS_ERRI(CCEXPECS,ret != 0, "[%s]: Cols():: Table with ID = " __ZU__ " return error during call to Cols()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return cols;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERRI(CCEXPECS,1,"[%s]: Cols():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
size_t Cols(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERRI(CCEXPECS,sel == MAXSIZE_T,"[%s]: Cols():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	return Cols(obj, sel, row); 
}


bool Ignored(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return 0;
	CECS_ERRB(CCEXPECS,obj.Status != CCEXPORTMAT_READY,"[%s]: Ignored():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile, sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		bool ret = (obj.M[sel])->getIgnoreStatus();
		obj.Status = CCEXPORTMAT_READY;
		return ret;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERRB(CCEXPECS,1,"[%s]: Ignored():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
bool Ignored(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERRB(CCEXPECS,sel == MAXSIZE_T,"[%s]: Ignored():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	return Ignored(obj, sel); 
}



void DeleteLastRow(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	CECS_ERR(CCEXPECS,obj.Status != CCEXPORTMAT_READY,"[%s]: DeleteLastRow():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastRow();
		CECS_ERR(CCEXPECS,ret != 0,"[%s]: DeleteLastRow():: Table with ID = " __ZU__ " return error during call to DeleteLastRow()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1,"[%s]: DeleteLastRow():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
void DeleteLastRow(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: DeleteLastRow():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	DeleteLastRow(obj, sel);
}

void DeleteRow(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	CECS_ERR(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: DeleteRow():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteRow(row);
		CECS_ERR(CCEXPECS,ret != 0, "[%s]: DeleteRow():: Table with ID = " __ZU__ " return error during call to DeleteRow()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1, "[%s]: DeleteRow():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
void DeleteRow(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: DeleteRow():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	DeleteRow(obj, sel, row); 
}


void DeleteLastElement(CCEXP &obj, size_t sel, size_t row) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	CECS_ERR(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: DeleteLastElement():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastElement(row);
		CECS_ERR(CCEXPECS,ret != 0, "[%s]: DeleteLastElement():: Table with ID = " __ZU__ " return error during call to DeleteLastElement()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1, "[%s]: DeleteLastElement():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
void DeleteLastElement(CCEXP &obj, const char* matname, size_t row) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: DeleteLastElement():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	DeleteLastElement(obj, sel, row); 
}


size_t getTableID(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t sel = obj.getTableIndexByName(matname);
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERRI(CCEXPECS,sel == MAXSIZE_T, "[%s]: getTableID():: Failed to find table with name [%s]!", obj.SavingFile, matname);
	return sel;
}

char* getTableName(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	obj.Status = CCEXPORTMAT_ACTIVE;
	const size_t N = obj.M.size();
	CECS_ERRN(CCEXPECS,sel >= N, "[%s]: getTableName():: Table with ID = " __ZU__ " was not found!... (Table ID >= CCEXP Tables)", obj.SavingFile, sel);
	char* matname = obj.M[sel]->getName();
	obj.Status = CCEXPORTMAT_READY;
	return matname;
}



void CleanTable(CCEXP &obj, size_t sel) {
	obj.ErrorId = 0;
	CECS_ERR(CCEXPECS,obj.Status != CCEXPORTMAT_READY, "[%s]: CleanTable():: CCEXP Table with ID [" __ZU__ "] has wrong status.", obj.SavingFile , sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->Reset();
		CECS_ERR(CCEXPECS,ret != 0, "[%s]: CleanTable():: Table with ID = " __ZU__ " return error during call to CleanTable()...", obj.SavingFile, sel);
		obj.Status = CCEXPORTMAT_READY;
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CECS_ERR(CCEXPECS,1, "[%s]: CleanTable():: Table with ID = " __ZU__ " was not found!...", obj.SavingFile, sel);
}
void CleanTable(CCEXP &obj, const char* matname) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	CECS_ERR(CCEXPECS,sel == MAXSIZE_T, "[%s]: CleanTable():: Failed to find table with name [%s]!", obj.SavingFile, matname);
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
	return;
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
	CECS_ERR(CCEXPECS,filename == NULL,"[%s]: Load():: Loading Filename is NULL!", obj.SavingFile);
	CECS_ERR(CCEXPECS,(obj.Status != CCEXPORTMAT_INIT) && (obj.Status != CCEXPORTMAT_READY), "[%s]: Open():: CCEXP object has wrong status!", obj.SavingFile);
	if ((obj.SavingFile[0] == 0) && (obj.SavingFile[1] == 0)) Initialize(obj, filename);
	
	obj.Status = CCEXPORTMAT_ACTIVE;
		if (obj.lfp != NULL) fclose(obj.lfp);
		
		// Open the file but do not close it. CCEXP::Close() is for this job.
		// obj.lfp = fopen(filename,"rb");
		__CCEXP_FOPEN__(obj.lfp, filename, "rb");
		FILE* lfp = obj.lfp;
		CECS_ERR(CCEXPECS,lfp == NULL,"[%s]: Open():: Failed to open for reading data." , filename );		

		uint32_t LoadSTBytes;
		fread(&LoadSTBytes, sizeof(uint32_t), 1, lfp);
		CECS_ERR(CCEXPECS,LoadSTBytes != sizeof(uint64_t),"[%s]: Open():: sizeof(uint64_t) is different than the one stored at the file." , filename );

		uint64_t LoadTotalTables;
		fread(&LoadTotalTables,sizeof(uint64_t),1,lfp);
		obj.LoadTotalTables = (size_t)LoadTotalTables;

		obj.LoadTableIndex = 0;
	obj.Status = CCEXPORTMAT_READY;
	return;
}



void Close(CCEXP &obj) {
	obj.ErrorId = 0;
	CECS_ERR(CCEXPECS,obj.lfp == NULL,"[%s]: Close():: Required file is already closed", obj.SavingFile );
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
			ccexp_snprintf(v[i].data(), 256,
				"[%3.1" __ZUn_ "]> [1x %5.0" __ZUn_ " ] [%i] :: (%s): %s",
				i, cols, 1-(int)(obj.M[i])->getIgnoreStatus(),
				(obj.M[i])->getType(), obj.M[i]->getName()
			);
		} else {
			ccexp_snprintf(v[i].data(), 256,
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
		fseek(lfp, (long)TableBytes, SEEK_CUR);

		if (flag == 0) {
			if (N > 1) {
				ccexp_snprintf(v[i].data(), 256,
					"[%3.1" __ZUn_ "]> [%5.0" __ZUn_ " {} ] [%i] :: (%s): %s",
					(size_t)i, (size_t)N, 1,
					loadType, loadName
				);
			} else {
				if (N > 0) N = DPL[0];
				ccexp_snprintf(v[i].data(), 256,
					"[%3.1" __ZUn_ "]> [1x %5.0" __ZUn_ " ] [%i] :: (%s): %s",
					(size_t)i, (size_t)N, 1,
					loadType, loadName
				);
			}
		} else if (flag == 1) {
			ccexp_snprintf(v[i].data(), 256, "%s", loadName);
		} else if (flag == 2) {
			ccexp_snprintf(v[i].data(), 256, "%s", loadType);
		}
	}
	fclose(lfp);
	return 0;
}

void*  getCECS() {
	return CCEXPECS.cecs();
}
const char* getCECSErrors(int typeId) {
	return CCEXPECS.str(typeId);
}


}; // namespace CCEXP;
