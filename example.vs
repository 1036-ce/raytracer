// Constant variables
#define PI      3.141592653589793
#define SAMPLES 2

// Materials
struct material {
    vec3 albedo;
    vec3 emission;
    float roughness;
    float specular;
	float metalness;
};

// Planes
struct Plane {
    vec3 p;
    vec3 n;
    material mat;
};

// Rays
struct Ray {
    vec3 orig;
    vec3 dir;
};

// Hittables
struct HitRecord {
    vec3 p;
    vec3 normal;
    float t;
    material mat;
};

// Spheres
struct Sphere {
    vec3 center;
    float radius;
    material mat;
};


// To create the scene
const int NUM_SPHERES = 5;
const int NUM_PLANES = 5;

Sphere spheres[NUM_SPHERES];
Plane planes[NUM_PLANES];

void init_scene() {

    // Light Sphere
	material lightSurface;
	lightSurface.albedo = vec3(1.0);
	lightSurface.emission = vec3(70.0);
	lightSurface.roughness =1.0;
	lightSurface.specular = 1.0;
	lightSurface.metalness = 1.0;
    spheres[0] = Sphere(vec3(0.0, 6.8, -2.0), 4.1, lightSurface);
    
    // Diffuse sphere
	material diffuseSurface;
	diffuseSurface.albedo = vec3(0.1, 0.1 ,0.10);
	diffuseSurface.emission = vec3(0.0);
	diffuseSurface.roughness = 0.9;
	diffuseSurface.specular = 0.1;
	diffuseSurface.metalness = 0.0;
    spheres[1] = Sphere(vec3(1.3, -1.5, -2.2), 1.5, diffuseSurface);
           
    // Metal sphere
	material metalSurface;
	metalSurface.albedo = vec3(0.5, 0.5 ,0.5);
	metalSurface.emission = vec3(0.0);
	metalSurface.roughness = 1.0;
	metalSurface.specular = 1.0;
	metalSurface.metalness = 1.0;
    spheres[2] = Sphere(vec3(0.0, -2.3, -0.8), 0.7, metalSurface);
    
    // Magenta metal sphere
	metalSurface.albedo = vec3(1.0, 0.1 ,1.0);
    spheres[3] = Sphere(vec3(-2.0, 1.3, -1.8), 0.9, metalSurface);
    
    // Cyan metal sphere
	metalSurface.albedo = vec3(0.2, 1.0 ,1.0);
    spheres[4] = Sphere(vec3(-1.5, -2.3, -3.0), 0.6, metalSurface);
   
    // Room material
	material roomSurface;
	roomSurface.albedo = vec3(0.8);
	roomSurface.emission = vec3(0.0);
	roomSurface.roughness = 1.0;
	roomSurface.specular = 0.0;
	roomSurface.metalness = 0.0;
    
	// Floor
    planes[0] = Plane(vec3(0.0, -3.0, 0.0), vec3(0,1,0), roomSurface);

	// Ceiling
    planes[1] = Plane(vec3(0.0, 3.0, 0.0), vec3(0.0,-1.0,0.0), roomSurface);
                     
    // Back wall            
    planes[2] = Plane(vec3(0, 0, -5), vec3(0,0,1.0), roomSurface);
                      
    // Left wall (red)
	roomSurface.albedo = vec3(0.95, 0, 0);
    planes[3] = Plane(vec3(-3, 0.0, 0.0), vec3(1.0,0.0,0.0), roomSurface);
    
    // Right wall (green)
	roomSurface.albedo = vec3(0.0, 0.95, 0);
    planes[4] = Plane(vec3(3.0, 0.0, 0.0), vec3(-1.0,0,0), roomSurface);
}


// -------------------------------------------------------------------------
// Randoms
// We have a applied these commonly used randomizer function from Shadertoy
// Source: https://www.shadertoy.com/view/XtyyDy
// -------------------------------------------------------------------------

vec2 randState;

float rand2D() {
    randState.x = fract(sin(dot(randState.xy + iTime, vec2(12.9898, 78.233))) * 43758.5453);
    randState.y = fract(sin(dot(randState.xy + iTime, vec2(12.9898, 78.233))) * 43758.5453);

    return randState.x;
}

vec3 random_in_unit_sphere() {
    // Returns a random direction in unit sphere (used in the BRDF)
    float phi = 2.0 * PI * rand2D();
    float cosTheta = 2.0 * rand2D() - 1.0;
    float u = rand2D();

    float theta = acos(cosTheta);
    float r = pow(u, 1.0 / 3.0);

    // Change of variables 
    // Spherical Coordinates -> Carthesian Coordinates, to get (x, y, z) values 
    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    return vec3(x, y, z);
}

// -------------------------------------------------------------------------
// Creating the BRDF (Bidirectional Reflective Distribution Function)
// In this section we create our three functions that are used in the Cook-Torrance Reflectance Equation
// These are: The Normal Distribution Function, The Geometry Function and The Fresnel Equation.
// -------------------------------------------------------------------------

// The Normal Distribution Function
float DistributionGGX(vec3 N, vec3 H, float a) {
    // N, normal of the surface
    // H, halfway vector
    // a, surface roughness
    
    // We are doing our approximations based on the Trowbridge-Reits GGX:
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    // The normal distribution function returns a value indicating how much o the surface's
    // microfacets are aligned to the incoming halfway vector.
    return nom / denom;
}

// The Geometry Function
float GeometrySchlickGGX(float NdotV, float k) {
    // NdotV, dot product between normal and direction of incoming light
    // k, a remapping of the surface roughness, where:
    // k = (surface roughness)^2/2, for IBL (image based lighting).
    
    // We are doing our approximations based on Schlick-GGX:
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    /* 
     The Geometry Function returns a value between 0 and 1 indicating where its 
     micro surface-details overshadows each other, causing light rays to be occluded. 
     This will affect how sharp the shadows of an object appears. 
    */
    return ggx1 * ggx2;
}

// The Fresnel Equation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    // cosTheta, the dot product between the surface's normal and the halfway vector.
    // F0, vector describing the base reflectivity of the surface

    /* 
     We are doing our approximation based on the Fesnel-Schlick:
     The Fresnel Equtation describes the ration of light that gets reflected,
     over the light that gets reflacted. 
    */
    
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

bool sphereIntersection(in Ray r,out HitRecord hit, float maxT) {
    /* 
     Function used to find intersection between the traced ray and spheres.
     r, the traced ray
     hit, current hitrecod
     maxT, largest distance
    */
    
    float minT = 0.0001;
    float currentT = maxT;
    vec3 normal;
    material mat;

    for (int i = 0; i < spheres.length(); ++i) { // Loop through all spheres
        // Code for ray-spehere intersection, the intersection is found by combining the equations and solve for t.
        vec3 oc = r.orig - spheres[i].center; // Difference ray origin and sphere's center.
        float a = dot(r.dir, r.dir);
        float b = 2.0 * dot(oc, r.dir);
        float c = dot(oc, oc) - spheres[i].radius * spheres[i].radius;
        float det = b*b - 4.0*a*c;
        float denominator = (2.0 * a);

        // Possible intersection
        if (denominator != 0.0 && det >= 0.0) {
            float t1 = (-b - sqrt(det)) / denominator;
            float t2 = (-b + sqrt(det)) / denominator;
            float t = min(t1, t2);
            if (minT < t && t < currentT) { 
                currentT = t;
                normal = normalize(r.orig + t * r.dir - spheres[i].center);
                mat = spheres[i].mat;
            }
        }  
    }
    if (currentT != maxT) {
        hit.t = currentT;
        hit.mat = mat;
        hit.normal = normal;
        return true;
    }
    else
        return false;
}

bool planeIntersection(in Ray r,out HitRecord hit, float maxT) {
    /*
    Checks for intersections with the plane obejcts in the scene.
    Updates hit attribute with relevant information. 
    */
    float minT = 0.0001;
    float currentT = maxT;
    vec3 normal;
    material mat;
    
    for (int j = 0; j < planes.length(); ++j) {
        vec3 p = planes[j].p;
        vec3 n = planes[j].n;

        float t = dot(p - r.orig, n) / dot(r.dir, n);
        if (minT < t && t < currentT) {
            currentT = t;
            normal = n;
            mat = planes[j].mat;
        }
    }
    if (currentT < maxT) {
        hit.t = currentT;
        hit.mat = mat;
        hit.normal = normal;
        return true;
    }
    else
        return false;
}


bool objectIntersection(in Ray r,out HitRecord hit, float maxT) {
    /*
    Sends a ray throught the scene and checks if any intersections occure.
    If the ray intersects a object the hit attribute is update with relevant
    information. 
    */
    HitRecord hitTemp;
    hit.t = maxT;
    
    bool rayIntersected = false;

        // Sphere intersection
    if (sphereIntersection(r, hitTemp, maxT)) {
        hit = hitTemp;
        rayIntersected = true;
    }

    // Plane intersection
    if (planeIntersection(r, hitTemp, hit.t)) {
        hit = hitTemp;
        rayIntersected = true;
    }
    
    return rayIntersected;
}

vec3 getReflection(Ray r, HitRecord hit) {
    /*
    Calculating the reflection direction based on the specular attribute of the material.
    Either it is a perfect reflection (specular = 1), a perfect diffuse (specular = 0) or
    something in between.
    */
    vec3 diffuseDir = normalize(hit.normal + random_in_unit_sphere());
    vec3 specularDir = reflect(r.dir, hit.normal);

    vec3 nextDir = mix(diffuseDir, specularDir, hit.mat.specular);
    return normalize(nextDir);
}

vec3 BDRFcontribution(Ray firstRay, Ray secondRay, HitRecord firstHit, vec3 Li) {
    /*
    This function calculates the Cook-Torrance BRDF contribution at an intersection point. 
    We use the method described at https://learnopengl.com/PBR/Theory, which is the same 
    method of calculations as in Unreal Engine 4.
    */

    vec3 halfway = firstRay.dir + firstHit.normal;
    float cosTheta = dot(secondRay.dir, firstHit.normal); // Angle between outgoing ray and surface normal
            
    float D = DistributionGGX(firstHit.normal, halfway, firstHit.mat.roughness); // The microfacet normals influence
    
    float k = (firstHit.mat.roughness * firstHit.mat.roughness) / 2.0; // Remapping of roughness for IBL
    float G = GeometrySmith(firstHit.normal, -firstRay.dir, secondRay.dir, k); // The surface overshadowing
    
    vec3 F0 = vec3(0.04); // The reflectivity when looking at the surface at a 0 degrees angle
    F0 = mix(F0, firstHit.mat.albedo, firstHit.mat.metalness); // More reflectivity depending on metalness
    vec3 F = fresnelSchlick(cosTheta, F0); // The amount of reflectivity depending on viewing angle

    vec3 numerator = D * G * F;
    float denominator = (4.0 * max(dot(-firstRay.dir, firstHit.normal), 0.0) * max(dot(secondRay.dir, firstHit.normal), 0.0));
    vec3 specular = numerator / max(denominator, 0.0001); 

    float NdotL = max(dot(secondRay.dir, firstHit.normal), 0.0);  
    return ((1.0-firstHit.mat.specular) * (firstHit.mat.albedo / PI) + specular) * Li/float(SAMPLES) * NdotL;
}

vec3 traceRay(Ray primaryRay) {
    /*
    Traces primary ray in the scene and allows the ray to bounce up to 4 times.
    The reflected light is approximated by calculating the sum of incoming light
    which in turn is the sum of reflected light at the next ray intersections.
    Depending on the SAMPLES-variable this function may take a long time to calculate
    and might freeze your computer, therfore we recommend to use no value larger than 10.
    */

    float minT = 0.0001; // Minimum distance between ray origin and intersection
    float maxT = 10e10; // Largest distance
    
    HitRecord primaryHit; // Struct for storing information about the intersection point
  
    // trace primary ray and see if we hit something       
    vec3 L0 = vec3(0.0); // The total emitted and reflected light at the first intersection point
    if (objectIntersection(primaryRay, primaryHit, maxT)) {

        HitRecord secondHit;
        secondHit.t = maxT;

        for(int depth1 = 0; depth1 < SAMPLES; ++depth1) {
            vec3 hitPoint = primaryRay.orig + primaryHit.t * primaryRay.dir;
            vec3 nextDir = getReflection(primaryRay, primaryHit);
            
            Ray secondaryRay = Ray(hitPoint, nextDir);
                  
            vec3 L1 = vec3(0.0);
            if(objectIntersection(secondaryRay, secondHit, maxT)) {
                HitRecord thirdHit;
                thirdHit.t = maxT;
                
                for(int depth2 = 0; depth2 < SAMPLES; ++depth2) {
                    hitPoint = secondaryRay.orig + secondHit.t * secondaryRay.dir;
                    nextDir = getReflection(secondaryRay, secondHit);
                       
                    Ray thridRay = Ray(hitPoint, nextDir);
                    
                    objectIntersection(thridRay, thirdHit, maxT);
                    
                    vec3 L2 = vec3(0.0);
					if(objectIntersection(thridRay, thirdHit, maxT)) {
						HitRecord fourthHit;
                		fourthHit.t = maxT;

						for(int depth3 = 0; depth3 < SAMPLES; ++depth3) {
							hitPoint = thridRay.orig + thirdHit.t * thridRay.dir;
							nextDir = getReflection(thridRay, thirdHit);
							Ray fourthRay = Ray(hitPoint, nextDir);
						
							objectIntersection(fourthRay, fourthHit, maxT);

							vec3 L3 = fourthHit.mat.emission;
							L2 += BDRFcontribution(thridRay, fourthRay, thirdHit, L3); // Adding Li sample contribution
						}
						L2 += thirdHit.mat.emission;
					}
					L1 += BDRFcontribution(secondaryRay, thridRay, secondHit, L2);
                }
                L1 += secondHit.mat.emission;
            }
            L0 += BDRFcontribution(primaryRay, secondaryRay, primaryHit, L1);
        }
        L0 += primaryHit.mat.emission;
        primaryHit.t = maxT;
    }

    return L0;
}



// -------------------------------------------------------------------------
// Frame accumulator
// -------------------------------------------------------------------------

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    // Setting an initial random state for the fragment
    randState = fragCoord.xy / iResolution.xy;
    vec2 uv = fragCoord.xy /iResolution.xy;
    
    if (iFrame == 0) {
        // Setting the color of the first frame to 0 to clear the buffer
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        // Image
        // This method for creating the camera model is mainly taken from
        // Peter Shirleys book called Ray Tracing in One Weekend.
        float aspectRatio = iResolution.x / iResolution.y;
        float imageWidth = 400.0;
        float imageHeight = trunc(imageWidth / aspectRatio);

        // Camera
        float viewportHeight = 2.0;
        float viewportWidth = aspectRatio * viewportHeight;
        float focalLength = 10.5;

        vec3 origin = vec3(0.0, 0.0, 8.0); // Camera center
        vec3 horizontal = vec3(viewportWidth/aspectRatio, 0, 0);
        vec3 vertical = vec3(0, viewportHeight, 0);
        vec3 lowerLeftCorner = origin - horizontal/2.0 - vertical/2.0 - vec3(0.0, 0.0, focalLength);
    
        // Initalize the scene geometry
        init_scene();
        
        // Calculate Ray
        vec2 pixelSize = (1.0 / iResolution.xy);
        vec2 randomJitter = vec2(rand2D(), rand2D());
        vec2 jitter = vec2(pixelSize * (randomJitter - 0.5)); // Used in the direction of the primary ray for a nicer sampeling
        
        vec3 primaryRayDir = normalize(lowerLeftCorner * vec3(aspectRatio, 1.0, 1.0) + (uv.x + jitter.x) * horizontal * aspectRatio + (uv.y + jitter.y) * vertical);
        Ray r = Ray(origin, primaryRayDir);
        
        // Trace primary ray in scene
        vec3 pixelColor = traceRay(r);
        
        // Fetching the previous image frame
        vec3 prev = texture(iChannel0, uv).xyz;

        // Averaging the new frame together with the prevoius frame
        vec3 new = (float(iFrame-1) * prev + pixelColor) / float(iFrame);
        
        // Output fragment color
        fragColor = vec4(new, 1.0);
    }
}
