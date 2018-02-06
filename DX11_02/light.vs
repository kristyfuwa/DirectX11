cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightMaterialBuffer
{
	float3 cameraPosition;
	float padding;
}

struct VertexInputType
{
	float4	position:POSITION;
	float4	normal:NORMAL;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV表示系统自定义格式;
	float4 color:COLOR;
};

PixelInputType ColorVertexShader(VertexInputType input)
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
	float N = mul(input.normal,(float3x3)worldMatrix);
	N=normalize(N);

	//世界坐标系中的顶点位置
	worldPosition=mul(input.position,worldMatrix);
	float3 P = worldPosition.xyz;

	//自发光颜色
	float3 emissive = float3(0.0,0.0,0.0);

	//计算环境光
	float3 ambient = float3(0.3,0.3,0.3);

	//计算漫反射
	float3 L = normalize(float3(-1.0,-1.0,1.0));
	float n_dot_l = max(dot(N,L),0);
	float3 diffuse = n_dot_l;

	//计算高光
	float3 V = normalize(cameraPosition - P);
	float3 H = normalize(L+V);
	float specularLight = pow(max(dot(N,H),0),5.0);

	if(n_dot_l <= 0)
		specularLight = 0;
	float specular = specularLight;

	output.color = emissive + ambient + diffuse + specular;
	output.color.w = 1.0f;

	return output;
}
