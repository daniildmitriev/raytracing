//
//  tracer.hpp
//  raytracing
//
//  Created by Даня on 26.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#ifndef tracer_hpp
#define tracer_hpp

#include <stdio.h>
#include <vector>
#include "geometry.hpp"

#define MAX_RAY_DEPTH 2

class Tracer {
    std::vector <Primitive*> objects;
public:
    Tracer(std::vector <Primitive*> &from_objects);
    Vector trace(const Vector &origin, const Vector &direction, const int &depth) const;
    Vector computeRefraction(const double &eta, const Vector &intersectionPoint, const Vector &intersectionNormal, const Vector &direction, const int &depth) const;
    std::pair<Vector, double> computeReflection(const Vector &intersectionPoint, const Vector &intersectionNormal, const Vector &direction, const int &depth) const;
    std::pair<const Primitive*, Vector> getIntersectedObjectAndPoint
                                        (const Vector &origin, const Vector &direction) const;
};

#endif /* tracer_hpp */
