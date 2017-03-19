// MIT License

// Copyright (c) 2017 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001/MVECTOR

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

#ifndef __MVECTOR_HPP__
#define __MVECTOR_HPP__

#include <stdlib.h>
#include <cstring>
#include <memory>
#include <cstdio>
#include <limits>

#define MVECTOR_USE__NEW

#ifdef _MSC_VER
	#define snprintf _snprintf_s
	#define __MVECTOR_FOPEN__(fp, fname, mode) fopen_s(&fp, fname, mode)
	#define __ZU__ "%Iu"
	#include <algorithm>
#endif
#ifndef _MSC_VER
	#define __MVECTOR_FOPEN__(fp, fname, mode) fp = fopen(fname, mode)
	#define __ZU__ "%zu"
#endif

#define MVECTOR_VERSION (0.003)
#define MVECTOR_STEP_ELEMENTS (1024)
#define MVECTOR_STEP_ELEMENTS_BACK (10*1024)

using namespace std;

namespace MVECTOR {

template<class T> class MVECTOR;
template<class T > class s_mvector_deleter;

#ifdef MVECTOR_USE__NEW
template<class T >
class s_mvector_deleter {
public: 
  void operator()(T* p) { 
    delete[] p; 
  }
};
#endif

template<class T>
class MVECTOR {
public:
	MVECTOR();
	MVECTOR(size_t elements_);
	MVECTOR(size_t elements_, T init_value_);
	~MVECTOR();
	
	void initialize(size_t elements_);
	void set_steps(
		size_t step_elements_,
		size_t step_elements_back_ = ((size_t)-1)
	);
	void resize(size_t new_elements_);
	void resize(size_t new_elements_, T value_);
	int clear();
	size_t size();
	size_t bytes();
	T* data();
	T &operator[](size_t index_);
	void push_back(T value_);
	void pop_back();

private:
	T* pdata;
	size_t elements;
	size_t mem_elements;
	size_t step_elements;
	size_t step_elements_back;
#ifdef MVECTOR_USE__NEW
	std::shared_ptr<T> sptr;
#endif
}; // class MVECTOR



//@#| ############### Template - Constructors ###############
template <class T> MVECTOR<T>::MVECTOR() :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
}

template <class T> MVECTOR<T>::~MVECTOR() {
	clear();
}

template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_
) :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
	initialize(elements_);
}


template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_,
	T init_value_
) :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
	initialize(elements_);
	for (size_t i = 0; i < elements; i++)
		pdata[i] = init_value_;	
}


template <class T>
void MVECTOR<T>::initialize(
	size_t elements_
) {
	if (elements_ == 0) return;
	size_t align_elements = ((elements_ / step_elements) + 1) * step_elements;
#ifndef MVECTOR_USE__NEW
	pdata = (T*)malloc(align_elements * sizeof(T));
#endif
#ifdef MVECTOR_USE__NEW
	sptr = std::shared_ptr<T>(new T[align_elements], s_mvector_deleter<T>());
	pdata = sptr.get();
#endif
	elements = elements_;
	mem_elements = align_elements;
}

template <class T>
void MVECTOR<T>::set_steps(
	size_t step_elements_,
	size_t step_elements_back_
) {
	step_elements = step_elements_;
	if (step_elements_back_ != ((size_t)-1))
		step_elements_back = step_elements_back_;
}
	


//@#: ############### .HPP file ###############
template <class T> void MVECTOR<T>::resize (size_t new_elements_) {
	if (new_elements_ == 0) { clear(); return; }
	bool do_copy = ((pdata != NULL) && (elements > 0));
	T* pdata_old = NULL;
	size_t align_elements = ((new_elements_ / step_elements) + 1) * step_elements;
#ifndef MVECTOR_USE__NEW
	if (do_copy) pdata_old = pdata;
	pdata = (T*)malloc(align_elements * sizeof(T));
	if (do_copy) {
		memcpy(pdata, pdata_old, elements * sizeof(T));
		free(pdata_old);
	}
#endif
#ifdef MVECTOR_USE__NEW
	std::shared_ptr<T> sptr_old;
	if (do_copy) {
		 sptr_old = sptr;
		 pdata_old = sptr_old.get();
	}
	sptr = std::shared_ptr<T>(new T[align_elements], s_mvector_deleter<T>());
	pdata = sptr.get();
	if (do_copy)
		for (size_t i = 0; i < elements; i++) pdata[i] = pdata_old[i];
#endif
	
	elements = new_elements_;
	mem_elements = align_elements;
}

template <class T> void MVECTOR<T>::resize (size_t new_elements_, T value_) {
	resize(new_elements_);
	for (size_t i = 0; i < new_elements_; i++)
		pdata[i] = value_;
}

template <class T> size_t MVECTOR<T>::bytes () {
	return mem_elements * sizeof(T);
}

template <class T> T* MVECTOR<T>::data() {
	return pdata;
}

template <class T> T& MVECTOR<T>::operator[](size_t index_) {
	// if (pdata == NULL) resize(1); // Crash if pdata == NULL.
	// if (index_ >= elements) return pdata[0]; // Crash if wrong index is called.
	return pdata[index_];
}

template <class T> void MVECTOR<T>::push_back(T value_) {
	elements++;
	if (elements >= mem_elements)
		resize(elements);
	pdata[elements-1] = value_;
}

template <class T> void MVECTOR<T>::pop_back() {
	if (elements == 0) return;
	elements--;
	if (mem_elements - elements >= step_elements_back) {
		size_t align_elements = ((elements / step_elements) + 1) * step_elements;
		if (align_elements != mem_elements)
			resize(elements);
	}
}
	

template <class T>
int MVECTOR<T>::clear() {
	if (pdata == NULL) return 1;
#ifndef MVECTOR_USE__NEW
	free(pdata);
#endif
#ifdef MVECTOR_USE__NEW
	sptr.reset();
#endif
	pdata = NULL;
	elements = 0;
	mem_elements = 0;
	return 0;
}

template <class T>
size_t MVECTOR<T>::size() {
	return elements;
}


}; // namespace MVECTOR


#endif









