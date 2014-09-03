varying float intensity;

void main()
{
	//vec3 lightDir = normalize(vec3(gl_LightSource[0].position));
    vec3 lightDir = vec3(-1.0, 0.2, -1.0);
	intensity = dot(lightDir,gl_Normal);
    
	gl_Position = ftransform();
}