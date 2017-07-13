#include "Transform3D.h"
#include "svdDynamic.h"
#include <stdio.h>
#include <math.h>

Transform3D Transform3D::identity() {
    return Transform3D(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);
}

Transform3D Transform3D::rotatedX(double theta) {
    return Transform3D(1, 0, 0, 0,
                       0, cos(theta), -sin(theta), 0,
                       0, sin(theta), cos(theta), 0,
                       0, 0, 0, 1);
}

Transform3D Transform3D::rotatedY(double theta) {
    return Transform3D(cos(theta), 0, sin(theta), 0,
                       0, 1, 0, 0,
                       -sin(theta), 0, cos(theta), 0,
                       0, 0, 0, 1);
}

Transform3D Transform3D::rotatedZ(double theta) {
    return Transform3D(cos(theta), -sin(theta), 0, 0,
                       sin(theta), cos(theta), 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);
}

Transform3D Transform3D::translated(double x, double y, double z) {
    return Transform3D(1, 0, 0, x,
                       0, 1, 0, y,
                       0, 0, 1, z,
                       0, 0, 0, 1);
}

Transform3D Transform3D::scaled(double x, double y, double z) {
    return Transform3D(x, 0, 0, 0,
                       0, y, 0, 0,
                       0, 0, z, 0,
                       0, 0, 0, 1);
}

Transform3D::Transform3D() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m[r][c] = 0.0;
        }
    }
}

Transform3D::Transform3D(double r0c0, double r0c1, double r0c2, double r0c3,
                         double r1c0, double r1c1, double r1c2, double r1c3,
                         double r2c0, double r2c1, double r2c2, double r2c3,
                         double r3c0, double r3c1, double r3c2, double r3c3){
    m[0][0] = r0c0; m[0][1] = r0c1; m[0][2] = r0c2; m[0][3] = r0c3;
    m[1][0] = r1c0; m[1][1] = r1c1; m[1][2] = r1c2; m[1][3] = r1c3;
    m[2][0] = r2c0; m[2][1] = r2c1; m[2][2] = r2c2; m[2][3] = r2c3;
    m[3][0] = r3c0; m[3][1] = r3c1; m[3][2] = r3c2; m[3][3] = r3c3;
}

//Assuming a, b, c are vectors, h is point.
Transform3D::Transform3D(const Point3D &a, const Point3D &b, const Point3D &c, const Point3D &d){
    m[0][0] = a.x; m[0][1] = b.x; m[0][2] = c.x; m[0][3] = d.x;
    m[1][0] = a.y; m[1][1] = b.y; m[1][2] = c.y; m[1][3] = d.y;
    m[2][0] = a.z; m[2][1] = b.z; m[2][2] = c.z; m[2][3] = d.z;
    m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
}

Transform3D::Transform3D(const Transform3D &t) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m[r][c] = t.m[r][c];
        }
    }
}

Transform3D& Transform3D::operator=(const Transform3D &t) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m[r][c] = t.m[r][c];
        }
    }
    return (*this);
}

Transform3D Transform3D::inverse() {
    Transform3D inv;
    
    // Because of the fact we're using homogeneous coordinates, we must be careful how
    // we invert the transformation matrix. What we need is the inverse of the
    // 3x3 Affine transform, and -1 * the translation component. If we just invert
    // the entire matrix, junk happens.
    // So, we need a 3x3 matrix for inversion:
    float T3x3[3][3], Tinv3x3[3][3];
    T3x3[0][0] = this->m[0][0];
    T3x3[0][1] = this->m[0][1];
    T3x3[0][2] = this->m[0][2];
    T3x3[1][0] = this->m[1][0];
    T3x3[1][1] = this->m[1][1];
    T3x3[1][2] = this->m[1][2];
    T3x3[2][0] = this->m[2][0];
    T3x3[2][1] = this->m[2][1];
    T3x3[2][2] = this->m[2][2];
    
    // Now for the translation component:
    float tx = -this->m[0][3];
    float ty = -this->m[1][3];
    float tz = -this->m[2][3];
    
    // Invert the affine transform
    float *U = NULL;
    float *s = NULL;
    float *V = NULL;
    float *rv1 = NULL;
    int singFlag = 0;
    
    // Wasn't invertible
    SVD(&T3x3[0][0], 3, 3, &U, &s, &V, &rv1);
    if (U==NULL||s==NULL||V==NULL)
    {
        fprintf(stderr,"Error: Matrix not invertible for this object, returning identity\n");
        return Transform3D::identity();
    }
    
    // Check if matrix is singular
    for (int i=0;i<3;i++) if (*(s+i)<1e-9) singFlag=1;
    if (singFlag)
    {
        fprintf(stderr,"Error: Transformation matrix is singular, returning identity\n");
        this->printTransform3D();
        return Transform3D::identity();
    }

    // Compute and store inverse matrix
    InvertMatrix(U,s,V,3,&Tinv3x3[0][0]);
    
    // Now stuff the transform into Tinv
    inv.m[0][0] = (double)Tinv3x3[0][0];
    inv.m[0][1] = (double)Tinv3x3[0][1];
    inv.m[0][2] = (double)Tinv3x3[0][2];
    inv.m[1][0] = (double)Tinv3x3[1][0];
    inv.m[1][1] = (double)Tinv3x3[1][1];
    inv.m[1][2] = (double)Tinv3x3[1][2];
    inv.m[2][0] = (double)Tinv3x3[2][0];
    inv.m[2][1] = (double)Tinv3x3[2][1];
    inv.m[2][2] = (double)Tinv3x3[2][2];
    inv.m[0][3] = Tinv3x3[0][0]*tx + Tinv3x3[0][1]*ty + Tinv3x3[0][2]*tz;
    inv.m[1][3] = Tinv3x3[1][0]*tx + Tinv3x3[1][1]*ty + Tinv3x3[1][2]*tz;
    inv.m[2][3] = Tinv3x3[2][0]*tx + Tinv3x3[2][1]*ty + Tinv3x3[2][2]*tz;
    inv.m[3][3] = 1;

    free(U);
    free(s);
    free(V);
    
    return inv;
}

// Note: this completely disregards the translation, since it must be an affine transpose
// and the translation showing up in the bottow row would violate that requirement
Transform3D Transform3D::transpose() const {
    return Transform3D(m[0][0], m[1][0], m[2][0], m[3][0],
                       m[0][1], m[1][1], m[2][1], m[3][1],
                       m[0][2], m[1][2], m[2][2], m[3][2],
                       0.0, 0.0, 0.0, 1.0); // last row is forced to be affine
}

Transform3D Transform3D::operator*(double scale) const {
    return Transform3D(m[0][0]*scale, m[0][1]*scale, m[0][2]*scale, m[0][3]*scale,
                       m[1][0]*scale, m[1][1]*scale, m[1][2]*scale, m[1][3]*scale,
                       m[2][0]*scale, m[2][1]*scale, m[2][2]*scale, m[2][3]*scale,
                       m[3][0]*scale, m[3][1]*scale, m[3][2]*scale, m[3][3]*scale);
}

Transform3D operator*(double scale, const Transform3D &t) {
    return t*scale;
}

Point3D Transform3D::operator*(const Point3D &p) const {
    Point3D row0(m[0][0], m[0][1], m[0][2], m[0][3]);
    Point3D row1(m[1][0], m[1][1], m[1][2], m[1][3]);
    Point3D row2(m[2][0], m[2][1], m[2][2], m[2][3]);
    Point3D row3(m[3][0], m[3][1], m[3][2], m[3][3]);
    return Point3D(row0.dot(p),
                   row1.dot(p),
                   row2.dot(p),
                   row3.dot(p));
}

Transform3D Transform3D::operator*(const Transform3D &t) const {
    Transform3D res;
    
    Point3D rowVecs[4] = {
        Point3D(m[0][0], m[0][1], m[0][2], m[0][3]),
        Point3D(m[1][0], m[1][1], m[1][2], m[1][3]),
        Point3D(m[2][0], m[2][1], m[2][2], m[2][3]),
        Point3D(m[3][0], m[3][1], m[3][2], m[3][3]),
    };
    Point3D colVecs[4] = {
        Point3D(t.m[0][0], t.m[1][0], t.m[2][0], t.m[3][0]),
        Point3D(t.m[0][1], t.m[1][1], t.m[2][1], t.m[3][1]),
        Point3D(t.m[0][2], t.m[1][2], t.m[2][2], t.m[3][2]),
        Point3D(t.m[0][3], t.m[1][3], t.m[2][3], t.m[3][3]),
    };
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            res.m[row][col] = rowVecs[row].dot(colVecs[col]);
        }
    }
    
    return res;
}

Point3D Transform3D::translationComponent() const {
    return Point3D(m[0][3], m[1][3], m[2][3], false);
}

void Transform3D::printTransform3D() {
    printf("{\n");
    printf("\t(%.2f, %.2f, %.2f, %.2f)\n", m[0][0], m[0][1], m[0][2], m[0][3]);
    printf("\t(%.2f, %.2f, %.2f, %.2f)\n", m[1][0], m[1][1], m[1][2], m[1][3]);
    printf("\t(%.2f, %.2f, %.2f, %.2f)\n", m[2][0], m[2][1], m[2][2], m[2][3]);
    printf("\t(%.2f, %.2f, %.2f, %.2f)\n", m[3][0], m[3][1], m[3][2], m[3][3]);
    printf("}\n");
}
