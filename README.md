# CCEXP
CCEXP (Class C EXPort) is a library in C++, based on templates and standard C/C++ libraries and functions, for organized data extraction and sharing between C/C++ code and other software (i.e. MatLab, etc). The sharing is currently done via files, which can store large amount of data.

The main purpose of CCEXP library is for easier data analysis and debugging issues.

Every time that you have to store & share data, you don't have to think about the format. Just use CCEXP.. :)

# HOW TO EXPORT
Using CCEXP is quite easy.

* Create as many CCEXP objects as you need.
	CCEXP::CCEXP EXP1, EXP2, ...;

* Initialize the wanted objects
	CCEXP::Initialize(EXP1,"Filename.ccexp");
	
* Add different tables of data, to describe any information that you might need.
	CCEXP::AddTable<char>(EXP1, "Username", "char");
	CCEXP::AddTable<uint8_t>(EXP1, "FaceImage", "uint8");
	CCEXP::AddTable<float>(EXP1, "Score", "single");
	
* Add as many data you need, in the form (rows/columns) you need (as far as RAM can support them, otherwise you need to save multiple files)
	CCEXP::AddRow(EXP1, "FaceImage", pdata0, 128*128);
	CCEXP::AddRow(EXP1, "FaceImage", pdata1, 128*128);
	
	CCEXP::AddValue<float>(EXP1,"Score", 0.24);
	CCEXP::AddValue<float>(EXP1,"Score", 0.25);
	CCEXP::NewLine(EXP1,"Score");
	CCEXP::AddValue<float>(EXP1,"Score", 10.25);
	
* Store the data when you need them
	CCEXP::StoreData(EXP1);
	
* Memory clean up will happen automatically when program finish, after a StoreData() call, or by using Reset();
	CCEXP::Reset(EXP1)
	
* Load Tables from a Data File
  // ToDo!
  
# HOW TO SHARE
Just share the data file! Then use the proper code to load the data. In MATLAB this is like:
	[r,d]=CCEXP('Filename.ccexp');

A list with all available tables is provided, with the table names you gave.
Each row of the table is a MatLab cell, like an 1-D array.
	