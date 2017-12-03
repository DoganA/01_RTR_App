/*
 * fragment shader
 *
 */

#version 150
struct PhongMaterial {
    vec3 k_ambient;
    vec3 k_diffuse;
    vec3 k_specular;
    float shininess;

};
struct PointLight {
    vec3 intensity;
    vec4 position_WC;
    int  pass;
};

struct ToonShader {
   bool toon;
   bool silhoutte;
   float threshold;
   int discretize;
};

struct Texture {
   int dichte;
   float radius;
   vec3 backgroundColor;
   vec3 circleColor;
};

// output - transformed to eye coordinates (EC)
in vec4 position_EC;
in vec3 normal_EC;

// output: fragment/pixel color
out vec4 outColor;


uniform PhongMaterial phong;
uniform vec3 ambientLightIntensity;


uniform ToonShader toonShader;
uniform PointLight light;
uniform Texture texture;

uniform mat4 viewMatrix;
uniform vec3 color;
uniform vec3 lightIntensity;


/*
 *  Calculate surface color based on Phong illumination model.silhoullette
 */

vec3 myphong(vec3 n, vec3 v, vec3 l) {

    // cosine of angle between light and surface normal.
    float ndotl = dot(n,l);

    // ambient / emissive part
    vec3 ambient = vec3(0,0,0);
    if(light.pass == 0) // only add ambient in first light pass
        ambient = phong.k_ambient * ambientLightIntensity;

    // surface back-facing to light?
    if(ndotl<=0.0)
        return ambient;
    else
        ndotl = max(ndotl, 0.0);

    // diffuse term
    vec3 diffuse =  phong.k_diffuse * light.intensity * ndotl;

    // reflected light direction = perfect reflection direction
    vec3 r = reflect(-l,n);

    // cosine of angle between reflection dir and viewing dir
    float rdotv = max( dot(r,v), 0.0);

    // specular contribution + gloss map
    vec3 specular = phong.k_specular * light.intensity * pow(rdotv, phong.shininess);

    // return sum of all contributions
    return ambient + diffuse + specular;

}

vec3 getDiscretizedColor( int i,float diffuse,vec3 colorX){
        float step = i/10.0f;
        colorX = diffuse < step ? colorX
                                  * step :colorX;
    return colorX;
}
vec3 toon(vec3 normal, vec3 cam, vec3 light, vec3 intensity, vec3 color) {

    if(toonShader.silhoutte && max(dot(cam, normal), 0.0) < toonShader.threshold)
    {
        color = vec3(0.5,0.6,0.3);
    }
    else
    {
        // diffuse
        float diffuse = max(dot(normal,light),0.0);

        for(int i = 0; i < toonShader.discretize; i++){
             color = getDiscretizedColor(i,diffuse, color);
        }

        // spec (highest prio)
        vec3 reflection = normalize(reflect(-cam, normal));
        float spec = pow(max(0.0, dot(reflection, light)), 10.0);
        // more than half highlight intensity?
        if (spec > 0.5)
            color = vec3(1, 1, 1);
   }

    return color;

}


void main() {

    // calculate all required vectors in camera/eye coordinates
    vec4 lightpos_EC = viewMatrix * light.position_WC;
    vec3 lightdir_EC = (lightpos_EC   - position_EC).xyz;
    vec3 viewdir_EC  = (vec4(0,0,0,1) - position_EC).xyz;

   vec3 phong_color =  myphong(normalize(normal_EC),
                           normalize(viewdir_EC),
                           normalize(lightdir_EC));
    outColor = vec4(phong_color,1);

    // toonize color
    if(toonShader.toon){
        // calculate color using phong, all vectors in eye coordinates

        outColor.rgb = toon(
            normalize(normal_EC),
            normalize(viewdir_EC),
            normalize(vec3(lightpos_EC)), light.intensity, phong_color);
        outColor.a = 1;
  }
}
