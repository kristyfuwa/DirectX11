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
	float4 position:SV_POSITION;//SV��ʾϵͳ�Զ����ʽ;
	float4 color:COLOR;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;
	float4 worldPosition;
	//����������չ���ĸ�������������Ϊ1���Ա��������
	input.position.w = 1.0f;

	// ����3�����󣬵õ�clip�ռ�����ꡣ
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//��������ϵ�еĶ��㷨����
	float N = mul(input.normal,(float3x3)worldMatrix);
	N=normalize(N);

	//��������ϵ�еĶ���λ��
	worldPosition=mul(input.position,worldMatrix);
	float3 P = worldPosition.xyz;

	//�Է�����ɫ
	float3 emissive = float3(0.0,0.0,0.0);

	//���㻷����
	float3 ambient = float3(0.3,0.3,0.3);

	//����������
	float3 L = normalize(float3(-1.0,-1.0,1.0));
	float n_dot_l = max(dot(N,L),0);
	float3 diffuse = n_dot_l;

	//����߹�
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
