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

from ccexp import *
EXP = CCEXP()
EXP.AddTable("Username","char")
EXP.AddTable("Number","int8")
EXP.AddTable("FloatNumber","single")
EXP.AddTable("DoubleNumber","double")

EXP.AddRow("Username", "Person-A")
EXP.AddRow("Number", [1, 2, 3])
EXP.AddRow("Username", "Person-B")
EXP.AddRow("Number", [4, 5, 6])
EXP.AddRow("FloatNumber", [4.781, 5.141, 6.111, 8.7])
EXP.AddRow("DoubleNumber", [4.781, 5.141, 6.111, 8.7])
EXP.AddRow("DoubleNumber", [0.1, 1.1])
EXP.AddRow("DoubleNumber", [])
EXP.AddRow("DoubleNumber", [121.0])
EXP.StoreData("Test.ccexp")




