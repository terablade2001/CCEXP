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

d = CCEXP("SharedData.ccexp",1);

print("------------- CHECKS --------------------")
print("Table_1 row(0) %s" % d["Table_1"].Data[0])
print("Table_1 row(1) %s" % d["Table_1"].Data[1])
print("Table_1 row(2) %s" % d["Table_1"].Data[2])

print("Table_2 row(0) %s" % d["Table_2"].Data[0])
print("Table_2 row(1) %s" % d["Table_2"].Data[1])
print("Table_2 row(2) %s" % d["Table_2"].Data[2])
print("Table_2 row(2) %s" % d["Table_2"].Data[3])

