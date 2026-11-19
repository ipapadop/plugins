// Parser for nested type-length-value (TLV) telemetry frames.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace tlv {

// One leaf field. `path` is the dot-separated list of tag numbers from the
// outermost container to the leaf, for example "3.1.7".
struct Field {
    std::string path;
    std::vector<std::uint8_t> value;
};

// Parses a frame. Each element is: 1-byte tag, 1-byte length, `length` bytes
// of value. Tags with bit 7 set are containers: their value is itself a
// sequence of elements. Throws std::runtime_error on a truncated element.
std::vector<Field> parse(const std::vector<std::uint8_t>& frame);

}  // namespace tlv
