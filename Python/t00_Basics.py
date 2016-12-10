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

from ccexp import CCEXP

d = CCEXP("DataFile.ccexp",1);


print("------------- CHECKS --------------------")
print("AddVal (Must display: 0,1,2,4,5,6,7,8,9,10,11): %s" % d["AddVal"].Data[0])
print("AddVal (Must display: 0,1,2,4,5,6,7): %s" % d["AddVal"].Data[1])
print("DeleteRow (Must display: 0,1,2,4,5,6,7,8): %s" % d["DeleteRow"].Data)
print("DeleteLastElement (Must display: 0,1): %s" % d["DeleteLastElement"].Data[1])
print("AppendRow (Must display: 0,1,2,3,4,21,22,23): %s" % d["AppendRow"].Data[1])
print("ReplaceRow (Must display: 31,32,33): %s" % d["ReplaceRow"].Data[1])
print("SetVal (Must display: 0,1,-11,3,-12): %s" % d["SetVal"].Data)
print("InitRowByScalar (Must display: [3 3],[7 7 7 7],[]): %s" % d["InitRowByScalar"].Data)
print("getTableID (Must display: [3, 5, 8]): %s" % d["getTableID"].Data)