#include "Object3D.h"

bool Object3D::isLightSource() {
    return isLight;
}

Object3D::Object3D(const Material &material, const ColourRGB &colour) :
material(material), colour(colour) {
    this->material = material;
    this->colour = colour;
    this->transform = Transform3D::identity();
    this->invTransform = Transform3D::identity();
    this->isLight = false;              // default
}

Object3D::Object3D(const Object3D &obj) :
material(obj.material), colour(obj.colour) {
    material = obj.material;
    colour = obj.colour;
    transform = obj.transform;
    invTransform = obj.invTransform;
    isLight = obj.isLight;
    textureImage = obj.textureImage;
}

Object3D& Object3D::operator=(const Object3D &obj) {
    material = obj.material;
    colour = obj.colour;
    transform = obj.transform;
    invTransform = obj.invTransform;
    isLight = obj.isLight;
    textureImage = textureImage;
    
    return (*this);
}

void Object3D::rotateX(double theta) {
    transform = Transform3D::rotatedX(theta) * transform;
    updateInverse();
}

void Object3D::rotateY(double theta) {
    transform = Transform3D::rotatedY(theta) * transform;
    updateInverse();
}

void Object3D::rotateZ(double theta) {
    transform = Transform3D::rotatedZ(theta) * transform;
    updateInverse();
}

void Object3D::translate(double x, double y, double z) {
    transform = Transform3D::translated(x, y, z) * transform;
    updateInverse();
}

void Object3D::scale(double x, double y, double z) {
    transform = Transform3D::scaled(x, y, z) * transform;
    updateInverse();
}

Point3D Object3D::originInWorld() const {
    return transform.translationComponent();
}

void Object3D::updateInverse() {
    invTransform = transform.inverse();
}

// Load a texture image from file and assign it to the
// specified object
void Object3D::loadTexture(const char *filename) {
    bool success = Image::readPPMimage(filename, &textureImage);
    if (!success) {
        printf("Error: could not load texture %s\n", filename);
    }
}
