/*
 * fragment shader for points-vert
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


struct Texture {
   float density;
   float radius;
   vec3 backgroundColor;
   vec3 circleColor;
   bool shouldDiscard;
};

// output - transformed to eye coordinates (EC)
in vec4 position_EC;
in vec3 normal_EC;

// output: fragment/pixel color
out vec4 outColor;


uniform PhongMaterial phong;
uniform vec3 ambientLightIntensity;

uniform PointLight light;
uniform Texture texture;

uniform mat4 viewMatrix;

uniform vec3 lightIntensity;
in vec2 fragTexCoord;
vec3 color;
/*
 *  Calculate surface color based on Phong illumination model.silhoullette
 */

vec3 myphong(vec3 n, vec3 v, vec3 l) {
    vec3 ambientColor;
    vec3 diffuseColor;
    //mod(foo, dichte) < radius
    float radius = texture.density/2*texture.radius;
    float denisty = texture.density;

    float mid_x = denisty/2;
    float mid_y = denisty/2;
    float coord_x = mod(fragTexCoord.x, denisty);
    float coord_y = mod(fragTexCoord.y, denisty);
    float distance = sqrt(pow(coord_x-mid_x, 2)+pow(coord_y-mid_y, 2));
    bool changeColor = distance <= radius;

    if(changeColor && texture.shouldDiscard)
        discard;

    ambientColor = changeColor ? texture.circleColor : phong.k_ambient;
    diffuseColor = changeColor ? texture.backgroundColor : phong.k_diffuse;

    // cosine of angle between light and surface normal.
    float ndotl = dot(n,l);

    // ambient / emissive part
    vec3 ambient = vec3(0,0,0);
    if(light.pass == 0) // only add ambient in first light pass
        ambient = ambientColor * ambientLightIntensity;

    // surface back-facing to light?
    if(ndotl<=0.0)
        return ambient;
    else
        ndotl = max(ndotl, 0.0);

    // diffuse term
    vec3 diffuse =  diffuseColor * light.intensity * ndotl;

    // reflected light direction = perfect reflection direction
    vec3 r = reflect(-l,n);

    // cosine of angle between reflection dir and viewing dir
    float rdotv = max( dot(r,v), 0.0);

    // specular contribution + gloss map
    vec3 specular = phong.k_specular * light.intensity * pow(rdotv, phong.shininess);

    // return sum of all contributions
    color= ambient + diffuse + specular;
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
}
