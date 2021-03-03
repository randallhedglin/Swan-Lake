CLS
RANDOMIZE TIMER

a$ = "scr32_snl"
b$ = "sysid"
c$ = "bwapps.ini"
d$ = "appcommon"
e$ = "inf32"
f$ = "SLS\ProfileData"
g$ = "Microsoft\74ABE3D7-BAF5-4ac2-B848-C655118023E5"
h$ = "slrcs32.sbw"
i$ = "swanlake.dat"

ts$ = a$ + "#" + b$ + "#" + c$ + "#" + d$ + "#" + e$ + "#" + f$ + "#" + g$ + "#" + h$ + "#" + i$ + "##"

c = LEN(ts$) - 1

PRINT c

DIM d(0 TO c)

FOR n = 0 TO c
  d(n) = n
NEXT

FOR n = 1 TO 10000
  n1 = INT(RND(1) * c)
  n2 = INT(RND(1) * c)
  SWAP d(n1), d(n2)
NEXT

OPEN "tsloc.dat" FOR OUTPUT AS #1

FOR n = 0 TO c
  v = d(n)

  a$ = "'" + MID$(ts$, v + 1, 1) + "'"
  IF a$ = "'\'" THEN a$ = "'\\'"
  IF a$ = "'#'" THEN a$ = "0"

  PRINT #1, "lpStr[" + LTRIM$(STR$(v)) + "]=" + a$
NEXT

CLOSE #1

