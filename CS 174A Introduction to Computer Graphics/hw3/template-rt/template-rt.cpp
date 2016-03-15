//
// template-rt.cpp
//

#define _CRT_SECURE_NO_WARNINGS
#include "matm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int g_width;
int g_height;

struct Ray
{
    vec4 origin;
    vec4 dir;
};

// TODO: add structs for spheres, lights and anything else you may need.
struct Sphere {

    string name; 

    // Translations
    float pos_x, pos_y, pos_z;

    // Scale
    float s_x, s_y, s_z;

    // Color
    vec4 color;

    float ka, kd, ks, kr;
    float n;                    // shininess factor

};

struct BG {
    float r;
    float g;
    float b;
};

struct Ambient {
    float r;
    float g;
    float b;
};

struct Light {
    string name;

    float pos_x; 
    float pos_y;
    float pos_z;

    vec4 color;
};

vector<Sphere> g_spheres;
vector<Light> g_lights;
Ambient g_ambient; 
BG g_bg; 

vector<vec4> g_colors;

float g_left;
float g_right;
float g_top;
float g_bottom;
float g_near;

string g_output;


// -------------------------------------------------------------------
// Utility Functions
inline vec3 toVec3( vec4 in ) { return vec3( in[0], in[1], in[2] ); }

inline mat4 getSphereMatrix( Sphere s ) { 
    mat4 result = mat4();
    result *= Translate(s.pos_x, s.pos_y, s.pos_z);
    result *= Scale(s.s_x, s.s_y, s.s_z);
    return result; 
}


// -------------------------------------------------------------------
// Input file parsing

vec4 toVec4(const string& s1, const string& s2, const string& s3)
{
    stringstream ss(s1 + " " + s2 + " " + s3);
    vec4 result;
    ss >> result.x >> result.y >> result.z;
    result.w = 1.0f;
    return result;
}

float toFloat(const string& s)
{
    stringstream ss(s);
    float f;
    ss >> f;
    return f;
}

void parseLine(const vector<string>& vs)
{
    const int num_labels = 11; 
    const string labels[] = {       
        "NEAR",                 // 0
        "LEFT",                 // 1
        "RIGHT",                // 2
        "BOTTOM",               // 3
        "TOP",                  // 4
        "RES",                  // 5
        "SPHERE",               // 6
        "LIGHT",                // 7
        "BACK",                 // 8
        "AMBIENT",              // 9
        "OUTPUT"                // 10
    };

    //TODO: add parsing of NEAR, LEFT, RIGHT, BOTTOM, TOP, SPHERE, LIGHT, BACK, AMBIENT, OUTPUT.

    unsigned label_id = find( labels, labels + num_labels, vs[0] ) - labels;
    switch(label_id) {
        case 0: g_near = toFloat( vs[1] ); break;     // NEAR
        case 1: g_left = toFloat( vs[1] ); break;     // LEFT
        case 2: g_right = toFloat( vs[1] ); break;    // RIGHT
        case 3: g_bottom = toFloat( vs[1] ); break;   // BOTTOM
        case 4: g_top = toFloat( vs[1] ); break;      // TOP
        case 5:                                       // RES
            g_width = (int)toFloat(vs[1]);
            g_height = (int)toFloat(vs[2]);
            g_colors.resize(g_width * g_height);
            break; 
        case 6: 
        {
            Sphere s;
            s.name = vs[1];
            s.pos_x = toFloat( vs[2] );
            s.pos_y = toFloat( vs[3] );
            s.pos_z = toFloat( vs[4] );
            s.s_x = toFloat( vs[5] );
            s.s_y = toFloat( vs[6] );
            s.s_z = toFloat( vs[7] );
            s.color = toVec4( vs[8], vs[9], vs[10] );
            s.ka = toFloat( vs[11] );
            s.kd = toFloat( vs[12] );
            s.ks = toFloat( vs[13] );
            s.kr = toFloat( vs[14] );
            s.n = toFloat( vs[15] );

            g_spheres.push_back(s);
            break;
        }
        case 7:
        {
            Light l;
            l.name = vs[1];
            l.pos_x = toFloat( vs[2] );
            l.pos_y = toFloat( vs[3] ); 
            l.pos_z = toFloat( vs[4] );
            l.color = toVec4( vs[5], vs[6], vs[7] );
            g_lights.push_back(l); 
            break;
        }
        case 8: g_bg.r = toFloat( vs[1] ); g_bg.g = toFloat( vs[2] ); g_bg.b = toFloat( vs[3] ); break;
        case 9: g_ambient.r = toFloat( vs[1] ); g_ambient.g = toFloat( vs[2] ); g_ambient.b = toFloat( vs[3] ); break;
        case 10: g_output = vs[1]; break;
    }


    
}

void loadFile(const char* filename)
{
    ifstream is(filename);
    if (is.fail())
    {
        cout << "Could not open file " << filename << endl;
        exit(1);
    }
    string s;
    vector<string> vs;
    while(!is.eof())
    {
        vs.clear();
        getline(is, s);
        istringstream iss(s);
        while (!iss.eof())
        {
            string sub;
            iss >> sub;
            vs.push_back(sub);
        }
        parseLine(vs);
    }
}


// -------------------------------------------------------------------
// Utilities

void setColor(int ix, int iy, const vec4& color)
{
    int iy2 = g_height - iy - 1; // Invert iy coordinate.
    g_colors[iy2 * g_width + ix] = color;
}


// -------------------------------------------------------------------
// Intersection routine


// TODO: add your ray-sphere intersection routine here.
float intersect(Ray ray, Sphere sphere) 
{
    mat4 inverse_matrix = mat4();
    if (InvertMatrix(getSphereMatrix(sphere), inverse_matrix)) {
        vec3 origin = toVec3(inverse_matrix * ray.origin);
        vec3 dir = toVec3(inverse_matrix * ray.dir);

        // ray = dir * t + origin
        // sphere = |P| - 1 = 0
        // |ray| - 1 = 0

        float a = pow(length(dir), 2.0f);
        float b = dot(origin, dir);
        float c = pow(length(origin), 2.0f) - 1;

        float d = b * b - a * c;
        if (d >= 0) {
            float sol1 = -b / a + sqrt(d) / a;
            float sol2 = -b / a - sqrt(d) / a; 

            return sol1 < sol2 ? sol1 : sol2; 
        }
        else {
            return -1; 
        }


    }
    else {
        cout << "Sphere is not invertible!" << endl; 
    }

}

vec3 illuminate(Ray ray, Sphere sphere, float t) {
    vec4 hit = ray.origin + t * ray.dir; 
    // vec4 normal_vec = normalize(vec4(hit.x - sphere.pos_x, hit.y - sphere.pos_y, hit.z - sphere.pos_z, 0.0f));
    vec4 normal_vec = hit - vec4(sphere.pos_x, sphere.pos_y, sphere.pos_z, 1.0f);
    mat4 trans = Scale(sphere.s_x, sphere.s_y, sphere.s_z);
    mat4 inv;
    InvertMatrix(transpose(trans), inv);
    normal_vec.w = 0;
    normal_vec = normalize(inv * inv * normal_vec);

    vec4 ambient = vec4(g_ambient.r, g_ambient.g, g_ambient.b, 0.0f) * sphere.color * sphere.ka;
    vec4 diffuse = vec4( 0.0f, 0.0f, 0.0f, 0.0f);
    vec4 specular = vec4( 0.0f, 0.0f, 0.0f, 0.0f);


    for (int i = 0; i < g_lights.size(); i++) {
        Light l = g_lights[i];
        vec4 light_vec = normalize(vec4(l.pos_x - hit.x, l.pos_y - hit.y, l.pos_z - hit.z, 0.0f));

        float d = dot(light_vec, normal_vec);

        if (d <= 0.0f) continue;
        diffuse += (sphere.color * d * l.color);

        vec4 r = normalize(((2 * d) * normal_vec) - light_vec); 
        vec4 v = normalize(ray.origin - hit);
        specular += (powf(dot(r, v), sphere.n) * l.color * sphere.ks);
    }


    diffuse = vec4(diffuse.x * sphere.kd, diffuse.y * sphere.kd, diffuse.z * sphere.kd, 0.0f);

    vec4 result = diffuse + ambient + specular;
    return toVec3(result); 
}



// -------------------------------------------------------------------
// Ray tracing

vec4 trace(const Ray& ray)
{
    vector<float> t_vals; 

    for (int i = 0; i < g_spheres.size(); i++) {
        t_vals.push_back(intersect(ray, g_spheres[i]));
    }

    int lowest_index = -1;
    float lowest = numeric_limits<float>::max();

    for (int i = 0; i < g_spheres.size(); i++) {
        if (t_vals[i] > 1 && t_vals[i] < lowest) { // make sure hit time is > 1
            lowest = t_vals[i];
            lowest_index = i;
        }
    }

    if (lowest_index >= 0) {
        Sphere s = g_spheres[lowest_index];
        vec3 in = illuminate(ray, s, lowest); // intensity
        return vec4(in.x, in.y, in.z, 1.0f);
        // return vec4(s.color.x, s.color.y, s.color.z, 1.0f);

    }

    // return BG if no intersection
    return vec4(g_bg.r, g_bg.g, g_bg.b, 1.0f);
}

vec4 getDir(int ix, int iy)
{
    vec4 dir;

    float x = g_left + ((float) ix / g_width) * (g_right - g_left);
    float y = g_bottom + ((float) iy / g_height) * (g_top - g_bottom);
    float z = -g_near;


    dir = vec4(x, y, z, 0.0f);
    return normalize(dir);
}

void renderPixel(int ix, int iy)
{
    Ray ray;
    ray.origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ray.dir = getDir(ix, iy);

    vec4 color = trace(ray);
    setColor(ix, iy, color);
}

void render()
{
    for (int iy = 0; iy < g_height; iy++)
        for (int ix = 0; ix < g_width; ix++)
            renderPixel(ix, iy);
}


// -------------------------------------------------------------------
// PPM saving

void savePPM(int Width, int Height, const char* fname, unsigned char* pixels) 
{
    FILE *fp;
    const int maxVal=255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname,"wb");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    for(int j = 0; j < Height; j++) {
        fwrite(&pixels[j*Width*3], 3, Width, fp);
    }

    fclose(fp);
}

void saveFile()
{
    // Convert color components from floats to unsigned chars.
    // TODO: clamp values if out of range.
    unsigned char* buf = new unsigned char[g_width * g_height * 3];
    for (int y = 0; y < g_height; y++)
        for (int x = 0; x < g_width; x++)
            for (int i = 0; i < 3; i++)
                buf[y*g_width*3+x*3+i] = (unsigned char)(((float*)g_colors[y*g_width+x])[i] * 255.0f);
    
    savePPM(g_width, g_height, g_output.c_str(), buf);
    delete[] buf;
}

void debug() 
{
    // cout << "LIGHTS: " << g_lights.size() << endl;
    // cout << "BACKGROUND COLOR: rgb(" << g_bg.r << ", " << g_bg.g << ", " << g_bg.b << ")" << endl;
    // cout << "AMBIENT COLOR: rgb(" << g_ambient.r << ", " << g_ambient.g << ", " << g_ambient.b << ")" << endl;
    // for (int i = 0; i < g_spheres.size(); i++) {
    //     cout << g_spheres[i].name << " : rgb(" << g_spheres[i].r << ", " << g_spheres[i].g << ", " << g_spheres[i].b << ")" << endl;
    // }

}


// -------------------------------------------------------------------
// Main

int main(int argc, char* argv[])
{


    if (argc < 2)
    {
        cout << "Usage: template-rt <input_file.txt>" << endl;
        exit(1);
    }
    loadFile(argv[1]);
    render();
    saveFile();
    if (argc == 3) {
        debug();
    }
	return 0;
}

