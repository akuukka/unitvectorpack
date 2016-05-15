# unitvectorpack

The idea:
 - 3 bits are reserved for signs of x,y and z. This leaves us 7/6 (in the 16 bit case) or 15/14 (in the 32 bit case) for X and Y components. Because we are dealing with unit vectors, there is no need to store any other information about the Z component than its sign.
 - Make all components positive.
 - Transform the vector to the plane that passes through points (1,0,0),(0,1,0) and (0,0,1). Now x<=1,y<=1, x+y <= 1. Thus, (x,y) is inside the triangle defined by points (1,0),(0,1) and (0,0).
 - If Y >= 0.5, then we transform x and y by the function (x,y) => (1-x,1-y). After this y is again less than 0.5. This gives us one extra bit of precision. Because this transform moves the point out of the triangle defined by points (1,0),(0,1) and (0,0), we can detect that the transform has been done when unpacking the integer. 