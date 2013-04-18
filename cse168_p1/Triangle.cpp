#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"


Triangle::Triangle(TriangleMesh * m, unsigned int i) :
    m_mesh(m), m_index(i)
{

}


Triangle::~Triangle()
{

}


void
Triangle::renderGL()
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    glBegin(GL_TRIANGLES);
        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}



bool
Triangle::intersect(HitInfo& result, const Ray& ray,float tMin, float tMax)
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & vA = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & vB = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & vC = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	//triangle mesh's normal
	Vector3 vN = cross(vB-vA,vC-vA); 
	vN.normalize();

	//t when ray intersects with triangle's plane
	result.t= (dot(vN,vA-ray.o))/(dot(ray.d,vN));

	//hitpoint
	Vector3 p=ray.o+result.t*ray.d;


//using Christer Ericson's barycentric algorithm (u,v,w) for (alpha,beta,gamma) respectively
    Vector3 v0 = vB - vA; 
	Vector3 v1 = vC - vA;
	Vector3 v2 = p - vA;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
	float beta = (d11 * d20 - d01 * d21) / denom;
    float gamma = (d00 * d21 - d01 * d20) / denom;
    float alpha = 1.0f - beta - gamma;

	//computer barycentric coordinates alpha,beta,gamma
	if(alpha > 1 || alpha < 0 || 
		beta > 1 || beta < 0){
			//printf("--no hit\n");
			return false;}
	if((alpha+beta)>1){
		//printf("no hit---\n");
		return false;}

	if(result.t <tMin || result.t>tMax)
		return false;

	result.P=p;
	result.N=vN;
	result.material=this->m_material;
	return true;
}