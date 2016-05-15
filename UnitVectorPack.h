/*
 
 UnitVectorPack.h
 
Pack unit vectors such as normal vectors into integer types.

 The template assumes that your vector's components can be accessed using the member variables x,y and z.
 This code is PUBLIC DOMAIN.
 
 Example usage:
 
	Vector3<float> v = random_unit_vector();
	uint16_t vi = packUnitVector<uint16_t, float>(v);
	Vector3<float> v2 = unpackUnitVector<Vector3<float>,float>(vi);
	float error = v1.distanceTo(v2); // Quite small!
 
*/

#include <cassert>

template<typename IntegerType, typename FloatType, typename VectorType, size_t TotalBits>
IntegerType packUnitVector(VectorType v) {
	static_assert(TotalBits % 2 == 0 && TotalBits >= 6 && TotalBits <= 64 && TotalBits <= sizeof(IntegerType)*8, "Incorrect TotalBits.");
	
	IntegerType r = 0;
	
	// First handle sign bits. Sign bit will be 1 if the component is >= 0. Also make the components positive now, if they were negative.
	r |= v.x >= 0 ? (1<<(TotalBits - 0 - 1)) : (v.x = -v.x, 0);
	r |= v.y >= 0 ? (1<<(TotalBits - 1 - 1)) : (v.y = -v.y, 0);
	r |= v.z >= 0 ? (1<<(TotalBits - 2 - 1)) : (v.z = -v.z, 0);
	
	// Project the vector to the plane that passes through standard basis vectors
	const FloatType s = v.x+v.y+v.z;
	const FloatType M = 1/s;
	v.x *= M;
	v.y *= M;
	v.z *= M;
	
	constexpr size_t XBitCount = (TotalBits-3)/2+1;
	constexpr size_t YBitCount = (TotalBits-3)/2;
	
	static_assert(XBitCount + YBitCount == TotalBits - 3, "");
	
	constexpr size_t XRange = (1<<XBitCount) - 1;
	constexpr size_t YRange = (1<<(XBitCount-1)) - 1;
	
	IntegerType ix = ( ((FloatType)(XRange-1))*v.x );
	IntegerType iy = ( ((FloatType)(XRange-1))*v.y );
	
	assert(ix <= XRange);
	assert(iy <= XRange);
	
	if (iy >= (1<<XBitCount)/2) {
		ix = XRange - ix;
		iy = XRange - iy;
	}
	
	assert(ix <= XRange);
	assert(iy <= YRange);
	
	r |= (ix<<(YBitCount)) | iy;
	
	return r;
}

template<typename VectorType, typename FloatType, typename IntegerType, size_t TotalBits>
VectorType unpackUnitVector(IntegerType vi) {
	static_assert(TotalBits % 2 == 0 && TotalBits >= 6 && TotalBits <= 64 && TotalBits <= sizeof(IntegerType)*8, "Incorrect TotalBits.");
	
	VectorType v = {(vi & (1<<(TotalBits-0-1))) != 0 ? 1.0f : -1.0f,(vi & (1<<(TotalBits-1-1))) != 0 ? 1.0f : -1.0f,(vi & (1<<(TotalBits-2-1))) != 0 ? 1.0f : -1.0f};
	
	constexpr size_t XBitCount = (TotalBits-3)/2+1;
	constexpr size_t YBitCount = (TotalBits-3)/2;
	constexpr size_t XRange = (1<<XBitCount) - 1;
	constexpr size_t YRange = (1<<(XBitCount-1)) - 1;
	
	IntegerType XBits = (vi&(XRange<<YBitCount)) >> YBitCount;
	IntegerType YBits = vi&YRange;
	
	if (XBits + YBits > XRange) {
		XBits = XRange - XBits;
		YBits = XRange - YBits;
	}
	
	v.x *= XBits/(FloatType)XRange;
	v.y *= YBits/(FloatType)XRange;
	v.z *= 1 - std::abs(v.x) - std::abs(v.y);
	
	const FloatType M = 1/std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	v.x *= M;
	v.y *= M;
	v.z *= M;
	
	return v;
}

template<typename VectorType, typename FloatType, typename IntegerType>
VectorType unpackUnitVector(IntegerType vi) {
	constexpr size_t TotalBits = sizeof(IntegerType)*8;
	return unpackUnitVector<VectorType,FloatType,IntegerType,TotalBits>(vi);
}

template<typename IntegerType, typename FloatType, typename VectorType>
IntegerType packUnitVector(VectorType v) {
	constexpr size_t TotalBits = sizeof(IntegerType)*8;
	return packUnitVector<IntegerType, FloatType, VectorType, TotalBits>(v);
}