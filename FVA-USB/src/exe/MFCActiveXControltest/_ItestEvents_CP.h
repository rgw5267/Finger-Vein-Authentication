#pragma once

using namespace ATL;

template <class T>
class CProxy_ItestEvents : public IConnectionPointImpl<T, &__uuidof(_ItestEvents), CComDynamicUnkArray>
{
	// ����: �������������������
public:
};
