#pragma once

template<class T> struct Property
{
	T& r;
	operator T() { return r; }
	void operator =(const T v) { r = v; }
};
