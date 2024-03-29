//-------------------------------------------------------------------------------------
// SimpleMath.cpp -- Simplified C++ Math wrapper for DirectXMath
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//-------------------------------------------------------------------------------------

#pragma once

// Off by default warnings
#pragma warning(                                                                                                       \
    disable : 4619 4616 4061 4265 4365 4571 4623 4625 4626 4628 4668 4710 4711 4746 4774 4820 4987 5026 5027 5031 5032 5039 5045 5219 26812)
// C4619/4616 #pragma warning warnings
// C4061 enumerator 'X' in switch of enum 'X' is not explicitly handled by a case label
// C4265 class has virtual functions, but destructor is not virtual
// C4365 signed/unsigned mismatch
// C4571 behavior change
// C4623 default constructor was implicitly defined as deleted
// C4625 copy constructor was implicitly defined as deleted
// C4626 assignment operator was implicitly defined as deleted
// C4628 digraphs not supported
// C4668 not defined as a preprocessor macro
// C4710 function not inlined
// C4711 selected for automatic inline expansion
// C4746 volatile access of '<expression>' is subject to /volatile:<iso|ms> setting
// C4774 format string expected in argument 3 is not a string literal
// C4820 padding added after data member
// C4987 nonstandard extension used
// C5026 move constructor was implicitly defined as deleted
// C5027 move assignment operator was implicitly defined as deleted
// C5031/5032 push/pop mismatches in windows headers
// C5039 pointer or reference to potentially throwing function passed to extern C function under - EHc
// C5045 Spectre mitigation warning
// C5219 implicit conversion from 'int' to 'float', possible loss of data
// 26812: The enum type 'x' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).

// Windows 8.1 SDK related Off by default warnings
#pragma warning(disable : 4471 4917 4986 5029)
// C4471 forward declaration of an unscoped enumeration must have an underlying type
// C4917 a GUID can only be associated with a class, interface or namespace
// C4986 exception specification does not match previous declaration
// C5029 nonstandard extension used

// Xbox One XDK related Off by default warnings
#pragma warning(disable : 4643 5043)
// C4643 Forward declaring in namespace std is not permitted by the C++ Standard
// C5043 exception specification does not match previous declaration

#ifdef __INTEL_COMPILER
#pragma warning(disable : 161 2960 3280)
// warning #161: unrecognized #pragma
// message #2960: allocation may not satisfy the type's alignment; consider using <aligned_new> header
// message #3280: declaration hides member
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wc++98-compat-local-type-template-args"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wmicrosoft-include"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Windows.h>

#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10 0x0A00
#endif

#ifndef WINAPI_FAMILY_GAMES
#define WINAPI_FAMILY_GAMES 6
#endif

#ifdef _GAMING_XBOX
#error This version of DirectX Tool Kit not supported for GDK
#elif defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#else
#include <d3d11_1.h>
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <exception>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <set>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4702)
#include <functional>
#pragma warning(pop)

#include <malloc.h>

#define _XM_NO_XMVECTOR_OVERLOADS_

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#if (DIRECTX_MATH_VERSION < 315)
#define XM_ALIGNED_STRUCT(x) __declspec(align(x)) struct
#endif

#pragma warning(push)
#pragma warning(disable : 4467 5038 5204 5220)
#include <wrl.h>
#pragma warning(pop)

#include <wincodec.h>

#if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)) || (defined(_XBOX_ONE) && defined(_TITLE))
#pragma warning(push)
#pragma warning(disable : 4471 5204)
#include <Windows.UI.Core.h>
#pragma warning(pop)
#endif

#include <mutex>

#include "simple_math.h"

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

namespace DirectX
{
namespace SimpleMath
{
const Vector2 Vector2::Zero = {0.f, 0.f};
const Vector2 Vector2::One = {1.f, 1.f};
const Vector2 Vector2::UnitX = {1.f, 0.f};
const Vector2 Vector2::UnitY = {0.f, 1.f};

const Vector3 Vector3::Zero = {0.f, 0.f, 0.f};
const Vector3 Vector3::One = {1.f, 1.f, 1.f};
const Vector3 Vector3::UnitX = {1.f, 0.f, 0.f};
const Vector3 Vector3::UnitY = {0.f, 1.f, 0.f};
const Vector3 Vector3::UnitZ = {0.f, 0.f, 1.f};
const Vector3 Vector3::Up = {0.f, 1.f, 0.f};
const Vector3 Vector3::Down = {0.f, -1.f, 0.f};
const Vector3 Vector3::Right = {1.f, 0.f, 0.f};
const Vector3 Vector3::Left = {-1.f, 0.f, 0.f};
const Vector3 Vector3::Forward = {0.f, 0.f, -1.f};
const Vector3 Vector3::Backward = {0.f, 0.f, 1.f};

const Vector4 Vector4::Zero = {0.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::One = {1.f, 1.f, 1.f, 1.f};
const Vector4 Vector4::UnitX = {1.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::UnitY = {0.f, 1.f, 0.f, 0.f};
const Vector4 Vector4::UnitZ = {0.f, 0.f, 1.f, 0.f};
const Vector4 Vector4::UnitW = {0.f, 0.f, 0.f, 1.f};

const Matrix Matrix::Identity = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

const Quaternion Quaternion::Identity = {0.f, 0.f, 0.f, 1.f};
} // namespace SimpleMath
} // namespace DirectX

/****************************************************************************
 *
 * Viewport
 *
 ****************************************************************************/

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
static_assert(sizeof(DirectX::SimpleMath::Viewport) == sizeof(D3D11_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, x) == offsetof(D3D11_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, y) == offsetof(D3D11_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, width) == offsetof(D3D11_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, height) == offsetof(D3D11_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, minDepth) == offsetof(D3D11_VIEWPORT, MinDepth),
              "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, maxDepth) == offsetof(D3D11_VIEWPORT, MaxDepth),
              "Layout mismatch");
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
static_assert(sizeof(DirectX::SimpleMath::Viewport) == sizeof(D3D12_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, x) == offsetof(D3D12_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, y) == offsetof(D3D12_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, width) == offsetof(D3D12_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, height) == offsetof(D3D12_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, minDepth) == offsetof(D3D12_VIEWPORT, MinDepth),
              "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::Viewport, maxDepth) == offsetof(D3D12_VIEWPORT, MaxDepth),
              "Layout mismatch");
#endif

#if defined(__dxgi1_2_h__) || defined(__d3d11_x_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
RECT DirectX::SimpleMath::Viewport::ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth,
                                                       UINT backBufferHeight, int outputWidth,
                                                       int outputHeight) noexcept
{
    RECT rct = {};

    switch (int(scaling))
    {
    case DXGI_SCALING_STRETCH:
        // Output fills the entire window area
        rct.top = 0;
        rct.left = 0;
        rct.right = outputWidth;
        rct.bottom = outputHeight;
        break;

    case 2 /*DXGI_SCALING_ASPECT_RATIO_STRETCH*/:
        // Output fills the window area but respects the original aspect ratio, using pillar boxing or letter boxing as
        // required Note: This scaling option is not supported for legacy Win32 windows swap chains
        {
            assert(backBufferHeight > 0);
            float aspectRatio = float(backBufferWidth) / float(backBufferHeight);

            // Horizontal fill
            float scaledWidth = float(outputWidth);
            float scaledHeight = float(outputWidth) / aspectRatio;
            if (scaledHeight >= float(outputHeight))
            {
                // Do vertical fill
                scaledWidth = float(outputHeight) * aspectRatio;
                scaledHeight = float(outputHeight);
            }

            float offsetX = (float(outputWidth) - scaledWidth) * 0.5f;
            float offsetY = (float(outputHeight) - scaledHeight) * 0.5f;

            rct.left = static_cast<LONG>(offsetX);
            rct.top = static_cast<LONG>(offsetY);
            rct.right = static_cast<LONG>(offsetX + scaledWidth);
            rct.bottom = static_cast<LONG>(offsetY + scaledHeight);

            // Clip to display window
            rct.left = std::max<LONG>(0, rct.left);
            rct.top = std::max<LONG>(0, rct.top);
            rct.right = std::min<LONG>(outputWidth, rct.right);
            rct.bottom = std::min<LONG>(outputHeight, rct.bottom);
        }
        break;

    case DXGI_SCALING_NONE:
    default:
        // Output is displayed in the upper left corner of the window area
        rct.top = 0;
        rct.left = 0;
        rct.right = std::min<LONG>(static_cast<LONG>(backBufferWidth), outputWidth);
        rct.bottom = std::min<LONG>(static_cast<LONG>(backBufferHeight), outputHeight);
        break;
    }

    return rct;
}
#endif

RECT DirectX::SimpleMath::Viewport::ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight) noexcept
{
    float safew = (float(backBufferWidth) + 19.f) / 20.f;
    float safeh = (float(backBufferHeight) + 19.f) / 20.f;

    RECT rct;
    rct.left = static_cast<LONG>(safew);
    rct.top = static_cast<LONG>(safeh);
    rct.right = static_cast<LONG>(float(backBufferWidth) - safew + 0.5f);
    rct.bottom = static_cast<LONG>(float(backBufferHeight) - safeh + 0.5f);

    return rct;
}
