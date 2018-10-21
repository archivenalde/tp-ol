Maximize
 obj: 287 a1A + 215 a1B + 228 a2 + 292 a3 + 204 a4 +
      207 b1A + 135 b1B + 148 b2 + 212 b3 + 148 b4 +
      157 c1A + 85  c1B + 98  c2 + 162 c3 + 112 c4 +
      487 d1A + 415 d1B                   + 371 d4 +
      337 g1A + 265 g1B                   + 264 g4 +
      87  f1A + 15  f1B                   + 61  f4
Subject To
 c1: a1A + a1B + a2 +a3+
     b1A + b1B + b2 + b3+
     c1A + c1B + c2 + c3+
     d1A + d1B +
     g1A + g1B +
     f1A + f1B
     <= 5000

 c2: d1A + d1B + d4 <= 3845

 c3: 2000 a1A + 2000 a1B + 2000 a2 + 2000 a3 +
     1200 b1A + 1200 b1B + 1200 b2 + 1200 b3 +
     700 c1A + 700 c1B + 700 c2 + 700 c3
     <= 2440000

 c4:   4000 d1A + 4000 d1B +
     2500 g1A + 2500 g1B
     <= 4160000

 c5: a1A + a1B + a2 + a3 + a4 <= 2754
 c6: a2 <= 357
 c7: a3 <= 500

 c8: b1A + b1B + b2 + b3 + b4 <= 850
 c9: b2 <= 197
 c10: b3 <= 130

 c11: c1A + c1B + c2 + c3 + c4 <= 855
 c12: c2 <= 39
 c13: c3 <= 170

 c14: d1A + d1B + d4 <= 1598
 c15: g1A + g1B + g4 <= 405
 c16: f1A + f1B + f4 <= 1761

 c17 : a1B + b1B + c1B + d1B + g1B + f1B => 500

Bounds

End
