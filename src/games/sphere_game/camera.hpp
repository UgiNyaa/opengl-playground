#ifndef CAMERA_HPP
#define CAMERA_HPP

#define TAU 6.28318530717958647692f

struct Camera
{
    float radius = 7;
    float theta = 1;
    float phi = TAU-0.3f;

    void add_theta(float add) 
    { 
        theta += add;
        if (theta >= (TAU/2))
            theta -= TAU/2; 
    }
    void add_phi(float add) 
    { 
        phi += add;
        if (phi >= TAU)
            phi -= TAU; 
    }
};

#endif /* CAMERA_HPP */