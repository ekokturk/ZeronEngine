#ifdef ZE_SHADER_SPIRV
	#define location(n) [[vk::location((n))]]

	#ifdef SHADER_VERTEX
		#pragma shader_stage(vertex)
	#endif
	#ifdef SHADER_FRAGMENT
		#pragma shader_stage(fragment)
	#endif
	#ifdef SHADER_COMPUTE
		#pragma shader_stage(compute)
	#endif
#else
	#define location(n)
	#define shader_stage(n)
#endif

#pragma pack_matrix(row_major)