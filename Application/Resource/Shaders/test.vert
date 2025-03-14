#version 330 core
// Shader types
const float SHADER_FILLED_CIRCLE = 2.0;
const float SHADER_STROKED_CIRCLE = 3.0;
const float SHADER_FONT = 4.0;
const float SHADER_LINE_A = 5.0;
const float SHADER_LINE_B = 6.0;
const float SHADER_LINE_C = 7.0;
const float SHADER_LINE_D = 8.0;
const float SHADER_LINE_E = 9.0;
const float SHADER_LINE_F = 10.0;

// Minimum line width
const float MIN_WIDTH = 1.0;

layout(location = 0)in vec3 a_position;
layout(location = 1)in vec4 a_color;
layout(location = 2)in vec4 a_shaderParams;

out vec4 v_shaderParams;
out vec2 v_circleCoords;
out vec4 v_color;
out vec2 v_texCoord;

// Uniforms
uniform float u_worldPixelSize;
uniform vec2 u_screenPixelSize;
uniform float u_pixelSizeMultiplier;
uniform float u_minLinePixelWidth;
uniform vec2 u_antialiasingOffset;
uniform mat4 u_modelViewProjectionMatrix; 

float roundr(float f, float r) {
    return floor(f / r + 0.5) * r;
}

vec4 roundv(vec4 x, vec2 t) {
    return vec4(roundr(x.x, t.x), roundr(x.y, t.y), x.z, x.w);
}

void computeLineCoords(bool posture, vec2 vs, vec2 vp, vec2 texcoord, vec2 dir, float lineWidth, bool endV) {
    float lineLength = length(vs);
    vec4 screenPos = u_modelViewProjectionMatrix * vec4(a_position, 1.0) + vec4(1, 1, 0, 0);
    float w = ((lineWidth == 0.0) ? u_worldPixelSize : lineWidth);
    float pixelWidth = roundr(w / u_worldPixelSize, 1.0);
    float aspect = (lineLength + w) / w;
    vec2 s = sign(vec2(u_modelViewProjectionMatrix[0][0], u_modelViewProjectionMatrix[1][1]));

    if (pixelWidth < 1.0)
        pixelWidth = 1.0;

    if (pixelWidth > 1.0 || u_pixelSizeMultiplier > 1.0) {
        vec2 offsetNorm = (vs + vp) * pixelWidth / lineLength * 0.5;
        vec4 screenOffset = vec4(s.x * offsetNorm.x * u_screenPixelSize.x, s.y * offsetNorm.y * u_screenPixelSize.y, 0, 0);
        vec4 adjust = vec4(-1, -1, 0, 0);

        if (mod(pixelWidth * u_pixelSizeMultiplier, 2.0) > 0.9) {
            adjust += vec4(u_screenPixelSize.x, u_screenPixelSize.y, 0, 0) * 0.5;
        }

        gl_Position = roundv(screenPos, u_screenPixelSize) + adjust + screenOffset;
        v_shaderParams[0] = SHADER_LINE_A;
    } else {
        vec4 pos0 = screenPos;
        pos0.xy += (posture ? dir.xy : dir.yx) * u_screenPixelSize / 2.0;

        if (posture) {
            pos0.y -= u_screenPixelSize.y * sign(vs.y) * 0.5;
        } else {
            pos0.x += u_screenPixelSize.x * sign(vs.x) * 0.5;
        }

        gl_Position = pos0 - vec4(1, 1, 0, 0);
        v_shaderParams[0] = SHADER_LINE_B;
    }

    v_shaderParams[1] = aspect;
    v_texCoord = vec2(aspect * texcoord.x, texcoord.y);
    v_color = a_color;
}

void computeCircleCoords(float mode, float vertexIndex, float radius, float lineWidth) {
    vec4 delta;
    vec4 center = roundv(u_modelViewProjectionMatrix * vec4(a_position, 1.0) + vec4(1, 1, 0, 0), u_screenPixelSize);
    float pixelWidth = roundr(lineWidth / u_worldPixelSize, 1.0);
    float pixelR = roundr(radius / u_worldPixelSize, 1.0);

    if (mode == SHADER_STROKED_CIRCLE)
        pixelR += pixelWidth / 2.0;

    vec4 adjust = vec4(-1, -1, 0, 0);

    if (pixelWidth < 1.0)
        pixelWidth = 1.0;

    if (vertexIndex == 1.0) {
        v_circleCoords = vec2(-sqrt(3.0), -1.0);
        delta = vec4(-pixelR * sqrt(3.0), -pixelR, 0, 0);
    } else if (vertexIndex == 2.0) {
        v_circleCoords = vec2(sqrt(3.0), -1.0);
        delta = vec4(pixelR * sqrt(3.0), -pixelR, 0, 0);
    } else if (vertexIndex == 3.0) {
        v_circleCoords = vec2(0.0, 2.0);
        delta = vec4(0, 2 * pixelR, 0, 0);
    } else if (vertexIndex == 4.0) {
        v_circleCoords = vec2(-sqrt(3.0), 0.0);
        delta = vec4(0, 0, 0, 0);
    } else if (vertexIndex == 5.0) {
        v_circleCoords = vec2(sqrt(3.0), 0.0);
        delta = vec4(0, 0, 0, 0);
    } else if (vertexIndex == 6.0) {
        v_circleCoords = vec2(0.0, 2.0);
        delta = vec4(0, 0, 0, 0);
    }

    v_shaderParams[2] = pixelR;
    v_shaderParams[3] = pixelWidth;

    delta.x *= u_screenPixelSize.x;
    delta.y *= u_screenPixelSize.y;

    gl_Position = center + delta + adjust;
    v_color = a_color;
}

void main() {
    float mode = a_shaderParams[0];
    v_shaderParams = a_shaderParams;
    float lineWidth = v_shaderParams.y;
    vec2 vs = v_shaderParams.zw;
    vec2 vp = vec2(-vs.y, vs.x);
    bool posture = abs(vs.x) < abs(vs.y);

    if (mode == SHADER_LINE_A)
        computeLineCoords(posture, -vs, vp, vec2(-1, -1), vec2(-1, 0), lineWidth, false);
    else if (mode == SHADER_LINE_B)
        computeLineCoords(posture, -vs, -vp, vec2(-1, 1), vec2(1, 0), lineWidth, false);
    else if (mode == SHADER_LINE_C)
        computeLineCoords(posture, vs, -vp, vec2(1, 1), vec2(1, 0), lineWidth, true);
    else if (mode == SHADER_LINE_D)
        computeLineCoords(posture, vs, -vp, vec2(-1, -1), vec2(1, 0), lineWidth, true);
    else if (mode == SHADER_LINE_E)
        computeLineCoords(posture, vs, vp, vec2(-1, 1), vec2(-1, 0), lineWidth, true);
    else if (mode == SHADER_LINE_F)
        computeLineCoords(posture, -vs, vp, vec2(1, 1), vec2(-1, 0), lineWidth, false);
    else if (mode == SHADER_FILLED_CIRCLE || mode == SHADER_STROKED_CIRCLE)
        computeCircleCoords(mode, v_shaderParams.y, v_shaderParams.z, v_shaderParams.w);
    else {
        gl_Position = u_modelViewProjectionMatrix * vec4(a_position, 1.0);
        v_color = a_color;
    }

    gl_Position.xy += u_antialiasingOffset;
}