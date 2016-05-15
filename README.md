# unitvectorpack

Efficient way to store 3D unit vectors (such as normals) into integers of desired size. Even 16 bits is enough for many applications. 

The idea:
 - 3 bits are reserved for signs of x,y and z. This leaves us 7/6 (in the 16 bit case) or 15/14 (in the 32 bit case) for x and y components. Because we are dealing with unit vectors, there is no need to store any other information about the Z component than its sign.
 - Transform the vector to the octant with x>=0, y>=, z>=0.
 - Further transform the vector to the plane that passes through points (1,0,0),(0,1,0) and (0,0,1). Now x<=1,y<=1, x+y <= 1. Thus, (x,y) is inside the triangle defined by points (1,0),(0,1) and (0,0).
 - If y >= 0.5, then we transform x and y by the function (x,y) => (1-x,1-y). After this y is again less than 0.5. This gives us one extra bit of precision. Because this transform moves the point out of the triangle defined by points (1,0),(0,1) and (0,0), we can detect that the transform has been applied when we unpack the integer. 
 
Example:
 
    Vector3<float> v = random_unit_vector();
    uint16_t vi = packUnitVector<uint16_t, float>(v);
    Vector3<float> v2 = unpackUnitVector<Vector3<float>,float>(vi);
    float error = v1.distanceTo(v2); // Quite small!
    
You can pass unnormalized vectors to packUnitVector. When the resulting integers are unpacked, you get unit vectors pointing to (approximately) the same direction as the original vector.