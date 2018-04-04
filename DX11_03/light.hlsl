#define NUM_LIGHTS 2

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightBuffer
{
	float4 lightPosition[NUM_LIGHTS]; //��Դλ��
	float4 lightColor[NUM_LIGHTS];   //��Դ��ɫ
	float4 globalAmbient[NUM_LIGHTS]; //��Դ�Ļ����ⷴ��ϵ��
	float4 attenuation[NUM_LIGHTS]; //˥��ϵ��
	float4 spotattenuation[NUM_LIGHTS];
	float3 lightDirection[NUM_LIGHTS]; //ƽ�йⷽ��,����spotlight�¹�ķ���
	float shininess[NUM_LIGHTS]; //�߹�ָ��
}

cbuffer MaterialBuffer
{
	float4 Ke;
	float4 Ka;
};
cbuffer CameraBuffer
{
	float4 cameraPosition;
};

struct VertexInputType
{
	float4	position:POSITION;
	float3	normal:NORMAL;
	float4 Kd:DIFFUSE;
	float4 Ks:SPECULAR;
};


struct PixelInputType
{
	float4 position:SV_POSITION;//SV��ʾϵͳ�Զ����ʽ;
	float3 worldnormal:NORMAL;
	float4 worldposition : POSITION;
	float4 Kd:DIFFUSE;
	float4 Ks:SPECULAR;
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

//һ������spot lightϵ���ĺ���
float dualConeSpotlight(float3 P, float3 lightpos, float3 lightdir, float  cosInnerCone, float cosOuterCone)
{

	float3 V = normalize(P - lightpos);

	float cosDirection = dot(V, normalize(lightdir));

	return smoothstep(cosOuterCone, cosInnerCone, cosDirection);
}


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

	float4 Kd = input.Kd;
	float4 Ks = input.Ks;


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
	float spotEffect;
	float atte;
	float d;
	int		i;

	for (i = 0; i < NUM_LIGHTS; ++i)
	{
		//�Է�����ɫ
		emissive = Ke;

		//���㻷����
		ambient = Ka * globalAmbient[i];


		//����������
		//��LightDirection���Ǵ�ƽ�й�,��spotlight����´����ķ���
		spotEffect = dualConeSpotlight(P, lightPosition[i].xyz, lightDirection[i], spotattenuation[i].x, spotattenuation[i].y);

		//��Դλ�ü�����λ��
		L = normalize(lightPosition[i].xyz - P);
		d = distance(lightPosition[i].xyz, P);

		//˥��ϵ��
		atte = 1 / (attenuation[i].x + attenuation[i].y*d + attenuation[i].z * d * d);
		diffuseLight = max(dot(N, L), 0);
		diffuse = Kd * lightColor[i] * diffuseLight * atte * spotEffect;

		//����߹�
		V = normalize(cameraPosition.xyz - P);
		H = normalize(L + V);
		specularLight = pow(max(dot(N, H), 0), shininess[i]);

		if (diffuseLight <= 0)
			specularLight = 0;
		specular = Ks * lightColor[i] * specularLight * atte * spotEffect;
		finalcolor += emissive + ambient + diffuse + specular;
	}
	return finalcolor;
}