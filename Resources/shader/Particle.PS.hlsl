//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}
#include "Particle.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
    float32_t shininess;
};
struct DirectionalLight {
	float32_t4 color; //!< ライトの色
	float32_t3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{

    PixelShaderOutput output;

    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    // アルファがゼロなら描画破棄
    if (input.color.a == 0.0 || textureColor.a == 0.0)
    {
        discard;
    }

    // 最終アルファ
    float alpha = textureColor.a * input.color.a;

    // RGB も Premultiplied（＝ 最終アルファを掛けた値にする）
    float3 baseColor = textureColor.rgb * input.color.rgb;

    if (gMaterial.enableLighting != 0)
    {
        float nl = max(0, dot(input.normal, -gDirectionalLight.direction)) * gMaterial.shininess;

        if (nl <= 0.01f)
            nl = 0.3f;
        else if (nl <= 0.3f)
            nl = 0.5f;
        else
            nl = 1.0f;

        baseColor *= nl;
    }

    output.color.rgb = baseColor * alpha; // ← Premultiplied alpha
    output.color.a = alpha;

    return output;

};