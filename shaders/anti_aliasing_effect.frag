#version 330

uniform sampler2D colorMap;
uniform float step_x;
uniform float step_y;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    const int filter_size = 1;
    if(uv.x < 0.5)
    {
        float factor = 1./((filter_size * 2. + 1.) * (filter_size * 2. + 1.));
        vec3 col = vec3(0., 0., 0.);
        for (int x = -filter_size; x <= filter_size; x++)
        {
            for (int y = -filter_size; y <= filter_size; y++)
            {
                col += factor * texture(colorMap, uv + vec2(x * step_x, y * step_y)).rgb;
            }
        }
        color = vec4(col, 1.);
    }
    else {
        color = texture(colorMap, uv).rgba;
    }
}
