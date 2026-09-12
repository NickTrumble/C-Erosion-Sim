#version 330 core

in vec2 textureCoordinate;

//taken from cpp files
uniform sampler2D heightMap;
uniform float minHeight;
uniform float maxHeight;
uniform int colourMapType;

out vec4 fragmentColour;

vec3 terrainColour(float height)
{
    const vec3 colours[7] = vec3[7](
        vec3(18.0, 52.0, 86.0) / 255.0,
        vec3(52.0, 118.0, 168.0) / 255.0,
        vec3(210.0, 190.0, 140.0) / 255.0,
        vec3(91.0, 145.0, 76.0) / 255.0,
        vec3(43.0, 98.0, 57.0) / 255.0,
        vec3(125.0, 116.0, 105.0) / 255.0,
        vec3(238.0, 243.0, 246.0) / 255.0
    );

    float position = height * 6.0;
    int lower = int(floor(position));
    int upper = min(lower + 1, 6);

    return mix(colours[lower], colours[upper], fract(position));
}

vec3 heatColour(float height)
{
    const vec3 colours[5] = vec3[5](
        vec3(25.0, 38.0, 120.0) / 255.0,
        vec3(32.0, 177.0, 208.0) / 255.0,
        vec3(245.0, 218.0, 58.0) / 255.0,
        vec3(239.0, 119.0, 35.0) / 255.0,
        vec3(190.0, 36.0, 39.0) / 255.0
    );

    float position = height * 4.0;
    int lower = int(floor(position));
    int upper = min(lower + 1, 4);

    return mix(colours[lower], colours[upper], fract(position));
}

void main()
{
    float height = texture(heightMap, textureCoordinate).r;

    float range = max(maxHeight - minHeight, 0.0001);
    float normalisedHeight = clamp(
        (height - minHeight) / range,
        0.0,
        1.0
    );

    vec3 colour;

    if (colourMapType == 1) {
        colour = terrainColour(normalisedHeight);
    } else if (colourMapType == 2) {
        colour = heatColour(normalisedHeight);
    } else {
        colour = vec3(normalisedHeight);
    }

    //rgb 
    fragmentColour = vec4(colour, 1.0);
}