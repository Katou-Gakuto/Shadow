#pragma once
#include <type_traits>
#include <utility>

// C++14óp void_t
template<typename...>
using void_t = void;

// falseç\ë¢
template<typename, typename = void>
struct TemplateType_Equal : std::false_type {};

// trueç\ë¢
template<typename T>
struct TemplateType_Equal<T, void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type
{
};