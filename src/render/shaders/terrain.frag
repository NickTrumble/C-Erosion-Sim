#version 330 core

in vec2 textureCoordinate;

//taken from  c++
uniform sampler2D heightMap;
uniform float minHeight;
uniform float maxHeight;

out vec4 fragmentColour;


void main(){
    float height = texture(heightMap, textureCoordinate).r;

    float range = max(maxHeight - minHeight, 0.0001);
    float normalisedHeight = clamp(
        (height - minHeight) / range,
        0.0,
        1.0
    );

    //rgb
    fragmentColour = vec4(vec3(normalisedHeight), 1.0);
}