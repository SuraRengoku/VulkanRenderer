// HLSL Shader Model 6.0+
struct UniformBufferObject {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

[[vk::binding(0, 0)]]
cbuffer UBO : register(b0) {
    UniformBufferObject ubo;
};

struct VSInput {
    float3 inPosition : POSITION;
    float3 inColor    : COLOR0;
    float2 inTexCoord : TEXCOORD0;
};

struct VSOutput {
    float4 pos          : SV_Position;
    float3 fragColor    : TEXCOORD0;
    float2 fragTexCoord : TEXCOORD1;
};

VSOutput main(VSInput input) {
    VSOutput output;
    float4 worldPS = float4(input.inPosition, 1.0);
    output.pos = mul(ubo.proj, mul(ubo.view, mul(ubo.model, worldPS)));
    output.fragColor = input.inColor;
    output.fragTexCoord = input.inTexCoord;
    return output;
}



