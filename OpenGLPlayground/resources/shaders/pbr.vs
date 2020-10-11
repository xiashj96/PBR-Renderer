#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 FragPos;   // in world space
out vec2 TexCoords;
out mat3 TBN;    // for normal mapping


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    // create TBN matrix
	vec3 T = normalize(mat3(model) * aTangent);
	vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal);
	// gram-schmidt process
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);
}