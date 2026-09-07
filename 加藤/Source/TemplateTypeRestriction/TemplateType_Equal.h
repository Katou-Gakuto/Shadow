#pragma once
#include <type_traits>

// falseç\ë¢
template<typename, typename = void>
struct TemplateType_Equal : std::false_type {};

// trueç\ë¢
template<typename T>
struct TemplateType_Equal<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type
{
};