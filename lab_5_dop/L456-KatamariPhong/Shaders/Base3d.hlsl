#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

struct VS_IN
{
	float4 pos : POSITION0;
	float4 tex : TEXCOORD0;
	float4 normal : NORMAL0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 tex : TEXCOORD;
	float4 normal : NORMAL;
	float4 viewPos : VIEWPOS;
	float4 worldPos : WORLDPOS;
};



struct LightInfo
{
    float4 LightPos; // xyz: position or direction, w: unused
    float4 LightColor; // rgb: color, a: intensity
    float4 AmbientSpecularPowType; // x: ambient strength, y: specular strength, z: falloff power, w: type (0: dir, 1: point)
};

cbuffer cbPerScene : register(b3)
{
    LightInfo lightinfo[10];
    int numLights;
    float3 padding; // Align to 16 bytes
};

cbuffer cbPerObject : register(b2)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldView;
	float4x4 gInvTrWorldView;

    float4 emissionParams;
};

cbuffer cbPerScene : register(b0)
{
	float4 lightPos;
	float4 lightColor;
	float4 ambientSpecularPowType;
	float4x4 gT;
};

cbuffer cbCascade : register(b1)
{
	float4x4 gViewProj[CASCADE_COUNT + 1];
	float4 gDistances;
};

Texture2D DiffuseMap : register(t0);
Texture2DArray CascadeShadowMap : register(t1);
SamplerState Sampler : register(s0);
SamplerComparisonState DepthSampler : register(s1);

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos.xyz, 1.0f), gWorldViewProj);
	output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), gInvTrWorldView);
	output.viewPos = mul(float4(input.pos.xyz, 1.0f), gWorldView);
	output.worldPos = mul(float4(input.pos.xyz, 1.0f), gWorld);
	
	return output;
}

float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN)
{
	float depthValue = abs(posViewSpace.z);

	int layer = -1;
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		if (depthValue < gDistances[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = CASCADE_COUNT;
	}

	float4 posLightSpace = mul(float4(posWorldSpace.xyz, 1.0), gViewProj[layer]);
	float3 projCoords = posLightSpace.xyz / posLightSpace.w;

	projCoords = (mul(float4(projCoords, 1.0f), gT)).xyz;
	float currentDepth = projCoords.z;

	if (currentDepth > 1.0f)
	{
		return 0.0f;
	}

	float bias = max(0.05f * (1.0f - dotN), 0.005f);
	const float biasModifier = 0.5f;
	if (layer == CASCADE_COUNT)
	{
		bias *= 1 / (1000.0 * biasModifier);
	}
	else
	{
		bias *= 1 / (gDistances[layer] * biasModifier);
	}

	// PCF
	float shadow = 0.0f;
	float2 texelSize = 1.0f / 1024.0f;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			shadow += CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy + float2(x, y) * texelSize, layer), currentDepth - bias);
		}
	}
	shadow /= 9.0f;

	//float shadow = CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy, layer), currentDepth);

	return shadow;
}

float3 GammaEncode(float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

// освещение фонга
float4 PSMain(PS_IN input) : SV_Target
{
	
	float4 norm = normalize(input.normal);
	

    //float4 norm = input.normal;
	
	// это тени ПАДАЮЩИЕ!
	float shadow = 0;
	//if (emissionParams.w < 0.5f)
    //    shadow = ShadowCalculation(input.worldPos, input.viewPos, dot(norm, lightPos));
	//float shadow = ShadowCalculation(input.worldPos, input.viewPos, dot(norm, lightPos));
	
	float4 ambientDL = ambientSpecularPowType.x * float4(lightColor.xyz, 1.0f);
	float4 objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
	
    //return objColor;
	
    if (emissionParams.w > 0.5f)
    {
        float4 result;
        result.xyz = objColor; // убираем тени на объекте
        result.xyz += emissionParams.xyz * 100; // добавляем цвет источника света
        return float4(result.xyz, 1.0f);
    }
	
	
	float diffDL = max(dot(norm, lightPos), 0.0f);

	float4 diffuseDL = diffDL * float4(lightColor.xyz, 1.0f);

	float4 reflectDirDL = reflect(-lightPos, norm);
	float3 viewDirDL = - normalize(input.viewPos.xyz);
	
	float specDL = pow(max(dot(viewDirDL, reflectDirDL.xyz), 0.0f), ambientSpecularPowType.z);
	
	
	float4 specularDL = ambientSpecularPowType.y * specDL * float4(lightColor.xyz, 1.0f);

    float4 coeff_DL = (ambientDL + (1.0f - shadow) * (diffuseDL + specularDL));
	
	//float4 result = (ambientDL + (1.0f - shadow) * (diffuseDL + specularDL)) * objColor;
	
	
    float4 resultPL = (0.0f, 0.0f, 0.0f, 0.0f);
	
    for (int i = 0; i < 2; ++i)
    {
        LightInfo pointLight = lightinfo[i];
		
        float light_aspt_coeff = 1.0f;
		
        float4 ambientPL = (pointLight.AmbientSpecularPowType.x * light_aspt_coeff) * float4(pointLight.LightColor.xyz, 1.0f);
        
        //float3 lightDir = normalize(pointLight.LightPos.xyz - input.worldPos.xyz);
		//lightDir = mul(lightDir, gWorldView)
        float3 lightDir = normalize(pointLight.LightPos.xyz - input.viewPos.xyz);
		
        //float attenuation = 1.0f;
        float faloww_coeff = 1.0;
        float dist = length(pointLight.LightPos.xyz - input.viewPos.xyz);
        //float attenuation = 1.0f / pow(dist, pointLight.AmbientSpecularPowType.z * faloww_coeff); // z = falloff power
        float attenuation = 1.0f / (1 + 0.7 * dist + 1.8 * dist * dist);
        float att_coeff = 2.0f;
        attenuation *= att_coeff;
		
        float diff_strenght = 0.3f;
       // float diff = max(dot(norm.xyz, lightDir), 0.0f); //* diff_strenght;
        //float diff = max(dot(norm, float4(lightDir, 0.0f)), 0.0f);
        float diff = max(dot(norm.xyz, lightDir), 0.0f);
		
       // return float4(abs(norm.xyz), 1.0f);
        //return float4(diff.xxx, 1.0f);
		
        float4 diffuse = diff * float4(pointLight.LightColor.xyz, 1.0f);
		
        //return float4(normalize(input.normal.xyz) * 0.5 + 0.5, 1.0f);
        //return float4(diff.xxx, 1.0f);

        float3 reflectDir = reflect(-lightDir, norm.xyz);
		
		float3 viewDir = -normalize(input.viewPos.xyz);
	
        float spec = pow(max(dot(viewDir, reflectDir.xyz), 0.0f), pointLight.AmbientSpecularPowType.z * light_aspt_coeff);
		float4 specular = pointLight.AmbientSpecularPowType.y * light_aspt_coeff * spec * float4(pointLight.LightColor.xyz, 1.0f);

        //float4 result = (ambientPL + (1.0f - shadow) * (diffuse + specular)) * objColor;
        //result += objColor;
        //float4 coeff_directional = (ambientPL + (1.0f - shadow) * (diffuse + specular));
        //float4 coeff_point = (ambientPL + attenuation * (diffuse + specular));
        
		// отрицательный спекюлар?? получается??
        float4 coeff_point = (ambientPL + attenuation * (diffuse + specular));
        //float4 result = (ambient + attenuation * (diffuse + specular)) * objColor;
		
		
        //resultPL += diffuse;
        //resultPL += specular;
        resultPL += attenuation * (diffuse + specular); //* objColor;
		
		//float4 resultPL = coeff_point * objColor;
		//(ambientPL + attenuation * (diffuse + specular)) * objColor;
        //return float4(objColor.xyz, 1.0f);
		//return float4(diffuse.xyz, 1.0f);
        //return float4(specular.xyz, 1.0f);
		
		//return float4(ambientPL.xyz, 1.0f);
        
		//return float4(pointLight.AmbientSpecularPowType.xyz, 1.0f);
        //return float4(1.0f, 0.0f, 0.0f, 1.0f); // возвращает чистый красный!
        //return float4(((float4(0.4f, 0.5f, 32, 0.0f)).x * float4(1.0f, 1.0f, 1.0f, 1.0f))); // это то, что должно вернуть
		
		//return float4(resultPL.xyz, 1.0f);
        
		//return float4(specularDL.xyz, 1.0f);
        //return float4(ambientDL.xyz, 1.0f);
    }
    resultPL += coeff_DL;
	
    resultPL *= objColor;
	
    //float3 red = float3(1, 0, 0);
    //float3 gammaRed = pow(red, 1.0f / 2.2f);
    //return float4(gammaRed, 1);
	
    //return (GammaEncode(float3(1.0f, 0.0f, 1.0f)), 1.0f);
    //return result * 100;
	
	
	//return float4(result.xyz, 1.0f);
	//return float4(objColor.xyz, 1.0f);
	//return float4(diffuseDL.xyz, 1.0f);
    //return float4(specularDL.xyz, 1.0f);
    //return float4(ambientDL.xyz, 1.0f);

	//return float4((objColor).xyz, 1.0f);
	
    return float4(resultPL.xyz, 1.0f);
}
