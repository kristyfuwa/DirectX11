cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightMaterialBuffer
{
	float4 lightPosition; //光源位置
	float4 lightColor;   //光源颜色
	float4 globalAmbient; //光源的环境光反射系数
	float4 cameraPosition; //摄像机的位置
	float4 Ke;  //材质的自发光
	float4 Ka;  //材质的环境光系数
	float4 Kd;  //材质的漫反射系数
	float4 Ks;  //材质的高光系数
	float3 lightDirection; //平行光方向
	float shininess; //高光指数
}

struct VertexInputType
{
	float4	position:POSITION;
	float3	normal:NORMAL;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV表示系统自定义格式;
	float4 color:COLOR;
};


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
	float3 N = mul(input.normal,(float3x3)worldMatrix);
	N=normalize(N);

	//世界坐标系中的顶点位置
	worldPosition=mul(input.position,worldMatrix);
	float3 P = worldPosition.xyz;

	//自发光颜色
	float4 emissive = Ke;

	//计算环境光
	float4 ambient = Ka * globalAmbient;

	//计算漫反射
	float3 L = -normalize(lightDirection);
	float n_dot_l = max(dot(N,L),0);
	float4 diffuse = Kd * lightColor * n_dot_l;

	//计算高光
	float3 V = normalize(cameraPosition.xyz - P);
	float3 H = normalize(L+V);
	float specularLight = pow(max(dot(N,H),0), shininess);

	if(n_dot_l <= 0)
		specularLight = 0;
	float4 specular = Ks * lightColor * specularLight;

	output.color = emissive + ambient + diffuse + specular;

	return output;
}


float4 LightPixelShader(PixelInputType input):SV_TARGET
{
	return input.color;
}