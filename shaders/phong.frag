#version 330

uniform mat4 VMatrix;

uniform vec3 lightPos;

uniform vec3 ambientMat;
uniform vec3 diffuseMat;
uniform vec3 specMat;
uniform float opacity;

in vec3 pos;
in vec3 nor;

out vec4 fragColour;

void main(void)
{
    // Define material specs
    float specPow = 5.;
    
    // Compute vectors
    vec3 N = normalize(nor);
//    if(!gl_FrontFacing)
//    {
//        N = -N;
//    }
    vec3 L = normalize((VMatrix * vec4(lightPos, 1.f)).xyz - pos);
    vec3 E = normalize(-pos);
    vec3 R = normalize(reflect(-L,N));
    
    // Calculate colour
    vec3 ambient = ambientMat;
    vec3 diffuse = clamp( diffuseMat * max(dot(N,L), 0.0) , 0.0, 1.0 ) ;
    vec3 spec = clamp ( specMat * pow(max(dot(R,E), 0.0), specPow) , 0.0, 1.0 );
    fragColour = vec4(ambient + diffuse + spec, opacity);
}
