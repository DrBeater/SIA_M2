#include "integrator.h"
#include "scene.h"
#include "material.h"
#include "warp.h"

class AO : public Integrator
{
public:
    AO(const PropertyList &props) {
        m_sampleCount = props.getInteger("samples", 10);
        m_cosineWeighted = props.getBoolean("cosine", true);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const
    {
        Color3f radiance = Color3f::Zero();

        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            Normal3f normal = hit.normal();
            Point3f pos = ray.at(hit.t());

            float visibility = 0.0f;
            for (int i = 0; i < m_sampleCount; ++i)
            {
                Point2f sample = Point2f(Eigen::internal::random<float>(0, 1),
                                         Eigen::internal::random<float>(0, 1));

                Vector3f d = (m_cosineWeighted ? Warp::squareToCosineHemisphere(sample) : Warp::squareToUniformHemisphere(sample));

                Vector3f u = normal.cross(Vector3f(1.0f, 0.0f, 0.0f));
                Vector3f v = u.cross(normal);

                Vector3f p = d.x() * u + d.y() * v + d.z() * normal;

                Ray ao_ray(pos + normal * Epsilon, p, true);
                Hit ao_hit;
                scene->intersect(ao_ray, ao_hit);
                if (!ao_hit.foundIntersection())
                {
                    float cosTheta = std::max(normal.dot(p),0.0f);
                    float pdf = (m_cosineWeighted ? Warp::squareToCosineHemispherePdf(d) : Warp::squareToUniformHemispherePdf(d));
                    visibility += (cosTheta / M_PI) / pdf;
                }
            }

            radiance += Color3f(visibility / m_sampleCount);
        }

        return radiance;
    }


    std::string toString() const {
      return tfm::format("AO[\n"
                         "  samples = %f\n"
                         "  cosine-weighted = %s]\n",
                         m_sampleCount, 
                         m_cosineWeighted ? "true" : "false");
  }

private:
    int m_sampleCount;
    bool m_cosineWeighted; 
};

REGISTER_CLASS(AO, "ao")
