//
//  converter.cpp
//  raytracing
//
//  Created by Даня on 27.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#include "converter.hpp"

std::vector <Primitive*> Converter::getFromFile(const char* filename) const {
    freopen(filename, "r", stdin);
    std::string s;
    Vector va, vb, vc;
    std::vector <Primitive*> result;
    std::cin >> s;
    while (s != "endsolid") {
        if (s == "loop") {
            std::cin >> s;
            std::cin >> va;
            std::cin >> s;
            std::cin >> vb;
            std::cin >> s;
            std::cin >> vc;
            result.push_back(new Triangle(va, vc, vb, Vector(0.90, 0.76, 0.46), 0, 0.0));
        }
        std::cin >> s;
    }
    return result;
}