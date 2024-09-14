#pragma once

#include <cmath>
#include <Windows.h>
#include <stdint.h>

#define PI 3.141592653589793f
#define ISZERO(v) !(v.x && v.y && v.z)

typedef struct _VIEW_MATRIX_T {
    float matrix[4][4];
    float* operator[](int index) {
        return matrix[index];
    }
} VIEW_MATRIX, *PVIEWMATRIX;

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;

    inline float length() {
        return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    }

    inline float getDistance(Vec3 const& other) {
        return Vec3{ x - other.x, y - other.y, z - other.z }.length();
    }

    inline Vec3 const& operator-(Vec3 const& op2) {
        return Vec3{ x - op2.x, y - op2.y, z - op2.z };
    }

    inline Vec3 const& operator+(Vec3 const& op2) {
        return Vec3{ x + op2.x, y + op2.y, z + op2.z };
    }
    
    inline Vec3 const& operator/(float divisor) {
        return Vec3{ x/divisor, y/divisor, z/divisor };
    }

    inline Vec3 const& operator*(float factor) {
        return Vec3{ x*factor, y*factor, z*factor };
    }

    inline Vec3 const& toAngles() {
        return Vec3{
            //sin is opposite/hypot 
            -atanf(z/hypot(x, y))*(180.0f/PI),
            //tan is op/adj
            atan2f(y,x)*(180.0f/PI),
            0.0f
        };
    }

    bool world2screen(VIEW_MATRIX& vMatrix, _Out_ Vec2* out) {
        float X, Y, W, screenx, screeny;
        X = vMatrix[0][0]*x + vMatrix[0][1]*y + vMatrix[0][2]*z + vMatrix[0][3];
        Y = vMatrix[1][0]*x + vMatrix[1][1]*y + vMatrix[1][2]*z + vMatrix[1][3];
        W = vMatrix[3][0]*x + vMatrix[3][1]*y + vMatrix[3][2]*z + vMatrix[3][3];

        X /= W;
        Y /= W;

        screenx = 1280;
        screeny = 720;

        out->x = screenx*(1 + X)*0.5;
        out->y = screeny*(1 - Y)*0.5;

        // W < 0 means localPlayer not facing, x/y out of screen boundaries means not
        //visible    
        if (W < 0.01f || (out->x > screenx || out->x < 0) || (out->y > screeny ||out->y < 0)) return false;
        //potentially do +0.5f for x and -0.5f for y?

        return true;
    }
};