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