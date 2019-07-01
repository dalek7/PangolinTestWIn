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

void mPlane::DetectPlane(std::vector<Vector3> vpts)
{
	if (vpts.size() < 10)
		return;


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



void mPlane::plane(Vector3 &p0, Vector3 &p1, Vector3 &p2)
{
	// 3개의 점이 주어지면 평면의 방정식을 찾는다.
	double x1 = p1.x - p0.x;
	double y1 = p1.y - p0.y;
	double z1 = p1.z - p0.z;

	double x2 = p2.x - p0.x;
	double y2 = p2.y - p0.y;
	double z2 = p2.z - p0.z;

	double x3 = y1*z2 - z1*y2;
	double y3 = z1*x2 - x1*z2;
	double z3 = x1*y2 - y1*x2;

	double l = sqrt(x3*x3 + y3*y3 + z3*z3);

	double a = x3 / l;
	double b = y3 / l;
	double c = z3 / l;
	double d = -(a*p0.x + b*p0.y + c*p0.z);

	m_normal.x = a;
	m_normal.y = b;
	m_normal.z = c;
	m_d = d;
}


double mPlane::length(Vector3 &pt)
{
	double a = m_normal.x;
	double b = m_normal.y;
	double c = m_normal.z;
	double d = m_d;

	// 점 pt로부터 평면에 내린 수선의 길이를 계산한다.
	double k = (a*pt.x + b*pt.y + c*pt.z + d) / (a*a + b*b + c*c);
	double ka = k*a, kb = k*b, kc = k*c;
	return sqrt(ka*ka + kb*kb + kc*kc);
}