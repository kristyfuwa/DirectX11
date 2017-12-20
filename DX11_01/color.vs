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
	float4 position:SV_POSITION;//SV��ʾϵͳ�Զ����ʽ;
	float4 color:COLOR;
};


PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;
	//����������չ���ĸ�������������Ϊ1���Ա��������
	input.position.w = 1.0f;

	// ����3�����󣬵õ�clip�ռ�����ꡣ
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//ֱ������������ɫ(.����֮�����ɫ������ò�ֵ�ķ�ʽ���㣩
	output.color = input.color;

	return output;
}
