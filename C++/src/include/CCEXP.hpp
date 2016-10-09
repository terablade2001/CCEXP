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

#ifndef __CCEXP_HPP__
#define __CCEXP_HPP__

// Include C/C++ Libraries
#include <stdlib.h>
#include <cstring>
#include <memory>
#include <list>
#include <vector>
#include <cstdio>
#include <limits>
#include <iostream>

#define CCEXP_VERSION (0.021)
#define TRACK_ANALYTIC_ERRORS

#ifndef __FNAME__
	#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define CCEXPORTMAT_INIT		(0)
#define CCEXPORTMAT_READY		(1)
#define CCEXPORTMAT_ACTIVE		(2)
#define CCEXPORTMAT_DIED		(3)


#ifdef TRACK_ANALYTIC_ERRORS
	#define CCEXP_ERR(obj, errid, str, args...) { \
		char ccexp_errStr[2048]={0}; sprintf(ccexp_errStr, str, args); \
		if ((obj).Errors.size() < 128) (obj).Errors.push_back(string(ccexp_errStr)); \
		return errid; }

	#define __CCEXP_ERR_DISPLAY(obj, N) { \
		size_t n=size_t(N); if (n > 0) { \
		vector<string>* v; size_t TN;  \
		CCEXP::GetErrors(obj, v, TN); size_t DN = std::min(n, TN); \
		if (TN != 0) { cout << endl << "[" << __FNAME__ << ", " << __LINE__ << "]: CCEXP Err Display >> " << endl; \
		for (size_t i = 0; i < DN; i++) cout << " * " << (*v)[i].c_str() << endl; \
		(obj).Errors.clear(); }} }
#endif
#ifndef TRACK_ANALYTIC_ERRORS
	#define CCEXP_ERR(obj, errid, str, args...) return errid;
	#define __CCEXP_ERR_DISPLAY(obj, N) //...
#endif

using namespace std;

namespace CCEXP {
	
enum ERROR {
    StatusNotInit = -9999, /// CCEXP object had status different that CCEXPORTMAT_INIT. (i.e. not Reseted before re-Initialize)
    StatusNotReady, /// CCEXP object was not at READY state when a new call to it was requested (thread-safe).
	StatusIsWrong, /// Generic wrong status message (i.e. for Load())
    IO_Error, /// Access to a I/O file failed (i.e. FILE* fp = fopen(...) == NULL )
    MaximumRowsReached, /// When Adding a new row, and maximum rows have been reached!
	ReqRowLargerThanInTable, /// When a requested row index is larger than the maximum rows in Table.
	ReqColumnLargerThanInRow, /// When a requested column index is larger than the maximum columns in a Table's row.
	TableNotFound, /// When a table is not found...
	TableHasNoRows, /// When a table has no rows, but it's requested to do something on a row.
	DublicatedTable, /// When a table is added which has the same name with other table.
	CCEXP_AlreadyClosed, /// When CCEXP::Close() is called to an alreaded closed file.
	
    Other /// Any other error...
};

class CCEXPBase;
template<class T> class CCEXPMat;
class CCEXP;

//@#| ############### Virtual Base Class ###############
class CCEXPBase {
	public:
	virtual ~CCEXPBase() {}
	virtual int CompareName(const char* Name) = 0;
	virtual bool getIgnoreStatus(void) = 0;
	virtual int setIgnoreStatus(bool status) = 0;
	virtual int NewLine(int empty) = 0;
	virtual int NoNewRow(void) = 0;
	virtual int Rows(size_t &rows) = 0;
	virtual int Cols(size_t row, size_t &cols) = 0;
	virtual int DeleteLastRow(void) = 0;
	virtual int DeleteRow(size_t row) = 0;
	virtual int DeleteLastElement(size_t row) = 0;
	virtual char* getName(void) = 0;
	virtual int StoreData(FILE *fp) = 0;
	virtual int Reset(void) = 0;
};

//@#| ############### CCEXPMat - Table Class ###############
template<class T>
class CCEXPMat : public CCEXPBase {
	public:
		CCEXPMat();
		~CCEXPMat();

		int Initialize(const char* Name, const char* typeName, size_t MaxRows, CCEXP* par);
		int AddValue(T val);
		int AddRow(T* ptr, size_t n);
		int AppendRow(size_t row, T* ptr, size_t n);
		int ReplaceRow(size_t row, T* ptr, size_t n);
		int SetVal(size_t row, size_t col, T val);
		int InitRowByScalar(size_t row, T val, size_t n);
		
		int CompareName(const char* Name);
		bool getIgnoreStatus(void);
		int setIgnoreStatus(bool status);
		int NewLine(int empty);
		int NoNewRow(void);
		int Rows(size_t &rows);
		int Cols(size_t row, size_t &cols);
		int DeleteLastRow(void);
		int DeleteRow(size_t row);
		int DeleteLastElement(size_t row);
		char* getName(void);
		int StoreData(FILE* fp);
		int Reset(void);

		
	private:
		char name[64];
		char type[64];
		bool IgnoreM;
		bool newLineFlag;
		list<vector<T>> data;
		size_t _maxRows;
		CCEXP* __parent;
};

//@#| ############### CCEXP : This is the Global Class ###############
class CCEXP {
	public:
  
	FILE* fp;
	int Status;
	int STCounter;
	bool isActive;
	
	FILE* lfp;
	size_t LoadTotalTables;
	size_t LoadTableIndex;
	
	vector<shared_ptr<CCEXPBase>> M;
	char SavingFile[256];
	vector<string> Errors;
	
	CCEXP();
	~CCEXP();
	
	size_t getTableIndexByName(const char* Name);
	size_t checkDuplicatedNames(const char* Name);
	
	void setStatus(int st);
};












//@#| ############### CCEXPBase Functions definitions ###############
template<class T> CCEXPMat<T>::CCEXPMat() : IgnoreM(false), newLineFlag(true) { }
template<class T> CCEXPMat<T>::~CCEXPMat() { }


template<class T> int CCEXPMat<T>::Initialize(const char* Name, const char* typeName, size_t MaxRows, CCEXP* par) {
	__parent = par; // Keep a pointer to the parent object for error tracking.
	memset(name,0,64); memset(type,0,64); // Set all bytes to 0.
	sprintf(name,"%s",Name); sprintf(type,"%s",typeName);
	data.clear();
	_maxRows = (MaxRows == 0) ?  std::numeric_limits<size_t>::max() : MaxRows;
	return 0;
}

template<class T> int CCEXPMat<T>::AddValue(T val) {
	if (IgnoreM) return 0; // This is not an error - We just ignore the Table...
	const size_t N = data.size();
	if ((N == 0) || (newLineFlag==true)) {
		vector<T> dl; dl.clear(); data.push_back(dl);
		newLineFlag = false;
	}
	data.back().push_back(val);
	return 0;
}

template<class T> int CCEXPMat<T>::AddRow(T* ptr, size_t n) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N >= _maxRows) CCEXP_ERR(*__parent, ERROR::MaximumRowsReached , "CCEXPMat::AddRow():: Maximum Rows Reached!", 0);
	vector<T> dl; dl.resize(n);
	memcpy(dl.data(), ptr, n * sizeof(T));
	data.push_back(dl);
	newLineFlag=true;
	return 0;
}

template<class T> int CCEXPMat<T>::AppendRow(size_t row, T* ptr, size_t n) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::AppendRow():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	const size_t currentData = (*it).size();
	(*it).resize(currentData + n);
	T* Dptr = (*it).data();
	memcpy(&Dptr[currentData], ptr, sizeof(T) * n);
	return 0;
}

template<class T> int CCEXPMat<T>::ReplaceRow(size_t row, T* ptr, size_t n) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::ReplaceRow():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	(*it).resize(n);
	memcpy((*it).data(), ptr, sizeof(T) * n);
	return 0;
}

template<class T> int CCEXPMat<T>::SetVal(size_t row, size_t col, T val) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::SetVal():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	const size_t M = (*it).size();
	if (col >= M) CCEXP_ERR(*__parent, ERROR::ReqColumnLargerThanInRow , "CCEXPMat::SetVal():: Requested Column is not exist yet! (col >= %lu)", (uint64_t)M);
	(*it)[col] = val;
	return 0;
}

template<class T> int CCEXPMat<T>::InitRowByScalar(size_t row, T val, size_t n) {
	if (IgnoreM) return 0;
	if (row != -1) {
		const size_t N = data.size();
		if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::InitRowByScalar():: Requested Row is not exist yet! (row >= _maxRows)", 0);
		auto it = std::next(data.begin(), row);
		(*it).clear();
		if (n > 0) (*it).resize(n, val);
	} else {
		vector<T> dl; dl.clear();
		if (n > 0) dl.resize(n, val);
		data.push_back(dl);
	}
	return 0;
}







template<class T> int CCEXPMat<T>::CompareName(const char* Name) {
	return strcmp(name,Name);
}

template<class T> bool CCEXPMat<T>::getIgnoreStatus(void) { return IgnoreM; }

template<class T> int CCEXPMat<T>::setIgnoreStatus(bool status) { IgnoreM = status; return 0; }



template<class T> int CCEXPMat<T>::NewLine(int empty) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N >= _maxRows) CCEXP_ERR(*__parent, ERROR::MaximumRowsReached , "CCEXPMat::NewLine():: Maximum Rows (%lu) already Reached!", (uint64_t)_maxRows);
	newLineFlag=true;
	if (empty == 1) {
		vector<T> dl; dl.clear(); data.push_back(dl);
	}
	return 0;
}

template<class T> int CCEXPMat<T>::NoNewRow(void) {
	if (IgnoreM) return 0;
	newLineFlag=false;
	return 0;
}

template<class T> int CCEXPMat<T>::Rows(size_t &rows) {
	if (IgnoreM) return 0;
	rows = data.size();
	return 0;
}

template<class T> int CCEXPMat<T>::Cols(size_t row, size_t &cols) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::Cols():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	cols = (*it).size();
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteLastRow(void) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N == 0) CCEXP_ERR(*__parent, ERROR::TableHasNoRows , "CCEXPMat::DeleteLastRow():: Table has 0 rows!", 0);
	data.pop_back();
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteRow(size_t row) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::DeleteRow():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	data.erase(it);
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteLastElement(size_t row) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::DeleteLastElement():: Requested Row is not exist yet! (row >= _maxRows)", 0);
	auto it = std::next(data.begin(), row);
	if ((*it).size() > 0)  (*it).pop_back();
	return 0;
}

template<class T> char* CCEXPMat<T>::getName(void) { return name; }

template<class T> int CCEXPMat<T>::StoreData(FILE* fp) {
	if (IgnoreM) return 0;
	if (fp == NULL) CCEXP_ERR(*__parent, ERROR::IO_Error , "CCEXPMat::StoreData():: File pointer is NULL!", 0);
	
	size_t N = data.size();
	size_t typeSize = sizeof(T);
	
	fwrite(name,sizeof(char),64,fp); /// Table, Name
	fwrite(type,sizeof(char),64,fp); /// Table, Type (string)
	fwrite(&typeSize,sizeof(size_t),1,fp); /// Table, type size in Bytes
	fwrite(&N,sizeof(size_t),1,fp); /// Table, number of rows
	fwrite(&_maxRows,sizeof(size_t),1,fp); /// Maximum rows of the table.
	if (N > 0) {
		vector<size_t> DPL; DPL.resize(N);
		int i = 0;
		for (std::vector<T> n : data) DPL[i++] = n.size();
		fwrite(DPL.data(), sizeof(size_t), N, fp);
		DPL.clear();
		for (std::vector<T> n : data) fwrite(n.data(), typeSize, n.size(), fp);
	}
	return 0;
}


template<class T> int CCEXPMat<T>::Reset(void) {
	data.clear(); newLineFlag = true;
	return 0;
}







//@#| ############### CCEXP API Functions ###############
//@#: ############### .CPP file ###############
int Initialize(CCEXP &obj, const char* fname, const char* Path = NULL, bool isactive = true);
int StoreData(CCEXP &obj, const char* FName = NULL);
int StoreIData(CCEXP &obj);
int Open(CCEXP &obj, const char* filename);
int Close(CCEXP &obj);
int NewLine(CCEXP &obj, const char *matname, int empty = 0);
int NewLine(CCEXP &obj, size_t sel, int empty = 0);
int NoNewRow(CCEXP &obj, const char *matname);
int NoNewRow(CCEXP &obj, size_t sel);
int Rows(CCEXP &obj, const char* matname, size_t &rows);
int Rows(CCEXP &obj, size_t sel, size_t &rows);
int Cols(CCEXP &obj, const char* matname, size_t row, size_t &cols);
int Cols(CCEXP &obj,  size_t sel, size_t row, size_t &cols);
int DeleteLastRow(CCEXP &obj, const char* matname);
int DeleteLastRow(CCEXP &obj, size_t sel);
int DeleteRow(CCEXP &obj, const char* matname, size_t row);
int DeleteRow(CCEXP &obj, size_t sel, size_t row);
int DeleteLastElement(CCEXP &obj, const char* matname, size_t row);
int DeleteLastElement(CCEXP &obj, size_t sel, size_t row);
int getTableID(CCEXP &obj, const char* matname, size_t &sel);
int getTableName(CCEXP &obj, size_t sel, char* &matname);
int Reset(CCEXP &obj);
int GetErrors(CCEXP &obj, vector<string>* &ptrError, size_t &NumberOfErrors);
size_t NumberOfTables(CCEXP &obj);
int DBG_SetStatus(CCEXP &obj, int status);

//@#: ############### Templates API Functions ###############
template<class T> inline int AddTable (
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "AddTable():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t rd = obj.checkDuplicatedNames(matname);
	if (rd > 0) CCEXP_ERR(obj, ERROR::DublicatedTable , "AddTable()::  Table with name [%s] already exist!", matname);
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(matname, typeName, MaxRows, &obj);
	if (ret != 0) CCEXP_ERR(obj, ret, "AddTable():: Internal error occured during initialization() (error: %i)!", ret);
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}

template<class T> inline int AddTableI(
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "AddTableI():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t rd = obj.checkDuplicatedNames(matname);
	if (rd > 0) CCEXP_ERR(obj, ERROR::DublicatedTable , "AddTable()::  Table with name [%s] already exist!", matname);
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(matname, typeName, MaxRows, &obj);	
	int ret2 = U->setIgnoreStatus(true);
	if (ret != 0) CCEXP_ERR(obj, ret, "AddTable():: Internal error occured during initialization() (error: %i)!", ret);
	if (ret2 != 0) CCEXP_ERR(obj, ret2, "AddTable():: Internal error occured during setIgnoreStatus() (error: %i)!", ret2);
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}

template<class T> inline int LoadTable(CCEXP &obj, const char* name, const char* type) {
	if (!obj.isActive) return 0;
	int prevStatus = obj.Status;	
	// If any error occurs, then it should be critical error!
	obj.Status = CCEXPORTMAT_ACTIVE;	
	if (obj.checkDuplicatedNames(name) > 0)
		CCEXP_ERR(obj , ERROR::DublicatedTable , "LoadTable():: Table [%s] already exist in CCEXP object!", name);
	if ((prevStatus != CCEXPORTMAT_INIT) && (prevStatus != CCEXPORTMAT_READY))
		CCEXP_ERR(obj , ERROR::StatusIsWrong , "LoadTable():: CCEXP object has wrong status!", 0);
	FILE* lfp = obj.lfp;
	if (lfp == NULL) CCEXP_ERR(obj, ERROR::IO_Error, "LoadTable():: File pointer is NULL. Use CCEXP::Open() first!", 0);
	char loadName[64]={0};
	char loadType[64]={0};
	size_t typeSize;
	size_t N;
	size_t LastTablePosByte;
	size_t MaxRows=0;
	
	bool TableFound = false;
	// Search from current LoadTableIndex to the Total Tables of files.
	for (size_t i = obj.LoadTableIndex; i < obj.LoadTotalTables; i++) {
		LastTablePosByte = ftell(lfp);
		fread(loadName,sizeof(char),64,lfp);
		fread(loadType,sizeof(char),64,lfp);
		fread(&typeSize,sizeof(size_t),1,lfp);
		fread(&N,sizeof(size_t),1,lfp);
		fread(&MaxRows,sizeof(size_t),1,lfp);
		if (strcmp(loadName,name)==0) {
			TableFound = true; obj.LoadTableIndex = i+1; break;
		}
		vector<size_t> DPL; DPL.resize(N);
		fread(DPL.data(), sizeof(size_t), N, lfp);
		size_t TableBytes = 0;
		for (size_t c = 0; c < N; c++)
			TableBytes += DPL[c]*typeSize;
		fseek(lfp, TableBytes, SEEK_CUR);
	}
	if (!TableFound) {
		// If the Table has not been found in the file, search again from the start
		fseek(lfp, sizeof(uint32_t) + sizeof(size_t), SEEK_SET);
		
		for (size_t i = 0; i < obj.LoadTableIndex; i++) {
			LastTablePosByte = ftell(lfp);
			fread(loadName,sizeof(char),64,lfp);
			fread(loadType,sizeof(char),64,lfp);
			fread(&typeSize,sizeof(size_t),1,lfp);
			fread(&N,sizeof(size_t),1,lfp);
			fread(&MaxRows,sizeof(size_t),1,lfp);
			if (strcmp(loadName,name)==0) {
				TableFound = true; obj.LoadTableIndex = i+1; break;
			}
			vector<size_t> DPL; DPL.resize(N);
			fread(DPL.data(), sizeof(size_t), N, lfp);
			size_t TableBytes = 0;
			for (size_t c = 0; c < N; c++)
				TableBytes += DPL[c]*typeSize;
			fseek(lfp, TableBytes, SEEK_CUR);
		}
	}
	
	if (obj.LoadTableIndex >= obj.LoadTotalTables) obj.LoadTableIndex = 0;
	
	// If the Table hot not been found, then abort with error.
	if(!TableFound) {
		fseek(lfp, LastTablePosByte, SEEK_SET);
		CCEXP_ERR(obj, ERROR::IO_Error, "LoadTable():: Failed to find table [%s] into the last opened file!", name);
	}
	
	// At this point the table has been found, and we have to reload it!
	if (strcmp(loadType, type) != 0) {
		fseek(lfp, LastTablePosByte, SEEK_SET);
		CCEXP_ERR(obj, ERROR::IO_Error, "LoadTable():: Table [%s] is of \"%s\" type, while requested LoadTable is of \"%s\" type!", name, loadType, type);
	}
	// TODO:: Check directly the given type (typeid(T).name()) with another
	// section of ccexp files would be a solution, but some compilers does not
	// support typeid(T).name() correctly. Would it there be any better solution?
		// if (typeSize != sizeof(T)) CCEXP_ERR(obj, ERROR::IO_Error, "LoadTable():: Table [%s] is of %i byte data, while requested %i byte data!", typeSize, sizeof(T));
	
	// Create a new Table...
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(loadName, loadType, MaxRows, &obj);
	if (ret != 0) CCEXP_ERR(obj, ret, "LoadTable():: Internal error occured while copying Table [%s] from an opened file!", name);
	// Copy data from file to the new table.
	vector<size_t> DPL; DPL.resize(N);
	fread(DPL.data(), sizeof(size_t), N, lfp);
	vector<T> rowData;
	size_t maxCols=0;
	for (size_t row = 0; row < N; row++) {
		const size_t columns = DPL[row];
		if (columns > maxCols) { maxCols = columns; rowData.resize(maxCols); }
		fread(rowData.data(), sizeof(T), columns, lfp);
		ret = U->AddRow(rowData.data(), columns);
		if (ret != 0) CCEXP_ERR(obj, ret, "LoadTable():: Internal error occured while copying Table's [%s], row [%lu] from an opened file!", name, (uint64_t)row);
	}
	// Warning: newLineFlag should be set to false, thus use to be able
	// add data directly to the end of the last row if he want.
	U->NoNewRow();

	obj.Status = CCEXPORTMAT_READY;

}

template<class T> inline int AddRow(
	CCEXP &obj,
	size_t sel,
	T* ptr,
	size_t n
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "AddRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AddRow(ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "AddRow():: Internal error occured during AddRow() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "AddRow():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> inline int AddRow(
	CCEXP &obj, const char* matname, T* ptr, size_t n
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "AddRow():: Failed to find table with name [%s]!", matname);
	return AddRow(obj, sel, ptr, n);
}


template<class T> inline int AddVal(
	CCEXP &obj, size_t sel, T val
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "AddVal():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AddValue(val);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "AddVal():: Internal error occured during AddVal() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "AddVal():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> int AddVal(
	CCEXP &obj, const char* matname, T val
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "AddVal():: Failed to find table with name [%s]!", matname);
	return AddVal(obj, sel, val);
}


template<class T> inline int AppendRow(
	CCEXP &obj, size_t sel, size_t row, T* ptr, size_t n
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "AppendRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AppendRow(row, ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "AppendRow():: Internal error occured during AppendRow() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "AppendRow():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> inline int AppendRow(
	CCEXP &obj, const char* matname, size_t row, T* ptr, size_t n
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "AppendRow():: Failed to find table with name [%s]!", matname);
	return AppendRow(obj, sel, row, ptr, n);
}


template<class T> inline int ReplaceRow(
	CCEXP &obj, size_t sel, size_t row, T* ptr, size_t n
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "ReplaceRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->ReplaceRow(row, ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "ReplaceRow():: Internal error occured during ReplaceRow() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "ReplaceRow():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> inline int ReplaceRow(
	CCEXP &obj, const char* matname, size_t row, T* ptr, size_t n
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "ReplaceRow():: Failed to find table with name [%s]!", matname);
	return ReplaceRow(obj, sel, row, ptr, n);
}


template<class T> inline int SetVal(
	CCEXP &obj, size_t sel, size_t row, size_t col, T val
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "SetVal():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->SetVal(row, col, val);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "SetVal():: Internal error occured during SetVal() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "SetVal():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> inline int SetVal(
	CCEXP &obj, const char* matname, size_t row, size_t col, T val
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "SetVal():: Failed to find table with name [%s]!", matname);
	return SetVal(obj, sel, row, col, val);
}


template<class T> inline int InitRowByScalar(
	CCEXP &obj, size_t sel, size_t row, T val, size_t n
) {
	if (!obj.isActive) return 0;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR(obj , ERROR::StatusNotReady , "InitRowByScalar():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->InitRowByScalar(row, val, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR(obj, ret, "InitRowByScalar():: Internal error occured during InitRowByScalar() (error: %i)!", ret);
		return 0;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR(obj, ERROR::TableNotFound, "InitRowByScalar():: Failed to find table with ID [%lu]!", (uint64_t)sel);
}
template<class T> inline int InitRowByScalar(
	CCEXP &obj, const char* matname, size_t row, T val, size_t n
) {
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == -1) CCEXP_ERR(obj, ERROR::TableNotFound, "InitRowByScalar():: Failed to find table with name [%s]!", matname);
	return InitRowByScalar(obj, sel, row, val, n);
}

}; // namespace CCEXP

#endif