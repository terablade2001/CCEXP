#! /user/bin/env python
# MIT License

# Copyright (c) 2016 - 2019 Vasileios Kon. Pothos (terablade2001)
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
import sys
import struct

class CCEXP:
  def __init__(self):
    self.obj = {}
    if sys.version_info[0] < 3:
      self.PyTypes = { "char" : "c", "int8" : "b",  "uint8" : "B",   "bool" : "?", "int16" : "h", "uint16" : "H", "int32" : "i", "uint32" : "I", "int64" : "q", "uint64" : "Q", "single" : "f", "double" : "d", "size_t" : "Q" }
    else:
      self.PyTypes = { "char" : "c", "int8" : "b",  "uint8" : "B",   "bool" : "?", "int16" : "h", "uint16" : "H", "int32" : "i", "uint32" : "I", "int64" : "q", "uint64" : "Q", "single" : "f", "double" : "d", "size_t" : "N" }
    self.PyTypesBytes = { "char" : 1, "int8" : 1, "uint8" : 1, "bool" : 1, "int16" : 2, "uint16" : 2, "int32" : 4, "uint32" :4,   "int64" : 8, "uint64" : 8, "single" : 4, "double" : 8, "size_t" : 8 }
    return

  def AddTable(self, tableName, tableType):
    p = [p for p,i in enumerate(self.obj.keys()) if p == tableName]
    if not p:
      self.obj[tableName]={}
      self.obj[tableName]["data"]={}
      self.obj[tableName]["type"]=tableType
      self.obj[tableName]["cnt"]=0
    else:
      S = "Table [%s] already exist!"%(tableName)
      raise Exception(S)
    return

  def AddRow(self, tableName, rowdata):
    p = [p for p in self.obj.keys() if p == tableName]
    if not p:
      S = "Table [%s] does not exist!"%(tableName)
      raise Exception(S)
    else:
      cnt = self.obj[tableName]["cnt"]
      self.obj[tableName]["data"][cnt] = rowdata
      self.obj[tableName]["cnt"] += 1
    return

  def StoreData(self, filename):
    sizeT = self.PyTypes["size_t"]
    NTables = len(self.obj.keys())
    ByteSize = 8
    with open(filename,"wb") as fout:
      fout.write(struct.pack('I', ByteSize))
      fout.write(struct.pack(sizeT, NTables))

      for tableName in self.obj.keys():
        named = tableName
        TableType = self.obj[tableName]["type"]
        PackType  = self.PyTypes[TableType]
        PackBytes = self.PyTypesBytes[TableType]
        typed = TableType
        named = named + "\0"*(64-len(named))
        typed = typed + "\0"*(64-len(typed))
        named = named.encode('utf8')
        typed = typed.encode('utf8')
        if (len(named) > 64):
          raise Exception("UTF8 of name is > 64")
        if (len(typed) > 64):
          raise Exception("UTF8 of type is > 64")
        fout.write(named)
        fout.write(typed)
        nRows = self.obj[tableName]["cnt"]
        fout.write(struct.pack(sizeT, PackBytes))
        fout.write(struct.pack(sizeT, nRows))
        fout.write(struct.pack(sizeT, nRows))
        cols = []
        for row in range(0, nRows):
          cols = len(self.obj[tableName]["data"][row])
          fout.write(struct.pack(sizeT,cols))
        for row in range(0, nRows):
          arr = self.obj[tableName]["data"][row]
          if type(arr) is str:
            arr = bytearray(arr,'utf8')
            fout.write(arr)
          else:
            for v in arr:
              fout.write(struct.pack(PackType, v))





def CCEXPRead(filename, flag):
  if sys.version_info[0] < 3:
    PyTypes = { "char" : "c", "int8" : "b",  "uint8" : "B",   "bool" : "?", "int16" : "h", "uint16" : "H", "int32" : "i", "uint32" : "I", "int64" : "q", "uint64" : "Q", "single" : "f", "double" : "d", "size_t" : "Q" }
  else:
    PyTypes = { "char" : "c", "int8" : "b",  "uint8" : "B",   "bool" : "?", "int16" : "h", "uint16" : "H", "int32" : "i", "uint32" : "I", "int64" : "q", "uint64" : "Q", "single" : "f", "double" : "d", "size_t" : "N" }
  d={}

  class CCCEXP:
    Name=[]
    Type=[]
    TypeSize=[]
    Data=[]

  sizeT = PyTypes["size_t"]
  fin = open(filename,"rb")
  ByteSize = (struct.unpack('I', fin.read(4)))[0]
  NTables = (struct.unpack(sizeT, fin.read(ByteSize)))[0]

  for x in range(NTables):
    name = (fin.read(64).decode('utf-8')).replace(str("\0"), "")
    typed = (fin.read(64).decode('utf-8')).replace(str("\0"), "")
    PyType = PyTypes[typed]
    typeSize = (struct.unpack(sizeT, fin.read(ByteSize)))[0]
    nElements = (struct.unpack(sizeT, fin.read(ByteSize)))[0]
    MaxRows = (struct.unpack(sizeT, fin.read(ByteSize)))[0]
    if (nElements > MaxRows):
      nElements = MaxRows
    Table = CCCEXP()
    Table.Name = name
    Table.Type = typed
    Table.TypeSize = typeSize
    Table.Data=[]
    if sys.version_info[0] < 3:
      cols = []
      for i in range(nElements):
        ccols = struct.unpack_from(sizeT, fin.read(ByteSize))
        cols.append(ccols[0])
      for i in range(nElements):
        row=[]
        for c in range(cols[i]):
          byte = struct.unpack_from(PyType, fin.read(typeSize))
          row.append(byte[0])
        if Table.Type == 'char':
          row = str(b''.join(row).decode('utf8'))
        Table.Data.append(row)
    else:
      it = struct.iter_unpack(sizeT, fin.read(ByteSize*nElements))
      for t in it: # For each row of the table t[0] is the elements of the row
        elit = struct.iter_unpack(PyType, fin.read(typeSize*t[0]))
        row=[]
        for el in elit:
          row.append(el[0])
        if Table.Type == 'char':
          row = str(b''.join(row).decode('utf8'))
        Table.Data.append(row)
    d[Table.Name] = Table

  fin.close()

  if (flag == 1):
    N = len(d)
    print("--- [%s] ---:  Tables: %i" % (filename, N))
    Keys = list(d.keys())
    for x in range(N):
      k = Keys[x]
      rows = len(d[k].Data)
      if rows == 1:
        print("%i: %s (%s) -> [1 row x %i elements]" % (x, d[k].Name, d[k].Type, len(d[k].Data[0])))
      else:
        print("%i: %s (%s) -> [%i rows]" % (x, d[k].Name, d[k].Type, rows))
  return d