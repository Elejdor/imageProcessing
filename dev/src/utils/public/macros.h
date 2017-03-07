#pragma once

#define SC_BREAK() __debugbreak()
#define SC_INLINE inline
#define SC_FORCEINLINE __forceinline
#define SC_ASSERT( cond, format, ... ) if ( !( cond ) ) { __debugbreak(); } // TODO: MessageBox, only in debug
#define SC_VALIDATE( expr ) expr //TODO: only in debug