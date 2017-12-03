/*
 *
 * vertex shader for simple point
 *
 */

#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

// in: position and normal vector in model coordinates (_MC)
in vec3 position_MC;
in vec3 normal_MC;

// position + normal vector in eye coordinates (_EC)
out vec4 position_EC;
out vec3 normal_EC;

//for texture
in vec2 texcoord;
out vec2 fragTexCoord;


void main(void) {

    // position to eye coordinates
    position_EC = modelViewMatrix * vec4(position_MC,1);

    // normal to eye coordinates
    normal_EC = normalMatrix * normal_MC;

    // position to clip coordinates
    gl_Position = projectionMatrix * position_EC;

    fragTexCoord  = texcoord;

}
