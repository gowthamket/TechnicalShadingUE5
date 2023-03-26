float3 rayOrigin = 1 - (viewDir - worldPos);
float3 rayStep = viewDir * -1;

float3 lightDirection = normalize(lightPos);

struct sdfShapes
{
	float donut(float3 p, float size, float cutout)
	{
		float2 q = float2(length(p.xz) - size, p.y);
		return length(q) - cutout;
	}
};
sdfShapes sdf;

for (int i = 0; i < 256; i++)
{
	float dist = sdf.donut(rayOrigin, 50, 25);

	if (dist < 0.01)
	{
		float eps = 0.001;
		float3 normal = normalize(float3(
			sdf.donut(float3(rayOrigin.x + eps, rayOrigin.y, rayOrigin.z), 50, 25)
			- sdf.donut(float3(rayOrigin.x - eps, rayOrigin.y, rayOrigin.z), 50, 25),
			sdf.donut(float3(rayOrigin.x, rayOrigin.y + eps, rayOrigin.z), 50, 25)
			- sdf.donut(float3(rayOrigin.x, rayOrigin.y - eps, rayOrigin.z), 50, 25),
			sdf.donut(float3(rayOrigin.x, rayOrigin.y, rayOrigin.z + eps), 50, 25)
			- sdf.donut(float3(rayOrigin.x, rayOrigin.y, rayOrigin.z - eps), 50, 25),
			));

		float diffuse = max(dot(normal, lightDirection), 0);
		float3 reflection = reflect(lightDirection, normal);
		float3 viewDirection = normalize(-worldPos - rayOrigin);
		float specular = pow(max(dot(reflection, viewDirection), 0), 256);

		return (diffuse * float3(1, 0, 0)) + (specular * float3(1, 1, 1));
	}

	opacityMask = 1;
	rayOrigin += rayStep;
}

opacityMask = 0;
return float3(0, 0, 0);
