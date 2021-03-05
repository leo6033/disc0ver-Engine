#version 420 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;

/*********************/

/*uniform*/
/*get screen size*/
uniform vec2 _screen_size;
uniform float _rdSeed[4];
uniform int _maxSampeler;
/*Camera Data*/
struct Camera
{
    vec3 lookFrom;
    vec3 left_buttom;
    vec3 horizontal;
    vec3 vertical;
};
uniform Camera _camera;

/**********************************/
/*random*/
int rdCount=0;
float RandXY(float x, float y){
     return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}
float Rand(){
    float a = RandXY(TexCoord.x, _rdSeed[0]);
    float b = RandXY(_rdSeed[1], TexCoord.y);
    float c = RandXY(rdCount++, _rdSeed[2]);
    float d = RandXY(_rdSeed[3], a);
    float e = RandXY(b, c);
    float f = RandXY(d, e);

    return f;
}
/**********************************/
/*Ray*/
struct Ray
{
    vec3 origin;
    vec3 direction; 
};
Ray Ray_Con(vec3 Origin, vec3 Direction)
{
    Ray r;
    r.origin=Origin;
    r.direction=Direction;
    return r;
}
vec3 Ray_PointTo(Ray ray, float t)
{
    return ray.origin + t * ray.direction;
}
/**********************************/
struct Material_Ray
{
    int materialType;
    vec3 albedo;
    float refractive_Indices;
};
Material_Ray Material_Ray_Con(int MaterialType,vec3 al, float Refractive_Indices)
{
    Material_Ray newMa;
    newMa.materialType = MaterialType;
    newMa.albedo=  al;
    newMa.refractive_Indices=  Refractive_Indices;
    return newMa;
}
/**********************************/
/*Hitable*/
/*Hit record*/
struct HitRecord
{
    float t; //  t
    vec3 p; //  point
    vec3 normal; 
    Material_Ray material_ray;
};
/***************/
/*Sphere: first, a sphere!*/
struct Sphere
{
    vec3 center;
    float radius;
    Material_Ray material_ray;
};
Sphere Sphere_Con(vec3 Center, float Radius, Material_Ray ma)
{
    Sphere newSp;
    newSp.center=Center;newSp.radius=Radius;
    newSp.material_ray = ma;
    return newSp;
}
bool Sphere_Hit(Sphere sp, Ray ray, 
    float tMin, float tMax, out HitRecord rec)
{
    rec.material_ray = sp.material_ray;
    vec3 oc = ray.origin - sp.center;
    float a = dot(ray.direction,ray.direction);
    float b = dot(oc,ray.direction);
    float c = dot(oc,oc)-sp.radius*sp.radius;
    float discriminant = b*b-a*c;
    if(discriminant > 0)
    {
        float temp = (-b-sqrt(discriminant))/a;
        if(temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = Ray_PointTo(ray,rec.t);
            rec.normal = (rec.p - sp.center) / sp.radius;
            return true;
        }
        temp = (-b+sqrt(discriminant))/a;
        if(temp < tMax && temp > tMin){
    // rec.material_ray = sp.material_ray;
            rec.t = temp;
            rec.p = Ray_PointTo(ray,rec.t);
            rec.normal = (rec.p - sp.center) / sp.radius;
            return true;
        }
        return false;
    }
    return false;
}

bool SphereList_Hit(Sphere sp[20], int spSize, Ray ray, 
    float tMin, float tMax, out HitRecord rec)
{
    HitRecord recTmp;
    bool isHited = false;
    float closet_so_far = tMax;
    for (int i = 0; i < spSize; i++)
    {
        if (Sphere_Hit(sp[i], ray, tMin, closet_so_far, recTmp))
        {
            isHited = true;
            closet_so_far = recTmp.t;
            rec = recTmp;
        }
    }
    return isHited;
}

/***************/
/*Material*/
bool Refract(vec3 vin, vec3 normal, float refRate, out vec3 refracted)
{
    vec3 uvin = normalize(vin);
    float dt = dot(uvin, normal);
    float discrimination = 1 - refRate * refRate * (1 - dt * dt);
    if (discrimination > 0)
    {
        refracted = refRate * (uvin - normal * dt) - normal * sqrt(discrimination);
        return true;
    }
    return false;
}
float Schlick(float cosine, float refRate)
{
    float r0 = (1 - refRate) / (1 + refRate);
    r0 *= r0;
    return r0 + (1 - r0) *  pow((1 - cosine), 5);
}
vec3 random_in_unit_ball()
{
    vec3 random_in_unit_ball_re;
    do
    {
        random_in_unit_ball_re = 2.0f * vec3(Rand(),Rand(),Rand()) - vec3(1.0, 1.0, 1.0);
    } while (length(random_in_unit_ball_re) > 1.0f);
    return random_in_unit_ball_re;
}
bool Scatter_Lambertian(Material_Ray material_ray,Ray rayIn, HitRecord rec, out vec3 attenuation, out Ray scattered)
{
    vec3 target;
    target = rec.p + rec.normal + random_in_unit_ball();
    
    scattered = Ray_Con(rec.p, target - rec.p);
    attenuation = material_ray.albedo;
    return true;
}
bool Scatter_Metal(Material_Ray material_ray,Ray rayIn, HitRecord rec, inout vec3 attenuation, inout Ray scattered)
{
    vec3 reflected = reflect(rayIn.direction, rec.normal);
    scattered = Ray_Con(rec.p, reflected);
    attenuation = material_ray.albedo;
    return (dot(scattered.direction,rec.normal) > 0);
}
bool Scatter_Dielectric(Material_Ray material_ray,Ray rayIn, HitRecord rec, out vec3 attenuation, out Ray scattered)
{
    vec3 refracted;
    vec3 reflected = reflect(rayIn.direction, rec.normal);
    attenuation = vec3(1.0,1.0,1.0);

    float cosine;

    bool isRefracted;

    if (dot(rayIn.direction, rec.normal) > 0)
    {
        isRefracted = Refract(rayIn.direction, -rec.normal, material_ray.refractive_Indices, refracted);
        cosine = material_ray.refractive_Indices * dot(normalize(rayIn.direction), rec.normal);
    }
    else 
    {
        isRefracted = Refract(rayIn.direction, rec.normal, 1.0f / material_ray.refractive_Indices, refracted);
        cosine = -dot(normalize(rayIn.direction), rec.normal);
    }

    if (isRefracted)
    {
        if (Rand() <= Schlick(cosine, material_ray.refractive_Indices))
        {
            isRefracted = false;
        }

    }
    // 判断是否全反射
    scattered = Ray_Con(rec.p, isRefracted == true ? refracted : reflected);
    return true;
}
/***************/
/**********************************/

vec3 Irradiance(Ray ray, Sphere sp, int depth)
{
    vec3 reColor = vec3(1.0,1.0,1.0);
    while(0 < depth--)
    {
        HitRecord rec;
        if(Sphere_Hit(sp, ray, 0.001, 10000, rec))
        {
            vec3 attenuation;
            Ray scatterRay;
            bool isScatter=false;
            switch(sp.material_ray.materialType)
            {
                case 0:
                    isScatter=Scatter_Lambertian(sp.material_ray, ray, rec, attenuation,scatterRay);
                    break;
                case 1:
                    isScatter=Scatter_Metal(sp.material_ray, ray, rec, attenuation,scatterRay);
                    break;
                default:
                    break;
            }
            if(isScatter)
            {
                // reColor *= 0.5 * vec3(rec.normal + vec3(1,1,1)); //  normal test
                reColor *= attenuation;
                ray = scatterRay;
            }
            else
            {
                break;
            }
        }
        else
        {
            vec3 nor = ray.direction;
            float t = 0.5 + 0.5 * nor.y;
            reColor *= (1-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
            break;
        }
    }
    return reColor;
}
vec3 Irradiance_List(Ray ray, Sphere sp[20], int spSize, int depth)
{
    vec3 reColor = vec3(1.0,1.0,1.0);
    while(0 < depth--)
    {
        HitRecord rec;
        if(SphereList_Hit(sp,spSize, ray, 0.01, 10000, rec))
        {
            vec3 attenuation;
            Ray scatterRay;
            bool isScatter=false;
            switch(rec.material_ray.materialType)
            {
                case 0:
                    isScatter=Scatter_Lambertian(rec.material_ray, ray, rec, attenuation, scatterRay);
                    break;
                case 1:
                    isScatter=Scatter_Metal(rec.material_ray, ray, rec, attenuation, scatterRay);
                    break;
                case 2:
                    isScatter = Scatter_Dielectric(rec.material_ray, ray, rec, attenuation, scatterRay);
                    break;
                default:
                    break;
            }
            if(isScatter)
            {
                // reColor *= 0.5 * vec3(rec.normal + vec3(1,1,1)); //  normal test
                reColor *= attenuation;
                ray = scatterRay;
            }
            else
            {
                vec3 nor = ray.direction;
                float t = 0.5 + 0.5 * nor.y;
                reColor *= (1-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
                break;
            }
        }
        else
        {
            vec3 nor = ray.direction;
            float t = 0.5 + 0.5 * nor.y;
            reColor *= (1-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
            break;
        }
    }
    return reColor;
}
/**********************************/
/*Camera*/
Ray CreateRayFromCamera(vec2 uv)
{
    Ray newRay = Ray_Con(_camera.lookFrom,
     _camera.left_buttom + uv.x * _camera.horizontal 
     + uv.y * _camera.vertical - _camera.lookFrom);
    return newRay;
}

/*unit color*/
void NormalColor(inout vec3 Color)
{
    Color.x = min(1.0, max(0.0, Color.x));
    Color.y = min(1.0, max(0.0, Color.y));
    Color.z = min(1.0, max(0.0, Color.z));
}

vec3 RayTracingFrag()
{
    Sphere sphere1 = Sphere_Con(vec3(0,0,-1.5),0.5, Material_Ray_Con(1, vec3(1.0,0.8,0.6),1.7));
    Sphere sphere2 = Sphere_Con(vec3(-1.0,0,-1.5),0.5, Material_Ray_Con(0, vec3(0.9,0.5,0.4),1.7));
    Sphere sphere3 = Sphere_Con(vec3(1.0,0,-1.5),0.5, Material_Ray_Con(2, vec3(0.5,0.8,1.0),1.7));
    Sphere sphere4 = Sphere_Con(vec3(0,-1000.6,-1.0),1000.0,Material_Ray_Con(0, vec3(1.0,0.8,0.6),1.7));

    Sphere sphereList[20];
    sphereList[0] = sphere1;
    sphereList[1] = sphere2;
    sphereList[2] = sphere3;
    sphereList[3] = sphere4;

    vec2 uv;
    uv.x = (gl_FragCoord.x) / _screen_size.x;
    uv.y = (gl_FragCoord.y) / _screen_size.y;

    vec3 color;
    Ray ray;
    int i=0;
    int maxSampeler = _maxSampeler;
    while(i < maxSampeler)
    {
        vec2 randVec = vec2(Rand(),Rand());
        uv.x = (gl_FragCoord.x+randVec.x) / _screen_size.x;
        uv.y = (gl_FragCoord.y+randVec.y) / _screen_size.y;
        ray = Ray_Con(_camera.lookFrom,
            _camera.left_buttom + (uv.x) * _camera.horizontal 
            + (uv.y) * _camera.vertical - _camera.lookFrom);
        // color += Irradiance(ray, sphere1, 3);
        color += Irradiance_List(ray, sphereList, 4, 6);
        i++;
    }
    color /= maxSampeler;
    color = sqrt(color);
    NormalColor(color);
    return color;
}

void main()
{
    vec3 color;
    color += RayTracingFrag();
    NormalColor(color);
    FragColor = vec4(color,1);
}
