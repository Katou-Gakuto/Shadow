#pragma once

/*
＜このヘッダについて＞
このヘッダは自作の様々なクラスを使用する際に、クラスの拡張を自動で行えるようにするためのヘッダです。
このヘッダの中身を変更ことで、各クラスの定義などが自動で変更されます。

＜このヘッダの使用方法＞
このヘッダは基本的に

// ---このマクロ宣言において、最も重要なヘッダファイル(※パスではない)---
// このマクロ宣言の概要
#define ～～～

が並んでいます。
使用するクラス名のマクロ宣言はそのままにし、
使用するクラス名のマクロ宣言はコメントアウトしてください。
そうすることで場合に応じたクラスの自動拡張が行われます。
*/

// ---UseDeclaration.h---
// このヘッダを使っているという証拠(このマクロ宣言は絶対にコメントアウトしないでください)
#define USE_DECLARETION_ACTIVE (1)

/* 以下本文 */
#ifdef USE_DECLARETION_ACTIVE

// ---VECTOR.h---
// VECTOR2D(2次元ベクトルを表すクラス)の使用を明示的にするマクロ
#define USE_MY_VECTOR2D (1)

// ---VECTOR.h---
// VECTOR3D(3次元ベクトルを表すクラス)の使用を明示的にするマクロ
#define USE_MY_VECTOR3D (1)

// ---MyMatrix.h---
// MyMatrix(行の個数と列の個数を自由に設定できる行列のクラス)の使用を明示的にするマクロ
#define USE_MY_MATRIX (1)

// ---EulerOrder.h---
// 自作の回転系クラスの使用を明示的にするマクロ
#define USE_MY_ROTATION (1)
#ifdef USE_MY_ROTATION

// ---MyQuaternion.h---
// MyQuaternion(任意軸の回転量を表すのクラス)の使用を明示的にするマクロ
#define USE_MY_QUATERNION (1)

// ---MyEulerAngle.h---
// MyEulerAngle(オイラー角によって回転量を表すクラス)の使用を明示的にするマクロ
#define USE_MY_EULERANGLE (1)

#endif // USE_MY_ROTATION
#endif // USE_DECLARETION_ACTIVE