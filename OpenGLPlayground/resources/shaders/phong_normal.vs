#version 330 core

// Phong-Blinn Shader with normal mapping
// calculate lighting in world space

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// create TBN matrix
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal);
	// gram-schmidt process
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	vs_out.TBN = mat3(T, B, N);
}