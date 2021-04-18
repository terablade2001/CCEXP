% MIT License

% Copyright (c) 2016 - 2020 Vasileios Kon. Pothos (terablade2001)
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

% Version 0.106

function CCEXP_WRITE(fname,V)
	% Setup size_t variable definition
	size_tBytes = 8; size_t = 'uint64';

	fp = fopen(fname,'wb');
	if (fp == -1); error(['File: [' fname '] could not be created!..']); end;

	fwrite(fp,size_tBytes,'uint32');

	NT = length(V); % NT is the number of Tables that are going to be stored.
	fwrite(fp,NT,size_t);

	for i=1:NT % For every Table
		% Get the name of the Table and store it
		name = [uint8(V{i}{1,1}) 0]; L=length(name);
		name = [name zeros(1,64-L)];  
		fwrite(fp, name, 'uint8');

		% Get the type of the Table and store it
		type = [uint8(V{i}{1,2}) 0]; L=length(type);
		type = [type zeros(1,64-L)];   
		fwrite(fp,type,'uint8');

		% Store the typeSize of the Table (bytes per element)
		fwrite(fp, V{i}{1,3}, size_t); 

		% Get the Rows of the table and store it. Then use it.
		N = length(V{i}{1,4});
		fwrite(fp, N, size_t);

		% Get MaxRows of the Table (0 = no limit)
		MaxRows = V{i}{1,5};
		% Limit Data that are going to be stored.
		if (MaxRows == 0); MaxRows = intmax(size_t); end;
		if (N > MaxRows); N = MaxRows; end;
		fwrite(fp, MaxRows, size_t);

		% Create an array of the columns per row, and store it.
		DPL = zeros(1,N,size_t);
		for j=1:N
			DPL(j) = numel(V{i}{1,4}{j});
		end
		fwrite(fp, DPL, size_t);

		% Store the data for every row..
		for j=1:N
			Data = V{i}{1,4}{j};
			DataDims = length(size(Data));
			PermuteIDX = [1:DataDims];
			PermuteIDX(1:2)=[2 1];
			Data=permute(Data,PermuteIDX);
			fwrite(fp, Data(:), V{i}{1,2});
		end
	end
	fclose(fp);