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
#include <stdio.h>
#include <limits>

#define CCEXP_VERSION (0.002)

#define CCEXPORTMAT_INIT		(0)
#define CCEXPORTMAT_READY		(1)
#define CCEXPORTMAT_ACTIVE		(2)
#define CCEXPORTMAT_DIED		(3)

using namespace std;

namespace CCEXP {

//@#| ############### Virtual Base Class ###############
class CCEXPBase {
	public:
	virtual ~CCEXPBase() {}
	virtual int CompareName(const char* Name) = 0;
	virtual bool getIgnoreStatus(void) = 0;
	virtual int setIgnoreStatus(bool status) = 0;
	virtual int NewLine(int empty) = 0;
	virtual int Rows(size_t &rows) = 0;
	virtual int Cols(size_t row, size_t &cols) = 0;
	virtual int StoreData(FILE *fp) = 0;
	virtual int Reset(void) = 0;
};

//@#| ############### CCEXPMat - Table Class ###############
template<class T>
class CCEXPMat : public CCEXPBase {
	public:
		CCEXPMat();
		~CCEXPMat();

		int Initialize(const char* Name, const char* typeName, size_t MaxRows);
		int AddValue(T val);
		int AddRow(T* ptr, size_t n);

		int CompareName(const char* Name);
		bool getIgnoreStatus(void);
		int setIgnoreStatus(bool status);
		int NewLine(int empty);
		int Rows(size_t &rows);
		int Cols(size_t row, size_t &cols);
		int StoreData(FILE* fp);
		int Reset(void);

	private:
		char name[64];
		char type[64];
		bool IgnoreM;
		bool newLineFlag;
		list<vector<T>> data;
		size_t _maxRows;
};

//@#| ############### CCEXP : This is the Global Class ###############
class CCEXP {
	public:
	FILE* fp;
	int TotalMatrices;
	int LoadIndex;
	int Status;
	int STCounter;
	bool isActive;
	
	vector<shared_ptr<CCEXPBase>> M;
	char SavingFile[256];
	
	CCEXP();
	~CCEXP();
};












//@#| ############### CCEXPBase Functions definitions ###############
template<class T> CCEXPMat<T>::CCEXPMat() : IgnoreM(false), newLineFlag(true) { }
template<class T> CCEXPMat<T>::~CCEXPMat() { }

template<class T> int CCEXPMat<T>::Initialize(const char* Name, const char* typeName, size_t MaxRows) {
	sprintf(name,"%s",Name); sprintf(type,"%s",typeName);
	data.clear();
	_maxRows = (MaxRows == 0) ?  std::numeric_limits<size_t>::max() : MaxRows;
	return 0;
}

template<class T> int CCEXPMat<T>::CompareName(const char* Name) {
	return strcmp(name,Name);
}

template<class T> bool CCEXPMat<T>::getIgnoreStatus(void) { return IgnoreM; }

template<class T> int CCEXPMat<T>::setIgnoreStatus(bool status) { IgnoreM = status; return 0; }

template<class T> int CCEXPMat<T>::AddValue(T val) {
	if (IgnoreM) return -1;
	const size_t N = data.size();
	if (N >= _maxRows) return -1;
	if ((N == 0) || (newLineFlag==true)) {
		vector<T> dl; dl.clear(); data.push_back(dl);
		newLineFlag = false;
	}
	data.back().push_back(val);
	return 0;
}

template<class T> int CCEXPMat<T>::AddRow(T* ptr, size_t n) {
	if (IgnoreM) return -1;
	const size_t N = data.size();
	if (N >= _maxRows) return -1;
	vector<T> dl; dl.resize(n);
	memcpy(dl.data(), ptr, n * sizeof(T));
	data.push_back(dl);
	newLineFlag=true;
	return 0;
}

template<class T> int CCEXPMat<T>::Rows(size_t &rows) {
	if (IgnoreM) return -1;
	rows = data.size();
	return 0;
}

template<class T> int CCEXPMat<T>::Cols(size_t row, size_t &cols) {
	if (IgnoreM) return -1;
	const size_t N = data.size();
	if (row >= N) return -1;
	auto it = std::next(data.begin(), row);
	cols = (*it).size();
	return 0;
}

template<class T> int CCEXPMat<T>::NewLine(int empty) {
	if (IgnoreM) return -1;
	const size_t N = data.size();
	if (N >= _maxRows) return -1;
	newLineFlag=true;
	if (empty == 1) {
		vector<T> dl; dl.clear(); data.push_back(dl);
	}
	return 0;
}


template<class T> int CCEXPMat<T>::StoreData(FILE* fp) {
	if (fp == NULL) return -1;
	if (IgnoreM) return -1;
	
	size_t N = data.size();
	size_t typeSize = sizeof(T);
	
	fwrite(name,sizeof(char),64,fp); /// Table, Name
	fwrite(type,sizeof(char),64,fp); /// Table, Type (string)
	fwrite(&typeSize,sizeof(size_t),1,fp); /// Table, type size in Bytes
	fwrite(&N,sizeof(size_t),1,fp); /// Table, number of rows
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
int NewLine(CCEXP &obj, const char *matname, int empty = 0);
int Reset(CCEXP &obj);

//@#: ############### Templates API Functions ###############
template<class T> inline int AddTable (
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) return -1;	
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	return U->Initialize(matname, typeName, MaxRows);
	return -1;
}

template<class T> inline int AddTableI(
	CCEXP &obj,
	const char* matname,
	const char* typeName,
	size_t MaxRows = 0
) {
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) return -1;
	obj.M.push_back(shared_ptr<CCEXPBase>((CCEXPBase*) new CCEXPMat<T>));
	CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[obj.M.size()-1].get());
	int ret = U->Initialize(matname, typeName, MaxRows);
	if (ret != 0) return ret;
	return U->setIgnoreStatus(true);
}

template<class T> inline int AddRow(
	CCEXP &obj,
	const char* matname,
	T* ptr,
	size_t n
) {
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) return -1;
	size_t i, sel;
	const size_t N = obj.M.size();
	bool ArrayFound = false;
	for (i=0; i < N; i++) { 
		if ((obj.M[i])->CompareName(matname) == 0) { sel=i; ArrayFound = true; break; }
	}
	if (ArrayFound) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		return U->AddRow(ptr, n);
	}
	return -1;
}

template<class T> inline int AddRowi(
	CCEXP &obj,
	size_t sel,
	T* ptr,
	size_t n
) {
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) return -1;
	if (sel < obj.M.size()) {
		CCEXPMat<T>* U = static_cast<CCEXPMat<T>*>(obj.M[sel].get());
		return U->AddRow(ptr, n);
	}
	return -1;
}

}; // namespace CCEXP

#endif