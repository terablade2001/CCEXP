// MIT License

// Copyright (c) 2016 Vasileios Kon. Pothos (terablade2001)

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
namespace CCEXP {
	
CCEXP::CCEXP() :
	Status(CCEXPORTMAT_INIT),
	fp(NULL),
	TotalMatrices(-1),
	LoadIndex(0),
	STCounter(0),
	isActive(true)
{ 
	Errors.clear();
}

CCEXP::~CCEXP() {
	Status = CCEXPORTMAT_DIED;
}

size_t CCEXP::getTableIndexByName(const char* Name) {
	size_t sel = -1;
	for (size_t i = 0; i < M.size(); i++)
		if ((M[i])->CompareName(Name) == 0) { sel=i; break; }
	return sel;	
}


//@#: ############### CCEXP API Functions ###############
int Initialize(CCEXP &obj, const char* fname, const char* Path, bool isactive) {
	obj.Errors.clear();
	obj.isActive = isactive;
	if (!obj.isActive) return 0; // This is normal return, if the object is non-active. It's not an error.
	if (obj.Status != CCEXPORTMAT_INIT) CCEXP_ERR(obj , ERROR::StatusNotInit , "Initialize():: CCEXP object with name [%s] has wrong status. Confirm you have reset it first." , fname );
	if (Path == NULL) sprintf(obj.SavingFile,"%s",fname);
	else sprintf(obj.SavingFile,"%s/%s", Path, fname);
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}



int StoreData(CCEXP &obj, const char* FName) {
	const char* SelName = (FName == NULL)? const_cast<char*>(obj.SavingFile) : FName ;
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "StoreData():: CCEXP object with filename [%s] has wrong status." , SelName );
	obj.Status = CCEXPORTMAT_ACTIVE;
	FILE *fp;
	fp = fopen(SelName,"wb");
	
	if (fp == NULL) {
		obj.Status = CCEXPORTMAT_READY;
		CCEXP_ERR(obj , ERROR::IO_Error , "StoreData():: Failed to open for writting the file [%s]." , SelName );
	}

	size_t MatricesToSave = 0;
	const size_t N = obj.M.size();
	for (size_t i=0; i < N; i++) {
		if (! (obj.M[i])->getIgnoreStatus()) { MatricesToSave++; }
	}
	
	if (fp != NULL) {
		uint32_t STBytes = sizeof(size_t);
		fwrite(&STBytes, sizeof(uint32_t), 1, fp); /// Store the size of (size_t)
	
		fwrite(&MatricesToSave,sizeof(size_t),1,fp);
		for (size_t i=0; i < N; i++) {
			if (! (obj.M[i])->getIgnoreStatus()) {
				(obj.M[i])->StoreData(fp);
			}
		}
		fclose(fp);
	}
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}



int StoreIData(CCEXP &obj) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "StoreIData():: CCEXP object with filename [%s] has wrong status. Confirm you have reset it first." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	char fname[256]={0};
	sprintf(fname, "%s.%4.4i", obj.SavingFile, obj.STCounter++);
	if (StoreData(obj, fname) != 0) {
		obj.Status = CCEXPORTMAT_READY;
		CCEXP_ERR(obj, ERROR::Other, "StoreIData():: Call to StoreData() failed...", 0);
	}
	const size_t N = obj.M.size();
	for (size_t i=0; i < N; i++) { (obj.M[i])->Reset(); }
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}



int NewLine(CCEXP &obj, size_t sel, int empty) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "NewLine():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->NewLine(empty);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "NewLine():: Table with ID = %lu return error during call to NewLine()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "NewLine():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int NewLine(CCEXP &obj, const char *matname, int empty) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "NewLine():: Failed to find table with name [%s]!", matname);
	return NewLine(obj, sel, empty); 
}



int Rows(CCEXP &obj, size_t sel, size_t &rows) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "Rows():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->Rows(rows);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "Rows():: Table with ID = %lu return error during call to Rows()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "Rows():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int Rows(CCEXP &obj, const char* matname, size_t &rows) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "Rows():: Failed to find table with name [%s]!", matname);
	return Rows(obj, sel, rows); 
}


int Cols(CCEXP &obj, size_t sel, size_t row, size_t &cols) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "Cols():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->Cols(row, cols);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "Cols():: Table with ID = %lu return error during call to Cols()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "Cols():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int Cols(CCEXP &obj, const char* matname, size_t row, size_t &cols) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "Cols():: Failed to find table with name [%s]!", matname);
	return Cols(obj, sel, row, cols); 
}

int DeleteLastRow(CCEXP &obj, size_t sel) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "DeleteLastRow():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastRow();
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "DeleteLastRow():: Table with ID = %lu return error during call to DeleteLastRow()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteLastRow():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int DeleteLastRow(CCEXP &obj, const char* matname) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteLastRow():: Failed to find table with name [%s]!", matname);
	return DeleteLastRow(obj, sel); 
}

int DeleteRow(CCEXP &obj, size_t sel, size_t row) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "DeleteRow():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteRow(row);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "DeleteRow():: Table with ID = %lu return error during call to DeleteRow()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteRow():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int DeleteRow(CCEXP &obj, const char* matname, size_t row) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteRow():: Failed to find table with name [%s]!", matname);
	return DeleteRow(obj, sel, row); 
}


int DeleteLastElement(CCEXP &obj, size_t sel, size_t row) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "DeleteLastElement():: CCEXP Table with ID [%lu] has wrong status." , (uint64_t)sel );
	if (sel < obj.M.size()) {
		int ret = (obj.M[sel])->DeleteLastElement(row);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "DeleteLastElement():: Table with ID = %lu return error during call to DeleteLastElement()...", (uint64_t)sel);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteLastElement():: Table with ID = %lu was not found!...", (uint64_t)sel);
}
int DeleteLastElement(CCEXP &obj, const char* matname, size_t row) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "DeleteLastElement():: Failed to find table with name [%s]!", matname);
	return DeleteLastElement(obj, sel, row); 
}


int getTableID(CCEXP &obj, const char* matname, size_t &sel) {
	obj.Status = CCEXPORTMAT_ACTIVE;
	sel = obj.getTableIndexByName(matname);	
	obj.Status = CCEXPORTMAT_READY;
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "getTableID():: Failed to find table with name [%s]!", matname);
}

int getTableName(CCEXP &obj, size_t sel, char* &matname) {
	obj.Status = CCEXPORTMAT_ACTIVE;
	const size_t N = obj.M.size();
	if (sel >= N) CCEXP_ERR(obj, ERROR::TableNotFound, "getTableName():: Table with ID = %lu was not found!... (Table ID >= CCEXP Tables)", (uint64_t)sel);
	matname = obj.M[sel]->getName();
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}


int Reset(CCEXP &obj) {
	obj.Status = CCEXPORTMAT_INIT;
	obj.M.clear();
	obj.TotalMatrices = -1;
	obj.LoadIndex = 0;
	obj.STCounter = 0;
	obj.isActive = true;
	return 0;
}

int GetErrors(
	CCEXP &obj,
	vector<string>* &ptrError,
	size_t &NumberOfErrors
) {
	obj.Status = CCEXPORTMAT_ACTIVE;
	NumberOfErrors = obj.Errors.size();
	ptrError = &(obj.Errors);
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}

}; // namespace CCEXP;
