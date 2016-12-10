#! /user/bin/env python
# MIT License

# Copyright (c) 2016 Vasileios Kon. Pothos (terablade2001)
# https://github.com/terablade2001/CCEXP

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
import struct

def CCEXP(filename, flag):

	PyTypes = {
		"char" : "c",
		"int8" : "b",
		"uint8" : "B",
		"bool" : "?",
		"int16" : "h",
		"uint16" : "H",
		"int32" : "i",
		"uint32" : "I",
		"int64" : "q",
		"uint64" : "Q",
		"single" : "f",
		"double" : "d",
		"size_t" : "N"
	}
	d={};

	class CCCEXP: 
		Name=[]
		Type=[]
		TypeSize=[]
		Data=[]

	fin = open(filename,"rb");
	ByteSize = (struct.unpack('I', fin.read(4)))[0]
	NTables = (struct.unpack('N', fin.read(ByteSize)))[0]
	
	for x in range(NTables):
		name = (fin.read(64).decode('utf-8')).replace(str("\0"), "")
		type = (fin.read(64).decode('utf-8')).replace(str("\0"), "")
		PyType = PyTypes[type]
		typeSize = (struct.unpack('N', fin.read(ByteSize)))[0]
		nElements = (struct.unpack('N', fin.read(ByteSize)))[0]
		MaxRows = (struct.unpack('N', fin.read(ByteSize)))[0]
		if (nElements > MaxRows):
			nElements = MaxRows;
		Table = CCCEXP()
		Table.Name = name
		Table.Type = type
		Table.TypeSize = typeSize
		Table.Data=[]
		it = struct.iter_unpack('N', fin.read(ByteSize*nElements))
		for t in it: # For each row of the table t[0] is the elements of the row
			elit = struct.iter_unpack(PyType, fin.read(typeSize*t[0]))
			row=[]
			for el in elit:
				row.append(el[0])
			Table.Data.append(row)
		d[Table.Name] = Table;

	fin.close()

	if (flag == 1):
		N = len(d)
		print("Tables: %i" % (N))
		Keys = list(d.keys());
		for x in range(N):
			k = Keys[x];
			rows = len(d[k].Data)
			if rows == 1:
				print("%i: %s (%s) -> [1 row x %i elements]" % (x, d[k].Name, d[k].Type, len(d[k].Data[0])))
			else:
				print("%i: %s (%s) -> [%i rows]" % (x, d[k].Name, d[k].Type, rows))
		
	return d