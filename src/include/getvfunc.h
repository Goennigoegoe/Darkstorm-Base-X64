#pragma once

//Credits: Casual_Hacker

inline void**& getvtable( void* inst, size_t offset = 0 )
{
	return *reinterpret_cast<void***>( (size_t)inst + offset );
}
inline const void** getvtable( const void* inst, size_t offset = 0 )
{
	return *reinterpret_cast<const void***>( (size_t)inst + offset );
}
template< typename Fn >
inline Fn getvfunc( const void* inst, size_t index, size_t offset = 0 )
{
	return reinterpret_cast<Fn>( getvtable( inst, offset )[ index ] );
}
template< typename Fn >
inline Fn getvfunc( void* inst, const unsigned short index )
{
	return reinterpret_cast<Fn>( getvtable( inst, 0 )[ index ] );
}

template <typename T> inline T find_vfunc(void* vmt, unsigned int index) {
    return (*static_cast<T**>(vmt))[index]; // create an array of pointers
}
//===================================================================================
