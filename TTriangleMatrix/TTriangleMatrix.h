#pragma once
#include "TMathVector.h"
#include "TVector.h"
#include "TMemData.h"
template <typename Q>
class TTriangleMatrix : private TMathVector<TMathVector<Q>> {
	public:
		using TVector<Q>::size();
		using TMathVector<Q>::size()
};