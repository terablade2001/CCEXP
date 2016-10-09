from ccexp import CCEXP

d = CCEXP("Output.ccexp",1);

print("------------- CHECKS --------------------")
print("A_New_Table: (Must display: 0 1 2 3 4): %s" % d[2].Data[0])
