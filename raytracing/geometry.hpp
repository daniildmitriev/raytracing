//
//  geometry.hpp
//  raytracing
//
//  Created by Даня on 14.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#ifndef geometry_hpp
#define geometry_hpp

#include <iostream>
#include <cmath>

#define EPS 1e-7
#define BIAS 1e-4

class Vector {
public:
    double x, y, z;
    Vector();
    Vector(double xx);
    Vector(double xx, double yy, double zz);
    Vector& normalize();
    Vector operator * (const double &f) const;
    Vector operator / (const double &f) const;
    Vector operator * (const Vector &v) const;
    double dot(const Vector &v) const;
    Vector cross(const Vector &v) const;
    Vector operator - (const Vector &v) const;
    Vector operator + (const Vector &v) const;
    Vector& operator += (const Vector &v);
    Vector& operator *= (const Vector &v);
    Vector operator - () const;
    double length2() const;
    double length() const;
    friend std::ostream & operator << (std::ostream &os, const Vector &v);
    friend std::istream & operator >> (std::istream &is, Vector &v);
};

class Primitive {
public:
    Vector center;
    Vector surfaceColor, emissionColor;
    double transparency, reflection;
    virtual bool intersect(const Vector &rayorig, const Vector &raydir, double &t) const = 0;
    virtual Vector getNormal(const Vector &point) const = 0;
    virtual Vector getCenter() const = 0;
};

class Sphere: public Primitive {
public:
    double radius, radius2;
    Sphere(const Vector &c, const double &r, const Vector &sc,
           const double &refl = 0, const double &transp = 0, const Vector &ec = 0);
    bool intersect(const Vector &rayorig, const Vector &raydir, double &t) const;
    Vector getNormal(const Vector &point) const;
    Vector getCenter() const;
};

class Parallelogram: public Primitive  {
public:
    Vector v, vA, vB;
    Parallelogram(const Vector &nv, const Vector &nvA, const Vector &nvB, const Vector &sc,
                  const double &refl = 0, const double &transp = 0, const Vector &ec = 0);
    bool intersect(const Vector &rayorig, const Vector &raydir, double &t) const;
};

class Triangle: public Primitive {
public:
    Vector v1, v2, v3;
    Triangle(const Vector &nv1, const Vector &nv2, const Vector &nv3, const Vector &sc,
             const double &refl = 0, const double &transp = 0, const Vector &ec = 0);
    bool intersect(const Vector &rayoring, const Vector &raydir, double &t) const;
    Vector getNormal(const Vector &point) const;
    Vector getCenter() const;
};

Vector genRandomColor();
double mix(const double &a, const double &b, const double &mix);

#endif /* geometry_hpp */
