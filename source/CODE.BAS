CLS

DIM c$(64)

FOR n = 0 TO 25
  c$(n) = CHR$(n + 65)
NEXT

FOR n = 0 TO 25
  c$(n + 26) = CHR$(n + 97)
NEXT

FOR n = 0 TO 9
  c$(n + 52) = CHR$(n + 48)
NEXT

c$(62) = "-"
c$(63) = "_"

RANDOMIZE TIMER

FOR n = 1 TO 10000
  n1 = INT(RND(1) * 64)
  n2 = INT(RND(1) * 64)
  SWAP c$(n1), c$(n2)
NEXT

DIM d(64)

FOR n = 0 TO 63
  d(n) = n
NEXT

FOR n = 1 TO 10000
  n1 = INT(RND(1) * 64)
  n2 = INT(RND(1) * 64)
  SWAP d(n1), d(n2)
NEXT

OPEN "code.dat" FOR OUTPUT AS #1

FOR c = 0 TO 63
  n = d(c)
  PRINT #1, "g_lpNybCode[" + LTRIM$(STR$(n)) + "]=" + c$(n)
NEXT

CLOSE #1

