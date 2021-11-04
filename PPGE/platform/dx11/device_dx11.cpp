#include "platform/dx11/device_dx11.h"

namespace PPGE
{
DeviceDX11::DeviceDX11()
{
    m_d3d11_device = NULL;
    m_d3d11_immediate_context = NULL;
    D3D_DRIVER_TYPE d3d_driver_type = D3D_DRIVER_TYPE_HARDWARE;

    UINT device_flags = 0;
#if defined(PPGE_PLATFORM_WIN)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_level;
    HRESULT hr =
        D3D11CreateDevice(0,               // chosen adapter. '0' means default adapter
                          d3d_driver_type, // driver type to be used
                          0,               // if driver type is software this is handle to software driver
                          device_flags,    // device flags to configure started device
                          0, 0, // feature level array and the size of it. '0' for both means default feature level
                          D3D11_SDK_VERSION, // available sdk version
                          &m_d3d11_device,   // [out] pointer to device that is created
                          &feature_level, // [out] if feature level array is provided, the first supported feature level
                          &m_d3d11_immediate_context // [out] poitner to immediate context created
        );
    PPGE_HR(hr);
    PPGE_ASSERT((feature_level != D3D_FEATURE_LEVEL_11_1),
                "PPGE requires DX feature level 11_1 to use DX11 as the rendering API.");
}
} // namespace PPGE