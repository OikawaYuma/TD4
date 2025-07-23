//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}
#include "Line.hlsli"

struct Camera
{
    float32_t3 worldPosition;
};
SamplerState gSampler : register(s0);
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{

    PixelShaderOutput output;
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    if (input.color.a == 0.0)
    {
        discard;
    }
    output.color =input.color;
    return output;

};