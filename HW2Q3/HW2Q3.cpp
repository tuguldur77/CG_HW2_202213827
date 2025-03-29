#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <random>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

// Window dimensions
int Width = 512;
int Height = 512;
vector<vec3> OutputImage(Width* Height, vec3(0.0f));

// Camera parameters 
vec3 eye(0.0f, 0.0f, 0.0f);
vec3 u(1.0f, 0.0f, 0.0f), v(0.0f, 1.0f, 0.0f), w(0.0f, 0.0f, 1.0f);
float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f, d = 0.1f;

// Light source
vec3 lightPos(-4.0f, 4.0f, -3.0f);
vec3 lightColor(1.0f, 1.0f, 1.0f);

// Ray structure
struct Ray {
    vec3 origin, direction;
    Ray(const vec3& o, const vec3& d) : origin(o), direction(normalize(d)) {}
};

// Material structure
struct Material {
    vec3 ka, kd, ks;
    float specularPower;
};

// Sphere structure
struct Sphere {
    vec3 center;
    float radius;
    Material material;

    bool intersect(const Ray& ray, float& t) const {
        vec3 oc = ray.origin - center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(oc, ray.direction);
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;
        float sqrt_d = std::sqrt(discriminant);
        float t1 = (-b - sqrt_d) / (2.0f * a);
        float t2 = (-b + sqrt_d) / (2.0f * a);

        t = (t1 > 0) ? t1 : t2;
        return t > 0;
    }
};

// Plane structure
struct Plane {
    vec3 normal;
    float d;
    Material material;

    bool intersect(const Ray& ray, float& t) const {
        float denom = dot(normal, ray.direction);
        if (abs(denom) > 1e-6) {
            t = -(dot(normal, ray.origin) + d) / denom;
            return (t > 0);
        }
        return false;
    }
};

// Check for shadows
bool isInShadow(const vec3& point, const vector<Sphere>& spheres, const Plane& plane) {
    vec3 lightDir = normalize(lightPos - point);
    Ray shadowRay(point + lightDir * 1e-3f, lightDir);
    float t;

    for (const auto& sphere : spheres) {
        if (sphere.intersect(shadowRay, t)) return true; // The point is in shadow.
    }

    if (plane.intersect(shadowRay, t)) return true; // The point is in shadow.

    return false;
}

// Compute Phong shading
vec3 phongIllumination(const vec3& point, const vec3& normal, const Material& material, const vec3& viewDir, const vector<Sphere>& spheres, const Plane& plane) {
    if (isInShadow(point, spheres, plane)) {
        return material.ka * lightColor; // Only ambient component in shadow
    }

    vec3 lightDir = normalize(lightPos - point);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = material.ka * lightColor;
    vec3 diffuse = material.kd * glm::max(dot(normal, lightDir), 0.0f) * lightColor;
    vec3 specular = material.ks * pow(glm::max(dot(reflectDir, viewDir), 0.0f), material.specularPower) * lightColor;

    return ambient + diffuse + specular;
}

// Apply gamma correction
vec3 gammaCorrection(const vec3& color, float gamma) {
    return vec3(pow(color.r, 1.0f / gamma), pow(color.g, 1.0f / gamma), pow(color.b, 1.0f / gamma));
}

// Render function with antialiasing
void render() {
    vector<Sphere> spheres = {
        { vec3(-4.0f, 0.0f, -7.0f), 1.0f, {{0.2f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f} },
        { vec3(0.0f, 0.0f, -7.0f), 2.0f, {{0.0f, 0.2f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, 32.0f} },
        { vec3(4.0f, 0.0f, -7.0f), 1.0f, {{0.0f, 0.0f, 0.2f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, 0.0f} }
    };
    Plane plane = { vec3(0.0f, 1.0f, 0.0f), 2.0f, {{0.2f, 0.2f, 0.2f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, 0.0f} };

    float gamma = 2.2f;
    int N = 64; // Number of samples per pixel
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int iy = 0; iy < Height; ++iy) {
        for (int ix = 0; ix < Width; ++ix) {
            vec3 color(0.0f);

            // Sample rays within the pixel
            for (int i = 0; i < N; ++i) {
                // Random offsets within the pixel
                float u_ = l + (r - l) * (ix + dis(gen)) / Width;
                float v_ = b + (t - b) * (iy + dis(gen)) / Height;
                vec3 M = eye + u_ * u + v_ * v - d * w;
                Ray ray(eye, M - eye);

                float min_t = numeric_limits<float>::max();

                // Check for intersections with spheres
                for (const auto& sphere : spheres) {
                    float t;
                    if (sphere.intersect(ray, t) && t < min_t) {
                        min_t = t;
                        vec3 point = ray.origin + t * ray.direction;
                        vec3 normal = normalize(point - sphere.center);
                        vec3 viewDir = normalize(eye - point);
                        color += phongIllumination(point, normal, sphere.material, viewDir, spheres, plane);
                    }
                }

                // Check for intersection with the plane
                float t_plane;
                if (plane.intersect(ray, t_plane) && t_plane < min_t) {
                    min_t = t_plane;
                    vec3 point = ray.origin + t_plane * ray.direction;
                    vec3 viewDir = normalize(eye - point);
                    color += phongIllumination(point, plane.normal, plane.material, viewDir, spheres, plane);
                }
            }

            // Average the color and apply gamma correction
            color /= N;
            OutputImage[iy * Width + ix] = gammaCorrection(color, gamma);
        }
    }
}

// Main function setup
void resize_callback(GLFWwindow*, int nw, int nh) {
    Width = nw;
    Height = nh;
    glViewport(0, 0, nw, nh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(Width), 0.0, static_cast<double>(Height), 1.0, -1.0);
    OutputImage.reserve(Width * Height * 3);
    render();
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;
    if (!glfwInit()) return -1;
    window = glfwCreateWindow(Width, Height, "OpenGL Viewer(Ray Trace with Antialiasing)", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    resize_callback(NULL, Width, Height);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
