#include "MemoryManager.h"

ArmaTools::MemoryManager::MemoryManager( )
{
	this->arma_proc_id = GetProcId( L"arma_x64.exe" );
	this->arma_img_base = GetModuleBaseAddress( this->arma_proc_id, L"arma_x64.exe" );
	this->arma_handle = GetProcHandle( this->arma_proc_id );
}

int32_t ArmaTools::MemoryManager::GetProcId( const std::wstring& proc_name )
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof( processInfo );

	HANDLE proc_list_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if ( proc_list_snapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First( proc_list_snapshot, &processInfo );

	if ( !proc_name.compare( processInfo.szExeFile ) )
	{
		CloseHandle( proc_list_snapshot );
		return processInfo.th32ProcessID;
	}

	while ( Process32Next( proc_list_snapshot, &processInfo ) )
	{
		if ( !proc_name.compare( processInfo.szExeFile ) )
		{
			CloseHandle( proc_list_snapshot );
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle( proc_list_snapshot );
	return 0;
}

std::intptr_t ArmaTools::MemoryManager::GetModuleBaseAddress( int32_t proc_id, const std::wstring& module_name )
{
	std::intptr_t module_base_address;

	HANDLE proc_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id );
	if ( proc_snapshot != INVALID_HANDLE_VALUE )
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof( modEntry );
		if ( Module32First( proc_snapshot, &modEntry ) )
		{
			do
			{
				if ( !module_name.compare( modEntry.szModule ) )
				{
					module_base_address = ( std::intptr_t )modEntry.modBaseAddr;
					break;
				}
			} while ( Module32Next( proc_snapshot, &modEntry ) );
		}
	}
	CloseHandle( proc_snapshot );
	return module_base_address;
}

HANDLE ArmaTools::MemoryManager::GetProcHandle( int32_t proc_id )
{
	return OpenProcess( PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, proc_id );
}
