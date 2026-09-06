#pragma once

// ステート変更条件情報
template<typename stateNumber, typename conditionData>
struct STATE_CHANGE_CRITERIA_DATA
{
    stateNumber ChangeNumber;    // 変更ステートナンバー
    bool(*ChangeFlag)(conditionData* changeConditionData);   // 変更フラグ用関数

    STATE_CHANGE_CRITERIA_DATA() = default;

    STATE_CHANGE_CRITERIA_DATA(stateNumber changeNumber, bool (*changeFlag)(conditionData*))
    : ChangeNumber(changeNumber)
    , ChangeFlag(changeFlag)
    {
    }
};