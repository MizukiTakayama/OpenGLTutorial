

/// フラグメントシェーダ
#version 400

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;

void main() {
	vec3 lightVec = vertexData.lightPosition.xyz - vPosition;
	float lightPower = 1 / dot(lightVec, lightVec);
	float cosTheta = clamp(dot(vec3(0,0,1), normalize(lightVec)), 0, 1);
	outColor = vColor;
	outColor.rgb *=
		(cosTheta * lightPower * vertexData.lightColor.rgb) +
		vertexData.ambientColor.rgb;

	outTexCoord = vTexCoord;
	gl_Position = vertexData.matMVP * vec4(vPosition, 1);
}