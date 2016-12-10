% MIT License

% Copyright (c) 2016 Vasileios Kon. Pothos (terablade2001)
% https://github.com/terablade2001/CCEXP

% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, including without limitation the rights
% to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
% copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions:

% The above copyright notice and this permission notice shall be included in all
% copies or substantial portions of the Software.

% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
% SOFTWARE.

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


% ################ PART #2 #####################################################
% Load CCEXP file
clear all; close all; clc;

[r,d]=CCEXP('SharedData.ccexp');

disp(d.Table_1)
disp(d.Table_2)

