#include "integrator.h"
#include "scene.h"
#include "material.h"
#include "warp.h"

class DirectMats : public Integrator
{
public:
    DirectMats(const PropertyList &props) {

        m_sampleCount = props.getInteger("samples",4);
        m_IS = props.getBoolean("IS", false);
        
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        Hit hit;
        scene->intersect(ray, hit);

        if (hit.foundIntersection()){
            
            Normal3f normal = hit.normal();

            const Material* material = hit.shape()->material();
            
            for(int i=0; i<m_sampleCount; ++i){
                
                float pdf;
                Vector3f r = material->us(normal, -ray.direction, pdf);
                
                if(normal.dot(r)<0){
                    r=-r;
                }
                
                Color3f color_bg = scene->backgroundColor(r);
                float cos = std::max(0.f,r.dot(normal));
                Color3f BRDF = material->premultBrdf(-ray.direction, r, normal, hit.texcoord());
                
                if(pdf>0){
                    radiance += (color_bg * BRDF * cos) / pdf;
                }
            }
            
        }else{
            return scene->backgroundColor(ray.direction);
        }
         return radiance / m_sampleCount;
    }

    std::string toString() const {
        return tfm::format("DirectMats[\n"
                           "  samples = %f\n"
                           "  IS = %s\n"
                           " ]\n",
                           m_sampleCount,
                           m_IS ? "true" : "false");
    }

private:
    int m_sampleCount;
    bool m_IS;
};

REGISTER_CLASS(DirectMats, "direct_mats")