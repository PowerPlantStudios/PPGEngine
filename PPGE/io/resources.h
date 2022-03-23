#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/resource_loaders.h"
#include "system/logger_system.h"

namespace PPGE
{
struct Resource
{
    size_t hash_id = 0;

    virtual void visit(ResourceLoader &, const std::filesystem::path &) = 0;

    inline bool operator==(const Resource &other) const
    {
        return this->hash_id == other.hash_id;
    }

    inline bool operator!=(const Resource &other) const
    {
        return !operator==(other);
    }
};

struct LazyResource : public Resource
{
    std::filesystem::path data;

    void visit(ResourceLoader &loader, const std::filesystem::path &path_to_resource) override final
    {
        loader.LoadLazyResource(*this, path_to_resource);
    }
};

struct ByteResource : public Resource
{
    std::vector<std::byte> data;

    void visit(ResourceLoader &loader, const std::filesystem::path &path_to_resource) override final
    {
        loader.LoadByteResource(*this, path_to_resource);
    }
};

struct TextResource : public Resource
{
    std::string data;

    void visit(ResourceLoader &loader, const std::filesystem::path &path_to_resource) override final
    {
        loader.LoadTextResource(*this, path_to_resource);
    }
};

struct ImageResource : public Resource
{
    size_t width;

    size_t height;

    size_t channel;

    std::vector<uint8_t> data;

    void visit(ResourceLoader &loader, const std::filesystem::path &path_to_resource) override final
    {
        loader.LoadImageResource(*this, path_to_resource);
    }
};
} // namespace PPGE
