cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4	position:POSITION;
	float4	color:COLOR;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV表示系统自定义格式;
	float4 color:COLOR;
};


PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;
	//顶点坐标扩展成四个分量，并设置为1，以便矩阵运算
	input.position.w = 1.0f;

	// 乘以3个矩阵，得到clip空间的坐标。
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//直接输出顶点的颜色(.顶点之间的颜色，会采用插值的方式计算）
	output.color = input.color;

	return output;
}
