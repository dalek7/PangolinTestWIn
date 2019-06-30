/*
 *  ddMath2.h
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  hduMath.h
 */

#ifndef TT_MATH2_H
#define TT_MATH2_H

#ifdef __cplusplus



template <class T>
inline void ShiftWithNew(T* arr, const T &n_val, int sz)
{

	int i;
	for(i=0; i< sz-1; i++)
	{
		arr[sz-1-i] = arr[sz-2-i];
		
	}
	arr[0] = n_val;

    //if (val < 0) return -val;
    
}

// added 2013/2/16 @CHAUD 1186
// from hduMath.h

template <class T>
inline T AbsValue(const T &val)
{
    if (val < 0) return -val;
    return val;
}


template <class T>
T EvalLine(const T &p0, const T &p1, typename T::EltType t)
{
    return p0 + (p1 - p0) * t;
}

template <class T>
T Lerp(const T &v0, const T &v1, double t)
{
    return v0 * (1.0 - t) + v1 * t;
}



template <class T>
T Clamp(const T &val, const T &MIN, const T &MAX)
{
    if (val < MIN) return MIN;
    if (val > MAX) return MAX;
    return val;
}

template <class T>
T Round(const T &val)
{
    return floor(val + 0.5);
}


template <class T>
bool IsEqual(const T &v0, const T &v1, const T &epsilon)
{
    return (AbsValue(v0 - v1) <= epsilon);
}

#endif /* __cplusplus */

#endif