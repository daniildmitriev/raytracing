//
//  geometry.cpp
//  raytracing
//
//  Created by Даня on 14.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#include "geometry.hpp"

Vector::Vector() : x(double(0)), y(double(0)), z(double(0)) {}
Vector::Vector(double xx) : x(xx), y(xx), z(xx) {}
Vector::Vector(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

Vector& Vector::normalize() {
    double nor2 = length2();
        if (nor2 > 0) {
            double invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
}

Vector Vector::operator * (const double &f) const {
    return Vector(x * f, y * f, z * f);
}

Vector Vector::operator / (const double &f) const {
    return Vector(x / f, y / f, z / f);
}

Vector Vector::operator * (const Vector &v) const {
    return Vector(x * v.x, y * v.y, z * v.z);
}

double Vector::dot(const Vector &v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector Vector::cross(const Vector &v) const {
    double e1 = y * v.z - z * v.y;
    double e2 = z * v.x - x * v.z;
    double e3 = x * v.y - y * v.x;
    return Vector(e1, e2, e3);
}

Vector Vector::operator - (const Vector &v) const {
    return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator + (const Vector &v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
}

Vector& Vector::operator += (const Vector &v) {
    x += v.x, y += v.y, z += v.z; return *this;
}

Vector& Vector::operator *= (const Vector &v) {
    x *= v.x, y *= v.y, z *= v.z; return *this;
}

Vector Vector::operator - () const {
    return Vector(-x, -y, -z);
}

double Vector::length2() const {
    return x * x + y * y + z * z;
}

double Vector::length() const {
    return sqrt(length2());
}

std::ostream & operator << (std::ostream &os, const Vector &v) {
    os << "[" << v.x << " " << v.y << " " << v.z << "]";
    return os;
}

std::istream & operator >> (std::istream &is, Vector &v) {
    is >> v.x >> v.y >> v.z;
    v.x -= 150;
    v.y -= 80;
    v.z -= 400;
    return is;
}

//std::istream & operator >> (std::istream &is, Vector &v) {
//    is >> v.z >> v.x >> v.y;
//    v.y *= -1;
//    return is;
//}

Sphere::Sphere(const Vector &c, const double &r, const Vector &sc,
               const double &refl, const double &transp, const Vector &ec):
               radius(r), radius2(r * r) {
        center = c;
        transparency = transp;
        reflection = refl;
        surfaceColor = sc;
        emissionColor = ec;
}
bool Sphere::intersect(const Vector &rayorig, const Vector &raydir, double &t) const {
        Vector l = center - rayorig;
        double tca = l.dot(raydir);
        if (tca < 0) return false;
        double d2 = l.dot(l) - tca * tca;
        if (d2 > radius2) return false;
        double thc = sqrt(radius2 - d2);
        t = tca - thc;
        if (t < 0) {
            t = tca + thc;
        }
        
        return true;
}
    
Vector Sphere::getNormal(const Vector &point) const {
        return point - center;
}
    
Vector Sphere::getCenter() const {
        return center;
}

Parallelogram::Parallelogram(const Vector &nv, const Vector &nvA, const Vector &nvB, const Vector &sc,
                  const double &refl, const double &transp, const Vector &ec) {
        v = nv;
        vA = nvA;
        vB = nvB;
        center = v + vA * 0.5 + vB * 0.5;
        transparency = transp;
        reflection = refl;
        surfaceColor = sc;
        emissionColor = ec;
        
}

bool Parallelogram::intersect(const Vector &rayorig, const Vector &raydir, double &t) const {
        return false;
}

Triangle::Triangle(const Vector &nv1, const Vector &nv2, const Vector &nv3, const Vector &sc,
             const double &refl, const double &transp, const Vector &ec) {
        v1 = nv1;
        v2 = nv2;
        v3 = nv3;
        transparency = transp;
        reflection = refl;
        surfaceColor = sc;
        emissionColor = ec;
}

//bool Triangle::intersect(const Vector &rayorig, const Vector &raydir, double &t) const {
//    Vector e1 = v2 - v1;
//    Vector e2 = v3 - v1;
//    Vector pvec = raydir.cross(e2);
//    double det = e1.dot(pvec);
//    if (det < EPS) {
//        return false;
//    }
//    Vector tvec = rayorig - v1;
//    double u = tvec.dot(pvec);
//    if (u < 0 || u > det) {
//        return false;
//    }
//    Vector qvec = tvec.cross(e1);
//    double v = raydir.dot(qvec);
//    if (v < 0 || v > det) {
//        return false;
//    }
//    t = e2.dot(qvec);
//    double inv_det = 1. / det;
//    t *= inv_det;
//    return true;
//}

bool Triangle::intersect(const Vector &rayorig, const Vector &raydir, double &t) const {
    // Find vectors for two edges sharing vert0
     Vector edge1 = v2 - v1;
     Vector edge2 = v3 - v1;
    
    // Begin calculating determinant - also used to calculate U parameter
    Vector pvec = raydir.cross(edge2);
    
    // If determinant is near zero, ray lies in plane of triangle
    double det = edge1.dot(pvec);
    
    Vector tvec;
    
    if( det > 0 )
    {
        tvec = rayorig - v1;
    }
    else
    {
        tvec = v1 - rayorig;
        det = -det;
    }
    
    if( det < EPS )
        return false;
    
    // Calculate U parameter and test bounds
    double u = tvec.dot(pvec);
    if( u < 0 || u > det )
        return false;
    
    Vector qvec = tvec.cross(edge1);
    
    // Calculate V parameter and test bounds
    double v = raydir.dot(qvec);
    if( v < 0 || (u + v) > det )
        return false;
    
    // Calculate t, scale parameters, ray intersects triangle
    t = edge2.dot(qvec);
    double fInvDet = 1.0 / det;
    t *= fInvDet;
    return true;
}

Vector Triangle::getNormal(const Vector &point) const {
    Vector e1 = v3 - v1;
    Vector e2 = v2 - v1;
    return e2.cross(e1);
}

Vector Triangle::getCenter() const {
    return v1 / 3.0 + v2 / 3.0 + v3 / 3.0;
}

class Quadrangle {
    
};

Vector genRandomColor() {
    return Vector((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
}

double mix(const double &a, const double &b, const double &mix) {
    return b * mix + a * (1 - mix);
}