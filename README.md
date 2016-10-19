# CCEXP

## What is CCEXP ?

CCEXP (Class C EXPort) is a library in C++, based on templates and standard C/C++ libraries and functions, for organized data extraction and sharing between C++ code and other software (i.e. MatLab/Octave, Python etc). The sharing is currently done via files, which can store large amount of data.

## Why should I use CCEXP ?

* Data Export / Analysis / Debugging
* Data Sharing in different platforms (C++, MatLab/Octave, Python)

### Data Export / Analysis / Debugging
You make a piece of code and need to export different runtime values and data that are going to be analyzed in another software (i.e. MatLab/Octave, Python etc). Instead of writing down your own code using fopen/fstream etc, you can just include the CCEXP.hpp and CCEXP.cpp files in your project and use them, for exporting multiple different types of data in an organized form.

An example:
```C++
#include "CCEXP.hpp"
...
// Initialize an CCEXP object and add your Tables...
CCEXP::CCEXP FileA;
CCEXP::Initialize(FileA, "FileNameA.ccexp");
	CCEXP::AddTable <uint8_t>(FileA,"Image","uint8");
	CCEXP::AddTable <float>(FileA,"Resulted_Coeffs","single");

...
// On a running environment, capture images and calc data..
for (int i=0; i < NImages; i++) {
	uint8_t* pImage = CaptureImage(Camera.getImage());
	vector<float> Coeffs = CalcCoeffs(pImage);

	// Add the data fast and easy to CCEXP for export
	CCEXP::AddRow<uint8_t>(FileA, "Image", pImage, 640*480);
	CCEXP::AddRow<float>(FileA, "Resulted_Coeffs", Coeffs.data(), Coeffs.size());    
}

...
// During termination store the file with all the captures and data..
CCEXP::StoreData(FileA);
CCEXP::Reset(FileA);
```

At MatLab/Octave you can read and check if everything is ok, as:
```Octave
[~,d]=CCEXP('FileNameA');
N=1;
% Get Nth Image
testImage = reshape(d.Image{N},[256 256]}';
% Get Nth Images' coeffs
testCoeffs = d.Resulted_Coeffs{N};
```

And what if you need to stop exporting the 'Image' table (because the camera's
data are ok, and you don't need anymore those data)?... With CCEXP you can
ignore or not easily each Table:
```C++
// Just add an 'I' (I-gnore) at the AddTable function, and the table will be
// completely ignored. If need again to enable data of this table, remove the
// 'I' from the name. An easy way to enable/disable export data without messing
// up with a lot of the code!
CCEXP::AddTableI<uint8_t>(FileA,"Image","uint8");
```

### Data Sharing in different platforms (C++, MatLab/Octave, Python)
CCEXP is a very good tool, if you need to exchange exported data from C++ to another collaborator which works in any (supported) language. The CCEXP I/O system is hidden to clients, thus as far as all collaborators understand the data organizaton of CCEXP (Files, Tables(of different types), Rows and Colums) and the relative API in each language, the data can be shared easily between the different platforms.

Below an analytical example of data sharing from a MatLab/Octave script is given, following by an example of C++ data import to a program.

```Octave
clear all; close all; clc;

% ################ PART #1 #####################################################
%Suppose we have to share those data...

% Table 1 Rows (float)
T1_1 = [1.1 2.2 3.3 4.4 5.5];
T1_2 = [];
T1_3 = [6.6 7.7];

% Table 2 Rows (uint8)
T2_1 = [1];
T2_2 = [1 2];
T2_3 = [1 0 3];
T2_4 = [1 0 3 4];
T2_5 = [1 0 3 4 5];
T2_6 = [1 0 3 4 5 6];


% Then we have to convert the data to the following format:

% Table 1
% 1. Each row of a table is becoming a Cell
TVData1{1} = T1_1;
TVData1{2} = T1_2;
TVData1{3} = T1_3;
% 2. The table is created as following:
% Name, Type, Bytes per Element, The Table-data-cell, MaxRows (0=ALL)
Table1 = {'Table_1', 'single', 4, TVData1, 0 };


% Table 2
TVData2{1} = T2_1;
TVData2{2} = T2_2;
TVData2{3} = T2_3;
TVData2{4} = T2_4;
TVData2{5} = T2_5;
TVData2{6} = T2_6;
Table2 = {'Table_2', 'uint8', 1, TVData2, 4}; % Store only 4 first rows!

% Final CELL
% Create the Cell with all the wanted Tables (using ';')
CCEXPData = {Table1; Table2};

% Now use CCEXP_WRITE:
CCEXP_WRITE('SharedData.ccexp',CCEXPData);
```

The "SharedData.ccexp" file can now be shared with a C++ program and loaded as follows:
```C++
#include "CCEXP.hpp"
...
// Define a CCEXP object, open the data file and load any wanted table.
CCEXP::CCEXP LD;

CCEXP::Open(LD,"SharedData.ccexp");
	CCEXP::LoadTable<float>(LD,"Table_1","single");
	CCEXP::LoadTable<uint8_t>(LD,"Table_2","uint8");
CCEXP::Close(LD);

// Then you can access elements like this:
for (size_t r = 0; r < CCEXP::Rows(LD, "Table_2"); r++) {
	for (size_t c = 0; c < CCEXP::Cols(LD, "Table_2", r); c++) {
		uint8_t* v1 = CCEXP::getVal<uint8_t>(LD, "Table_2", r, c);
		if (v1 != NULL) printf("%u ", v1);
	}
}
```