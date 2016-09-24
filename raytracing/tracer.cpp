//
//  tracer.cpp
//  raytracing
//
//  Created by Даня on 26.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#include "tracer.hpp"

Tracer::Tracer(std::vector<Primitive*> &from_objects) {
    objects = from_objects;
}

std::pair<const Primitive*, Vector> Tracer::getIntersectedObjectAndPoint
(const Vector &origin, const Vector &direction) const {
    double tnear = INFINITY;
    const Primitive* intersectedObject = NULL;
    for (unsigned i = 0; i < objects.size(); ++i) {
        double t = INFINITY;
        if (objects[i]->intersect(origin, direction, t)) {
            if (t < tnear) {
                tnear = t;
                intersectedObject = objects[i];
            }
        }
    }
    return std::make_pair(intersectedObject, origin + direction * tnear);
}

Vector Tracer::computeRefraction(const double &eta, const Vector &intersectionPoint, const Vector &intersectionNormal, const Vector &direction, const int &depth) const {
    double cosi = -intersectionNormal.dot(direction);
    double k = 1 - eta * eta * (1 - cosi * cosi);
    Vector refrdir = direction * eta + intersectionNormal * (eta *  cosi - sqrt(k));
    refrdir.normalize();
    return trace(intersectionPoint - intersectionNormal * BIAS, refrdir, depth + 1);
}

std::pair<Vector, double> Tracer::computeReflection(const Vector &intersectionPoint, const Vector &intersectionNormal, const Vector &direction, const int &depth) const {
    double facingratio = -direction.dot(intersectionNormal);
    double fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
    Vector refldir = direction - intersectionNormal * 2 * direction.dot(intersectionNormal);
    refldir.normalize();
    return std::make_pair(trace(intersectionPoint + intersectionNormal * BIAS, refldir, depth + 1), fresneleffect);
}


Vector Tracer::trace(const Vector &origin, const Vector &direction, const int &depth) const{
    std::pair<const Primitive*, Vector> intersectection = getIntersectedObjectAndPoint(origin, direction);
    const Primitive* intersectedObject = intersectection.first;
    Vector intersectionPoint = intersectection.second;
    if (!intersectedObject) return Vector(2);
    Vector objectColor = 0;
    Vector intersectionNormal = intersectedObject->getNormal(intersectionPoint);
    intersectionNormal.normalize();
    bool inside = false;
    if (direction.dot(intersectionNormal) > 0) intersectionNormal = -intersectionNormal, inside = true;
    if ((intersectedObject->transparency > 0 || intersectedObject->reflection > 0) && depth < MAX_RAY_DEPTH) {
        std::pair<Vector, double> reflectionInfo = computeReflection(intersectionPoint, intersectionNormal, direction, depth + 1);
        Vector reflection = reflectionInfo.first;
        double fresneleffect = reflectionInfo.second;
        Vector refraction = 0;
        if (intersectedObject->transparency) {
            double eta = (inside) ? 1.1 : 0.9;
            refraction = computeRefraction(eta, intersectionPoint, intersectionNormal, direction, depth + 1);
        }
        objectColor = (reflection * fresneleffect + refraction * (1 - fresneleffect) *
                       intersectedObject->transparency) * intersectedObject->surfaceColor;
    } else {
        for (unsigned i = 0; i < objects.size(); ++i) {
            if (objects[i]->emissionColor.x > 0) {
                bool clearWay = true;
                Vector lightDirection = objects[i]->getCenter() - intersectionPoint;
                lightDirection.normalize();
                for (unsigned j = 0; j < objects.size(); ++j) {
                    if (i != j && objects[j] != intersectedObject) {
                        double t;
                        if (objects[j]->intersect(intersectionPoint + intersectionNormal * BIAS, lightDirection, t)) {
                            clearWay = false;
                            break;
                        }
                    }
                }
                if (clearWay) {
                    objectColor += intersectedObject->surfaceColor *
                    std::max(double(0), intersectionNormal.dot(lightDirection)) * objects[i]->emissionColor;
                }
            }
        }
    }
    
    return objectColor + intersectedObject->emissionColor;
}