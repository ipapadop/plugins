#include "tlv_parser.hpp"

#include <stdexcept>

namespace tlv {
namespace {

void parse_into(const std::uint8_t* data, std::size_t size,
                const std::string& prefix, std::vector<Field>& out)
{
    std::size_t pos = 0;
    while (pos < size) {
        if (size - pos < 2) {
            throw std::runtime_error("truncated header");
        }
        const std::uint8_t tag = data[pos];
        const std::uint8_t len = data[pos + 1];
        pos += 2;
        if (size - pos < len) {
            throw std::runtime_error("truncated value");
        }
        const std::string path =
            prefix.empty() ? std::to_string(tag & 0x7F)
                           : prefix + "." + std::to_string(tag & 0x7F);
        if (tag & 0x80) {
            parse_into(data + pos, len, path, out);
        } else {
            out.push_back(Field{path, std::vector<std::uint8_t>(
                                          data + pos, data + pos + len)});
        }
        pos += len;
    }
}

}  // namespace

std::vector<Field> parse(const std::vector<std::uint8_t>& frame)
{
    std::vector<Field> out;
    parse_into(frame.data(), frame.size(), "", out);
    return out;
}

}  // namespace tlv
