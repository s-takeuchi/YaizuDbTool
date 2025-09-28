#pragma once

class FilteringCondition
{
public:
	static const int COLUMNNAME_LENGTH = 65;
	static const int COLUMNVAL_LENGTH = 64;

	wchar_t ColumnName[COLUMNNAME_LENGTH];
	int OpeType;
	wchar_t ColumnVal[COLUMNVAL_LENGTH];
	FilteringCondition* Next;
};
