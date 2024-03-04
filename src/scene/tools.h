#pragma once


// Convert
template<typename T, typename Q>// glm::vec3 to float3
void ToFloat3(T origin, Q* target) 
{ 
	target[0] = origin.x;
	target[1] = origin.y;
	target[2] = origin.z;
}
template<typename T, typename Q> // glm::vec4 to float4
void ToFloat4(T origin, Q* target)
{
	target[0] = origin.x;
	target[1] = origin.y;
	target[2] = origin.z;
	target[3] = origin.w;
}
template<typename Q, typename T>// float3 to glm::vec3  
T ToVec3(Q* origin) 
{ 
	return T(origin[0], origin[1], origin[2]); 
}
template<typename Q, typename T>// float4 to glm::vec4 
T ToVec4(Q* origin)
{
	return T(origin[0], origin[1], origin[2], origin[3]);
}




