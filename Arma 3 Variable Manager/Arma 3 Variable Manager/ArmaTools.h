#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>

#include "MemoryManager.h"

namespace ArmaTools
{
#define __BASE static constexpr std::ptrdiff_t
#define __OFFSET static constexpr std::intptr_t

	class World : public MemoryManager
	{
	public:
		World( );

		std::intptr_t world_rva;
	private:
		__BASE world_offset = 0x257D508;
	};

	class VariableManager : public World
	{
	public:
		VariableManager( );

		virtual void DumpVariables( );
		virtual void EditVariable( const std::string variable, float value);

		std::intptr_t namespace_rva;
	private:
		__OFFSET namespace_offset = 0x1218;
		
		/* First Is Variable Name, Second Is Variable's Value & Third Is Variables Address */
		std::vector<std::pair<std::string, std::pair<float, std::intptr_t>>> GetAllMissionVariables( );
	};
}