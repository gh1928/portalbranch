uniform sampler2D sampler_normal;

void main(void)
{
    vec3 normal_map = texture2D(sampler_normal, gl_TexCoord[0].xy).rgb;
    gl_FragColor = vec4(normal_map,1.0);
}