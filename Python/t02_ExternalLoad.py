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
