#pragma once
#include <Windows.h>
#include <string>
#include <cstdint>
#include <tlhelp32.h>
#include <vector>

namespace ArmaTools
{
	class MemoryManager
	{
	public:
		MemoryManager( );

		std::intptr_t arma_img_base;
		
                /* Simple ReadProcessMemory WinApi Wrapper */
		template <class T>
		auto ReadVirtualMemory( std::intptr_t address ) -> T {
			T _buffer;
			ReadProcessMemory( this->arma_handle, reinterpret_cast< LPVOID >( address ), &_buffer, sizeof( T ), nullptr );
			return _buffer;
		}

		auto ReadVirtualMemory( std::intptr_t address, void* buffer, size_t size) {
			ReadProcessMemory( this->arma_handle, reinterpret_cast< LPVOID >( address ), &buffer, size, nullptr );
			return buffer;
		}

		template <class T>
		auto WriteVirtualMemory( T value, std::intptr_t address ) -> void {
			WriteProcessMemory( this->arma_handle, reinterpret_cast< LPVOID >( address ), &value, sizeof( T ), nullptr );
		}

		std::string ReadArmaString( std::intptr_t address ) {
			std::vector<char> buffer( size_t( ReadVirtualMemory<unsigned __int32>( address + 0x8 ) ), char{ 0 } );
			ReadVirtualMemory( address + 0x10, static_cast< void* >( &buffer [ 0 ] ), buffer.size( ) );
			return std::string( buffer.data( ) );
		}

	private:
		int32_t GetProcId( const std::wstring& proc_name );
		std::intptr_t GetModuleBaseAddress( int32_t proc_id, const std::wstring& module_name );
	    HANDLE GetProcHandle( const int32_t proc_id );

		int32_t arma_proc_id;
		HANDLE arma_handle;
	};
}
