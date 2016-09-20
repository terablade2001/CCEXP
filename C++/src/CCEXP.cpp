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
{ }

CCEXP::~CCEXP() {
	Status = CCEXPORTMAT_DIED;
}



//@#: ############### CCEXP API Functions ###############
int Initialize(CCEXP &obj, const char* fname, const char* Path, bool isactive) {
	obj.isActive = isactive;
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_INIT) { return -1; }	
	if (Path == NULL) sprintf(obj.SavingFile,"%s",fname);
	else sprintf(obj.SavingFile,"%s/%s", Path, fname);
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}

int StoreData(CCEXP &obj, const char* FName) {
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) { return -1; }
	obj.Status = CCEXPORTMAT_ACTIVE;
	FILE *fp;
	if (FName == NULL) fp = fopen(obj.SavingFile,"wb");
	else fp = fopen(FName,"wb");
	
	if (fp == NULL) return -1;

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
	if (!obj.isActive) return -1;
	if (obj.Status != CCEXPORTMAT_READY) return -1;
	obj.Status = CCEXPORTMAT_ACTIVE;
	char fname[256]={0};
	sprintf(fname, "%s.%4.4i", obj.SavingFile, obj.STCounter++);
	StoreData(obj, fname);
	const size_t N = obj.M.size();
	for (size_t i=0; i < N; i++) { (obj.M[i])->Reset(); }	
	obj.Status = CCEXPORTMAT_READY;
	return 0;
}

int NewLine(
	CCEXP &obj,
	const char *matname,
	int empty
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
		return (obj.M[sel])->NewLine(empty);
		// TODO:: Return should pass via interface, to change status before return (thread-safe target)
	}
	return -1;
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
}; // namespace CCEXP;
