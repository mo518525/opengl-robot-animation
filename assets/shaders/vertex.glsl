#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 ModelView;
uniform mat4 Rotate;

out vec3 FragPos;      // Weltposition
out vec3 Normal;       // Normale im Welt-Raum
out vec3 VertColor;    // Farbe vom Vertex

void main()
{
    // gesamte Modellmatrix (globale Rotation + lokale Transformation)
    mat4 model = Rotate * ModelView;

    vec4 worldPos = model * vec4(inPos, 1.0);
    FragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * inNormal);

    VertColor = inColor;

    gl_Position = Projection * View * worldPos;
}
