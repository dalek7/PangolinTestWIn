#include "mPlane.h"


mPlane::mPlane(float a, float b, float c, float d) : 
				m_normal(a, b, c),
				m_d(d)
{


	float length = m_normal.length();

	if (length > 0.0)
    {
        m_d /= length;
        //m_normal /= length;
		m_normal = m_normal.normalize();

    }

	
}

void mPlane::GetFourPoints(Vector3 &p1,Vector3 &p2,Vector3 &p3,Vector3 &p4, float w, float h)
{
	
	Vector3 u,v;
	Vector3 tmp = normal() - Vector3(0,0,1);


	if (IsEqual<float>(tmp.length(), 0.0, 0.001))
    {        
        u=Vector3(1,0,0);
		v=Vector3(0,1,0);
    }
	else
	{
		Vector3 p1n = normal();
		u = p1n * Vector3(0,0,1);
		v = p1n * u;

		u= u.normalize();
		v= v.normalize();
	}



	//// "arbitrary" point
	Vector3 p0 = pointOnPlane();

	Vector3 fu =  u * w;
	Vector3 fv =  v * h;

	p1 = p0 - fu - fv;
	p2 = p0 + fu - fv;
	p3 = p0 + fu + fv;
	p4 = p0 - fu + fv;


}