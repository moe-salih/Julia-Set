
#ifndef VEC_HPP
#define VEC_HPP

#include <type_traits>

template<typename T> concept arithmatic = std::is_arithmetic_v<T>;

template<arithmatic T> struct vec2 { T x,y; };
template <arithmatic T> struct vec3 { T x,y,z; };
template <arithmatic T> struct vec4 { T x,y,z,w; };

using vec2i = vec2<int>;
using vec3i = vec3<int>;
using vec4i = vec4<int>;

using vec2f = vec2<float>;
using vec3f = vec3<float>;
using vec4f = vec4<float>;

using vec2d = vec2<double>;
using vec3d = vec3<double>;
using vec4d = vec4<double>;

#endif//VEC_HPP
