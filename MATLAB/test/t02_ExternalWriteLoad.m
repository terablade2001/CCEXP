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

