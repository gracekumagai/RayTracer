#include "Ray3D.h"

Ray3D::Ray3D(Point3D origin, Point3D direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray3D::Ray3D(const Ray3D &ray) {
    origin = ray.origin;
    direction = ray.direction;
}

Ray3D& Ray3D::operator=(const Ray3D &ray) {
    origin = ray.origin;
    direction = ray.direction;
    return *this;
}

Point3D Ray3D::rayPosition(double lambda) const {
    return origin + lambda*direction;
}

Ray3D Ray3D::bias(const Point3D &normal) const {
    return Ray3D(origin + biasAmount*normal, direction);
}

Ray3D Ray3D::offset(Point3D amount) const {
    return Ray3D(origin, direction + amount).normalized();
}

Ray3D Ray3D::normalized() const {
    return Ray3D(origin, direction.normalized());
}
