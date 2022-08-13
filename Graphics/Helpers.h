#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // For HRESULT
#include <exception>
#include <memory>
#include <random>

// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples

//#define CHWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
//#define CHWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
//#define CHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}