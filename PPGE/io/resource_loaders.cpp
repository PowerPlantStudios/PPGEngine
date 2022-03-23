#include "resource_loaders.h"

#include "io/resources.h"

namespace PPGE
{
void ByteLoader::LoadByteResource(ByteResource &resource, const std::filesystem::path &path_to_resource)
{
    std::basic_fstream<std::byte> in(path_to_resource);
    std::vector<std::byte> buffer{std::istreambuf_iterator<std::byte>(in), {}};
    resource.data.swap(buffer);

    std::hash<std::string_view> hasher;
    std::string_view code(reinterpret_cast<char *>(resource.data.data()), resource.data.size());
    resource.hash_id = hasher(code);
}

void TextLoader::LoadTextResource(TextResource &resource, const std::filesystem::path &path_to_resource)
{
    std::basic_fstream<char> in(path_to_resource);
    std::string buffer{std::istreambuf_iterator<char>(in), {}};
    resource.data.swap(buffer);

    std::hash<std::string> hasher;
    resource.hash_id = hasher(resource.data);
}

void LazyLoader::LoadLazyResource(LazyResource &resource, const std::filesystem::path &path_to_resource)
{
    resource.data = path_to_resource;

    std::hash<std::string> hasher;
    resource.hash_id = hasher(resource.data.string());
}
} // namespace PPGE