% MIT License

% Copyright (c) 2016 - 2017 Vasileios Kon. Pothos (terablade2001)
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

% Files required:
% * CCEXP.m
% * DataFile.cexp, produced by t00_Basics.cpp at C++ tests.

clear all; close all; clc;
[r,d] = CCEXP('DataFile.ccexp'); % Load the CCEXP data file and display contents

ROWS = length(d.T_U8);
fprintf('Number of Rows at Table T_U8: %i\n', ROWS);
for i=1:ROWS
	fprintf('*** Row(%i) has %i data: \n', i, length(d.T_U8{i}));
	if (i <= 5)
		d.T_U8{i}
	end
end

% Check if T_F32 data are as expected (127, 128 should be [], and 129 should have data).
d.T_F32{127:129}


fprintf('Checking AddVal()\n');
fprintf('d.AddVal{1} must have values: 0 to 3*4-1=11 .... \n');
d.AddVal{1}

fprintf('Checking AddVal() + DeleteLastRow() + AddVal()\n');
fprintf('d.AddVal{2} must have values: 0 to 7 .... \n');
d.AddVal{2}

fprintf('Checking AddVal() + DeleteRow(3)\n');
fprintf('d.DeleteRow{1:8} must have values: 0,1,2,4,5,6,7,8 .... \n');
d.DeleteRow{1:8}

fprintf('Checking DeleteLastElement(n)\n');
fprintf('d.DeleteLastElement{2} must have values: 0,1 .... \n');
d.DeleteLastElement{2}

fprintf('Checking AppendRow\n');
fprintf('d.AppendRow{2} must have values: 0,1,2,3,4,21,22,23 .... \n');
d.AppendRow{2}

fprintf('Checking ReplaceRow\n');
fprintf('d.ReplaceRow{2} must have values: 31,32,33 .... \n');
d.ReplaceRow{2}

fprintf('Checking SetVal\n');
fprintf('d.SetVal{1} must have values: 0,1,-11,3,-12 .... \n');
d.SetVal{1}

fprintf('Checking InitRowByScalar\n');
fprintf('d.InitRowByScalar{1:3} must have values: [3 3],[7 7 7 7],[] .... \n');
d.InitRowByScalar{1:3}

fprintf('Checking getTableID\n');
fprintf('d.getTableID{1} must have values: 3, 5, 8 .... \n');
d.getTableID{1}