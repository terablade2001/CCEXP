% MIT License

% Copyright (c) 2016 Vasileios Kon. Pothos (terablade2001)

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

function [r, d] = CCEXP(fname, logg)
	if (nargin == 1); logg = 0; end;
	fp=fopen(fname,'rb');
    if (fp == -1); error(['File: [' fname '] was not found!..']); end;
	
	size_t_Bytes = fread(fp,1,'uint32'); % Read the Bytes of "size_t" at C++ code.
	if (size_t_Bytes == 4); size_t = 'uint32'; end; % More options can be added / Changed.
	if (size_t_Bytes == 8); size_t = 'uint64'; end; % More options can be added / Changed.
	
	N = fread(fp,1,size_t); % Read the number of tables in the cexp file.
	
	for i=1:N % For every table, load ...
	
		name = fread(fp,64,'char')'; % Name of the table
		type = fread(fp,64,'char')'; % Data type of the table

		% Check and convert data to strings.
        if (sum(name(:)) == 0)
            r(i).name = sprintf('NameNotFound_%i',i);
			error(['Name Not Found! >>: ' r(i).name]);
        elseif (sum(type(:)) == 0)
			r(i).type = sprintf('TypeNotFound_%i',i);
			error(['Type Not Found! >>: ' r(i).type]);
		else
            p=find(name == 0); name = name(1:p-1);
            r(i).name = char(name);
            p=find(type == 0); type = type(1:p-1);
			r(i).type = char(type);
        end
		
		
		r(i).typeSize = fread(fp,1,[size_t '=>' size_t]); % Get the bytes of the specific type.
		
		NumOfRows = fread(fp, 1, size_t); % Load the number of rows of the table
		
		DPL = fread(fp, NumOfRows(1), size_t); % Load the elements per row data of the table

		TB = sum(DPL); % Calculate total elements of the table
		
		data = fread(fp, TB, [r(i).type '=>' r(i).type])'; % Load all the elements casted to the correct type.

		% Update the r-Structure.
		r(i).data = {};
		c=1;
		for k=1:NumOfRows
			if (DPL(k) > 0)
				se = max(1, c+DPL(k)-1);
				r(i).data{k} = data(c:se); c=c+DPL(k);
			else
				r(i).data{k} = [];
			end
		end
		r(i).size = DPL;
		d.(r(i).name) = r(i).data;
		d.(['s' (r(i).name)]) = r(i).size;
	end
	fclose(fp);

	
	% Log data out.
	if (logg == 0)
		fprintf('\n%s::>\n',fname);
		for i=1:length(r);
			if (length(r(i).data) == 1)
				fprintf('[%3.0i]> [1x %5.0i] :: (%s): %s\n',i,length(r(i).data{1}),r(i).type, r(i).name);
			else
				fprintf('[%3.0i]> [%5.0i {}] :: (%s): %s\n',i,size(r(i).data,2),r(i).type, r(i).name);
			end
			v.(r(i).name) = r(i);
		end
		fprintf('\n')
	else
		fprintf('\n%s::> ... loaded!\n',fname);
	end;
end