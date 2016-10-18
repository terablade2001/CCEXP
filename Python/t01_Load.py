from ccexp import CCEXP

d = CCEXP("Output.ccexp",1);

print("------------- CHECKS --------------------")
print("A_New_Table: (Must display: 0 1 2 3 4): %s" % d["A_New_Table"].Data[0])
