#include "pipeline_state_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/shader_d3d11_impl.h"
#include "platform/d3d11/shader_resource_binding_d3d11_impl.h"

namespace PPGE
{
static DXGI_FORMAT GetD3D11ElementFormat(ElementValueType element_value, uint8_t element_count, bool is_normalized)
{
    DXGI_FORMAT element_format = DXGI_FORMAT_UNKNOWN;
    switch (element_value)
    {
    case PPGE::ElementValueType::ELEMENT_VALUE_INT8:
        if (is_normalized)
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R8_SNORM;
                break;
            case 2:
                element_format = DXGI_FORMAT_R8G8_SNORM;
                break;
            case 4:
                element_format = DXGI_FORMAT_R8G8B8A8_SNORM;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for int8 can only be 1, 2 or 4");
                break;
            }
        }
        else
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R8_SINT;
                break;
            case 2:
                element_format = DXGI_FORMAT_R8G8_SINT;
                break;
            case 4:
                element_format = DXGI_FORMAT_R8G8B8A8_SINT;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for int8 can only be 1, 2 or 4");
                break;
            }
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_INT16:
        if (is_normalized)
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R16_SNORM;
                break;
            case 2:
                element_format = DXGI_FORMAT_R16G16_SNORM;
                break;
            case 4:
                element_format = DXGI_FORMAT_R16G16B16A16_SNORM;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for int16 can only be 1, 2 or 4");
                break;
            }
        }
        else
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R16_SINT;
                break;
            case 2:
                element_format = DXGI_FORMAT_R16G16_SINT;
                break;
            case 4:
                element_format = DXGI_FORMAT_R16G16B16A16_SINT;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for int16 can only be 1, 2 or 4");
                break;
            }
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_INT32:
        PPGE_ASSERT(!is_normalized, "Normalized int32 formats are not supported.")
        switch (element_count)
        {
        case 1:
            element_format = DXGI_FORMAT_R32_SINT;
            break;
        case 2:
            element_format = DXGI_FORMAT_R32G32_SINT;
            break;
        case 3:
            element_format = DXGI_FORMAT_R32G32B32_SINT;
            break;
        case 4:
            element_format = DXGI_FORMAT_R32G32B32A32_SINT;
            break;
        default:
            PPGE_ASSERT(false, "Number of components for int32 can only be 1, 2, 3 or 4");
            break;
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT8:
        if (is_normalized)
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R8_UNORM;
                break;
            case 2:
                element_format = DXGI_FORMAT_R8G8_UNORM;
                break;
            case 4:
                element_format = DXGI_FORMAT_R8G8B8A8_UNORM;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for unsigned int8 can only be 1, 2 or 4");
                break;
            }
        }
        else
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R8_UINT;
                break;
            case 2:
                element_format = DXGI_FORMAT_R8G8_UINT;
                break;
            case 4:
                element_format = DXGI_FORMAT_R8G8B8A8_UINT;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for unsigned int8 can only be 1, 2 or 4");
                break;
            }
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT16:
        if (is_normalized)
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R16_UNORM;
                break;
            case 2:
                element_format = DXGI_FORMAT_R16G16_UNORM;
                break;
            case 4:
                element_format = DXGI_FORMAT_R16G16B16A16_UNORM;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for unsigned int16 can only be 1, 2 or 4");
                break;
            }
        }
        else
        {
            switch (element_count)
            {
            case 1:
                element_format = DXGI_FORMAT_R16_UINT;
                break;
            case 2:
                element_format = DXGI_FORMAT_R16G16_UINT;
                break;
            case 4:
                element_format = DXGI_FORMAT_R16G16B16A16_UINT;
                break;
            default:
                PPGE_ASSERT(false, "Number of components for unsigned int16 can only be 1, 2 or 4");
                break;
            }
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT32:
        PPGE_ASSERT(!is_normalized, "Normalized unsigned int32 formats are not supported.")
        switch (element_count)
        {
        case 1:
            element_format = DXGI_FORMAT_R32_UINT;
            break;
        case 2:
            element_format = DXGI_FORMAT_R32G32_UINT;
            break;
        case 3:
            element_format = DXGI_FORMAT_R32G32B32_UINT;
            break;
        case 4:
            element_format = DXGI_FORMAT_R32G32B32A32_UINT;
            break;
        default:
            PPGE_ASSERT(false, "Number of components for unsigned int32 can only be 1, 2, 3 or 4");
            break;
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_FLOAT16:
        PPGE_ASSERT(!is_normalized, "Normalized float16 formats are not supported.")
        switch (element_count)
        {
        case 1:
            element_format = DXGI_FORMAT_R16_FLOAT;
            break;
        case 2:
            element_format = DXGI_FORMAT_R16G16_FLOAT;
            break;
        case 4:
            element_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
            break;
        default:
            PPGE_ASSERT(false, "Number of components for float16 can only be 1, 2 or 4");
            break;
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_FLOAT32:
        PPGE_ASSERT(!is_normalized, "Normalized float32 formats are not supported.")
        switch (element_count)
        {
        case 1:
            element_format = DXGI_FORMAT_R32_FLOAT;
            break;
        case 2:
            element_format = DXGI_FORMAT_R32G32_FLOAT;
            break;
        case 3:
            element_format = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case 4:
            element_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        default:
            PPGE_ASSERT(false, "Number of components for float32 can only be 1, 2, 3 or 4");
            break;
        }
        break;
    case PPGE::ElementValueType::ELEMENT_VALUE_UNDEFINED:
        PPGE_ASSERT(false, "Element value is undefined for the input layout element.");
        break;
    default:
        PPGE_ASSERT(false, "Element value is unknown for the input layout element.");
        break;
    }
    return element_format;
}

PipelineStateD3D11Impl::PipelineStateD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp,
                                               const GfxPipelineStateCreateDesc &desc)
    : PipelineStateBaseType(std::move(device_sp), desc)
{
    for (size_t i = 0; i < desc.sr_create_desc.range_num; i++)
    {
        ShaderResourceRangeCreateDesc range_cd = desc.sr_create_desc.range[i];
        for (size_t j = 0; j < range_cd.sr_name_num; j++)
        {
            m_bindable_resource_descs.push_back(std::make_tuple(std::string(range_cd.sr_name[j]), range_cd.sr_desc));
        }
    }

#define PUSH_SHADER_BACK(ShaderType, Idx)                                                                              \
    if (desc.commited_##ShaderType)                                                                                    \
    {                                                                                                                  \
        m_active_shader_indicies[Idx] = m_shaders.size();                                                              \
        m_shaders.push_back(std::move(std::static_pointer_cast<PPGEShaderD3D11>(desc.commited_##ShaderType)));         \
    }

    PUSH_SHADER_BACK(vs, 0);
    PUSH_SHADER_BACK(ps, 1);
    PUSH_SHADER_BACK(gs, 2);
    PUSH_SHADER_BACK(hs, 3);
    PUSH_SHADER_BACK(ds, 4);

    PPGE_ASSERT(m_active_shader_indicies[0] != -1 && m_active_shader_indicies[0] < m_shaders.size(),
                "D3D11 graphics pipeline cannot be created without commited vertex shader.");
    auto vs_byte_code = m_shaders[m_active_shader_indicies[0]]->GetByteCode();

    auto d3d11_device = m_device_sp->GetD3D11Device();

    D3D11_BLEND_DESC d3d11_blend_desc = {};
    {
        const auto &blend_desc = m_gfx_ps_desc.blend_state_desc;
        d3d11_blend_desc.AlphaToCoverageEnable = blend_desc.alpha_to_coverage_enable ? TRUE : FALSE;
        d3d11_blend_desc.IndependentBlendEnable = blend_desc.independent_blend_enable ? TRUE : FALSE;
        for (int i = 0; i < 8; i++)
        {
            const auto &rt_desc = blend_desc.render_target[i];
            PPGE_ASSERT(!rt_desc.logic_op_enable, "Logic op on RT is not supported for D3D11.");
            auto &d3d11_rt_desc = d3d11_blend_desc.RenderTarget[i];

            d3d11_rt_desc.BlendEnable = rt_desc.blend_enable ? TRUE : FALSE;
            d3d11_rt_desc.SrcBlend = PPGEBlendTypeToD3D11Blend(rt_desc.src_blend);
            d3d11_rt_desc.DestBlend = PPGEBlendTypeToD3D11Blend(rt_desc.dst_blend);
            d3d11_rt_desc.BlendOp = PPGEBlendOpTypeToD3D11BlendOp(rt_desc.blend_op);

            d3d11_rt_desc.SrcBlendAlpha = PPGEBlendTypeToD3D11Blend(rt_desc.src_blend_alpha);
            d3d11_rt_desc.DestBlendAlpha = PPGEBlendTypeToD3D11Blend(rt_desc.dst_blend_alpha);
            d3d11_rt_desc.BlendOpAlpha = PPGEBlendOpTypeToD3D11BlendOp(rt_desc.blend_op_alpha);

            d3d11_rt_desc.RenderTargetWriteMask =
                PPGEColorWriteEnableFlagsToD3D11ColorWriteEnableFlags(rt_desc.render_target_write_mask);
        }
    }
    PPGE_HR(d3d11_device->CreateBlendState(&d3d11_blend_desc, &m_blend_state_ptr));

    D3D11_RASTERIZER_DESC d3d11_rasterizer_desc = {};
    {
        const auto &rasterizer_state_desc = m_gfx_ps_desc.rasterizer_state_desc;
        d3d11_rasterizer_desc.FillMode = PPGEFillModeTypeToD3D11FillMode(rasterizer_state_desc.fill_mode);
        d3d11_rasterizer_desc.CullMode = PPGECullModeTypeToD3D11CullMode(rasterizer_state_desc.cull_mode);
        d3d11_rasterizer_desc.FrontCounterClockwise = rasterizer_state_desc.front_counter_clockwise ? TRUE : FALSE;
        d3d11_rasterizer_desc.DepthBias = static_cast<INT>(rasterizer_state_desc.depth_bias);
        d3d11_rasterizer_desc.DepthBiasClamp = static_cast<FLOAT>(rasterizer_state_desc.depth_bias_clamp);
        d3d11_rasterizer_desc.SlopeScaledDepthBias = static_cast<FLOAT>(rasterizer_state_desc.slope_scaled_depth_bias);
        d3d11_rasterizer_desc.DepthClipEnable = rasterizer_state_desc.depth_clip_enable ? TRUE : FALSE;
        d3d11_rasterizer_desc.ScissorEnable = rasterizer_state_desc.scissor_enable ? TRUE : FALSE;
        d3d11_rasterizer_desc.AntialiasedLineEnable = rasterizer_state_desc.antialiased_line_enable ? TRUE : FALSE;
        d3d11_rasterizer_desc.MultisampleEnable = TRUE;
    }
    PPGE_HR(d3d11_device->CreateRasterizerState(&d3d11_rasterizer_desc, &m_rasterizer_state_ptr));

    D3D11_DEPTH_STENCIL_DESC d3d11_depth_stencil_desc = {};
    {
        const auto &depth_stencil_state_desc = m_gfx_ps_desc.depth_stencil_desc;
        d3d11_depth_stencil_desc.DepthEnable = depth_stencil_state_desc.depth_test_enable ? TRUE : FALSE;
        d3d11_depth_stencil_desc.DepthWriteMask =
            depth_stencil_state_desc.depth_write_enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        d3d11_depth_stencil_desc.DepthFunc =
            PPGEComperisonFuncTypeToD3D11ComparisonFunc(depth_stencil_state_desc.depth_func);
        d3d11_depth_stencil_desc.StencilEnable = depth_stencil_state_desc.stencil_test_enable ? TRUE : FALSE;
        d3d11_depth_stencil_desc.StencilReadMask = static_cast<UINT8>(depth_stencil_state_desc.stencil_read_mask);
        d3d11_depth_stencil_desc.StencilWriteMask = static_cast<UINT8>(depth_stencil_state_desc.stencil_write_mask);
        d3d11_depth_stencil_desc.FrontFace.StencilFailOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.front_face.stencil_fail_op);
        d3d11_depth_stencil_desc.FrontFace.StencilDepthFailOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.front_face.stencil_depth_fail_op);
        d3d11_depth_stencil_desc.FrontFace.StencilPassOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.front_face.stencil_pass_op);
        d3d11_depth_stencil_desc.FrontFace.StencilFunc =
            PPGEComperisonFuncTypeToD3D11ComparisonFunc(depth_stencil_state_desc.front_face.stencil_func);
        d3d11_depth_stencil_desc.BackFace.StencilFailOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.back_face.stencil_fail_op);
        d3d11_depth_stencil_desc.BackFace.StencilDepthFailOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.back_face.stencil_depth_fail_op);
        d3d11_depth_stencil_desc.BackFace.StencilPassOp =
            PPGEStencilOpTypeToD3D11StencilOp(depth_stencil_state_desc.back_face.stencil_pass_op);
        d3d11_depth_stencil_desc.BackFace.StencilFunc =
            PPGEComperisonFuncTypeToD3D11ComparisonFunc(depth_stencil_state_desc.back_face.stencil_func);
    }
    PPGE_HR(d3d11_device->CreateDepthStencilState(&d3d11_depth_stencil_desc, &m_depth_stencil_state_ptr));

    PPGE_ASSERT(!(m_gfx_ps_desc.input_layout_desc.elements_num > Num_Of_Layout_Elements),
                "Input layout element count exceeds the limit.");
    if (m_gfx_ps_desc.input_layout_desc.elements_num > 0)
    {
        m_layout_strides.fill(0);
        std::array<D3D11_INPUT_ELEMENT_DESC, Num_Of_Layout_Elements> d3d11_elements{};
        for (uint32_t i = 0; i < m_gfx_ps_desc.input_layout_desc.elements_num; i++)
        {
            const auto &element = m_gfx_ps_desc.input_layout_desc.elements[i];
            auto &d3d11_element = d3d11_elements[i];
            d3d11_element.SemanticName = element.semantic_name;
            d3d11_element.SemanticIndex = static_cast<UINT>(element.semantic_index);
            d3d11_element.Format =
                GetD3D11ElementFormat(element.element_value, element.element_count, element.is_normalized);
            d3d11_element.InputSlot = static_cast<UINT>(element.input_slot);
            d3d11_element.AlignedByteOffset = static_cast<UINT>(element.aligned_byte_offset);
            d3d11_element.InputSlotClass =
                PPGEElementClassificationTypeToD3D11InputClassification(element.element_classification);
            d3d11_element.InstanceDataStepRate =
                element.element_classification == ElementClassificationType::ELEMENT_CLASSIFICATION_PER_VERTEX_DATA
                    ? 0
                    : static_cast<UINT>(element.instance_data_step_rate);
            m_layout_strides[element.input_slot] += GetElementValueSize(element.element_value) * element.element_count;
        }
        PPGE_HR(d3d11_device->CreateInputLayout(
            d3d11_elements.data(), static_cast<UINT>(m_gfx_ps_desc.input_layout_desc.elements_num),
            vs_byte_code->GetBufferPointer(), vs_byte_code->GetBufferSize(), &m_input_layout_ptr));
    }
}

PipelineStateD3D11Impl::~PipelineStateD3D11Impl()
{
}

ID3D11BlendState *PipelineStateD3D11Impl::GetD3D11BlendStateRaw() const
{
    return m_blend_state_ptr;
}

ID3D11RasterizerState *PipelineStateD3D11Impl::GetD3D11RasterizerStateRaw() const
{
    return m_rasterizer_state_ptr;
}

ID3D11DepthStencilState *PipelineStateD3D11Impl::GetD3D11DepthStencilStateRaw() const
{
    return m_depth_stencil_state_ptr;
}

ID3D11InputLayout *PipelineStateD3D11Impl::GetD3D11InputLayoutRaw() const
{
    return m_input_layout_ptr;
}

ID3D11VertexShader *PipelineStateD3D11Impl::GetD3D11VertexShaderRaw() const
{
    ID3D11VertexShader *d3d11_vs_shader = nullptr;
    auto vs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_VERTEX);
    if (vs_index > -1 && vs_index < m_shaders.size())
    {
        if (auto vs_sp = m_shaders[vs_index])
        {
            auto d3d11_shader_ptr = vs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11VertexShader),
                                                     reinterpret_cast<void **>(&d3d11_vs_shader)));
        }
    }
    return d3d11_vs_shader;
}

ID3D11PixelShader *PipelineStateD3D11Impl::GetD3D11PixelShaderRaw() const
{
    ID3D11PixelShader *d3d11_ps_shader = nullptr;
    auto ps_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_PIXEL);
    if (ps_index > -1 && ps_index < m_shaders.size())
    {
        if (auto ps_sp = m_shaders[ps_index])
        {
            auto d3d11_shader_ptr = ps_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11PixelShader),
                                                     reinterpret_cast<void **>(&d3d11_ps_shader)));
        }
    }
    return d3d11_ps_shader;
}

ID3D11GeometryShader *PipelineStateD3D11Impl::GetD3D11GeometryShaderRaw() const
{
    ID3D11GeometryShader *d3d11_gs_shader = nullptr;
    auto gs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_GEOMETRY);
    if (gs_index > -1 && gs_index < m_shaders.size())
    {
        if (auto gs_sp = m_shaders[gs_index])
        {
            auto d3d11_shader_ptr = gs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11GeometryShader),
                                                     reinterpret_cast<void **>(&d3d11_gs_shader)));
        }
    }
    return d3d11_gs_shader;
}

ID3D11DomainShader *PipelineStateD3D11Impl::GetD3D11DomainShaderRaw() const
{
    ID3D11DomainShader *d3d11_ds_shader = nullptr;
    auto ds_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_DOMAIN);
    if (ds_index > -1 && ds_index < m_shaders.size())
    {
        if (auto ds_sp = m_shaders[ds_index])
        {
            auto d3d11_shader_ptr = ds_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11DomainShader),
                                                     reinterpret_cast<void **>(&d3d11_ds_shader)));
        }
    }
    return d3d11_ds_shader;
}

ID3D11HullShader *PipelineStateD3D11Impl::GetD3D11HullShaderRaw() const
{
    ID3D11HullShader *d3d11_hs_shader = nullptr;
    auto hs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_HULL);
    if (hs_index > -1 && hs_index < m_shaders.size())
    {
        if (auto hs_sp = m_shaders[hs_index])
        {
            auto d3d11_shader_ptr = hs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11HullShader),
                                                     reinterpret_cast<void **>(&d3d11_hs_shader)));
        }
    }
    return d3d11_hs_shader;
}

ID3D11ComputeShader *PipelineStateD3D11Impl::GetD3D11ComputeShaderRaw() const
{
    ID3D11ComputeShader *d3d11_cs_shader = nullptr;
    auto cs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_COMPUTE);
    if (cs_index > -1 && cs_index < m_shaders.size())
    {
        if (auto cs_sp = m_shaders[cs_index])
        {
            auto d3d11_shader_ptr = cs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11ComputeShader),
                                                     reinterpret_cast<void **>(&d3d11_cs_shader)));
        }
    }
    return d3d11_cs_shader;
}

std::shared_ptr<PPGEShaderResourceBinding> PipelineStateD3D11Impl::CreateShaderResourceBinding()
{
    return std::make_shared<ShaderResourceBindingD3D11Impl>(shared_from_this(), m_bindable_resource_descs);
}
} // namespace PPGE