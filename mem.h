#pragma once
#include "pch.h"
#include <vector>

#include <windows.h>

namespace mem
{
	uintptr_t FindDMAAddr(uintptr_t ptr, std::vector<unsigned int> offsets);
}

