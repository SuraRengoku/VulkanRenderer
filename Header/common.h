//
// Created by SuraRengoku on 17.04.25.
//

#ifndef VULKANRENDERER_COMMON_H
#define VULKANRENDERER_COMMON_H

#include <string>

enum class SPVStage : int {
    VERT,
    FRAG,
    GEO,
    TESSEL,
    COMP,
    RAYT
};

enum class TextureType : int {
    Albedo,
    Normal,
    Roughness,
    Metallic,
    AmbientOcclusion,
    Emissive,
    Height,
    Opacity,
    Specular,
    Glossiness,
    Displacement,
    Cavity,
    Sheen,
    Clearcoat,
    Anisotropy,
    Lightmap,
    CubeMap,
    Mask,
};

enum class MaterialType : int {
    Lambert,       
    Phong,          
    BlinnPhong,     
    PBRRough, 
    PBRGlossy,
    Unlit,          
    Transparent,    
    Emissive,       
    Subsurface,     
    Sheen,          
    Clearcoat,      
    Anisotropic,    
    Custom          
};

template<typename Enum>
std::string EnumToString(Enum e) {
    return "Unknown Enum";
}

template<>
inline std::string EnumToString<SPVStage>(SPVStage stage) {
    switch (stage) {
        case SPVStage::VERT: return "Vertex";
        case SPVStage::FRAG: return "Fragment";
        case SPVStage::GEO: return "Geometry";
        case SPVStage::TESSEL: return "Tessellation";
        case SPVStage::COMP: return "Compute";
        case SPVStage::RAYT: return "RayTracing";
        default: return "Unknown";
    }
}

template<>
inline std::string EnumToString<TextureType>(TextureType type) {
    switch (type) {
        case TextureType::Albedo:            return "Albedo";
        case TextureType::Normal:            return "Normal";
        case TextureType::Roughness:         return "Roughness";
        case TextureType::Metallic:          return "Metallic";
        case TextureType::AmbientOcclusion:  return "AO";
        case TextureType::Emissive:          return "Emissive";
        case TextureType::Height:            return "Height";
        case TextureType::Opacity:           return "Opacity";
        case TextureType::Specular:          return "Specular";
        case TextureType::Glossiness:        return "Glossiness";
        case TextureType::Displacement:      return "Displacement";
        case TextureType::Cavity:            return "Cavity";
        case TextureType::Sheen:             return "Sheen";
        case TextureType::Clearcoat:         return "Clearcoat";
        case TextureType::Anisotropy:        return "Anisotropy";
        case TextureType::Lightmap:          return "Lightmap";
        case TextureType::CubeMap:           return "CubeMap";
        case TextureType::Mask:              return "Mask";
        default:                             return "Unknown";
    }
}

template<>
inline std::string EnumToString<MaterialType>(MaterialType type) {
    switch (type) {
        case MaterialType::Lambert:         return "Lambert";
        case MaterialType::Phong:           return "Phong";
        case MaterialType::BlinnPhong:      return "BlinnPhong";
        case MaterialType::PBRRough:        return "PBRRough";
        case MaterialType::PBRGlossy:       return "PBRGlossy";
        case MaterialType::Unlit:           return "Unlit";
        case MaterialType::Transparent:     return "Transparent";
        case MaterialType::Emissive:        return "Emissive";
        case MaterialType::Subsurface:      return "Subsurface";
        case MaterialType::Sheen:           return "Sheen";
        case MaterialType::Clearcoat:       return "Clearcoat";
        case MaterialType::Anisotropic:     return "Anisotropic";
        case MaterialType::Custom:          return "Custom";
        default:                            return "Unknown";
    }
}

#endif // VULKANRENDERER_COMMON_H
