#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 21) out;

#define PI 3.141592653589793

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

out vec4 fragColor;

uniform int layer;
uniform vec4 topColor;
uniform vec4 btnColor;
uniform float width;
uniform float height;
uniform float time;
uniform float windDirection; // 0 ~ 360 
uniform float windIntensity; // 0 ~ 90

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

float rand(vec3 v)
{
    return fract(sin(dot(v.xyz, vec3(12.9898, 78.233, 53.539))) * 43758.5453);
}

mat4 rotateAxis(float angle, vec3 axis)
{
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - cos(angle);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat4
    (
        t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0,
        t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0,
        0, 0, 0, 1
    );
}

mat4 translate(float xOffset, float yOffset, float zOffset)
{
    return mat4
    (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        xOffset, yOffset, zOffset, 1
    );
}

void generateGrass(vec4 position)
{
    float randHeight = ((rand(position.xyz) * 2 - 1) * 0.1 + 1) * height;
    float randWidth = ((rand(position.yxz) * 2 - 1) * 0.1 + 1) * width;
    float randOffsetX = (rand(position.zxy) * 2 - 1) * (width);
    float randOffsetZ = (rand(position.zyx) * 2 - 1) * (width);
    int vertexCount = 2 * layer + 1;
    vec3 offset = {0.f, 0.f, 0.f};
    float inc = 1.f / layer;
    vec4 randPos = translate(randOffsetX, 0.f, randOffsetZ) * position;
    vec4 last[2];
    for(int i = 0; i < vertexCount; i++)
    {
        int level = i / 2;
        fragColor = btnColor + level * inc * (topColor - btnColor);
        mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        float windAngleY = (rand(position.zyx) * 2 - 1) * PI / 4 + radians(windDirection - 90);
        if(level == 0)
        {
            float rootAngleY = 2 * PI * rand(position.yzx);
            offset.x = (i % 2 == 0) ? - randWidth / 2 : randWidth / 2;
            offset.y = 0;
            vec4 t = randPos + rotateAxis(rootAngleY, vec3(0,1,0)) * vec4(offset, 0.f);
            gl_Position = mvp * t;
            last[i % 2] = randPos + rotateAxis(windAngleY, vec3(0,1,0)) * vec4(offset, 0.f);
        }
        else
        {
            float windAngleX = (sin(time) * 0.2 + 0.8) * (rand(position.xyz) * 2 - 1) * PI / 12 + radians(level * inc * windIntensity);
            offset.x = (i % 2 == 0) ? (randWidth * inc / 2) : - (randWidth * inc / 2);
            offset.y = inc * randHeight;
            vec4 t = last[i % 2] + rotateAxis(windAngleY, vec3(0, 1, 0)) * rotateAxis(windAngleX, vec3(1, 0, 0)) * vec4(offset,0.f);
            gl_Position = mvp * t;
            last[i % 2] = t;
        }
        EmitVertex();
    }
    EndPrimitive();
}

void main() {
    generateGrass(gl_in[0].gl_Position);
}