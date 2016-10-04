#! /user/bin/env python
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
	d=[]

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
		d.append(Table)	

	fin.close()

	if (flag == 1):
		N = len(d)
		print("Arrays: %i" % (N))
		for x in range(N):
			rows = len(d[x].Data)
			if rows == 1:
				print("%i: %s (%s) -> [1 row x %i elements]" % (x, d[x].Name, d[x].Type, len(d[x].Data[0])))
			else:
				print("%i: %s (%s) -> [%i rows]" % (x, d[x].Name, d[x].Type, rows))
		
	return d