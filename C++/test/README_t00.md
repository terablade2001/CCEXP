# Build and Test t00_Basics.cpp example

## Compile and Run the example
In this example I'm using __CCEXP__USE_MVECTOR flag, which uses ns_MVECTOR::MVECTOR<> class instead of std::vector<> class.
This also gives the ability to track allocated memory from MVECTOR. At the end of the program the total allocated memory should be 0 bytes due to clear() call.

### Compile example
Having a clean git repository (i.e. 'git clean -dfx') use the following commands:
```
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
# cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release ..
make && cls && CCEXP t00
```

After the 'CCEXP t00' command the produced executable should display:

To confirm results copy ./MATLAB/test/t00_Basics.m and ./MATLAB/src/CCEXP.m files to ./build/ folder and execute the t00_Basics.m via Octave/MatLab:
You can confirm there that all data have been written correctly to the 'DataFile.ccexp'.

```
[t00_Basics.cpp: 53]: #1 : Total Bytes: 0


**** TEST:: Error must occur at Line [60]! *******
[t00_Basics.cpp, 60]: CCEXP Err Display >>
 * Initialize():: CCEXP object with name [DataFile.ccexp] has wrong status. Confirm you have reset it first.
[t00_Basics.cpp: 62]: #2 : Total Bytes: 0


**** TEST:: Error must occur at Line [70]! *******
[t00_Basics.cpp, 70]: CCEXP Err Display >>
 * AddTable()::  Table with name [T_U8] already exist!
[t00_Basics.cpp: 72]: #3 : Total Bytes: 16384

[t00_Basics.cpp: 117]: #4 : Total Bytes: 802816


**** TEST:: Error must occur at Line [126]! *******
[t00_Basics.cpp, 126]: CCEXP Err Display >>
 * CCEXPMat::NewRow():: Maximum Rows (3) already Reached!
 * NewRow():: Table with ID = 2 return error during call to NewRow()...
 * NewRow():: CCEXP Table with ID [2] has wrong status.
[t00_Basics.cpp: 128]: #5 : Total Bytes: 802816

Table T_U8 has [256] rows

Row [255] of Table T_U8 has [255] columns
[t00_Basics.cpp: 142]: #6 : Total Bytes: 802816


**** TEST:: Error must occur at Line [150]! *******
[t00_Basics.cpp, 150]: CCEXP Err Display >>
 * CCEXPMat::Cols():: Requested Row is not exist yet! (row >= _maxRows) (!0!)
 * Cols():: Table with ID = 0 return error during call to Cols()...


**** TEST:: Error must occur at Line [155]! *******
[t00_Basics.cpp, 155]: CCEXP Err Display >>
 * Cols():: Failed to find table with name [T_UU88]!

[t00_Basics.cpp: 158]: #7 : Total Bytes: 802816
[t00_Basics.cpp: 176]: #8 : Total Bytes: 811008
[t00_Basics.cpp: 196]: #9 : Total Bytes: 856064


Table Name of Table with ID (3) is: AddVal
Table Name of Table with ID (5) is: DeleteLastElement
Table Name of Table with ID (8) is: SetVal
[t00_Basics.cpp: 252]: #10 : Total Bytes: 901120



--- MVECTOR Size before Reset: 901120 bytes.
--- MVECTOR Size after Reset: 0 bytes.

... Program End ...
```
