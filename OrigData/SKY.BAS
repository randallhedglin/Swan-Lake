'output file
DATA "c:\windows\msremote.sfs\progra~1\swanla~1\data\sky.x"

'textures: name, file
DATA 1
DATA "Sky","sky.jpg"

'scale factor
DATA 250

'data paths
DATA 1
'note: CW for inside visible; CCW for outside visible

'start point (x,y,z), height, stretch factor, texture
DATA 0.0000,1.0000,-0.7500,1.5000,6.0000,"Sky"
'number of segments, top=bottom flag
DATA 8,1
'segments (x,y)
DATA 0.7071,0.7071
DATA 1.0000,0.0000
DATA 0.7071,-0.7071
DATA 0.0000,-1.0000
DATA -0.7071,-0.7071
DATA -1.0000,0.0000
DATA -0.7071,0.7071
DATA 0.0000,1.0000

' END DATA
' --------------------------------------------------------------------------
' BEGIN OUTPUT

CLS
DIM pl AS DOUBLE
DIM ln AS DOUBLE

'open output file
READ f$
OPEN f$ FOR OUTPUT AS #1

'output header
PRINT #1, "xof 0302txt 0032"
PRINT #1, ""

'output textures
READ nt
IF nt > 0 THEN
  FOR c = 1 TO nt
    READ tn$, tf$
    PRINT #1, "Material mat"; tn$
    PRINT #1, "{"
    PRINT #1, "  +1.0000000E+00; +1.0000000E+00; +1.0000000E+00; +1.0000000E+00;;"
    PRINT #1, "  +0.0000000E+00;"
    PRINT #1, "  +0.0000000E+00; +0.0000000E+00; +0.0000000E+00;;"
    PRINT #1, "  +0.0000000E+00; +0.0000000E+00; +0.0000000E+00;;"
    PRINT #1, "  TextureFilename { "; CHR$(34); tf$; CHR$(34); "; }"
    PRINT #1, "}"
    PRINT #1, ""
  NEXT
END IF

'read global scale factor
READ gsf

'output paths
READ np
FOR c = 1 TO np
  READ sx, sy, sz
  READ ht, sf
  READ tn$
  READ ns, fl
  sx = sx * gsf
  sy = sy * gsf
  sz = sz * gsf
  ht = ht * gsf
  REDIM x(ns), y(ns), u(ns), x2(ns), y2(ns)
  x(0) = sx: y(0) = sy: u(0) = 0
  cx = sx: cy = sy
  pl = 0
  FOR s = 1 TO ns
    READ x(s), y(s)
    x(s) = x(s) * gsf
    y(s) = y(s) * gsf
    pl = pl + SQR((x(s) - cx) * (x(s) - cx) + (y(s) - cy) * (y(s) - cy))
    cx = x(s): cy = y(s)
  NEXT
  FOR s = 0 TO ns
    IF fl = 1 THEN
      x2(s) = x(s)
      y2(s) = y(s)
    ELSE
      READ x2(s), y2(s)
      x2(s) = x2(s) * gsf
      y2(s) = y2(s) * gsf
    END IF
  NEXT
  nt = INT((pl / (sf * ht)) + .5)
  cx = sx: cy = sy
  ln = 0
  FOR s = 1 TO ns
    ln = ln + SQR((x(s) - cx) * (x(s) - cx) + (y(s) - cy) * (y(s) - cy))
    cx = x(s): cy = y(s)
    u(s) = (ln / pl) * nt
  NEXT
  u(ns) = nt
  n$ = "Path" + RIGHT$(STR$(c), LEN(STR$(c)) - 1)
  PRINT #1, "Mesh msh"; n$
  PRINT #1, "{"
  PRINT #1, "  "; RIGHT$(STR$(ns * 4), LEN(STR$(ns * 4)) - 1); ";"
  FOR s = 1 TO ns
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^, +#.#######^^^^;,"; x(s - 1); sz; y(s - 1)
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^, +#.#######^^^^;,"; x2(s - 1); sz + ht; y2(s - 1)
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^, +#.#######^^^^;,"; x2(s); sz + ht; y2(s)
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^, +#.#######^^^^;"; x(s); sz; y(s);
    IF s <> ns THEN PRINT #1, "," ELSE PRINT #1, ";"
  NEXT
  PRINT #1, ""
  PRINT #1, "  "; RIGHT$(STR$(ns), LEN(STR$(ns)) - 1); ";"
  FOR s = 1 TO ns
    PRINT #1, "  4;"; STR$(((s - 1) * 4)); ","; STR$(((s - 1) * 4) + 1); ","; STR$(((s - 1) * 4) + 2); ","; STR$(((s - 1) * 4) + 3); ";";
    IF s <> ns THEN PRINT #1, "," ELSE PRINT #1, ";"
  NEXT
  PRINT #1, ""
  PRINT #1, "  MeshMaterialList"
  PRINT #1, "  {"
  PRINT #1, "    1;"
  PRINT #1, "    "; RIGHT$(STR$(ns), LEN(STR$(ns)) - 1); ";"
  FOR s = 1 TO ns
    PRINT #1, "    0";
    IF s <> ns THEN PRINT #1, "," ELSE PRINT #1, ";"
  NEXT
  PRINT #1, "    { mat"; tn$; " }"
  PRINT #1, "  }"
  PRINT #1, ""
  PRINT #1, "  MeshTextureCoords"
  PRINT #1, "  {"
  PRINT #1, "  "; RIGHT$(STR$(ns * 4), LEN(STR$(ns * 4)) - 1); ";"
  FOR s = 1 TO ns
    u1 = u(s - 1)
    u2 = u(s)
    um = (u1 + u2) / 2
    DO WHILE um <= 0
      u1 = u1 + 1
      u2 = u2 + 1
      um = um + 1
    LOOP
    DO WHILE um >= 1
      u1 = u1 - 1
      u2 = u2 - 1
      um = um - 1
    LOOP
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^;,"; u1; .995
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^;,"; u1; .005
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^;,"; u2; .005
    PRINT #1, USING "  +#.#######^^^^, +#.#######^^^^;"; u2; .995;
    IF s <> ns THEN PRINT #1, "," ELSE PRINT #1, ";"
  NEXT
  PRINT #1, "  }"
  PRINT #1, "}"
  PRINT #1, ""
NEXT

'close output file
CLOSE #1

