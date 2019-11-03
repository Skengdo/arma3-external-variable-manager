#include "ArmaTools.h"
#include "MemoryManager.h"

int main( int argc, char* argv[] )
{
	ArmaTools::MemoryManager* memory_manager = new ArmaTools::MemoryManager( );
	ArmaTools::World* world_manager = new ArmaTools::World( );
	ArmaTools::VariableManager* variable_manager = new ArmaTools::VariableManager( );

	variable_manager->DumpVariables( );
	variable_manager->EditVariable( "exile_poptabs", 9999999.f );

	return 0;
}