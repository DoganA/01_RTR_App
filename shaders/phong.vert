/*
 *
 * vertex shader for simple phong
 *
 */

#version 150

// transformation matrices
uniform mat4 modelViewProjectionMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

// in: position and normal vector in model coordinates (_MC)
in vec3 position_MC;
in vec3 normal_MC;

out vec4 position_EC;
out vec3 normal_EC;

void main(void) {

    // vertex/fragment position in eye coordinates
    position_EC = modelViewMatrix * vec4(position_MC,1);

    // position in clip coordinates
    gl_Position  = projectionMatrix * position_EC;

    // normal direction in eye coordinates
    normal_EC  = normalMatrix * normal_MC;

}


