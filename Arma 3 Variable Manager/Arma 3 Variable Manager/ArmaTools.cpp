#include "ArmaTools.h"

ArmaTools::World::World( )
{
	this->world_rva = ReadVirtualMemory<std::intptr_t>( this->arma_img_base + this->world_offset );
}

ArmaTools::VariableManager::VariableManager( )
{
	this->namespace_rva = ReadVirtualMemory<std::intptr_t>( this->world_rva + this->namespace_offset );
}

std::vector<std::pair<std::string, std::pair<float, std::intptr_t>>> ArmaTools::VariableManager::GetAllMissionVariables( )
{
	std::vector<std::pair<std::string, std::pair<float, std::intptr_t>>> variables_list;

	auto curr_namespace = ReadVirtualMemory<std::intptr_t>( this->namespace_rva + 0x20 );

	if ( !curr_namespace )
		return {};

	auto curr_namespace_size = ReadVirtualMemory<unsigned __int32>( this->namespace_rva + 0x28 );

	for ( auto i = 0u; i <= curr_namespace_size; ++i )
	{
		auto curr_namespace_entry = ReadVirtualMemory<std::intptr_t>( curr_namespace + ( 0x18 * i ) );

		if ( !curr_namespace_entry )
			return {};

		auto curr_namespace_entry_size = ReadVirtualMemory<unsigned __int32>( curr_namespace + ( ( i * 0x18 ) + 0x8 ) );

		if ( curr_namespace_entry ) {

			for ( auto i_second = 0u; i_second <= curr_namespace_entry_size; ++i_second )
			{
				auto namespace_table_entry = ReadVirtualMemory<std::intptr_t>( curr_namespace_entry + ( ( 0x28 * i_second ) + 0x8 ) );

				if ( !namespace_table_entry )
					return {};

				auto curr_variable_tag = ReadArmaString( namespace_table_entry );

				auto curr_variable = ReadVirtualMemory<std::intptr_t>( namespace_table_entry + ( ( 0x28 * i_second ) + 0x18 ) );

				if ( !curr_variable )
					return {};

				float variable_value = ReadVirtualMemory<float>( curr_variable + 0x18 );

				variables_list.push_back( std::make_pair( curr_variable_tag, std::make_pair( variable_value, curr_variable )) );
			}
		}
	}
}

void ArmaTools::VariableManager::DumpVariables( )
{
	std::ofstream output_file( "variable_dump.txt" );

	for ( std::pair<std::string, std::pair<float, std::intptr_t>> current_variable : this->GetAllMissionVariables( ) )
	{
		output_file << current_variable.first << " = " << current_variable.second.first << " @ " << current_variable.second.second << std::endl;
	}

	output_file.close( );
}

void ArmaTools::VariableManager::EditVariable( const std::string variable, float value )
{
	for ( std::pair<std::string, std::pair<float, std::intptr_t>> current_variable : this->GetAllMissionVariables( ) )
	{
		if ( current_variable.first.find( variable ) != std::string::npos )
		{
			WriteVirtualMemory<float>( value, current_variable.second.second );
			return;
		}
	}
}