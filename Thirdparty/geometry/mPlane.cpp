#include "mPlane.h"
void get_samples(std::vector<Vector3>& samples, int no_samples, std::vector<Vector3> data, int no_data);
bool find_in_samples(std::vector<Vector3> samples, int no_samples_to_search, Vector3 data);
bool compute_model_parameter(std::vector<Vector3> samples, int no_samples, mPlane &model);
double model_verification(std::vector<Vector3> &inliers, std::vector<unsigned int>& vidx_inliers, mPlane estimated_model, std::vector<Vector3> data, int no_data, double distance_threshold);

using namespace std;
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

void mPlane::Desc()
{
	std::cout << m_normal.x << "x +" << m_normal.y << "y+ " << m_normal.z << "z + " <<m_d << std::endl;
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
			
double mPlane::	ransac_plane_fitting(std::vector<Vector3> vpts, std::vector<unsigned int>& vidx_inliers, std::vector<Vector3>& vpts_inliers, mPlane &model, double distance_threshold)
{
	int no_data = vpts.size();
	const int no_samples = 3;

	if (no_data < no_samples) {
		return 0.;
	}

	//sPoint *samples = new sPoint[no_samples];
	std::vector<Vector3> samples(no_samples);
	//samples.reserve(no_samples);

	
	std::vector<Vector3> inliers;// (no_data);
	//inliers.reserve(no_data);
	
	mPlane estimated_model;
	mPlane model_best;
	double max_nb_valid= 0.;

	int max_iteration = (int)(1 + log(1. - 0.99) / log(1. - pow(0.5, no_samples)));

	for (int i = 0; i < max_iteration; i++) {
		// 1. hypothesis

		// 원본 데이터에서 임의로 N개의 셈플 데이터를 고른다.
		get_samples(samples, no_samples, vpts, no_data);

		// 이 데이터를 정상적인 데이터로 보고 모델 파라메터를 예측한다.
		compute_model_parameter(samples, no_samples, estimated_model);
		// if (!estimated_model.convert_std_form ()) { --i; continue; }

		// 2. Verification

		// 원본 데이터가 예측된 모델에 잘 맞는지 검사한다.
		std::vector<unsigned int> vidx;
		double nb_valid = model_verification(inliers, vidx, estimated_model, vpts, no_data, distance_threshold);
		std::cout << i << "/" << max_iteration << "\t" << nb_valid << "/" << no_data << "  "; 
		// 만일 예측된 모델이 잘 맞는다면, 이 모델에 대한 유효한 데이터로 새로운 모델을 구한다.
		if (max_nb_valid < nb_valid) {
			max_nb_valid = nb_valid;

			vpts_inliers = inliers;
			vidx_inliers = vidx;
			int no_inliers = inliers.size();
			compute_model_parameter(inliers, no_inliers, model_best); // no_inliers > 3
			cout << "++++++";
			
			
			// model.convert_std_form ();
		}

		cout << std::endl;
	}

	model = model_best;

	return 0;
}


double model_verification(std::vector<Vector3> &inliers, std::vector<unsigned int>& vidx_inliers, mPlane estimated_model, std::vector<Vector3> data, int no_data, double distance_threshold)
{
	//no_inliers = 0;
	inliers.clear();
	vidx_inliers.clear();
	double nb_valid = 0.;

	for (int i = 0; i<no_data; i++) {
		// 직선에 내린 수선의 길이를 계산한다.
		double distance = estimated_model.length(data[i]);

		// 예측된 모델에서 유효한 데이터인 경우, 유효한 데이터 집합에 더한다.
		if (distance < distance_threshold) {
			vidx_inliers.push_back(i); 
			nb_valid += 1.;
			inliers.push_back(data[i]);
		}
	}
	return nb_valid;
}


bool find_in_samples(std::vector<Vector3> samples, int no_samples_to_search, Vector3 data)
{
	//int no_samples = samples.size();
	for (int i = 0; i<no_samples_to_search; ++i) {
		if (samples[i].x == data.x && samples[i].y == data.y && samples[i].z == data.z) {
			return true;
		}
	}
	return false;
}
//void get_samples(sPoint *samples, int no_samples, sPoint *data, int no_data)
void get_samples(std::vector<Vector3>& samples, int no_samples, std::vector<Vector3> data, int no_data)
{
	// 데이터에서 중복되지 않게 N개의 무작위 셈플을 채취한다.
	//int no_samples = samples.size();
	//int no_data = data.size();

	for (int i = 0; i<no_samples; ) {
		int j = rand() % no_data;

		if (!find_in_samples(samples, i, data[j])) {
			samples[i] = data[j];
			++i;
		}
	}
}

#include "MatrixAlgebra.h"
bool compute_model_parameter(std::vector<Vector3> samples, int no_samples, mPlane &model)
{
	if (no_samples < 3) {
		return false;
	}
	else if (no_samples == 3) {
		model.plane(samples[0], samples[1], samples[2]);
		
		return true;
	}
	else {
		
		// 점이 4개 이상일 때는 least square 방법으로 평면의 방정식을 찾는다.
		// 평면의 방정식이 ax + bx + cz + d = 0 형태이기 때문에
		// (x1, y1, z1), (x2, y2, z2), (x3, y3, z3), ... 점들을 대입하여 행렬 형태로 정리하면 다음과 같은 모양이 된다:
		//   [ x1 y1 z1  1]    [ a ]   [ 0   ] 
		//   [ x2 y2 z2  1] *  [ b ] = [ 0   ]
		//   [ x3 y3 z3  1]    [ c ]   [ 0   ]
		//   [ ...       1]    [ d ]   [ ... ]
		
		dMatrix A(no_samples, 4);
		
		for (int i = 0; i<no_samples; i++) {
			A(i, 0) = samples[i].x;
			A(i, 1) = samples[i].y;
			A(i, 2) = samples[i].z;
			A(i, 3) = 1.;
		}
	
		// 동차방정식 Ax=0 형태의 해를 SVD로 구한다.
		dMatrix V;
		dVector D;
		A.svd(V, D);
		
		// 여기서 해는 V의 마지막 열 벡터가 된다.
		/*
		model.a = V(0, 3);
		model.b = V(1, 3);
		model.c = V(2, 3);
		model.d = V(3, 3);
		*/
		model = mPlane(V(0, 3), V(1, 3), V(2, 3), V(3, 3)); // <a, b, c>

		return true;
		
	}
}


/*

double ransac_plane_fitting (sPoint *data, int no_data, sPlane &model, double distance_threshold)
{
const int no_samples = 3;

if (no_data < no_samples) {
return 0.;
}

sPoint *samples = new sPoint[no_samples];

int no_inliers = 0;
sPoint *inliers = new sPoint[no_data];

sPlane estimated_model;
double max_cost = 0.;

int max_iteration = (int)(1 + log(1. - 0.99)/log(1. - pow(0.5, no_samples)));

for (int i = 0; i<max_iteration; i++) {
// 1. hypothesis

// 원본 데이터에서 임의로 N개의 셈플 데이터를 고른다.
get_samples (samples, no_samples, data, no_data);

// 이 데이터를 정상적인 데이터로 보고 모델 파라메터를 예측한다.
compute_model_parameter (samples, no_samples, estimated_model);
// if (!estimated_model.convert_std_form ()) { --i; continue; }

// 2. Verification

// 원본 데이터가 예측된 모델에 잘 맞는지 검사한다.
double cost = model_verification (inliers, &no_inliers, estimated_model, data, no_data, distance_threshold);

// 만일 예측된 모델이 잘 맞는다면, 이 모델에 대한 유효한 데이터로 새로운 모델을 구한다.
if (max_cost < cost) {
max_cost = cost;

compute_model_parameter (inliers, no_inliers, model);
// model.convert_std_form ();
}
}

delete [] samples;
delete [] inliers;

return max_cost;
}

*/