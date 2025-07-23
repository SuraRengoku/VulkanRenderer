// HLSL Shader Model 6.0+
[[vk::binding(1, 0)]] Texture2D texSampler : register(t1);
[[vk::binding(1, 0)]] SamplerState texSamplerSampler : register(s1);

struct FSInput {
    float3 fragColor    : TEXCOORD0;
    float2 fragTexCoord : TEXCOORD1;
};

float4 main(FSInput input) : SV_TARGET {
    float3 sampledColor = texSampler.Sample(texSamplerSampler, input.fragTexCoord).rgb;
    return float4(input.fragColor * sampledColor, 1.0);
}
