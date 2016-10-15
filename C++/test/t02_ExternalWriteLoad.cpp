// Include the CCEXP library
#include "../src/include/CCEXP.hpp"

// Demo/Debugging tool. Enable or Comment ERROR_STOP, to stop on Errors or
// to force debugging behaviour and continue.
// * For this UnitsTests Demo I have not set ERROR_STOP thus every error
// just shows an error message and moves on. 

#define ERROR_STOP
#ifdef ERROR_STOP
	#define _DBG_ERROR_STOP_OR_CONTINUE_(x) \
		__CCEXP_ERR_DISPLAY((x),-1); if ((x).Status != 1) return (x).Status;
#else	
	#define _DBG_ERROR_STOP_OR_CONTINUE_(x) \
		__CCEXP_ERR_DISPLAY((x),-1); CCEXP::DBG_SetStatus((x), 1);
#endif

int main(int argc, char **argv) {
	// Load a Binary file which created with external software (i.e. Python, MatLab(Octave) etc)
	CCEXP::CCEXP LD;
	
	CCEXP::Open(LD,"SharedData.ccexp");
		CCEXP::LoadTable<float>(LD,"Table_1","single");
		CCEXP::LoadTable<uint8_t>(LD,"Table_2","uint8");
	CCEXP::Close(LD);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	size_t Rows_1 = 0, Rows_2 = 0;
	CCEXP::Rows(LD, "Table_1", Rows_1);
	CCEXP::Rows(LD, "Table_2", Rows_2);
	_DBG_ERROR_STOP_OR_CONTINUE_(LD);
	
	cout << endl << "Table_1 has [" << Rows_1 <<
	  "] rows, while Table_2 has [" << Rows_2 <<
	  "] rows." << endl << endl;
	
	// Print Table_1 data ...
	for (size_t r = 0; r < Rows_1; r++) {
		size_t col; CCEXP::Cols(LD, "Table_1", r, col);
		cout << "Table_1, row(" << r << ") = [";
		for (size_t c = 0; c < col; c++) {
			float v1; CCEXP::getVal<float>(LD, "Table_1", r, c, v1);
			cout << v1 << " ";
		}
		cout << "]" << endl;
	}
	
	cout << endl;
	
	// Print Table_2 data ...
	for (size_t r = 0; r < Rows_2; r++) {
		size_t col; CCEXP::Cols(LD, "Table_2", r, col);
		cout << "Table_2, row(" << r << ") = [";
		for (size_t c = 0; c < col; c++) {
			uint8_t v1; CCEXP::getVal<uint8_t>(LD, "Table_2", r, c, v1);
			printf("%u ", v1);
		}
		cout << "]" << endl;
	}

	_DBG_ERROR_STOP_OR_CONTINUE_(LD);

	return 0;
}
