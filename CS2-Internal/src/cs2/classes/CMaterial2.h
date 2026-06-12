#pragma once
#include <cstdint>
#include <optional>

enum EChamsMaterial : int {
	CHAMS_MATERIAL_GENERIC = 0,
	CHAMS_MATERIAL_UNLIT = 1,
	CHAMS_MATERIAL_SOLID = 2,
	CHAMS_MATERIAL_LATEX = 3,
	CHAMS_MATERIAL_COUNT
};

class CMaterial2 {
public:
	virtual const char* GetName() = 0;
	virtual const char* GetNameWithMod() = 0;
};

struct ChamsMaterialPair_t {
	CMaterial2* m_pVisible = nullptr;
	CMaterial2* m_pInvisible = nullptr;
};

struct MeshColor_t {
	uint8_t r = 255U;
	uint8_t g = 255U;
	uint8_t b = 255U;
	uint8_t a = 255U;
};

class CSceneAnimatableObject {
public:
	char pad0[0xB8];
	void* m_pUnkData;       // 0xB8
	uint32_t m_hOwnerIndex; // 0xC0
	char pad1[0x4C];
};

class CMeshData {
public:
	char pad0[24];                                    // 0x00
	CSceneAnimatableObject* m_pSceneAnimatableObject; // 0x18
	CMaterial2* m_pMaterial;                          // 0x20
	CMaterial2* m_pMaterial2;                         // 0x28
	char pad1[32];                                    // 0x30
	MeshColor_t m_Color;                              // 0x50
	char pad2[16];                                    // 0x54
};

class CMeshDrawPrimitive_t {
public:
	char pad0[24];
	CSceneAnimatableObject* m_pSceneAnimatableObject;
	CMaterial2* m_pMaterial;
	CMaterial2* m_pMaterial2;
	char pad1[32];
	MeshColor_t m_Color;
	char pad2[16];
};

class CMeshPrimitiveOutputBuffer_t {
public:
	CMeshDrawPrimitive_t* m_pPrimitives = nullptr;
	int m_iMaxPrimitives = 0;
	int m_iStartPrimitive = 0;
};

struct kv3_id_t {
	const char* m_name;
	uint64_t m_id1;
	uint64_t m_id2;
	kv3_id_t(const char* name, uint64_t id1, uint64_t id2)
		: m_name(name), m_id1(id1), m_id2(id2) {}
};

struct resource_binding_t { void* data; };
struct CMaterialStrongHandle_t { const resource_binding_t* binding = nullptr; };

using fn_LoadKV3_t = bool(__fastcall*)(void*, void*, const char*, const kv3_id_t*, const char*, unsigned int);
using fn_SetTypeKV3_t = void*(__fastcall*)(void*, unsigned int, unsigned int);
using fn_CreateMaterial_t = int64_t(__fastcall*)(void*, CMaterialStrongHandle_t*, const char*, void*, unsigned int, unsigned int);

namespace ChamsMaterials {
	inline constexpr char szVMatGeneric_Visible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "generic.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )";

	inline constexpr char szVMatGeneric_Invisible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "csgo_unlitgeneric.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 1
                F_DISABLE_Z_WRITE = 1
                F_DISABLE_Z_BUFFERING = 1
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )";

	inline constexpr char szVMatUnlit_Visible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
            {
                shader = "csgo_unlitgeneric.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )";

	inline constexpr char szVMatUnlit_Invisible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
				shader = "csgo_unlitgeneric.vfx"
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
				g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
				g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				F_IGNOREZ = 1
				F_DISABLE_Z_WRITE = 1
				F_DISABLE_Z_BUFFERING = 1
				F_RENDER_BACKFACES = 1
				g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )";

	inline constexpr char szVMatSolid_Visible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "solidcolor.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [9.0, 9.0, 9.0, 9.0]
			} )";

	inline constexpr char szVMatSolid_Invisible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
				shader = "solidcolor.vfx"
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
				g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
				g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				F_IGNOREZ = 1
				F_DISABLE_Z_WRITE = 1
				F_DISABLE_Z_BUFFERING = 1
				F_RENDER_BACKFACES = 1
				g_vColorTint = [9.0, 9.0, 9.0, 9.0]
			} )";

	inline constexpr char szVMatCharacter_Visible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "csgo_character.vfx"
                F_BLEND_MODE = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
                g_bFogEnabled = 0
                g_flMetalness = 0.000
                g_tMetalness = resource:"materials/default/default_metal_tga_8fbc2820.vtex"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_79a2e0d0.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_1b833b2a.vtex"
			} )";

	inline constexpr char szVMatCharacter_Invisible[] =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "csgo_character.vfx"
                F_DISABLE_Z_BUFFERING = 1
                F_DISABLE_Z_PREPASS = 1
                F_DISABLE_Z_WRITE = 1
                F_BLEND_MODE = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
                g_bFogEnabled = 0
                g_flMetalness = 0.000
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_79a2e0d0.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_1b833b2a.vtex"
                g_tMetalness = resource:"materials/default/default_metal_tga_8fbc2820.vtex"
			} )";
}