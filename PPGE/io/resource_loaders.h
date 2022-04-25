#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
struct LazyResource;
struct ByteResource;
struct TextResource;
struct ImageResource;

class ResourceLoader
{
  public:
    virtual void LoadLazyResource(LazyResource &, const std::filesystem::path &)
    {
        PPGE_ASSERT(false, "Loader not implemented.");
    }

    virtual void LoadByteResource(ByteResource &, const std::filesystem::path &)
    {
        PPGE_ASSERT(false, "Loader not implemented.");
    }

    virtual void LoadTextResource(TextResource &, const std::filesystem::path &)
    {
        PPGE_ASSERT(false, "Loader not implemented.");
    }

    virtual void LoadImageResource(ImageResource &, const std::filesystem::path &)
    {
        PPGE_ASSERT(false, "Loader not implemented.");
    }
};

class ByteLoader : public ResourceLoader
{
  public:
    void LoadByteResource(ByteResource &resource, const std::filesystem::path &path_to_resource) override final;
};

class TextLoader : public ResourceLoader
{
  public:
    void LoadTextResource(TextResource &resource, const std::filesystem::path &path_to_resource) override final;
};

class LazyLoader : public ResourceLoader
{
  public:
    void LoadLazyResource(LazyResource &resource, const std::filesystem::path &path_to_resource) override final;
};
} // namespace PPGE
