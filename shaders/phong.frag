/*
 * fragment shader for phong + textures + bumps
 *
 */

#version 150

// output - transformed to eye coordinates (EC)
in vec4 position_EC;
in vec3 normal_EC;

// output: fragment/pixel color
out vec4 outColor;

struct PhongMaterial {
    vec3 k_ambient;
    vec3 k_diffuse;
    vec3 k_specular;
    float shininess;

};
uniform PhongMaterial phong;
uniform vec3 ambientLightIntensity;

struct PointLight {
    vec3 intensity;
    vec4 position_WC;
    int  pass;
};
uniform PointLight light;

uniform mat4 viewMatrix;

/*
 *  Calculate surface color based on Phong illumination model.
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

void main() {

    // calculate all required vectors in camera/eye coordinates
    vec4 lightpos_EC = viewMatrix * light.position_WC;
    vec3 lightdir_EC = (lightpos_EC   - position_EC).xyz;
    vec3 viewdir_EC  = (vec4(0,0,0,1) - position_EC).xyz;

    // calculate color using phong, all vectors in eye coordinates
    vec3 final_color = myphong(normalize(normal_EC),
                               normalize(viewdir_EC),
                               normalize(lightdir_EC));

    // set output
    outColor = vec4(final_color, 1.0);
    // outColor = vec4(1,0,0, 1.0);

}
