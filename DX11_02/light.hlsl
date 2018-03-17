#define NUM_LIGHTS 2

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightMaterialBuffer
{
	float4 lightPosition[NUM_LIGHTS]; //光源位置
	float4 lightColor[NUM_LIGHTS];   //光源颜色
	float4 globalAmbient[NUM_LIGHTS]; //光源的环境光反射系数
	float4 cameraPosition; //摄像机的位置
	float4 Ke[NUM_LIGHTS];  //材质的自发光
	float4 Ka[NUM_LIGHTS];  //材质的环境光系数
	float4 Kd[NUM_LIGHTS];  //材质的漫反射系数
	float4 Ks[NUM_LIGHTS];  //材质的高光系数
	float4 attenuation[NUM_LIGHTS]; //衰减系数
	float3 lightDirection[NUM_LIGHTS]; //平行光方向
	float shininess[NUM_LIGHTS]; //高光指数
}

struct VertexInputType
{
	float4	position:POSITION;
	float3	normal:NORMAL;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV表示系统自定义格式;

	//float4 color:COLOR;
	float3 worldnormal:NORMAL;
	float4 worldposition : POSITION;
};

//PixelInputType LightVertexShader(VertexInputType input)
//{
//	PixelInputType output = (PixelInputType)0;
//	float4 worldPosition;
//	//顶点坐标扩展成四个分量，并设置为1，以便矩阵运算
//	input.position.w = 1.0f;
//
//	// 乘以3个矩阵，得到clip空间的坐标。
//	output.position = mul(input.position, worldMatrix);
//	output.position = mul(output.position, viewMatrix);
//	output.position = mul(output.position, projectionMatrix);
//
//	//世界坐标系中的顶点法向量
//	float3 N = mul(input.normal,(float3x3)worldMatrix);
//	N=normalize(N);
//
//	//世界坐标系中的顶点位置
//	worldPosition=mul(input.position,worldMatrix);
//	float3 P = worldPosition.xyz;
//
//	//自发光颜色
//	float4 emissive = Ke;
//
//	//计算环境光
//	float4 ambient = Ka * globalAmbient;
//
//	//用LightDirection就是纯平行光
//	//光源位置减顶点位置，是不考虑衰减的点光源
//
//	//计算漫反射
//	float3 L = normalize(lightPosition.xyz - P);
//	float n_dot_l = max(dot(N,L),0);
//	float4 diffuse = Kd * lightColor * n_dot_l;
//
//	//计算高光
//	float3 V = normalize(cameraPosition.xyz - P);
//	float3 H = normalize(L+V);
//	float specularLight = pow(max(dot(N,H),0), shininess);
//
//	if(n_dot_l <= 0)
//		specularLight = 0;
//	float4 specular = Ks * lightColor * specularLight;
//
//	output.color = emissive + ambient + diffuse + specular;
//
//	return output;
//}


//float4 LightPixelShader(PixelInputType input):SV_TARGET
//{
//	return input.color;
//}


PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;
	float4 worldPosition;
	//顶点坐标扩展成四个分量，并设置为1，以便矩阵运算
	input.position.w = 1.0f;

	// 乘以3个矩阵，得到clip空间的坐标。
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//世界坐标系中的顶点法向量
	float3 N = mul(input.normal, (float3x3)worldMatrix);
	output.worldnormal = N;

	//世界坐标系中的顶点位置
	worldPosition = mul(input.position, worldMatrix);
	output.worldposition = worldPosition;

	return output;
}


float4 LightPixelShader(PixelInputType input) :SV_TARGET
{
	float3 P = input.worldposition.xyz;
	float3 N = normalize(input.worldnormal);

	float4 finalcolor = 0;
	float4 emissive = 0;
	float4 ambient = 0;
	float3 L;
	float diffuseLight = 0;
	float4 diffuse = 0;
	float3 V = 0;
	float3 H = 0;
	float specularLight = 0;
	float4 specular = 0;
	float atte;
	float d;
	int		i;

	for (i = 0; i < NUM_LIGHTS; ++i)
	{
		//自发光颜色
		emissive = Ke[i];

		//计算环境光
		ambient = Ka[i] * globalAmbient[i];


		//计算漫反射
		//用LightDirection就是纯平行光
		//光源位置减顶点位置
		L = normalize(lightPosition[i].xyz - P);
		d = distance(lightPosition[i].xyz, P);

		//衰减系数
		atte = 1 / (attenuation[i].x + attenuation[i].y*d + attenuation[i].z * d * d);
		diffuseLight = max(dot(N, L), 0);
		diffuse = Kd[i] * lightColor[i] * diffuseLight * atte;

		//计算高光
		V = normalize(cameraPosition.xyz - P);
		H = normalize(L + V);
		specularLight = pow(max(dot(N, H), 0), shininess[i]);

		if (diffuseLight <= 0)
			specularLight = 0;
		specular = Ks[i] * lightColor[i] * specularLight * atte;
		finalcolor += emissive + ambient + diffuse + specular;
	}
	return finalcolor;
}