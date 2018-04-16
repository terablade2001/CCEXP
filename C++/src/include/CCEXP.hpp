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

#ifndef __CCEXP_HPP__
#define __CCEXP_HPP__

#define CCEXP_VERSION (0.071)

#define __CCEXP__USE_MVECTOR
// MVECTOR can be downloaded from https://github.com/terablade2001/MVECTOR

// Include C/C++ Libraries
#include <stdlib.h>
#include <cstring>
#include <memory>
#include <list>
#include <cstdio>
#include <limits>
#include <iostream>
#ifndef __CCEXP__USE_MVECTOR
	#include <vector>
	#define MVECTOR vector
	#define __USE_MVECTOR_NAMESPACE__ //..
	#define __CCEXP_VECTOR_STEPS(v,a,b) //..
	#define __CCEXP_VECTOR_CLEAR(v) (v).clear(); (v).resize(0);
#else
	#include "../../../sub_modules/MVECTOR/src/include/MVECTOR.hpp"
	// #include "MVECTOR.hpp"
	#define __USE_MVECTOR_NAMESPACE__ using namespace ns_MVECTOR;
	#define __CCEXP_VECTOR_STEPS(v,a,b) (v).set_steps((a),(b));
	#define __CCEXP_VECTOR_CLEAR(v) (v).clear();
#endif


/* ----------- Support for MSVC compilers --------
	As MSVC has basic differences with other compilers, I'm trying to modify 
my macros thus if #_MSC_VER is defined other code to be used.
	As some of the MSVC compilers does not support "%zu", I use __ZU__ 
definition which can change depending the compiler. 
   ----------------------------------------------- */
#ifdef _MSC_VER
	#define snprintf _snprintf_s
	#define __CCEXP_FOPEN__(fp, fname, mode) fopen_s(&fp, fname, mode)
	#define __ZU__ "%Iu"
	#define __ZUn_ "Iu"
	#include <algorithm>
#endif
#ifndef _MSC_VER
	#define __CCEXP_FOPEN__(fp, fname, mode) fp = fopen(fname, mode)
	#define __ZU__ "%zu"
	#define __ZUn_ "zu"
#endif


#define TRACK_ANALYTIC_ERRORS

#ifndef __FNAME__
	#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define CCEXPORTMAT_INIT		(0)
#define CCEXPORTMAT_READY		(1)
#define CCEXPORTMAT_ACTIVE		(2)
#define CCEXPORTMAT_DIED		(3)

#define MAXSIZE_T std::numeric_limits<std::size_t>::max()

#ifndef _MSC_VER
	#ifdef TRACK_ANALYTIC_ERRORS
		#define CCEXP_ERR(obj, errid, str, args...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, str, args); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			return errid; }

		#define CCEXP_ERR_V(obj, errid, str, args...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, str, args); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			obj.ErrorId = errid; return; }

		#define CCEXP_ERR_T(obj, val, errid, str, args...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, str, args); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			obj.ErrorId = errid; return (val); }

		#define __CCEXP_ERR_DISPLAY(obj, N) { \
			size_t n=size_t(N); if (n > 0) { \
			MVECTOR<string>* v; size_t TN = CCEXP::GetErrors(obj, v);\
			size_t DN = std::min(n, TN); \
			if (TN != 0) {\
			cerr << endl << "[" << __FNAME__ << ", " << __LINE__ << \
			"]: CCEXP Err Display >> " << endl; \
			for (size_t i = 0; i < DN; i++)\
				cerr << " * " << (*v)[i].c_str() << endl; \
			(obj).Errors.clear(); }}\
			}
	#endif
	#ifndef TRACK_ANALYTIC_ERRORS
		#define CCEXP_ERR(obj, errid, str, args...) return errid;
		#define CCEXP_ERR_V(obj, errid, str, args...) return;
		#define CCEXP_ERR_T(obj, val, errid, str, args...) return (val);
		#define __CCEXP_ERR_DISPLAY(obj, N) //...
	#endif
#endif

#ifdef _MSC_VER
	#ifdef TRACK_ANALYTIC_ERRORS
		#define CCEXP_ERR(obj, errid, str, ...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, 2048, str, __VA_ARGS__); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			return errid; }

		#define CCEXP_ERR_V(obj, errid, str, ...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, 2048, str, __VA_ARGS__); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			obj.ErrorId = errid; return; }

		#define CCEXP_ERR_T(obj, val, errid, str, ...) { \
			char ccexp_errStr[2049]={0};\
			snprintf(ccexp_errStr, 2048, 2048, str, __VA_ARGS__); \
			if ((obj).Errors.size() < 128)\
			(obj).Errors.push_back(string(ccexp_errStr)); \
			obj.ErrorId = errid; return (val); }

		#define __CCEXP_ERR_DISPLAY(obj, N) { \
			size_t n=size_t(N); if (n > 0) { \
			MVECTOR<string>* v; size_t TN = CCEXP::GetErrors(obj, v);\
			size_t DN = std::min(n, TN); \
			if (TN != 0) {\
			cerr << endl << "[" << __FNAME__ << ", " << __LINE__ << \
			"]: CCEXP Err Display >> " << endl; \
			for (size_t i = 0; i < DN; i++)\
				cerr << " * " << (*v)[i].c_str() << endl; \
			(obj).Errors.clear();  }}\
			}
	#endif
	#ifndef TRACK_ANALYTIC_ERRORS
		#define CCEXP_ERR(obj, errid, str, ...) return errid;
		#define CCEXP_ERR_V(obj, errid, str, ...) return;
		#define CCEXP_ERR_T(obj, val, errid, str, ...) return (val);
		#define __CCEXP_ERR_DISPLAY(obj, N) //...
	#endif
#endif

#define __CCEXP_PRINT_TABLES__(v) {\
		MVECTOR<MVECTOR<char>> DBG_Tables;\
		CCEXP::Analyze((v), DBG_Tables);\
		for (size_t i = 0; i < DBG_Tables.size(); i++) {\
			printf("%s\n",DBG_Tables[i].data());\
		}\
	}

#define CCEXP_MAX_ERRORCHAR_SIZE (4096)

using namespace std;
__USE_MVECTOR_NAMESPACE__

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
	virtual int NewRow(int empty) = 0;
	virtual int NoNewRow(void) = 0;
	virtual int Rows(size_t &rows) = 0;
	virtual int Cols(size_t row, size_t &cols) = 0;
	virtual int DeleteLastRow(void) = 0;
	virtual int DeleteRow(size_t row) = 0;
	virtual int DeleteLastElement(size_t row) = 0;
	virtual char* getName(void) = 0;
	virtual char* getType(void) = 0;
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

		int getRow(size_t row, MVECTOR<T>* &vres);

		int CompareName(const char* Name);
		bool getIgnoreStatus(void);
		int setIgnoreStatus(bool status);
		int NewRow(int empty);
		int NoNewRow(void);
		int Rows(size_t &rows);
		int Cols(size_t row, size_t &cols);
		int DeleteLastRow(void);
		int DeleteRow(size_t row);
		int DeleteLastElement(size_t row);
		char* getName(void);
		char* getType(void);
		int StoreData(FILE* fp);
		int Reset(void);

	private:
		char name[65];
		char type[65];
		bool IgnoreM;
		bool newRowFlag;
		list<MVECTOR<T>> data;
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
	int ErrorId;

	FILE* lfp;
	size_t LoadTotalTables;
	size_t LoadTableIndex;
	
	MVECTOR<shared_ptr<CCEXPBase>> M;
	char SavingFile[257];
	MVECTOR<string> Errors;
	MVECTOR<char> errorchar;

	CCEXP();
	CCEXP(const char* fname);
	~CCEXP();
	
	size_t getTableIndexByName(const char* Name);
	size_t checkDuplicatedNames(const char* Name);
	
	void setStatus(int st);
};












//@#| ############### CCEXPBase Functions definitions ###############
template<class T> CCEXPMat<T>::CCEXPMat() : IgnoreM(false), newRowFlag(true), _maxRows(std::numeric_limits<size_t>::max()) { }
template<class T> CCEXPMat<T>::~CCEXPMat() { }


template<class T> int CCEXPMat<T>::Initialize(const char* Name, const char* typeName, size_t MaxRows, CCEXP* par) {
	__parent = par; // Keep a pointer to the parent object for error tracking.
	memset(name,0,65); memset(type,0,65); // Set all bytes to 0.
	snprintf(name,64,"%s",Name); snprintf(type,64,"%s",typeName);
	__CCEXP_VECTOR_CLEAR(data);
	_maxRows = (MaxRows == 0) ?  std::numeric_limits<size_t>::max() : MaxRows;
	return 0;
}

template<class T> int CCEXPMat<T>::AddValue(T val) {
	if (IgnoreM) return 0; // This is not an error - We just ignore the Table...
	const size_t N = data.size();
	if ((N == 0) || (newRowFlag==true)) {
		MVECTOR<T> dl; dl.clear(); data.push_back(dl);
		newRowFlag = false;
	}
	data.back().push_back(val);
	return 0;
}

template<class T> int CCEXPMat<T>::AddRow(T* ptr, size_t n) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N >= _maxRows) CCEXP_ERR(*__parent, ERROR::MaximumRowsReached , "CCEXPMat::AddRow():: Maximum Rows Reached! (!%u!)", 0);
	MVECTOR<char> MM;
	{
		MVECTOR<T> dl;
		data.push_back(dl);
		(data.back()).resize(n);
		T* _ptr = data.back().data();
		memcpy(_ptr, ptr, n * sizeof(T));
	}
	newRowFlag=true;
	return 0;
}

template<class T> int CCEXPMat<T>::AppendRow(size_t row, T* ptr, size_t n) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::AppendRow():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
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
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::ReplaceRow():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
	auto it = std::next(data.begin(), row);
	(*it).resize(n);
	memcpy((*it).data(), ptr, sizeof(T) * n);
	return 0;
}

template<class T> int CCEXPMat<T>::SetVal(size_t row, size_t col, T val) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::SetVal():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
	auto it = std::next(data.begin(), row);
	const size_t M = (*it).size();
	if (col >= M) CCEXP_ERR(*__parent, ERROR::ReqColumnLargerThanInRow , "CCEXPMat::SetVal():: Requested Column is not exist yet! (col >= " __ZU__ ")", M);
	(*it)[col] = val;
	return 0;
}

template<class T> int CCEXPMat<T>::InitRowByScalar(size_t row, T val, size_t n) {
	if (IgnoreM) return 0;
	if (row != MAXSIZE_T) {
		const size_t N = data.size();
		if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::InitRowByScalar():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
		auto it = std::next(data.begin(), row);
		__CCEXP_VECTOR_CLEAR(*it);
		if (n > 0) (*it).resize(n, val);
	} else {
		MVECTOR<T> dl; dl.clear();
		if (n > 0) dl.resize(n, val);
		data.push_back(dl);
	}
	return 0;
}

template<class T> int CCEXPMat<T>::getRow(size_t row, MVECTOR<T>* &vres) {
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::getRowPtr():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);	
	auto it = std::next(data.begin(), row);
	vres = &(*it);
	return 0;
}





template<class T> int CCEXPMat<T>::CompareName(const char* Name) {
	return strcmp(name,Name);
}

template<class T> bool CCEXPMat<T>::getIgnoreStatus(void) { return IgnoreM; }

template<class T> int CCEXPMat<T>::setIgnoreStatus(bool status) { IgnoreM = status; return 0; }



template<class T> int CCEXPMat<T>::NewRow(int empty) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N >= _maxRows)
		CCEXP_ERR(*__parent, ERROR::MaximumRowsReached , "CCEXPMat::NewRow():: Maximum Rows (" __ZU__ ") already Reached!", _maxRows);

	newRowFlag=true;
	if (empty == 1) {
		MVECTOR<T> dl; dl.clear(); data.push_back(dl);
	}
	return 0;
}

template<class T> int CCEXPMat<T>::NoNewRow(void) {
	if (IgnoreM) return 0;
	newRowFlag=false;
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
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::Cols():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
	auto it = std::next(data.begin(), row);
	cols = (*it).size();
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteLastRow(void) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (N == 0) CCEXP_ERR(*__parent, ERROR::TableHasNoRows , "CCEXPMat::DeleteLastRow():: Table has 0 rows! (!%u!)", 0);
	data.pop_back();
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteRow(size_t row) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::DeleteRow():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
	auto it = std::next(data.begin(), row);
	data.erase(it);
	return 0;
}

template<class T> int CCEXPMat<T>::DeleteLastElement(size_t row) {
	if (IgnoreM) return 0;
	const size_t N = data.size();
	if (row >= N) CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable , "CCEXPMat::DeleteLastElement():: Requested Row is not exist yet! (row >= _maxRows) (!%u!)", 0);
	auto it = std::next(data.begin(), row);
	if ((*it).size() > 0)  (*it).pop_back();
	return 0;
}

template<class T> char* CCEXPMat<T>::getName(void) { return name; }

template<class T> char* CCEXPMat<T>::getType(void) { return type; }

template<class T> int CCEXPMat<T>::StoreData(FILE* fp) {
	if (IgnoreM) return 0;
	if (fp == NULL) CCEXP_ERR(*__parent, ERROR::IO_Error , "CCEXPMat::StoreData():: File pointer is NULL! (!%u!)", 0);
	
	uint64_t N = (uint64_t)data.size();
	uint64_t typeSize = (uint64_t)sizeof(T);
	uint64_t _maxRows64 = (uint64_t)_maxRows;
	
	fwrite(name,sizeof(char),64,fp); /// Table, Name
	fwrite(type,sizeof(char),64,fp); /// Table, Type (string)
	fwrite(&typeSize,sizeof(uint64_t),1,fp); /// Table, type size in Bytes
	fwrite(&N,sizeof(uint64_t),1,fp); /// Table, number of rows
	fwrite(&_maxRows64,sizeof(uint64_t),1,fp); /// Maximum rows of the table.
	if (N > _maxRows) N = _maxRows; /// Limit Rows ...
	
	if (data.size() > N)
		CCEXP_ERR(*__parent, ERROR::ReqRowLargerThanInTable ,
			"CCEXPMat::StoreData():: Table [%s] containing more Rows (" __ZU__ ") than MaxRows (" __ZU__ ")",
			name, data.size(), _maxRows
        );
	
	if (N > 0) {
		MVECTOR<uint64_t> DPL; DPL.resize(N);
		int i = 0;
		for (MVECTOR<T> n : data) DPL[i++] = (uint64_t)n.size();
		fwrite(DPL.data(), sizeof(uint64_t), N, fp);
		__CCEXP_VECTOR_CLEAR(DPL);
		for (MVECTOR<T> n : data) fwrite(n.data(), typeSize, n.size(), fp);
	}
	return 0;
}


template<class T> int CCEXPMat<T>::Reset(void) {
	__CCEXP_VECTOR_CLEAR(data);
	newRowFlag = true;
	return 0;
}






//@#| ############### CCEXP API Functions ###############
//@#: ############### .CPP file ###############
void   Initialize(CCEXP &obj, const char* fname, const char* Path = NULL, bool isactive = true);
void   StoreData(CCEXP &obj, const char* FName = NULL);
size_t StoreIData(CCEXP &obj);
void   Open(CCEXP &obj, const char* filename);
void   Close(CCEXP &obj);
void   NewRow(CCEXP &obj, const char *matname, int empty = 0);
void   NewRow(CCEXP &obj, size_t sel, int empty = 0);
void   NoNewRow(CCEXP &obj, const char *matname);
void   NoNewRow(CCEXP &obj, size_t sel);
size_t Rows(CCEXP &obj, const char* matname);
size_t Rows(CCEXP &obj, size_t sel);
size_t Cols(CCEXP &obj, const char* matname, size_t row);
size_t Cols(CCEXP &obj,  size_t sel, size_t row);
bool   Ignored(CCEXP &obj, size_t sel);
bool   Ignored(CCEXP &obj, const char* matname);
void   DeleteLastRow(CCEXP &obj, const char* matname);
void   DeleteLastRow(CCEXP &obj, size_t sel);
void   DeleteRow(CCEXP &obj, const char* matname, size_t row);
void   DeleteRow(CCEXP &obj, size_t sel, size_t row);
void   DeleteLastElement(CCEXP &obj, const char* matname, size_t row);
void   DeleteLastElement(CCEXP &obj, size_t sel, size_t row);
size_t getTableID(CCEXP &obj, const char* matname);
char*  getTableName(CCEXP &obj, size_t sel);
void   Reset(CCEXP &obj);
void   CleanTable(CCEXP &obj, const char* matname);
void   CleanTable(CCEXP &obj, size_t sel);
size_t GetErrors(CCEXP &obj, MVECTOR<string>* &ptrError);
char*  GetErrors(CCEXP &obj, size_t &rows);
size_t NumberOfTables(CCEXP &obj);
void   DBG_SetStatus(CCEXP &obj, int status);
int    Analyze(CCEXP &obj, MVECTOR<MVECTOR<char>> &v);
int    Analyze(const char* filename, MVECTOR<MVECTOR<char>> &v, int flag = 0);

//@#: ############### Templates API Functions ###############
template<class T> inline void AddTable (
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "AddTable():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t rd = obj.checkDuplicatedNames(matname);
	if (rd > 0) CCEXP_ERR_V(obj, ERROR::DublicatedTable , "AddTable()::  Table with name [%s] already exist!", matname);
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(matname, typeName, MaxRows, &obj);
	if (ret != 0) CCEXP_ERR_V(obj, ret, "AddTable():: Internal error occured during initialization() (error: %i)!", ret);
	obj.Status = CCEXPORTMAT_READY;
}

template<class T> inline void AddTableI(
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "AddTableI():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	size_t rd = obj.checkDuplicatedNames(matname);
	if (rd > 0) CCEXP_ERR_V(obj, ERROR::DublicatedTable , "AddTable()::  Table with name [%s] already exist!", matname);
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(matname, typeName, MaxRows, &obj);	
	int ret2 = U->setIgnoreStatus(true);
	if (ret != 0) CCEXP_ERR_V(obj, ret, "AddTable():: Internal error occured during initialization() (error: %i)!", ret);
	if (ret2 != 0) CCEXP_ERR_V(obj, ret2, "AddTable():: Internal error occured during setIgnoreStatus() (error: %i)!", ret2);
	obj.Status = CCEXPORTMAT_READY;
}

template<class T> inline void LoadTable(CCEXP &obj, const char* name, const char* type) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	int prevStatus = obj.Status;
	// If any error occurs, then it should be critical error!
	obj.Status = CCEXPORTMAT_ACTIVE;
	if ((prevStatus != CCEXPORTMAT_INIT) && (prevStatus != CCEXPORTMAT_READY))
		CCEXP_ERR_V(obj , ERROR::StatusIsWrong , "LoadTable():: CCEXP object has wrong status! (!%u!)", 0);
	FILE* lfp = obj.lfp;
	if (lfp == NULL) CCEXP_ERR_V(obj, ERROR::IO_Error, "LoadTable():: File pointer is NULL. Use CCEXP::Open() first! (!%u!)", 0);
	char loadName[64]={0};
	char loadType[64]={0};
	uint64_t typeSize=0;
	uint64_t N=0;
	uint64_t LastTablePosByte = 0;
	uint64_t MaxRows=0;
	bool TableFound = false;
	// Search from current LoadTableIndex to the Total Tables of files.
	for (uint64_t i = obj.LoadTableIndex; i < obj.LoadTotalTables; i++) {
		LastTablePosByte = ftell(lfp);
		fread(loadName,sizeof(char),64,lfp);
		fread(loadType,sizeof(char),64,lfp);
		fread(&typeSize,sizeof(uint64_t),1,lfp);
		fread(&N,sizeof(uint64_t),1,lfp);
		fread(&MaxRows,sizeof(uint64_t),1,lfp);
		if (N > MaxRows) N = MaxRows;
		if (strcmp(loadName,name)==0) {
			TableFound = true; obj.LoadTableIndex = i+1; break;
		}
		MVECTOR<uint64_t> DPL; DPL.resize(N);
		fread(DPL.data(), sizeof(uint64_t), N, lfp);
		uint64_t TableBytes = 0;
		for (uint64_t c = 0; c < N; c++)
			TableBytes += DPL[c]*typeSize;
		fseek(lfp, TableBytes, SEEK_CUR);
	}
	if (!TableFound) {
		// If the Table has not been found in the file, search again from the start
		fseek(lfp, sizeof(uint32_t) + sizeof(uint64_t), SEEK_SET);
		
		for (uint64_t i = 0; i < obj.LoadTableIndex; i++) {
			LastTablePosByte = ftell(lfp);
			fread(loadName,sizeof(char),64,lfp);
			fread(loadType,sizeof(char),64,lfp);
			fread(&typeSize,sizeof(uint64_t),1,lfp);
			fread(&N,sizeof(uint64_t),1,lfp);
			fread(&MaxRows,sizeof(uint64_t),1,lfp);
			if (N > MaxRows) N = MaxRows;
			if (strcmp(loadName,name)==0) {
				TableFound = true; obj.LoadTableIndex = i+1; break;
			}
			MVECTOR<uint64_t> DPL; DPL.resize(N);
			fread(DPL.data(), sizeof(uint64_t), N, lfp);
			uint64_t TableBytes = 0;
			for (uint64_t c = 0; c < N; c++)
				TableBytes += DPL[c]*typeSize;
			fseek(lfp, TableBytes, SEEK_CUR);
		}
	}
	
	// If the Table hot not been found, then abort with error.
	if(!TableFound) {
		fseek(lfp, LastTablePosByte, SEEK_SET);
		CCEXP_ERR_V(obj, ERROR::IO_Error, "LoadTable():: Failed to find table [%s] into the last opened file!", name);
	}
	
	// At this point the table has been found, and we have to reload it!
	if (strcmp(loadType, type) != 0) {
		fseek(lfp, LastTablePosByte, SEEK_SET);
		CCEXP_ERR_V(obj, ERROR::IO_Error, "LoadTable():: Table [%s] is of \"%s\" type, while requested LoadTable is of \"%s\" type!", name, loadType, type);
	}

	CCEXPMat<T> *U;
	if (obj.checkDuplicatedNames(name) > 0) { // If the table already exist!
		size_t idx = obj.getTableIndexByName(name);
		U = static_cast<CCEXPMat<T>*>(obj.M[idx].get());
		U->Reset();
	}
	else
	{// If the table does not exist create a new Table...
		obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
		U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	}

	int ret = U->Initialize(loadName, loadType, MaxRows, &obj);
	if (ret != 0) CCEXP_ERR_V(obj, ret, "LoadTable():: Internal error occured while copying Table [%s] from an opened file!", name);
	// Copy data from file to the new table.
	MVECTOR<uint64_t> DPL; DPL.resize(N);
	fread(DPL.data(), sizeof(uint64_t), N, lfp);
	MVECTOR<T> rowData;
	uint64_t maxCols=0;
	for (uint64_t row = 0; row < N; row++) {
		const uint64_t columns = DPL[row];
		if (columns > maxCols) { maxCols = columns; rowData.resize(maxCols); }
		fread(rowData.data(), sizeof(T), columns, lfp);
		ret = U->AddRow(rowData.data(), columns);
		if (ret != 0) CCEXP_ERR_V(obj, ret, "LoadTable():: Internal error occured while copying Table's [%s], row [" __ZU__ "] from an opened file!", name, (size_t)row);
	}
	// Warning: newRowFlag should be set to false, thus use to be able
	// add data directly to the end of the last row if he want.
	U->NoNewRow();
	
	if (obj.LoadTableIndex >= obj.LoadTotalTables) {
		obj.LoadTableIndex = 0;
		fseek(lfp, sizeof(uint32_t) + sizeof(uint64_t), SEEK_SET);
	}

	obj.Status = CCEXPORTMAT_READY;
}

template<class T> inline void AddRow(
	CCEXP &obj,
	size_t sel,
	T* ptr,
	size_t n
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "AddRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AddRow(ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "AddRow():: Internal error occured during AddRow() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "AddRow():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline void AddRow(
	CCEXP &obj, const char* matname, T* ptr, size_t n
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "AddRow():: Failed to find table with name [%s]!", matname);
	AddRow(obj, sel, ptr, n);
}


template<class T> inline void AddVal(
	CCEXP &obj, size_t sel, T val
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "AddVal():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AddValue(val);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "AddVal():: Internal error occured during AddVal() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "AddVal():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> void AddVal(
	CCEXP &obj, const char* matname, T val
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "AddVal():: Failed to find table with name [%s]!", matname);
	AddVal(obj, sel, val);
}


template<class T> inline void AppendRow(
	CCEXP &obj, size_t sel, size_t row, T* ptr, size_t n
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "AppendRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->AppendRow(row, ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "AppendRow():: Internal error occured during AppendRow() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "AppendRow():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline void AppendRow(
	CCEXP &obj, const char* matname, size_t row, T* ptr, size_t n
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "AppendRow():: Failed to find table with name [%s]!", matname);
	AppendRow(obj, sel, row, ptr, n);
}


template<class T> inline void ReplaceRow(
	CCEXP &obj, size_t sel, size_t row, T* ptr, size_t n
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "ReplaceRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->ReplaceRow(row, ptr, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "ReplaceRow():: Internal error occured during ReplaceRow() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "ReplaceRow():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline void ReplaceRow(
	CCEXP &obj, const char* matname, size_t row, T* ptr, size_t n
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "ReplaceRow():: Failed to find table with name [%s]!", matname);
	ReplaceRow(obj, sel, row, ptr, n);
}


template<class T> inline void SetVal(
	CCEXP &obj, size_t sel, size_t row, size_t col, T val
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "SetVal():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->SetVal(row, col, val);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "SetVal():: Internal error occured during SetVal() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "SetVal():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline void SetVal(
	CCEXP &obj, const char* matname, size_t row, size_t col, T val
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "SetVal():: Failed to find table with name [%s]!", matname);
	SetVal(obj, sel, row, col, val);
}


template<class T> inline void InitRowByScalar(
	CCEXP &obj, size_t sel, size_t row, T val, size_t n
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_V(obj , ERROR::StatusNotReady , "InitRowByScalar():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		int ret = U->InitRowByScalar(row, val, n);
		obj.Status = CCEXPORTMAT_READY;
		if (ret != 0) CCEXP_ERR_V(obj, ret, "InitRowByScalar():: Internal error occured during InitRowByScalar() (error: %i)!", ret);
		return;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_V(obj, ERROR::TableNotFound, "InitRowByScalar():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline void InitRowByScalar(
	CCEXP &obj, const char* matname, size_t row, T val, size_t n
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) CCEXP_ERR_V(obj, ERROR::TableNotFound, "InitRowByScalar():: Failed to find table with name [%s]!", matname);
	InitRowByScalar(obj, sel, row, val, n);
}


template<class T> inline MVECTOR<T>* getRow(
	CCEXP &obj, size_t sel, size_t row
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return NULL;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, NULL, ERROR::StatusNotReady , "getRow():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		MVECTOR<T>* vret;
		int ret = U->getRow(row, vret);
		if (ret != 0) CCEXP_ERR_T(obj, NULL, ret, "getRow():: Internal error occured during getRow() (error: %i)!", ret);
		obj.Status = CCEXPORTMAT_READY;
		return vret;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getRow():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline MVECTOR<T>* getRow(
	CCEXP &obj, const char* matname, size_t row
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) {
		CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getRow():: Failed to find table with name [%s]!", matname);
	}
	MVECTOR<T>* vret = getRow<T>(obj, sel, row);
	return vret;
}

template<class T> inline T* getRow(
	CCEXP &obj, size_t sel, size_t row, size_t& cols
) {
	obj.ErrorId = 0;
	MVECTOR<T>* v_row = getRow<T>(obj, sel, row);
	if (v_row != NULL) { cols = v_row->size(); return v_row->data(); }
	else { cols = 0; return NULL; }
}
template<class T> inline T* getRow(
	CCEXP &obj, const char* matname, size_t row, size_t &cols
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) {
		CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getRow():: Failed to find table with name [%s]!", matname);
	}
	T* vret = getRow<T>(obj, sel, row, cols);
	return vret;
}

template<class T> inline T* getVal(
	CCEXP &obj, size_t sel, size_t row, size_t col
) {
	obj.ErrorId = 0;
	if (!obj.isActive) return NULL;
	if (obj.Status != CCEXPORTMAT_READY) CCEXP_ERR_T(obj, NULL, ERROR::StatusNotReady , "getVal():: CCEXP object with filename [%s] has wrong status." , obj.SavingFile );
	obj.Status = CCEXPORTMAT_ACTIVE;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		MVECTOR<T>* vecres;
		int ret = U->getRow(row, vecres);
		if (ret != 0) CCEXP_ERR_T(obj, NULL, ret, "getVal():: Internal error occured during getVal() (error: %i)!", ret);
		size_t N = vecres->size();
		if (col >= N) CCEXP_ERR_T(obj, NULL, ERROR::MaximumRowsReached, "getVal(): Column " __ZU__ " does not exist at row " __ZU__ " of table with ID = " __ZU__ "", col, row, sel);
		T* vret = &((*vecres)[col]);
		obj.Status = CCEXPORTMAT_READY;
		return vret;
	}
	obj.Status = CCEXPORTMAT_READY;
	CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getVal():: Failed to find table with ID [" __ZU__ "]!", sel);
}
template<class T> inline T* getVal(
	CCEXP &obj, const char* matname, size_t row, size_t col
) {
	obj.ErrorId = 0;
	size_t sel = obj.getTableIndexByName(matname);
	if (sel == MAXSIZE_T) {
		CCEXP_ERR_T(obj, NULL, ERROR::TableNotFound, "getVal():: Failed to find table with name [%s]!", matname);
	}
	T* vret = getVal<T>(obj, sel, row, col);
	return vret;
}


}; // namespace CCEXP

#endif