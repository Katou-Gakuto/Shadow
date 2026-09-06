#pragma once
#include <iostream>
#include <type_traits>

#ifdef _DEBUG
// エラー時用
static bool BIT_FAILURE_FLAG = false;
#endif

// ビットフラグ用構造体
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value&& std::is_unsigned<T>::value>::type>
struct BIT_FLAG
{
public:
	T flags;// フラグ

	BIT_FLAG() : flags((T)0) {}
	BIT_FLAG(T flag) : flags(flag) {}

	operator T&(){
		return this->flags;
	}

	bool operator &(T flag) const {
		return ((this->flags & flag) != 0);
	}

	/*初期化*/
	inline void Init() { flags = (T)0; }
	/// <summary>指定のbit以外初期化</summary>
	inline void Init(T bit) { SetAndBit(bit); }
	/// <summary>指定のbit以外初期化</summary>
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void Init(ENUM_T bit) { SetAndBit(bit); }
	/*全ビット有効*/
	inline void SetAllBit() { flags = ~((T)0); }

	/*1のビットがあれば「true」を返す*/
	inline bool Bool() const {
		return this->flags != 0;
	}

	inline unsigned long long BitSize()
	{
		return sizeof(T) * 8ULL;
	}

	/*-----【指定ビット設定】-----*/
	inline void SetFlag(bool flag, int number){
		if (flag){
			EnableFlag(number);
		}
		else
		{
			DisableFlag(number);
		}
	}
	/*【指定ビット設定】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetFlag(bool flag, ENUM_T number) { SetFlag(flag, (int)number); }
	/*----------------------------*/


	/*-----【指定ビット有効化】-----*/
	inline void EnableFlag(int number){
#ifdef _DEBUG
		if (CheckNumber(number)){
			flags |= ((T)1 << number);
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		flags |= ((T)1 << number);
#endif
	}
	/*【指定ビット有効化】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void EnableFlag(ENUM_T number) { EnableFlag((int)number); }
	/*------------------------------*/


	/*-----【指定ビット無効化】-----*/
	inline void DisableFlag(int number){
#ifdef _DEBUG
		if (CheckNumber(number)){
			flags &= ~((T)1 << number);
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		flags &= ~((T)1 << number);
#endif
	}
	/*【指定ビット無効化】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void DisableFlag(ENUM_T number) { DisableFlag((int)number); }
	/*------------------------------*/


	/*-----【指定ビット反転】-----*/
	inline void InvertFlag(int number){
#ifdef _DEBUG
		if (CheckNumber(number)){
			flags ^= ((T)1 << number);
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		flags ^= ((T)1 << number);
#endif
	}
	/*【指定ビット反転】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void InvertFlag(ENUM_T number) { InvertFlag((int)number); }
	/*----------------------------*/
	

	/*-----【指定ビット取得】-----*/
	inline bool GetFlag_BitShift(int number) const {
#ifdef _DEBUG
		if (CheckNumber(number)){
			return ((flags & ((T)1 << number)) != 0);
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		return ((flags & ((T)1 << number)) != 0);
#endif
		return false;
	}
	/*【指定ビット取得】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline bool GetFlag_BitShift(ENUM_T number) const { return GetFlag_BitShift((int)number); }
	/*----------------------------*/


	/*-----【ビット取得】-----*/
	inline bool GetFlag_Bit(int bit) const {
#ifdef _DEBUG
		return ((flags & bit) != 0);
#else
		return ((flags & bit) != 0);
#endif
		return false;
	}
	/*【指定ビット取得】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline bool GetFlag_Bit(ENUM_T bit) const { return GetFlag_Bit((T)bit); }
	/*----------------------------*/


	/*-----【指定ビット数から数字を設定する】-----*/
	inline void SetNumber(T setNumber, T numberZone, int number)
	{
#ifdef _DEBUG
		if (CheckNumber(number)){
			flags = (flags & ~(numberZone << number)) | ((setNumber & numberZone) << number);
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		flags = (flags & ~(numberZone << number)) | ((setNumber & numberZone) << number);
#endif
	}
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetNumber(T setNumber, T numberZone, ENUM_T number) { SetNumber(setNumber, numberZone, (int)number); }
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetNumber(ENUM_T setNumber, ENUM_T numberZone, ENUM_T number) { SetNumber((T)setNumber, (T)numberZone, (int)number); }
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetNumber(ENUM_T setNumber, ENUM_T numberZone, int number) { SetNumber((T)setNumber, (T)numberZone, number); }
	/*--------------------------------------------*/

	/*-----【指定ビット数から数字を取得する】-----*/
	inline T GetNumber(T numberZone, int number, int rightBitNumber = -1) const
	{
		if (rightBitNumber == -1)
		{
			rightBitNumber = number;
		}

#ifdef _DEBUG
		if (CheckNumber(number)){
			return (flags & (numberZone << number)) >> rightBitNumber;
		}
		else
		{
			BIT_FAILURE_FLAG = true;
		}
#else
		return (flags & (numberZone << number)) >> rightBitNumber;
#endif
		return T(0);
	}
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline T GetNumber(T numberZone, ENUM_T number, ENUM_T rightBitNumber = (ENUM_T)-1) const { return GetNumber(numberZone, (int)number, (int)rightBitNumber); }
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline T GetNumber(ENUM_T numberZone, ENUM_T number, ENUM_T rightBitNumber = (ENUM_T)-1) const { return GetNumber((T)numberZone, (int)number, (int)rightBitNumber); }
	/*【指定ビット数から数字を設定する】*/
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline T GetNumber(ENUM_T numberZone, ENUM_T number, int rightBitNumber) const { return GetNumber((T)numberZone, (int)number, rightBitNumber); }
	/*--------------------------------------------*/

	/*Tのビット数を超えてないかを確認する(超えていれば「false」を返す)*/
	static inline bool CheckNumber(int number){
		if (((sizeof(T) * 8) > number) && (number >= 0)){
			return true;
		}
		return false;
	}

	/*指定のビットだけ1の整数を取得*/
	inline static BIT_FLAG BIT_GET(int number) { return BIT_FLAG((T)1 << number); }

	
	/*----- AND演算で設定 -----*/
	/// <summary>AND演算して設定する  &=</summary>
	inline void SetAndBit(T bitVariable)
	{
		flags &= bitVariable;
	}
	/// <summary>AND演算して設定する  &=</summary>
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetAndBit(ENUM_T bitVariable) { SetAndBit((T)bitVariable); }
	/*-------------------------*/

	/*----- OR演算で設定 -----*/
	/// <summary>OR演算して設定する  |=</summary>
	inline void SetOrBit(T bitVariable)
	{
		flags |= bitVariable;
	}
	/// <summary>OR演算して設定する  |=</summary>
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetOrBit(ENUM_T bitVariable) { SetOrBit((T)bitVariable); }
	/*------------------------*/

	/*----- XOR演算で設定 -----*/
	/// <summary>XOR演算して設定する  ^=</summary>
	inline void SetXorBit(T bitVariable)
	{
		flags ^= bitVariable;
	}
	/// <summary>XOR演算して設定する  ^=</summary>
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetXorBit(ENUM_T bitVariable) { SetXorBit((T)bitVariable); }
	/*-------------------------*/

	/*----- NOT演算で設定 -----*/
	/// <summary>No\OT演算して設定する  = ~</summary>
	inline void SetNotBit(T bitVariable)
	{
		flags = ~bitVariable;
	}
	/// <summary>NOT演算して設定する = ~</summary>
	template<typename ENUM_T, typename = typename std::enable_if<std::is_enum<ENUM_T>::value>::type>
	inline void SetNotBit(ENUM_T bitVariable) { SetNotBit((T)bitVariable); }
	/*-------------------------*/
};