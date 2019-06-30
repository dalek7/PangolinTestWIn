#ifndef __mPlane__
#define __mPlane__

// 2013/6/19
// Seung-Chan Kim @ KAIST

#include "Vectors.h"
#include "ttMath.h"
class mPlane
{
public:
	//mPlane( Vector3 &normal,  Vector3 &pt);
	//mPlane( Vector3 &pt1,  Vector3 &pt2,  Vector3 &pt3);
	mPlane(float a = 0.0,
                      float b = 1.0, 
                      float c = 0.0, 
                      float d = 0.0);



public:
	float a() {return m_normal.x;}
	float b() {return m_normal.y;}
	float c() {return m_normal.z;}
	float d() {return m_d;}

	Vector3 &normal() { return m_normal; }

public:

		

	Vector3 pointOnPlane() { return m_normal * -m_d; }  //"arbitrary" point
	


	void	GetFourPoints(Vector3 &p1,Vector3 &p2,Vector3 &p3,Vector3 &p4, float w=1.0, float h=1.0);


private:

    Vector3 m_normal; // <a, b, c>
    float m_d;


};



#endif