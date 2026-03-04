#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 objectColor = VertColor;

    // Ambient
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor * objectColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    // Specular
    float specularStrength = 0.4;
    int shininess = 32;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
