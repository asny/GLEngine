#version 150

uniform mat4 VMatrix;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat4 NMatrix;

uniform vec3 lightPos;

uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;

in vec3 position;
in vec3 normal;

out vec4 colourV;

void main()
{
    // Define material specs
    float specPow = 5.;
    
    // Compute vectors
    vec3 p = (MVMatrix * vec4(position, 1.)).xyz;
    vec3 N = normalize(mat3(NMatrix) * normal);
    vec3 L = normalize((VMatrix * vec4(lightPos, 1.f)).xyz - p);
    vec3 E = normalize(-p);
    vec3 R = normalize(reflect(-L,N));
    
    // Calculate colour
    vec4 ambient = ambientMat;
    vec4 diffuse = clamp( diffuseMat * max(dot(N,L), 0.0) , 0.0, 1.0 ) ;
    vec4 spec = clamp ( specMat * pow(max(dot(R,E), 0.0), specPow) , 0.0, 1.0 );
    colourV = ambient + diffuse + spec;
    
    // Calculate position
    gl_Position = MVPMatrix * vec4(position, 1.);
}
