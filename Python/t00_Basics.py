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