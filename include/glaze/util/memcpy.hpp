#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstring>

#include "util/inline.hpp"

namespace glz
{
   template <class T>
   concept unsigned_endian_sensetive = std::unsigned_integral<T> && sizeof(T) > 1;

   template <class T, class V = std::remove_cvref_t<T>>
   concept byte_sized = sizeof(T) == 1 && (std::same_as<V, char> || std::same_as<V, std::byte>);

   /// Wrapper for memcpy that ensures little-endian behavior when copying to/from buffers from/to unsigned integers
   template <typename Dst, typename Src>
   GLZ_ALWAYS_INLINE void* memcpy(Dst* dest, const Src* source, size_t n)
   {
      if constexpr (std::endian::native == std::endian::big && unsigned_endian_sensetive<Dst> && byte_sized<Src>) {
         std::memcpy(dest, source, n);
         *dest = std::byteswap(*dest);
         return dest;
      }
      else if constexpr (std::endian::native == std::endian::big && byte_sized<Dst> && unsigned_endian_sensetive<Src>) {
         Src source_byte_swapped = std::byteswap(*source);
         return std::memcpy(dest, &source_byte_swapped, n);
      }
      else {
         return std::memcpy(dest, source, n);
      }
   }
}
