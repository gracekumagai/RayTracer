#ifndef Material_h
#define Material_h

#include <stdio.h>

class Material {
public:
    double ambient;
    double diffuse;
    double specular;
    double global;
    
    double opacity;   // Opacity - if less than 1 this is a semi transparent object
    // and refraction rays should be implemented
    double refractionIndex;
    double shinyness;   // Exponent for phong specular component
    double roughness;   // Influences the maximum possible angle of deviation for glossy surfaces
    
    Material(double ambient, double diffuse, double specular, double global,
             double opacity, double refractionIndex, double shinyness, double roughness);
    Material(){}
};

#endif
