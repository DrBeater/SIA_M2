#version 410 core

uniform vec4 light_pos;
uniform vec3 light_col;
uniform float specular_coef;
uniform mat4 inv_projection_matrix;
uniform int width;
uniform int height;

in vec3 vert_normal;
in vec4 gl_FragCoord;
in vec3 vert_color;

out vec4 out_color;

vec3 VSPositionFromDepth(vec2 texcoord, float z)
{
    // Get x/w and y/w from the viewport position
    vec4 positionNDC = vec4(2 * vec3(texcoord, z) - 1, 1.f);
    // Transform by the inverse projection matrix
    vec4 positionVS = inv_projection_matrix * positionNDC;
    // Divide by w to get the view-space position
    return positionVS.xyz / positionVS.w;  
}

vec3 shade(vec3 N, vec3 L, vec3 V, 
           vec3 color, float Ka, float Kd, float Ks,
           vec3 lightCol, float shininess){

    vec3 final_color = color * Ka * lightCol;	//ambient

    float lambertTerm = dot(N,L);		//lambert

    if(lambertTerm > 0.0) {
        final_color += color * Kd * lambertTerm * lightCol; 	//diffuse

        vec3 R = reflect(-L,N);
        float specular = pow(max(dot(R,V), 0.0), shininess);
        final_color +=  Ks * lightCol * specular;	//specular
    }

    return final_color;
}

void main(void)
{
    vec4 res = gl_FragCoord / (width*height);
    vec2 texcoord = vec2(res.x, res.y);

    vec4 vert_pos_view = vec4(VSPositionFromDepth(texcoord, gl_FragCoord.w), 1.f);
    
    vec3 l = vec3(light_pos - vert_pos_view);
    
    out_color.rgb = shade(normalize(vert_normal), normalize(l), -normalize(vert_pos_view.xyz),
                          vert_color, 0.2, 0.7, specular_coef, light_col/max(1,dot(l,l)*0.05), 5);

    out_color.a = 1.0;
}
