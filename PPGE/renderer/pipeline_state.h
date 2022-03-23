#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object.h"
#include "renderer/renderer_constants.h"
#include "renderer/shader.h"
#include "renderer/shader_resource_binding.h"
#include "renderer/shader_resource_variable.h"
#include "system/logger_system.h"

namespace PPGE
{
struct PipelineStateDesc : public DeviceObjectDesc
{
    PipelineType type = PipelineType::PIPELINE_UNKNOWN;
};

struct RenderTargetBlendDesc
{
    // Enable/Disable blending.
    bool blend_enable = false;
    // This blend option specifies the operation to perform on the RGB value that
    // the pixel shader outputs. The blend_op member defines how to combine the
    // src_blend and dst_blend operations.
    BlendType src_blend = BlendType::BLEND_ONE;
    // This blend option specifies the operation to perform on the current RGB value
    // in the render target. The blend_op member defines how to combine the src_blend
    // and dst_blend operations.
    BlendType dst_blend = BlendType::BLEND_ZERO;
    // This blend operation defines how to combine the src_blend and dst_blend operations.
    BlendOpType blend_op = BlendOpType::BLEND_OP_ADD;
    // This blend option specifies the operation to perform on the alpha value that the
    // pixel shader outputs. Blend options that end in _COLOR are not allowed.
    // The blend_op_alpha member defines how to combine the src_blend_alpha
    // and dst_blend_alpha operations.
    BlendType src_blend_alpha = BlendType::BLEND_ONE;
    // This blend option specifies the operation to perform on the current alpha value
    // in the render target. Blend options that end in _COLOR are not allowed.
    // The blend_op_alpha member defines how to combine the src_blend_alpha
    // and dst_blend_alpha operations.
    BlendType dst_blend_alpha = BlendType::BLEND_ZERO;
    // This blend operation defines how to combine the src_blend_alpha and dst_blend_alpha operations.
    BlendOpType blend_op_alpha = BlendOpType::BLEND_OP_ADD;
    // Enable/Disable a logical operation.
    bool logic_op_enable = false;
    // A LogicOpType typed value that specifies the logical operation to configure for the render target.
    LogicOpType logic_op = LogicOpType::LOGIC_OP_NOOP;
    // A ColorWriteEnableFlags typed write mask.
    ColorWriteEnableFlags render_target_write_mask = ColorWriteEnableFlags::COLOR_WRITE_ENABLE_ALL;
};

struct BlendStateDesc
{
    // Specifies whether to use alpha-to-coverage as a multisampling technique
    // when setting a pixel to a render target
    bool alpha_to_coverage_enable = false;
    // Specifies whether to enable independent blending in simultaneous render
    // targets. Set to TRUE to enable independent blending. If set to FALSE,
    // only the RenderTarget[0] members are used; RenderTarget[1..7] are ignored
    bool independent_blend_enable = false;
    // An array of RenderTargetBlendDesc structures that describe the blend states
    // for render targets; these correspond to the eight render targets that can be
    // bound to the output-merger stage at one time
    RenderTargetBlendDesc render_target[Num_Of_Render_Targets];
};

struct RasterizerStateDesc
{
    // Determines the fill mode to use when rendering
    FillModeType fill_mode = FillModeType::FILL_MODE_SOLID;
    // Indicates triangles facing the specified direction are not drawn
    CullModeType cull_mode = CullModeType::CULL_MODE_BACK;
    // Determines if a triangle is front- or back-facing. If this parameter is 'true',
    // a triangle will be considered front-facing if its vertices are counter-clockwise
    // on the render target and considered back-facing if they are clockwise.
    // If this parameter is 'false', the opposite is true.
    bool front_counter_clockwise = false;
    // Depth value added to a given pixel. For info about depth bias, see
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    // for more details.
    int32_t depth_bias = 0;
    // Maximum depth bias of a pixel. For info about depth bias, see
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    // for more details.
    float depth_bias_clamp = 0.0f;
    // Scalar on a given pixel's slope. For info about depth bias, see
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    // for more details.
    float slope_scaled_depth_bias = 0.0f;
    // Enable clipping based on distance.
    bool depth_clip_enable = true;
    // Enable scissor-rectangle culling. All pixels outside an active scissor rectangle are culled.
    bool scissor_enable = false;
    // Specifies whether to use the quadrilateral or alpha line anti-aliasing algorithm on
    // multisample antialiasing (MSAA) render targets. Set to 'true' to use the quadrilateral
    // line anti-aliasing algorithm and to 'false' to use the alpha line anti-aliasing algorithm.
    bool antialiased_line_enable = false;
};

struct StencilOpDesc
{
    // The stencil operation to perform when stencil testing fails.
    StencilOpType stencil_fail_op = StencilOpType::STENCIL_OP_KEEP;
    // The stencil operation to perform when stencil testing passes and depth testing fails.
    StencilOpType stencil_depth_fail_op = StencilOpType::STENCIL_OP_KEEP;
    // The stencil operation to perform when stencil testing and depth testing both pass.
    StencilOpType stencil_pass_op = StencilOpType::STENCIL_OP_KEEP;
    // A function that compares stencil data against existing stencil data.
    ComperisonFuncType stencil_func = ComperisonFuncType::COMPARISON_FUNC_ALWAYS;
};

struct DepthStencilDesc
{
    // Enable/Disable depth testing
    bool depth_test_enable = true;
    // Enable/Disable depth write
    bool depth_write_enable = true;
    // A function that compares depth data against existing depth data
    ComperisonFuncType depth_func = ComperisonFuncType::COMPARISON_FUNC_LESS;
    // Enable/Disable stencil testing
    bool stencil_test_enable = false;
    // Identify a portion of the depth-stencil buffer for reading stencil data
    uint8_t stencil_read_mask = 0xFF;
    // Identify a portion of the depth-stencil buffer for writing stencil data
    uint8_t stencil_write_mask = 0xFF;
    // Identify how to use the results of the depth test and the stencil test for pixels
    // whose surface normal is facing towards the camera
    StencilOpDesc front_face;
    // Identify how to use the results of the depth test and the stencil test for pixels
    // whose surface normal is facing away from the camera
    StencilOpDesc back_face;
};

inline uint8_t GetElementValueSize(ElementValueType element_value)
{
    switch (element_value)
    {
    case PPGE::ElementValueType::ELEMENT_VALUE_INT8:
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT8:
        return 1;
    case PPGE::ElementValueType::ELEMENT_VALUE_INT16:
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT16:
    case PPGE::ElementValueType::ELEMENT_VALUE_FLOAT16:
        return 2;
    case PPGE::ElementValueType::ELEMENT_VALUE_INT32:
    case PPGE::ElementValueType::ELEMENT_VALUE_UINT32:
    case PPGE::ElementValueType::ELEMENT_VALUE_FLOAT32:
        return 4;
    case PPGE::ElementValueType::ELEMENT_VALUE_UNDEFINED:
        return 0;
    default:
        PPGE_ASSERT(false, "Unknown value type.");
        return 0;
    }
}

struct InputElementDesc
{
    // The HLSL semantic associated with this element in a shader input-signature
    // For VK and GL backend its fate is undecided
    const char *semantic_name = nullptr;
    // For GL and VK backends, index specified in the vertex shader.
    // For D3D11 and D3D12 backends, its the semantic index for the element.
    // A semantic index modifies a semantic, with an integer index number.
    // A semantic index is only needed in a case where there is more than
    // one element with the same semantic.
    uint32_t semantic_index = 0;
    // Element type. See ElementValueType for possible values
    ElementValueType element_value = ElementValueType::ELEMENT_VALUE_UNDEFINED;
    // Num of elements. Allowed values are 1, 2 or 4 to satisfy GPU allignments
    uint8_t element_count = 0;
    // Normalized signed intigers are in range [-1, 1] and normalized unsigned
    // values are in range [0, 1].
    bool is_normalized = false;
    // An integer value that identifies the input-assembler.
    // See the Num_Of_Buffer_Slots in renderer defines.
    uint32_t input_slot = 0;
    // Optional. Offset (in bytes) from the start of the vertex. Do not set it
    // to define the current element directly after the previous one, including
    // any packing if necessary.
    uint32_t aligned_byte_offset = 0xffffffff;
    // Identifies the input data class for a single input slot. See
    // ElementClassificationType for possible values.
    ElementClassificationType element_classification =
        ElementClassificationType::ELEMENT_CLASSIFICATION_PER_VERTEX_DATA;
    // The number of instances to draw using the same per-instance data before
    // advancing in the buffer by one element. This value must be 0 for an element
    // that contains per-vertex data.
    uint32_t instance_data_step_rate = 0;

    constexpr InputElementDesc(const char *_semantic_name, uint32_t _semantic_index, ElementValueType _element_value,
                               uint8_t _element_count, uint32_t _input_slot)
        : semantic_name{_semantic_name}, semantic_index{_semantic_index}, element_value{_element_value},
          element_count{_element_count}, input_slot{_input_slot}
    {
    }

    constexpr InputElementDesc(const char *_semantic_name, uint32_t _semantic_index, ElementValueType _element_value,
                               uint8_t _element_count, uint32_t _input_slot, uint32_t _instance_data_step_rate)
        : semantic_name{_semantic_name}, semantic_index{_semantic_index}, element_value{_element_value},
          element_count{_element_count}, input_slot{_input_slot}, instance_data_step_rate{_instance_data_step_rate},
          element_classification{ElementClassificationType::ELEMENT_CLASSIFICATION_PER_INSTANCE_DATA}
    {
    }

    constexpr InputElementDesc(const char *_semantic_name, uint32_t _semantic_index, ElementValueType _element_value,
                               uint8_t _element_count, bool _is_normalized, uint32_t _input_slot,
                               uint32_t _aligned_byte_offset, ElementClassificationType _element_classification,
                               uint32_t _instance_data_step_rate)
        : semantic_name{_semantic_name}, semantic_index{_semantic_index}, element_value{_element_value},
          element_count{_element_count}, is_normalized{_is_normalized}, input_slot{_input_slot},
          aligned_byte_offset{_aligned_byte_offset}, element_classification{_element_classification},
          instance_data_step_rate{_instance_data_step_rate}
    {
    }
};

struct InputLayoutDesc
{
    InputElementDesc *elements = nullptr;
    uint32_t elements_num = 0;
};

struct GfxPipelineStateDesc : public PipelineStateDesc
{
    BlendStateDesc blend_state_desc;

    RasterizerStateDesc rasterizer_state_desc;

    DepthStencilDesc depth_stencil_desc;

    InputLayoutDesc input_layout_desc;

    PrimitiveTopologyType primitive_topology = PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    GfxPipelineStateDesc()
    {
        type = PipelineType::PIPELINE_GRAPHICS;
    }
};

struct RtxPipelineStateDesc : public PipelineStateDesc
{
};

struct PipelineStateCreateDesc
{
    ShaderResourceCreateDesc *srv = nullptr;
    uint32_t srv_num = 0;
};

struct GfxPipelineStateCreateDesc : public PipelineStateCreateDesc
{
    GfxPipelineStateDesc desc;

    std::shared_ptr<PPGEShader> commited_vs;

    std::shared_ptr<PPGEShader> commited_ps;

    std::shared_ptr<PPGEShader> commited_gs;

    std::shared_ptr<PPGEShader> commited_hs;

    std::shared_ptr<PPGEShader> commited_ds;

    std::shared_ptr<PPGEShader> commited_as;

    std::shared_ptr<PPGEShader> commited_ms;
};

struct RtxPipelineStateCreateDesc : public PipelineStateCreateDesc
{
    RtxPipelineStateDesc desc;
};

class PPGE_API PPGEPipelineState : public PPGEDeviceObject
{
  public:
    virtual const PipelineStateDesc &GetDesc() const = 0;

    virtual const GfxPipelineStateDesc &GetGraphicsPipelineStateDesc() const = 0;

    virtual const RtxPipelineStateDesc &GetRayTracingPipelineStateDesc() const = 0;

    virtual PipelineType GetPipelineType() const = 0;

    virtual std::shared_ptr<PPGEShaderResourceBinding> CreateShaderResourceBinding() = 0;
};
} // namespace PPGE
