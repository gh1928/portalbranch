#version 120
uniform vec2 resolution;
uniform sampler2D sampler_normal;
uniform sampler2D sampler_light;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 ambient_color = vec3(0.5,0.5,0.5);
uniform float ambient_intensity = 0.5;
uniform vec3 falloff;

void main()
{

    vec2 coord = gl_TexCoord[0].xy;
    vec3 normal_map = texture2D(sampler_normal, coord).rgb;
    vec3 light_map = texture2D(sampler_light,coord).rgb;
    vec3 lightDir = vec3((light_pos.xy - gl_FragCoord.xy) / resolution.xy, light_pos.z);
    lightDir.x *= resolution.x / resolution.y;

    float D = length(lightDir);
    vec3 N = normalize(normal_map * 2.0 - 1.0);
    vec3 L = normalize(lightDir.xyz);

    vec3 diffuse = light_color.rgb * max(dot(N,L),0.0);
    vec3 ambient = ambient_color * ambient_intensity;

    float attenuation = 1.0 / (falloff.x + (falloff.y * D) + falloff.z*D*D);

    vec3 intensity = ambient + diffuse * attenuation;

    gl_FragColor = vec4(light_map + intensity, 1.0 );
}