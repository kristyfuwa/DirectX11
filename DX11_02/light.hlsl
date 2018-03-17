#define NUM_LIGHTS 2

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightMaterialBuffer
{
	float4 lightPosition[NUM_LIGHTS]; //��Դλ��
	float4 lightColor[NUM_LIGHTS];   //��Դ��ɫ
	float4 globalAmbient[NUM_LIGHTS]; //��Դ�Ļ����ⷴ��ϵ��
	float4 cameraPosition; //�������λ��
	float4 Ke[NUM_LIGHTS];  //���ʵ��Է���
	float4 Ka[NUM_LIGHTS];  //���ʵĻ�����ϵ��
	float4 Kd[NUM_LIGHTS];  //���ʵ�������ϵ��
	float4 Ks[NUM_LIGHTS];  //���ʵĸ߹�ϵ��
	float4 attenuation[NUM_LIGHTS]; //˥��ϵ��
	float3 lightDirection[NUM_LIGHTS]; //ƽ�йⷽ��
	float shininess[NUM_LIGHTS]; //�߹�ָ��
}

struct VertexInputType
{
	float4	position:POSITION;
	float3	normal:NORMAL;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV��ʾϵͳ�Զ����ʽ;

	//float4 color:COLOR;
	float3 worldnormal:NORMAL;
	float4 worldposition : POSITION;
};

//PixelInputType LightVertexShader(VertexInputType input)
//{
//	PixelInputType output = (PixelInputType)0;
//	float4 worldPosition;
//	//����������չ���ĸ�������������Ϊ1���Ա��������
//	input.position.w = 1.0f;
//
//	// ����3�����󣬵õ�clip�ռ�����ꡣ
//	output.position = mul(input.position, worldMatrix);
//	output.position = mul(output.position, viewMatrix);
//	output.position = mul(output.position, projectionMatrix);
//
//	//��������ϵ�еĶ��㷨����
//	float3 N = mul(input.normal,(float3x3)worldMatrix);
//	N=normalize(N);
//
//	//��������ϵ�еĶ���λ��
//	worldPosition=mul(input.position,worldMatrix);
//	float3 P = worldPosition.xyz;
//
//	//�Է�����ɫ
//	float4 emissive = Ke;
//
//	//���㻷����
//	float4 ambient = Ka * globalAmbient;
//
//	//��LightDirection���Ǵ�ƽ�й�
//	//��Դλ�ü�����λ�ã��ǲ�����˥���ĵ��Դ
//
//	//����������
//	float3 L = normalize(lightPosition.xyz - P);
//	float n_dot_l = max(dot(N,L),0);
//	float4 diffuse = Kd * lightColor * n_dot_l;
//
//	//����߹�
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
	//����������չ���ĸ�������������Ϊ1���Ա��������
	input.position.w = 1.0f;

	// ����3�����󣬵õ�clip�ռ�����ꡣ
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//��������ϵ�еĶ��㷨����
	float3 N = mul(input.normal, (float3x3)worldMatrix);
	output.worldnormal = N;

	//��������ϵ�еĶ���λ��
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
		//�Է�����ɫ
		emissive = Ke[i];

		//���㻷����
		ambient = Ka[i] * globalAmbient[i];


		//����������
		//��LightDirection���Ǵ�ƽ�й�
		//��Դλ�ü�����λ��
		L = normalize(lightPosition[i].xyz - P);
		d = distance(lightPosition[i].xyz, P);

		//˥��ϵ��
		atte = 1 / (attenuation[i].x + attenuation[i].y*d + attenuation[i].z * d * d);
		diffuseLight = max(dot(N, L), 0);
		diffuse = Kd[i] * lightColor[i] * diffuseLight * atte;

		//����߹�
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