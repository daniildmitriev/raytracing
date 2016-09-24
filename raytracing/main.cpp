#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#include "tracer.hpp"
#include "converter.hpp"

void render(const Tracer *tracer) {
    unsigned width = 640, height = 480;
    Vector *image = new Vector[width * height], *pixel = image;
    double fov = 60, aspectratio = width / double(height);
    double angle = tan(M_PI * 0.5 * fov / 180.);  
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x =  0; x < width; ++x, ++pixel) {
            double xx = (2 * ((x + 0.5) / width) - 1) * angle * aspectratio;
            double yy = (1 - 2 * ((y + 0.5) / height)) * angle;
            Vector direction(xx, yy, -1);
            direction.normalize();
            *pixel = tracer->trace(Vector(0), direction, 0); // -5 -10 -10, 20 50 -10
        }
    }
    std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(double(1), image[i].x) * 255) <<
        (unsigned char)(std::min(double(1), image[i].y) * 255) <<
        (unsigned char)(std::min(double(1), image[i].z) * 255);
    }
    ofs.close();
    delete [] image;
}

int main(int argc, char **argv)
{
    std::vector<Primitive*> objects;
    
    // position, radius, surface color, reflectivity, transparency, emission color
    
    objects.push_back(new Triangle(Vector( -7.0,      5, -15),
                                   Vector( -12.0,      0, -10),
                                   Vector( -2.0,      0, -10), genRandomColor(), 0, 0.0));
    objects.push_back(new Triangle(Vector( -7.0,      -5 , -15),
                                   Vector( -12.0,      0, -10),
                                   Vector( -2.0,      0, -10), genRandomColor(), 0 , 0.0));
    objects.push_back(new Triangle(Vector(-7.0,      5, -15),
                                   Vector( -2.0,      0, -10),
                                   Vector( -2.0,      0, -20), genRandomColor(), 0, 0.0));
    objects.push_back(new Triangle(Vector( -7.0,      -5, -15),
                                   Vector( -2.0,      0, -10),
                                   Vector( -2.0,      0, -20), genRandomColor(), 0, 0.0));
    objects.push_back(new Sphere(Vector(4, 0, -5), 0.5, genRandomColor(), 1, 0));
//    objects.push_back(new Triangle(Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20), genRandomColor(), 1, 0.5));
//    objects.push_back(new Triangle(Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20), genRandomColor(), 1, 0.5));
//    objects.push_back(new Triangle(Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20), genRandomColor(), 1, 0.5));
//    objects.push_back(new Triangle(Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20),
//                                   Vector( 0.0,      0, -20), genRandomColor(), 1, 0.5));
    // light
    objects.push_back(new Sphere(Vector( 0.0,     0,  5),     1, Vector(0), 0, 0.0, Vector(1)));
    objects.push_back(new Sphere(Vector( 5.0,     0, -5),     1, Vector(0), 0, 0.0, Vector(1)));
    Tracer* tracer = new Tracer(objects);
    render(tracer);
    
    return 0;
}

//
//int main(int argc, char **argv) {
//    srand(time(0));
//    std::vector<Primitive*> objects;
//    
//    // position, radius, surface color, reflectivity, transparency, emission color
//    //    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, Vector(0.20, 0.20, 0.20), 0, 0));
////    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, genRandomColor(), 0, 0.0));
//    //    objects.push_back(new Sphere(Vector( 0.0, 10100 , -20), 10000, genRandomColor(), 0, 0.0));
//    
//    for (int i = 0; i < 25; i++) {
//        double transp = 0;
//        if (rand() % 4 > 1) {
//            transp = 0.5;
//        }
//        double x = rand() % 10 * ((rand() % 2) * 2 - 1);
//        double y = rand() % 5 * ((rand() % 2) * 2 - 1);
//        double z = rand() % 20 * ((rand() % 2) * 2 - 1) - 25;
//        objects.push_back(new Sphere(Vector(x, y, z), rand() % 3 + 2, genRandomColor(), 1, transp));
//    }
//    objects.push_back(new Sphere(Vector( 0.0,     20, -30),     3, Vector(0.00, 0.00, 0.00), 0, 0.0, Vector(3)));
//    Tracer* tracer = new Tracer(objects);
//    render(tracer);
//    
//    return 0;
//}


//int main(int argc, char **argv)
//{
//    srand(time(0));
//    std::vector<Primitive*> objects;
//    
//    // position, radius, surface color, reflectivity, transparency, emission color
//    //    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, Vector(0.20, 0.20, 0.20), 0, 0));
//    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, genRandomColor(), 0, 0.0));
////    objects.push_back(new Sphere(Vector( 0.0, 10100 , -20), 10000, genRandomColor(), 0, 0.0));
//    
//    objects.push_back(new Sphere(Vector( 0.0,      0, -20),     4, genRandomColor(), 1, 0.5));
//    objects.push_back(new Sphere(Vector( 4.0,     -1, -15),     2, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector( 5.3,      0, -25),     3, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector(-5.5,      -4, -15),     3, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector( 3.0,      2, -20),     2, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector(-4.5,      1, -10),     3, genRandomColor(), 1, 0.5));
//    objects.push_back(new Sphere(Vector( 6.3,      8, -35),     6, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector( -8.3,      4, -25),     3, genRandomColor(), 1, 0.0));
//    objects.push_back(new Sphere(Vector( 3.3,      6, -25),     3, genRandomColor(), 1, 0.5));
//    // light
//    objects.push_back(new Sphere(Vector( 0.0,     20, -30),     3, Vector(0.00, 0.00, 0.00), 0, 0.0, Vector(3)));
//    Tracer* tracer = new Tracer(objects);
//    render(tracer);
//    
//    return 0;
//}

//int main(int argc, char **argv)
//{
//    std::vector<Primitive*> objects;
//    
//    // position, radius, surface color, reflectivity, transparency, emission color
//    //    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, Vector(0.20, 0.20, 0.20), 0, 0));
//    objects.push_back(new Sphere(Vector( 0,      0, -3),     1, Vector(1.00, 0.32, 0.36), 1, 0.5));
//    objects.push_back(new Sphere(Vector( 0.0,     3, -10),     2, Vector(0.90, 0.76, 0.46), 1, 0.0));
//    objects.push_back(new Sphere(Vector( 1,      0 , -3),     0.5, Vector(0.65, 0.77, 0.97), 1, 0.0));
//    objects.push_back(new Sphere(Vector(-4,      0, -10),     2, Vector(0.90, 0.90, 0.90), 1, 0.0));
//    //    objects.push_back(
//    //                      new Triangle(Vector( 0.0,     4, -25), Vector( 4.5,      0 , -25), Vector(-4.5,      0, -25),
//    //                                   Vector(0.90, 0.76, 0.46), 0, 0.0));
//    //    objects.push_back(
//    //                      new Triangle(Vector( 0.0,     4, -25), Vector( -5.5,      5 , -35), Vector(-4.5,      0, -25),
//    //                                   Vector(0.90, 0.76, 0.46), 0, 0.0));
//    //    objects.push_back(
//    //                      new Triangle(Vector( 0.0,     4, -25), Vector( 4.5,      0 , -25), Vector(5.5,      5, -20),
//    //                                   Vector(0.90, 0.76, 0.46), 0, 0.0));
//    
//    //    objects.push_back(new Sphere(Vector( -5.001,     -10, -10.001),     0.001, Vector(0.90, 0.76, 0.46), 1, 0, Vector(0)));
//    // light
//    objects.push_back(new Sphere(Vector(2, 2,  2),     1 , Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(10)));
//    //    objects.push_back(new Sphere(Vector(0 , 20, -5),     1, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
//    //    objects.push_back(new Sphere(Vector(15, 0, -10),     5, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
//    //    objects.push_back(new Sphere(Vector(-5, -10, -8),     0.1, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
//    // std::cout << objects[0].getCenter();
//    Tracer* tracer = new Tracer(objects);
//    render(tracer);
//    
//    return 0;
//}


/*
 int main(int argc, char **argv)
 {
 srand48(13);
 Converter converter = Converter();
 std::vector<Primitive*> objects = converter.getFromFile("humanoid_tri.stl");
 
 // position, radius, surface color, reflectivity, transparency, emission color
 //    objects.push_back(new Sphere(Vector( 0.0, -10004, -20), 10000, Vector(0.20, 0.20, 0.20), 0, 0));
 objects.push_back(new Sphere(Vector( -4.0,      0, -35),     10, Vector(1.00, 0.32, 0.36), 1, 0.5));
 objects.push_back(new Sphere(Vector( 0.0,     4, -15),     2, Vector(0.90, 0.76, 0.46), 1, 0.0));
 //    objects.push_back(new Sphere(Vector( 4.5,      0 , -25),     3, Vector(0.65, 0.77, 0.97), 1, 0.0));
 //    objects.push_back(new Sphere(Vector(-4.5,      0, -25),     3, Vector(0.90, 0.90, 0.90), 1, 0.0));
 objects.push_back(
 new Triangle(Vector( 0.0,     4, -25), Vector( 4.5,      0 , -25), Vector(-4.5,      0, -25),
 Vector(0.90, 0.76, 0.46), 0, 0.0));
 objects.push_back(
 new Triangle(Vector( 0.0,     4, -25), Vector( -5.5,      5 , -35), Vector(-4.5,      0, -25),
 Vector(0.90, 0.76, 0.46), 0, 0.0));
 objects.push_back(
 new Triangle(Vector( 0.0,     4, -25), Vector( 4.5,      0 , -25), Vector(5.5,      5, -20),
 Vector(0.90, 0.76, 0.46), 0, 0.0));
 
 objects.push_back(new Sphere(Vector( -5.001,     -10, -10.001),     0.001, Vector(0.90, 0.76, 0.46), 1, 0, Vector(0)));
 // light
 objects.push_back(new Sphere(Vector(-15, 0, -10),     5, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
 objects.push_back(new Sphere(Vector(15, 0, -10),     5, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
 objects.push_back(new Sphere(Vector(15, 0, -10),     5, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
 objects.push_back(new Sphere(Vector(-5, -10, -8),     0.1, Vector(1.00, 0.32, 0.36), 0, 0.0, Vector(1.5)));
 // std::cout << objects[0].getCenter();
 Tracer* tracer = new Tracer(objects);
 render(tracer);
 
 return 0;
 }
 */

/*
 
 int main(int argc, char **argv)
 {
 srand48(13);
 Converter converter = Converter();
 std::vector<Primitive*> objects = converter.getFromFile("block100.stl");
 
 // position, radius, surface color, reflectivity, transparency, emission color
 objects.push_back(new Sphere(Vector(-75, -30,  -300),     10 , Vector(1.00, 0.32, 0.36), 1, 0.0, Vector(10 )));
 
 Tracer* tracer = new Tracer(objects);
 render(tracer);
 
 
 return 0;
 }
 
 */
