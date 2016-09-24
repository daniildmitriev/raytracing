//
//  converter.hpp
//  raytracing
//
//  Created by Даня on 27.05.16.
//  Copyright © 2016 mipt. All rights reserved.
//

#ifndef converter_hpp
#define converter_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "geometry.hpp"

class Converter {
public:
    std::vector <Primitive*> getFromFile(const char* filename) const;
};

#endif /* converter_hpp */
