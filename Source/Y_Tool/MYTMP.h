#pragma once

#include <type_traits>
#include <iostream>
#include <initializer_list>
#include <utility>

// このヘッダで使用するプログラミング方式は『メタテンプレートプログラミング』というらしい
// テンプレート関数やテンプレートクラスに型制限を設けたい時に使ってください
// ※可読性が著しく下がるので、必要なときのみ使用してください
// 
// MYTMP   : MY Template Meta Programing
// IF      : std::conditional_t<flag, TrueType, FalseType>を使い、どちらの型を使用するか
// VF      : Variadic Function (可変長引数関数)
// TG      : Type Get (型を取得するもの)
// TC      : Type Check (型チェックに関するもの)
// RCVR    : Remove Const Volatile Reference (この三つの修飾子を無視する)
// Creater : 型や関数がコンパイル時に定義されるかを司るもの
// Kit     : 簡単にテンプレートメタプログラミングをするためのキット
// PARTS   : MYTMP_Kit_を実装する上で使用しているもの。内部的な実装にしたいのでnamespaceの中で管理している。
// 

// SFINAEというのは『Substitution Failure Is Not An Error』の略称
// 
// テンプレート関数はビルド時に、テンプレート引数に合わせた関数を各パターン別に作る。
// なのでテンプレート関数を呼ぶということは、ビルドされた複数のオーバーロード関数をテンプレート引数によって自動で選択しているということ。
// そして選択したオーバーロード関数内で、演算子などからエラーが発生した場合コンパイルエラーになります。
// 
// さて、今回のSFINAEというのは、テンプレート引数の型チェックなどを行い、条件に合わなかった場合はオーバーロード関数の選択候補外にするというもの
// 
// 『%』演算子は整数にしか使えないが、テンプレート関数内で『%』演算子を使用する。
// そういう時に、テンプレート引数である型が浮動小数点数だった場合は、この『%』を使う関数を選択候補外にする。
// 
// そういう使い方ができるものです。
// 
// そしてここからとても大事。
// SFINAEは常に有効なものです。
// 
// 
// MYTMP FuncCreater
// enable_if_tは、flagが真ならTypeを定義、偽ならSFINAEで消えるクラス
// SFINAEで消えるというのは、オーバーロードの候補から削除するということ
// 
// void_t
// 常に働いているSFINAEを明示的に働かせるもの
// 
// このvoid_tはテンプレート引数に関わらずvoidになる。
// しかし、このテンプレート引数が無効な物。
// 
// 『underlying_type_t<int>』を例とすると、本来このコードはコンパイルエラーになります。
// ですが、c++14でデフォルトで働いているSFINAEが働く。
// その結果、コンパイルエラーにはならず、特殊化の候補から除外するだけになる。
// そしてこの『underlying_type_t<int>』というコードは、無効なコードになる。
// 
// しかしこのSFINAEの影響で、
// ・無効なコードを抱えたテンプレートメタ関数
// ・メタ関数を使った可変長引数関数
// この当たりのコードが全て、これまたSFINAEで無効なコードになる。
// 
// その結果、コンパイル時に無効なコードが削除され、
// 『引数リストに一致する……』
// というエラーが発生する。
// 
// さて、この問題を解決するのは『テンプレートの部分特殊化』です。
// c++14にはSFINAEと同じように、『テンプレート候補は、より特殊なものを選択する』というルールがあります。
// 
// この部分特殊化のルールとSFINAEを同時に使用すると良いことが起こります。
// 
// 例えば、SFINAEによってコードは無効化され、無効になったテンプレート引数は候補から除外される。
// つまりそのテンプレート引数では『特殊化できなくなる』
// ということ。
// 
// つまりどういうことか。
// 『underlying_type_t』
// のような無効化される可能性のあるコードを
// 『void_t<underlying_type_t<T>>』
// のように、void_t<>で囲む。
// 
// こうすることで『underlying_type_t<T>』が無効なコードになるかをvoid_tが確認する。
// そしてvoid_t部分で無効なコードになる。
// そうすると
// 『そのテンプレート引数は今回の特殊化の候補から外す』
// と、コンパイラは判断する。
// 
// 逆に特殊化無効なコードにならなかった場合、『より特殊なコード』を選択するルールがあるため、std::true_typeになる
// 
// ＜まとめ＞
// 無効なコードになる可能性のあるものは、
// 1. 基本形となる構造体を作成し、std::false_typeを継承させる
// 2. 部分特殊化された構造体を作成する
// 3. void_tを使い、無効化される可能性のあるコードを囲む
// 
// こうすることで
// c++14のルールである
// 『SFINAE(テンプレート引数が無効なコードだった場合、オーバーロードの候補から除外する)』
// 『オーバーロードする際はより特殊なものを選択する』
// を最大限利用することができる
// 
// ということ。そのために
// 『テンプレート引数が有効だった場合に辿り着く特殊化された構造体』
// 『無効化されたコードが辿り着く受け皿となる基本形の構造体』
// の二つを用意すればいい。
// 
// ちなみに頻出する『constexpr』修飾子ですが、これは
// 『コンパイル実行時に評価される可能性のある値』という意味。
// なんとなくニュアンスで察してくださいな。
// 

// 自作のvoid_t
// テンプレート引数が無効な物かを判断し
// 有効な場合はvoidを返し
// 無効な場合はSFINAEを起動させるクラス
template<class... Ts>
using void_t = void;

//------------------------------
// MYTMP_IF
//------------------------------
// MYTMP if
// flagの値によってTrueTypeか、FalseTypeになる値
template<bool flag, class TrueType, class FalseType>
using MYTMP_IF = std::conditional_t<flag, TrueType, FalseType>;

//------------------------------
// MYTMP_TG_
// MYTMP_TypeGet_
//------------------------------

// MYTMP TypeGet Remove Const
// constを無視したその型を取得するクラス
template<class T>
using  MYTMP_TG_RemoveC = std::remove_const_t<T>;

// MYTMP TypeGet Remove Volatile
// volatileを無視したその型を取得するクラス
template<class T>
using MYTMP_TG_RemoveV = std::remove_volatile_t<T>;

// MYTMP_TypeGet Remove Const Volatile
// const + volatileを無視したその型を取得するクラス
template<class T>
using MYTMP_TG_RemoveCV = std::remove_cv_t<T>;

// MYTMP_TypeGet Remove Reference
// 参照を無視したその型を取得するクラス
template<class T>
using MYTMP_TG_RemoveR = std::remove_reference_t<T>;

// MYTMP TypeGet Remove Const Volatile Reference
// const + volatile + 参照を無視したその型を取得するクラス
template<class T>
using MYTMP_TG_RemoveCVR = std::remove_reference_t<std::remove_cv_t<T>>;

// MYTMP TypeGet Remove Pointer
// ポインタを無視したその型を取得するクラス
template<class T>
using MYTMP_TG_RemoveP = std::remove_pointer_t<T>;

// MYTMP TypeGet Add Const
// constを付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddC = std::add_const_t<T>;

// MYTMP TypeGet Add Volatile
// volatileを付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddV = std::add_volatile_t<T>;

// MYTMP TypeGet Add Const Volatile
// const + volatileを付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddCV = std::add_cv_t<T>;

// MYTMP TypeGet Add Left Reference
// 右辺値参照を付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddLR = std::add_lvalue_reference_t<T>;

// MYTMP TypeGet Add Right Reference
// 右辺値参照を付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddRR = std::add_rvalue_reference_t<T>;

// MYTMP TypeGet Add Pointer
// ポインタを付与したその型を取得するクラス
template<class T>
using MYTMP_TG_AddP = std::add_pointer_t<T>;

// MYTMP TypeGet EnumBase
// enumの基底型を取得するクラス(enumについている参照、const等を外し取得する)
template<class T>
using MYTMP_TG_EnumBase = std::underlying_type_t<MYTMP_TG_RemoveCVR<T>>;

// MYTMP TypeGet Decay
// 配列・関数をポインタに変換しCVを無視した、値渡しする際の型を取得するクラス
template<class T>
using MYTMP_TG_Decay = std::decay_t<T>;






//------------------------------
// MYTMP_TC_
// (MYTMP_TypeCheck_)
//------------------------------

// その型がvoidかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_void
{
    static constexpr bool value = std::is_void<T>::value;
};

// その型がnullptrかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_nullptr
{
    static constexpr bool value = std::is_null_pointer<T>::value;
};

// その型が整数(enumを除く)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_int
{
    static constexpr bool value = std::is_integral<T>::value;
};

// その型が符号アリ整数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_signed
{
    static constexpr bool value = std::is_signed<T>::value;
};

// その型が符号ナシ整数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_unsigned
{
    static constexpr bool value = std::is_unsigned<T>::value;
};

// その型がenumかつ、その基底型が符号アリかを確認するクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T, class = void>
struct MYTMP_TC_SignedEnum
{
    static constexpr bool value = false;
};
template<class T>
struct MYTMP_TC_SignedEnum<T, void_t<std::underlying_type_t<T>>>
{
    static constexpr bool value = MYTMP_TC_signed<std::underlying_type_t<T>>::value;
};

// その型がenumかつ、その基底型が符号ナシかを確認するクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T, class = void>
struct MYTMP_TC_UnsignedEnum
{
    static constexpr bool value = false;
};
template<class T>
struct MYTMP_TC_UnsignedEnum<T, void_t<std::underlying_type_t<T>>>
{
    static constexpr bool value = MYTMP_TC_unsigned<std::underlying_type_t<T>>::value;
};

// その型が浮動小数点数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_floating
{
    static constexpr bool value = std::is_floating_point<T>::value;
};

// その型が配列かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_array
{
    static constexpr bool value = std::is_array<T>::value;
};

// その型がポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_ptr
{
    static constexpr bool value = std::is_pointer<T>::value;
};

// その型が左辺値参照(変数として保持されている値)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_LR
{
    static constexpr bool value = std::is_lvalue_reference<T>::value;
};

// その型が右辺値参照(変数として保持されていない値)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RR
{
    static constexpr bool value = std::is_rvalue_reference<T>::value;
};

// その型が参照かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_R
{
    static constexpr bool value = std::is_reference<T>::value;
};

// その型がクラスのメンバ変数へのポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_mpObj
{
    static constexpr bool value = std::is_member_object_pointer<T>::value;
};

// その型がクラスのメンバ関数へのポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_mpFunc
{
    static constexpr bool value = std::is_member_function_pointer<T>::value;
};

// その型がenumかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_enum
{
    static constexpr bool value = std::is_enum<T>::value;
};

// その型がclassかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_class
{
    static constexpr bool value = std::is_class<T>::value;
};

// その型がunionかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_union
{
    static constexpr bool value = std::is_union<T>::value;
};

// その型が関数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_function
{
    static constexpr bool value = std::is_function<T>::value;
};

// その型が基本型(整数型、浮動小数点数型、void、nullptr_t)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_fundamental
{
    static constexpr bool value = std::is_fundamental<T>::value;
};

// その型がインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_obj
{
    static constexpr bool value = std::is_object<T>::value;
};

// その型がスカラー(整数、浮動小数点数 ポインタ、メンバーポインタ)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_scalar
{
    static constexpr bool value = std::is_scalar<T>::value;
};

// その型が複合型(基本型意外)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_compound
{
    static constexpr bool value = std::is_compound<T>::value;
};

// その型がconst修飾されているかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_C
{
    static constexpr bool value = std::is_const<T>::value;
};

// その型がvolatile修飾されているかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_V
{
    static constexpr bool value = std::is_volatile<T>::value;
};

// その型がtrivial(コンストラクタやコピー、ムーブがユーザー未定義)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_trivial
{
    static constexpr bool value = std::is_trivial<T>::value;
};

// その型がstandard-layout(全ての変数がpublicで仮想関数が存在しない)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_standardLayout
{
    static constexpr bool value = std::is_standard_layout<T>::value;
};

// その型がPOD(trivial && standard-layout)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_POD
{
    static constexpr bool value = std::is_pod<T>::value;
};

// その型がmemcpy、memmoveできるかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_trivially_copyable
{
    static constexpr bool value = std::is_trivially_copyable<T>::value;
};

// その二つの型が同じ型かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class A, class B>
struct MYTMP_TC_Equal
{
    static constexpr bool value = std::is_same<A, B>::value;
};

// AをBに暗黙的キャストできるかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class A, class B>
struct MYTMP_TC_TypeCast
{
    static constexpr bool value = std::is_convertible<A, B>::value;
};

// TargetがBaseを継承しているかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class Target, class Base>
struct MYTMP_TC_UpCast
{
    static constexpr bool value = std::is_base_of<Base, Target>::value;
};

// デフォルトコンストラクタでインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_CreateDefault
{
    static constexpr bool value = std::is_default_constructible<T>::value;
};

// TargetがSrcでインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class Target, class Src>
struct MYTMP_TC_CreateSrc
{
    static constexpr bool value = std::is_constructible<Target, Src>::value;
};

// コピーコンストラクタでインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_CreateCopy
{
    static constexpr bool value = std::is_copy_constructible<T>::value;
};

// ムーブコンストラクタでインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_CreateMove
{
    static constexpr bool value = std::is_move_constructible<T>::value;
};

// A = Bが可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class A, class B>
struct MYTMP_TC_Ass
{
    static constexpr bool value = std::is_assignable<A, B>::value;
};

// コピー代入が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_AssCopy
{
    static constexpr bool value = std::is_copy_assignable<T>::value;
};

// ムーブ代入が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MY_VF_TT_AssMove
{
    static constexpr bool value = std::is_move_assignable<T>::value;
};

//------------------------------
// MYTMP_TC_RCVR_
// (MYTMP_TypeCheck_RemoveConstVolatileReference_)
//------------------------------

// その型が整数(enumを除く)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_int
{
    static constexpr bool value = MYTMP_TC_int<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が符号アリ整数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_signed
{
    static constexpr bool value = MYTMP_TC_signed<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が符号ナシ整数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_unsigned
{
    static constexpr bool value = MYTMP_TC_unsigned<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がenumかつ、その基底型が符号アリかを確認するクラス
template<class T>
struct MYTMP_TC_RCVR_SignedEnum
{
    static constexpr bool value = MYTMP_TC_SignedEnum<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がenumかつ、その基底型が符号ナシかを確認するクラス
template<class T>
struct MYTMP_TC_RCVR_UnsignedEnum
{
    static constexpr bool value = MYTMP_TC_UnsignedEnum<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が浮動小数点数かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_floating
{
    static constexpr bool value =MYTMP_TC_floating<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が配列かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_array
{
    static constexpr bool value =MYTMP_TC_array<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_ptr
{
    static constexpr bool value =MYTMP_TC_ptr<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がクラスのメンバ変数へのポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_mpObj
{
    static constexpr bool value =MYTMP_TC_mpObj<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がクラスのメンバ関数へのポインタかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_mpFunc
{
    static constexpr bool value =MYTMP_TC_mpFunc<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がenumかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_enum
{
    static constexpr bool value =MYTMP_TC_enum<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がclassかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_class
{
    static constexpr bool value =MYTMP_TC_class<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がunionかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_union
{
    static constexpr bool value =MYTMP_TC_union<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がclassかを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_function
{
    static constexpr bool value =MYTMP_TC_function<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が基本型(整数型、浮動小数点数型、void、nullptr_t)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_fundamental
{
    static constexpr bool value =MYTMP_TC_fundamental<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がインスタンス化が可能かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_obj
{
    static constexpr bool value =MYTMP_TC_obj<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型がスカラー(整数、浮動小数点数 ポインタ、メンバーポインタ)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_scalar
{
    static constexpr bool value =MYTMP_TC_scalar<MYTMP_TG_RemoveCVR<T>>::value;
};

// その型が複合型(基本型意外)かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class T>
struct MYTMP_TC_RCVR_compound
{
    static constexpr bool value =MYTMP_TC_compound<MYTMP_TG_RemoveCVR<T>>::value;
};

// その二つの型が同じ型かを返すクラス
// 『MYTMP_TC_～<T>::value』の形で結果のboolを取得できます
template<class A, class B>
struct MYTMP_TC_RCVR_Equal
{
    static constexpr bool value =MYTMP_TC_Equal<MYTMP_TG_RemoveCVR<A>, MYTMP_TG_RemoveCVR<B>>::value;
};

//------------------------------
// MYTMP_Creater
//------------------------------

// MYTMP CheckerBase
// integral_constantは型とその型の::valueを定義するクラス。
// 今回はboolによるフラグのみを定義する。
// なのでこの型をflagを渡して継承させることで、派生クラスにbool型の::valueを付け足すことができる
template <bool flag>
class MYTMP_CheckerBase : public std::integral_constant<bool, flag>
{
};

// 条件文(MYTMP_TC_<>::value)を受け取りCheckerクラスを生成するクラス
// class Checker : public MYTMP_Kit_TypeCheckerBase<MYTMP_TC_<>::value>{};
// or
// struct Checker : MYTMP_Kit_TypeCheckerBase<MYTMP_TC_<>::value>{};
template <bool flag>
class MYTMP_Kit_TypeCheckerBase : public MYTMP_CheckerBase<flag>
{
};

// c++14で使用する自作のconjunction
// というのは世を忍ぶ仮の姿。

// これはTypeCheckerの条件に合っているかを確認するTypeFilter
template <class...>
struct MYTMP_TypeFilter : std::true_type
{
};

// これはTypeCheckerの条件に合っているかを確認するTypeFilter
template <class TypeChecker>
struct MYTMP_TypeFilter<TypeChecker> : TypeChecker
{
    // B1がMYTMP_CheckerBaseを継承しているかを確認し
    // 継承していない場合はコンパイルエラーを起こす
    static_assert(
        MYTMP_TC_UpCast<TypeChecker, MYTMP_CheckerBase<true>>::value ||
        MYTMP_TC_UpCast<TypeChecker, MYTMP_CheckerBase<false>>::value,
        "MyTypeFilter parameters must derive from MYTMP_CheckerBase");
};

// これはTypeCheckerの条件に合っているかを確認するTypeFilter
template <class TypeChecker0, class... TypeCheckerN>
struct MYTMP_TypeFilter<TypeChecker0, TypeCheckerN...> : MYTMP_IF<TypeChecker0::value, MYTMP_TypeFilter<TypeCheckerN...>, TypeChecker0>
{
    // B1がMYTMP_CheckerBaseを継承しているかを確認し
    // 継承していない場合はコンパイルエラーを起こす
    static_assert(
        MYTMP_TC_UpCast<TypeChecker0, MYTMP_CheckerBase<true>>::value ||
        MYTMP_TC_UpCast<TypeChecker0, MYTMP_CheckerBase<false>>::value,
        "MyTypeFilter parameters must derive from MYTMP_CheckerBase");
};

// これはTypeCheckerの条件に合っているかを確認するTypeFilterの結果
template<class... TypeCheckers>
constexpr bool MYTMP_TypeFilter_v = MYTMP_TypeFilter<TypeCheckers...>::value;

// enable_ifを使い、削除するかどうか分岐させるエイリアス
// flagとReturnTypeを渡し、flagがfalseだった場合は型の存在が削除する
template<bool flag, class ReturnType = void>
using MYTMP_SFINAE = std::enable_if_t<flag, ReturnType>;

// 第1引数の型を第2引数のTypeCheckerで確認する関数。
// 条件に合致していた場合はした場合はそのstd::true_type、失敗だった場合はstd::false_typeになる
// 第1引数 : 確認したい型
// 第2引数 : MYTMP_Kit_TypeCheckerBaseを継承したTypeChecker
// 
// 変数の型を確認したい場合は第1引数にdecltype(T::variable)と渡してください
template<class TargetType, template<class> class TypeChecker, class = void>
class MYTMP_Kit_TypeAnalizer : public std::false_type {};

// 第1引数の型を第2引数のTypeCheckerで確認する関数。
// 条件に合致していた場合はした場合はそのstd::true_type、失敗だった場合はstd::false_typeになる
// 第1引数 : 確認したい型
// 第2引数 : MYTMP_Kit_TypeCheckerBaseを継承したTypeChecker
// 
// 変数の型を確認したい場合は第1引数にdecltype(T::variable)と渡してください
template<class TargetType, template<class> class TypeChecker>
class MYTMP_Kit_TypeAnalizer<TargetType, TypeChecker, MYTMP_SFINAE<MYTMP_TypeFilter<TypeChecker<TargetType>>::value>> : public std::true_type {};

//----------------------
// 通常関数関係
//----------------------

// SFINAEを使用し、型チェックやラムダチェック済みの関数を生成するためのクラス
// template<TargetType...>
// MYTMP_Kit_FuncCreater<ReturnType, ArgsChecker, TargetType...>::return_type FunctionName(TargetType&&... target){}
template<class ReturnType, template<class> class ArgsChecker, class... ArgumentType>
class MYTMP_Kit_FuncCreater : public MYTMP_SFINAE<MYTMP_TypeFilter<ArgsChecker<MYTMP_TG_Decay<ArgumentType>>...>::value, ReturnType>
{
public:
    using return_type = ReturnType;
};

// コンストラクタを型制限ありの可変長引数にできるクラス
// template<template class> Args, MYTMP_Kit_ConstructorCreater<TypeChecker, Args...> = 0>
// Constructor(Args... args) {}
template<template <class> class ArgsChecker, class... Args>
using MYTMP_Kit_ConstructorCreater = MYTMP_SFINAE<MYTMP_TypeFilter<ArgsChecker<Args>...>::value, int>;

// ラムダ式で関数を受け取り、可変長引数関数の引数毎のループで行う処理を設定する関数(渡すラムダの型チェックはしない)
// MYTMP_Kit_FuncLoopLambda([](auto x) { lambda },std::forward<TargetType>(target)...);
template<class LambdaFunction, class... TargetType>
void MYTMP_Kit_FuncLoopLambda(LambdaFunction &&function, TargetType&&... args)
{
    (void)std::initializer_list<int>
    {
        (function(std::forward<TargetType>(args)), 0)...
    };
}

//----------------------
// ラムダ関係
//----------------------

// 関数の返り値型と引数型を確認するクラス(ここの定義で特殊化しないことで、初めて使用できる)
// 普通のグローバル関数、ラムダ式、メンバ関数(const、非const両対応)の型確認ができる
template<typename>
struct FunctionReader;

// 通常の関数ポインタを取得し、分析する
template<typename ReturnType, typename... ArgumentType>
struct FunctionReader<ReturnType(*)(ArgumentType...)>
{
    using return_type = ReturnType;
    using args_tuple = std::tuple<ArgumentType...>;

    // 引数の数がどれだけあるかをsizeof...()で確認する
    static constexpr std::size_t arity = sizeof...(ArgumentType);

    // 指定されたindexの型を取得するテンプレートクラス
    template<std::size_t index>
    struct ArgumentIndexType
    {
        static_assert(index < arity, "index out of range");
        using type = std::tuple_element_t<index, args_tuple>;
    };
};

// constメンバ関数ポインタを取得し、分析する
template<typename ClassType, typename ReturnType, typename... ArgumentType>
struct FunctionReader<ReturnType(ClassType:: *)(ArgumentType...) const>
{
    using return_type = ReturnType;
    using args_tuple = std::tuple<ArgumentType...>;

    // 引数の数がどれだけあるかをsizeof...()で確認する
    static constexpr std::size_t arity = sizeof...(ArgumentType);

    // 指定されたindexの型を取得するテンプレートクラス
    template<std::size_t index>
    struct ArgumentIndexType
    {
        static_assert(index < arity, "index out of range");
        using type = std::tuple_element_t<index, args_tuple>;
    };
};

// 非constメンバ関数ポインタを取得し、分析する
template<typename ClassType, typename ReturnType, typename... ArgumentType>
struct FunctionReader<ReturnType(ClassType:: *)(ArgumentType...)> :
    FunctionReader<ReturnType(ClassType:: *)(ArgumentType...) const>
{
};

// ラムダ式を取得し、分析する
template<typename FunctionType>
struct FunctionReader :
    // ラムダ式はoperater()を持っているの
    // なのでそれを取得して、非constメンバ関数の形式を使用して関数を分析する
    FunctionReader<decltype(&FunctionType::operator())>
{
};

// ラムダ式の返り値と引数を解析し、結果を取得するための型(CheckerとFilterの両方の性質を兼ね備えるため、Analyzerです)
template<
    typename FunctionType,
    template<class> class ReturnChecker,
    template<class> class ArgsChecker,
    size_t ArgumentMin,
    size_t ArgumentMax,
    typename... ArgumentTypes>
struct FunctionAnalyzer
{
private:
    // 関数の型制限コンテキスト
    using FuncReader = FunctionReader<FunctionType>;

    // ReturnCheckerがMYTMP_CheckerBaseを継承しているかを確認し
    // 継承していない場合はコンパイルエラーを起こす
    static_assert(
        MYTMP_TC_UpCast<ReturnChecker, MYTMP_CheckerBase<true>>::value ||
        MYTMP_TC_UpCast<ReturnChecker, MYTMP_CheckerBase<false>>::value,
        "FunctionAnalyzer ReturnChecker must derive from MYTMP_CheckerBase");

    // ReturnCheckerがMYTMP_CheckerBaseを継承しているかを確認し
    // 継承していない場合はコンパイルエラーを起こす
    static_assert(
        MYTMP_TC_UpCast<ArgsChecker, MYTMP_CheckerBase<true>>::value ||
        MYTMP_TC_UpCast<ArgsChecker, MYTMP_CheckerBase<false>>::value,
        "FunctionAnalyzer ArgsChecker must derive from MYTMP_CheckerBase");

    // 今回の関数の返り値の型が条件に一致しているかを確認する関数
    static constexpr bool check_return()
    {
        return MYTMP_TypeFilter<ReturnChecker<MYTMP_TG_Decay<typename FuncReader::return_type>>>::value;
    }

    // 今回の関数の引数の数を設定し、引数の個数分計算を行う関数
    template<std::size_t... I>
    static constexpr bool check_args(std::index_sequence<I...>)
    {
        // 関数の返り値
        return

            // MyTypeFilterを呼び出し、MyTypeFilter::valueが今回の関数の返り値
            MYTMP_TypeFilter

            // 関数の引数型の型特性を確認するためのフィルタークラス
            <ArgsChecker

            // 値渡しの型に直す
            <std::decay_t

            // 今回の関数の引数型を指定のインデックスで取得する(ここで『...』はしない)
            <typename FuncReader::template ArgumentIndexType<I>::type>

            // 値渡しの型に直す部分を終わらせる
            >

            // 以上の事をパック毎に行い、フィルタークラスの引数を終了させる
            ...>

            // MyTypeFilterの結果を取得し、これがこの関数の返り値になる
            ::value;
    }

public:
    // この関数が有効か確認した結果
    static constexpr bool value =

        // ラムダ式の引数が指定の個数に納まっていた場合のみtrue
        ArgumentMin <= FuncReader::arity && FuncReader::arity <= ArgumentMax &&

        // 返り値の型が一致しているかを確認する
        check_return() &&

        // 引数型チェック
        check_args(std::make_index_sequence<FuncReader::arity>{});
};

// ラムダ式の返り値と引数を制限するための型
// template<typename FunctionType, typename... ArgumentType>
// class LambdaFilter : public MYTMP_Kit_LambdaCheckerBase<FunctionType, ReturnChecker, ArgsChecker, 1, 1, ArgumentType...>
template<
    typename FunctionType,
    template<class> class ReturnChecker,
    template<class> class ArgsChecker,
    size_t ArgumentMin,
    size_t ArgumentMax,
    typename... ArgumentTypes>
class MYTMP_Kit_LambdaCheckerBase: public MYTMP_CheckerBase<
    FunctionAnalyzer<FunctionType, ReturnChecker, ArgsChecker, ArgumentMin, ArgumentMax, ArgumentTypes>::value
>
{
};

// 制限のある可変長引数関数を受け取り、使用する関数の戻り値エイリアス
// template<TargetType...>
// MYTMP_Kit_LambdaFuncCreater<ReturnType, FunctionChecker, TargetType...> FunctionName(TargetType&&... target){}
template<
    class ReturnType,
    typename FunctionType,
    template<typename, typename...> class FunctionChecker,                              // ラムダ式の返り値と戻り値の型チェック用クラス
    typename... TargetType>
class MYTMP_Kit_FuncFuncCreater : public MYTMP_SFINAE<
    FunctionChecker<FunctionType, TargetType...>::value,
    ReturnType>
{
public:
    using return_type = ReturnType;
};

// 可変長引数の引数パックを丸ごと使用するラムダ式を簡単に使用するための関数
template<
    typename FunctionType,
    template<typename, typename...> class FunctionChecker,
    typename... ArgumentType>
typename MYTMP_Kit_FuncFuncCreater<                                                     // ラムダのファンクションクリエイターを起動する(ここで型チェックを行う)
    decltype(std::declval<FunctionType>()(std::declval<ArgumentType>()...)),            // 返り値の型推論(返り値の型チェックは行わない)
    FunctionType,                                                                       // 関数型そのもの
    FunctionChecker,                                                                    // 関数型フィルターを渡す
    ArgumentType...>                                                                    // 引数型パックを渡す
    ::return_type MYTMP_Kit_LambdaPackAll(FunctionType &&func, ArgumentType&&... arguments)
{
    return std::forward<FunctionType>(func)(std::forward<ArgumentType>(arguments)...);  // パックを丸ごと渡して関数を呼び出す
}

// 可変長引数の引数パックを一つ一つループで使用するラムダ式を簡単に使用するための関数(返り値void)
template<
    typename FunctionType,
    template<typename, typename...> class FunctionChecker,
    typename... ArgumentType>
typename MYTMP_Kit_FuncFuncCreater<
    void,                                                                               // 返り値はvoid固定(返り値の型推論ができないため？)
    FunctionType,                                                                       // 関数型そのもの
    FunctionChecker,                                                                    // 関数型フィルターを渡す
    ArgumentType...>                                                                    // 引数型パックを渡す
    ::return_type MYTMP_Kit_LambdaPackOneLoop(FunctionType &&func, ArgumentType&&... arguments)
{
    (void)std::initializer_list<int>
    {
        (std::forward<FunctionType>(func)(std::forward<ArgumentType>(arguments)), 0)...
    };
}

//----------------------
// コンストラクタ関係
//----------------------

// 
template<typename ClassType, typename... ArgumentTypes>
class ConstructorAnalyzer : public MYTMP_CheckerBase<MYTMP_TC_CreateSrc<ClassType, ArgumentTypes...>::value>
{
};

//----------------------
// 可変長テンプレート引数関係
//----------------------

// 
template<class... Types>
class MYTMP_TypeCount
    : public std::integral_constant<std::size_t, sizeof...(Types)>
{
};

template<class... Types>
constexpr std::size_t MYTMP_TypeCount_v = MYTMP_TypeCount<Types...>::value;

// 再帰してテンプレート引数で渡された型のインスタンスを生成する関数
template<std::size_t Index, class Base, class... Types>
struct MYTMP_NewByIndex;

// 再帰してテンプレート引数で渡された型のインスタンスを生成する関数
template<std::size_t Index, class Base>
struct MYTMP_NewByIndex<Index, Base>
{
    static Base *Create(std::size_t)
    {
        return nullptr;
    }
};

// 再帰してテンプレート引数で渡された型のインスタンスを生成する関数
template<std::size_t Index, class Base, class Head, class... Tail>
struct MYTMP_NewByIndex<Index, Base, Head, Tail...>
{
    static Base *Create(std::size_t index)
    {
        if (index == Index)
        {
            return new Head;
        }

        return MYTMP_NewByIndex<Index + 1, Base, Tail...>::Create(index);
    }
};

// テンプレート引数で渡された型のインスタンスを生成する関数
// newを使っているので生成された後のインスタンスは必ずdeleteしてください
template<class Base, class... Types>
Base *MYTMP_Kit_CreateByIndex(std::size_t index)
{
    return MYTMP_NewByIndex<0, Base, Types...>::Create(index);
}

// 型タグ
template<class T>
struct MYTMP_TypeTag
{
    using type = T;
};

template<class... Types>
struct MYTMP_TypeCaller
{
    template<class ReturnType, class Function>
    static ReturnType Call(
        Function &&function,
        std::size_t index)
    {
        ReturnType result{};
        std::size_t current = 0;

        MYTMP_Kit_LambdaPackOneLoop(
            [&](auto tag)
            {
                if (current == index)
                {
                    result = function(tag);
                }

                ++current;
            },
            MYTMP_TypeTag<Types>{}...
        );

        return result;
    }
};

// 以上がTMPに使用する関数群です。



// 以下がMYTMPを使ったクラスや関数の実装例です。
// 是非、参考にしてください

// 型条件クラスの実装例
template<class T>
class UnsignedOnly : public MYTMP_Kit_TypeCheckerBase<
    (MYTMP_TC_RCVR_UnsignedEnum<T>::value) ||
    (MYTMP_TC_RCVR_int<T>::value && MYTMP_TC_RCVR_unsigned<T>::value && !MYTMP_TC_RCVR_Equal<T, bool>::value)>
{
};

// 引数に型制限のある可変長引数関数の実装例
template<class... TargetType>
MYTMP_Kit_FuncCreater<int, UnsignedOnly, TargetType...> TestFunction(TargetType&&... target)
{
    MYTMP_Kit_FuncLoopLambda(
        [](auto&& x) 
        {
            std::cout << static_cast<unsigned long>(x) << '\n';
        },
        std::forward<TargetType>(target)...
    );
    return 0;
}

// 関数条件クラスの実装例(可変長引数関数を渡しても可？)
template<typename FunctionType, typename... ArgumentType>
class TestLambdaChecker : public MYTMP_Kit_LambdaCheckerBase<FunctionType, UnsignedOnly, UnsignedOnly, 1, 1, ArgumentType...>
{
};

// 使用する関数に制限を設けた、内部で受け取った関数を使用する可変長引数関数の実装例
template<typename FunctionType, typename... TargetType>
MYTMP_Kit_FuncFuncCreater<int, FunctionType, TestLambdaChecker, TargetType...> TestLambdaFunc(FunctionType &&func, TargetType &&... target)
{
    // 関数をそのまま使用する
    func(target...);

    // 引数パックを丸ごと渡し可変長引数対応のラムダ式を呼ぶ
    // このラムダ内でパック展開を行う
    MYTMP_Kit_LambdaPackAll<TestLambdaChecker>(func, target...);

    // 引数パックを丸ごと渡し可変長引数対応のラムダ式を呼ぶ
    // このラムダ内でパック展開を行う
    MYTMP_Kit_LambdaPackOneLoop<TestLambdaChecker>(func, target...);

    // 関数の返り値
    return 0;
}


/*
    ～std::underlying_type_t<T> と std::underlying_type<T>::type の違いについて～

    端的に言うとこの二つの違いは依存型か固定型かの違い。
    このテンプレートメタプログラミングはコンパイル時にプログラムの一部を動かしてしまうもの。
    なので、コンパイル時点でその::typeは型なのか値なのかがわからないと、うまく動かすことができない。

    だから「これは型ですよ」というためのものがある。
    それが『typename』です。
    typename ～::type; とすることで、初めてコンパイル時点で『::typeは型なのだ』と認識してくれる。
    でもtypenameを書いても、テンプレートクラスに依存するので、エラーが起こるときが多い。
    だから_tを使用するんです。



    ～テンプレートテンプレートクラスについて～

    テンプレートクラスを受け取る場合がある
    template<template<class> class Temp>

    まず『<template unknown> class Temp<unknown>』を作る必要がある。
    つまりテンプレートクラスを一度作る。

    そして目的のクラスを
    template<template<class> class TempTemp>
    class Target
    {
        TempTemp<int>
    };
    こう定義する。
    そして目的のクラスの変数を

    Target<Temp> param;
    こう定義すると、実際の型は
    class Target
    {
        Temp<int>
    };
    となる。

    つまりテンプレートテンプレートクラス
    TempTemp<???> → Temp<???>となる。



    ～パックについて～
    そもそもパックとは『キュー』のようなイメージで大丈夫。
    ただリストにまとめられるものが二つある。

    1. 型パック
    template<typename... TypePack>
    TypePack[int, double, char]

    2. 値パック
    template<int... ParamPack>
    ParamPack[1, -2, 3]


    こんな感じ。これがそれぞれのパック。
    そして次はパック展開。
    パック展開とは『パックに含まれる要素を順番に展開して処理すること』(by ChatGpt)らしいです。
    では次にパック展開の方法について

    template<typename Type>
    void print(Type param)
    {
        std::cout < param < endl;
    }

    ↑は至って普通のテンプレート関数です。パックの記述ががないでしょ？
    ↓はパックのあるテンプレート関数。

    template<typename HeadType, typename... Type>
    void print(HeadType head, Type... type)
    {
        std::cout < head < endl;
        print(type...);             // ←ここが大事！
    }

    コメントアウトにも描いてある通り、大事なのは『print(type...);』のところ
    まず、パックありverとなしverの関数を用意するんです。

    パックはキューとほぼ同じなので、キューの中身が1つしかない場合もあれば、2つある場合もある。
    そういうときにprint(type...)で再帰的に関数を呼び出す。
    もしtype... == { 1, 0.5, } の場合は
    print(1, 0.5);
    となる。
    でもtype... == { 1 } の場合は
    print(1);
    となる。
    こうやってパックありの関数となしの関数を用意し、再帰させることでパック展開を行う。










    ～MyTypeCheckerについて～
    std::conditional<>::typeの引数は
    std::conditional<bool condition, TypeA, TypeB>::typeとなっている。
    渡したconditionがtrueならTypeAを返し、falseならTypeBを返す。

    そして今回ポイントとなるのが、この実装だと
    『渡すTypeAやTypeBはstd::integral_constant<bool, ...>を継承している前提』
    であること。

    std::integral_constant<>は型と値を同時に持っているもので、つまりbool型でありながら、trueかfalseも持っている。
    これが




*/
