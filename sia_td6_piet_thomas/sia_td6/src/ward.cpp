#include "material.h"

Ward::Ward(const PropertyList &propList)
    : Diffuse(propList.getColor("diffuse",Color3f(0.2)))
{
    m_reflectivity = propList.getColor("reflectivity",Color3f(0.0));
    m_transmissivness = propList.getColor("transmissivness",Color3f(0.0));
    m_etaA = propList.getFloat("etaA",1);
    m_etaB = propList.getFloat("etaB",1);
    m_specularColor = propList.getColor("specular",Color3f(0.9));
    m_alphaX = propList.getFloat("alphaX",0.2);
    m_alphaY = propList.getFloat("alphaY",0.2);

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Ward::brdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{
    Vector3f d(0,1,0);
    Vector3f x = d.cross(normal).normalized();
    Vector3f y = x.cross(normal).normalized();

    Vector3f h = (viewDir + lightDir).normalized();

    //float hx = (h.dot(x) / m_alphaX) * (h.dot(x) / m_alphaX);
    //float hy = (h.dot(y) / m_alphaX) * (h.dot(y) / m_alphaY);

    float hx = (h.dot(x) / m_alphaY) * (h.dot(x) / m_alphaY);
    float hy = (h.dot(y) / m_alphaX) * (h.dot(y) / m_alphaX);

    Color3f fr = ((m_specularColor / (4 * M_PI * m_alphaY * m_alphaY * sqrt(lightDir.dot(normal) * viewDir.dot(normal)))) * exp(-((hx + hy) / pow(h.dot(normal),2)))) + m_diffuseColor/M_PI;

    return fr;

}

Vector3f Ward::is(const Normal3f& n, const Vector3f &i, float& pdf) const
{
    float u = Eigen::internal::random<float>(0,1);
    float v = Eigen::internal::random<float>(0,1);

    Vector3f x = Vector3f(0,1,0) - Vector3f(0,1,0).dot(n)*n;
    x.normalize();
    Vector3f y = n.cross(x);

    float phi_h = atanf((m_alphaY / m_alphaX) * tanf(2 * M_PI * v));

    float thetha_h = atanf(sqrtf(-logf(u) / (pow(cosf(phi_h),2) / pow(m_alphaX,2)) + (pow(sinf(phi_h),2) / pow(m_alphaY,2))));

    Vector3f h = sinf(thetha_h) * cosf(phi_h) * x + sinf(thetha_h) * sinf(phi_h) * y + cosf(thetha_h) * n;

    Vector3f o = 2 * i.dot(h) * h - i;

    return o;
}

Color3f Ward::premultBrdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{

    Vector3f h = (viewDir + lightDir).normalized();
    return m_specularColor * h.dot(lightDir) * h.dot(normal) * h.dot(normal) * h.dot(normal) * sqrt(viewDir.dot(normal) / lightDir.dot(normal));
    
}

std::string Ward::toString() const {
    return tfm::format(
        "Ward [\n" 
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  alphaX = %f  alphaY = %f\n"
        "]", m_diffuseColor.toString(),
             m_specularColor.toString(),
             m_alphaX, m_alphaY);
}

REGISTER_CLASS(Ward, "ward")
