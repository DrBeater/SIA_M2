/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "integrator.h"
#include "material.h"
#include "scene.h"
#include "photon.h"
#include "areaLight.h"
#include <memory>

class PhotonMapper : public Integrator
{
public:
    /// Photon map data structure
    typedef PointKDTree<Photon> PhotonMap;

    PhotonMapper(const PropertyList &props)
    {
        /* Lookup parameters */
        m_photonCount  = props.getInteger("photonCount", 1000000);
        m_photonRadius = props.getFloat("photonRadius", 0.0f /* Default: automatic */);
    }

    void preprocess(const Scene *scene)
    {
        std::cout << "preprocess" << std::endl;

        /* Allocate memory for the photon map */
        m_photonMap = std::unique_ptr<PhotonMap>(new PhotonMap());
        m_photonMap->reserve(m_photonCount);

        /* Estimate a default photon radius */
        if (m_photonRadius == 0) {
            Eigen::AlignedBox3f aabb;
            for(unsigned int i=0; i<scene->shapeList().size(); ++i) {
                aabb.extend(scene->shapeList()[i]->AABB());
            }
            m_photonRadius = (aabb.max()-aabb.min()).norm() / 500.0f;
        }

        /* Trace photons */
        m_nPaths = 0;

        while((int) m_photonMap->size() < m_photonCount) {
            m_nPaths++;
            Photon photon = scene->samplePhoton();

            while(photon.getPower().mean() > 0) 
            {
                Ray ray(photon.getPosition(), photon.getDirection());
                Hit hit;
                scene->intersect(ray, hit);

                if (!hit.foundIntersection())
                    break;

                Normal3f normal = hit.normal();
                Color3f power = photon.getPower();
                const Material* material = hit.shape()->material();

                Vector3f r;

                // reflexions
                if((material->reflectivity() > 1e-6).any())
                { 
                    r = (ray.direction - 2.*ray.direction.dot(hit.normal())*hit.normal()).normalized();
                    float cos_term = std::max(0.f,r.dot(normal));
                    power *= material->reflectivity()*cos_term;
                } 

                // refraction
                if((material->transmissivness() > 1e-6).any()) 
                { 
                    float etaA = material->etaA(), etaB = material->etaB();
                    bool entering = -normal.dot(ray.direction) > 0;
                    if(!entering) {
                        std::swap(etaA,etaB);
                        normal = -normal;
                    }
                    if(refract(normal,ray.direction,etaA,etaB,r)) {
                       float cos_term = std::max(0.f,-r.dot(normal));
                       power *= material->transmissivness() * cos_term;
                    }                   
                } 
                
                else {
                    float pdf = 0.;
                    r = material->is(hit.normal(), -ray.direction, pdf);
                    power *= material->premultBrdf(-ray.direction, r, hit.normal(), hit.texcoord());    
                }

                Point3f pos = ray.at(hit.t());
                Photon photon1(pos, -ray.direction, photon.getPower());
                m_photonMap->push_back(photon1);

                double ratio = power.mean() / photon.getPower().mean();
                double rouletteRusse = Eigen::internal::random<double>(0,1);

                if (ratio < rouletteRusse) 
                    break;  
                
                power *= ratio ;
                photon = Photon(pos, r, power);
            } 
        }

        /* Build the photon map */
        m_photonMap->build();

        /* Now let's do a lookup to see if it worked */
        std::vector<uint32_t> results;
        m_photonMap->search(Point3f(0, 0, 0) /* lookup position */, m_photonRadius, results);

        for (uint32_t i : results) {
            const Photon &photon = (*m_photonMap)[i];
            cout << "Found photon!" << endl;
            cout << " Position  : " << photon.getPosition().toString() << endl;
            cout << " Power     : " << photon.getPower().toString() << endl;
            cout << " Direction : " << photon.getDirection().toString() << endl;
        }
    }

    Color3f Li(const Scene *scene, const Ray & ray) const{

        Color3f radiance = Color3f::Zero();

        // stopping criteria:
        if(ray.recursionLevel>=m_maxRecursion) {
            return radiance;
        }

        /* Find the surface that is visible in the requested direction */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            Normal3f normal = hit.normal();
            Point3f pos = ray.at(hit.t());

            const Material* material = hit.shape()->material();

            // For shapes emitting light (an area light is placed aside them).
            if (hit.shape()->isEmissive()) {
                const AreaLight *light = static_cast<const AreaLight*>(hit.shape()->light());
                return std::max(0.f, normal.dot(-ray.direction)) * light->intensity(pos + light->direction(), pos);
            }

            bool is_phong = dynamic_cast<const Phong*>(material) ;
            bool is_ward = dynamic_cast<const Ward*>(material) ;

            if (!(is_phong || is_ward)) {
                std::vector<uint32_t> nearIntersect;
                m_photonMap->search(pos, m_photonRadius, nearIntersect);

                for (size_t i=0 ; i < nearIntersect.size() ; i++){
                    Photon photon = (*m_photonMap)[nearIntersect[i]];
                    Vector3f x = (pos-photon.getPosition()) / m_photonRadius;

                    double kernel = 0 ;
                    if (x.norm()<=1)
                        kernel = (3./M_PI) * pow(1.-x.squaredNorm(),2);                        
                    radiance += kernel * photon.getPower() * material->premultBrdf(-ray.direction, photon.getDirection(), normal, hit.texcoord());
                }
                
                radiance /= pow(m_photonRadius,2) * m_nPaths ;
                return radiance ;
            }

            const LightList &lights = scene->lightList();
            for(LightList::const_iterator it=lights.begin(); it!=lights.end(); ++it)
            {
                Vector3f lightDir;
                float dist;
                const AreaLight* aLight = dynamic_cast<const AreaLight*>(*it);
                Point3f y;
                if(aLight) {
                    y = aLight->position()
                              + aLight->size()[0]*aLight->uVec() * Eigen::internal::random<float>(-0.5,0.5)
                              + aLight->size()[1]*aLight->vVec() * Eigen::internal::random<float>(-0.5,0.5);

                    dist = (pos-y).norm();
                    lightDir = (y-pos).normalized();
                }else{
                    lightDir = (*it)->direction(pos, &dist);
                }
                Ray shadow_ray(pos + normal*Epsilon, lightDir, true);
                Hit shadow_hit;
                scene->intersect(shadow_ray,shadow_hit);
                Color3f attenuation = Color3f(1.f);
                if(shadow_hit.t()<dist){
                    if(!shadow_hit.shape()->isEmissive())
                        attenuation = 0.5f * shadow_hit.shape()->material()->transmissivness();
                    if((attenuation <= Epsilon).all())
                        continue;
                }

                float cos_term = std::max(0.f,lightDir.dot(normal));
                Color3f brdf = material->brdf(-ray.direction, lightDir, normal, hit.texcoord());
                if(aLight) {
                    float inv_pdf = aLight->size().x() * aLight->size().y();
                    radiance += aLight->intensity(pos,y) * cos_term * brdf * attenuation * inv_pdf;
                } else
                    radiance += (*it)->intensity(pos) * cos_term * brdf * attenuation;
            }

            Color3f color = material->ambientColor();
            float albedo = (color.r() + color.g() + color.b()) / 3.f;
            if (albedo  > Eigen::internal::random<float>(0, 1) || ray.recursionLevel <= 3) {
                Color3f indirect = Color3f::Zero();
                float pdf = 0.f;
                Vector3f r;
                if(m_IS) {
                    r = material->is(normal, -ray.direction, pdf);
                }else{
                    r = material->us(normal, -ray.direction, pdf);
                }
                if(normal.dot(r)<0)
                    r=-r;
                Ray reflexion_ray(pos + hit.normal()*Epsilon, r);
                reflexion_ray.recursionLevel = ray.recursionLevel + 1;
                Color3f brdf = material->brdf(-ray.direction, r, normal, hit.texcoord());
                float cos_term = std::max(0.f,r.dot(normal));
                indirect = (Li(scene, reflexion_ray) * cos_term * brdf);
                if(pdf>Epsilon) {
                    indirect /= pdf;

                    if(albedo > Epsilon && ray.recursionLevel > 3)
                        indirect /= albedo;
                    radiance += indirect;
                }
            }

            // reflexions
            if((material->reflectivity() > 1e-6).any())
            {
                Vector3f r = (ray.direction - 2.*ray.direction.dot(hit.normal())*hit.normal()).normalized();
                Ray reflexion_ray(pos + hit.normal()*Epsilon, r);
                reflexion_ray.recursionLevel = ray.recursionLevel + 1;
                float cos_term = std::max(0.f,r.dot(normal));
                radiance += material->reflectivity() * Li(scene, reflexion_ray) * cos_term;
            }

            // refraction
            if((material->transmissivness() > 1e-6).any())
            {
                float etaA = material->etaA(), etaB = material->etaB();
                bool entering = -normal.dot(ray.direction) > 0;
                if(!entering) {
                    std::swap(etaA,etaB);
                    normal = -normal;
                }
                Vector3f r;
                if(refract(normal,ray.direction,etaA,etaB,r)) {
                    Ray refraction_ray(pos - normal*Epsilon, r);
                    refraction_ray.recursionLevel = ray.recursionLevel + 1;
                    float cos_term = std::max(0.f,-r.dot(normal));
                    radiance += material->transmissivness() * Li(scene, refraction_ray) * cos_term;
                }
            }

        }else if(ray.recursionLevel == 0)
            return scene->backgroundColor();

        return radiance;
    }

    std::string toString() const
    {
        return tfm::format(
            "PhotonMapper[\n"
            "  photonCount = %i,\n"
            "  photonRadius = %f\n"
            "]",
            m_photonCount,
            m_photonRadius
        );
    }
private:
    int m_photonCount, m_nPaths, m_maxRecursion;
    bool m_IS;
    float m_photonRadius;
    std::unique_ptr<PhotonMap> m_photonMap;
};

REGISTER_CLASS(PhotonMapper, "photonmapper")
