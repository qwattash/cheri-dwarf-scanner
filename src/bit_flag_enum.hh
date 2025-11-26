/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2025 Alfredo Mazzinghi
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <concepts>

namespace cheri {

template <typename T> struct EnumTraits {
  static constexpr bool is_bitflag = false;
};

template <typename T>
concept BitFlagEnum = std::is_enum_v<T> && EnumTraits<T>::is_bitflag;

template <BitFlagEnum T> constexpr T operator|(T l, T r) {
  return static_cast<T>(static_cast<int>(l) | static_cast<int>(r));
}

template <BitFlagEnum T> constexpr T operator&(T l, T r) {
  return static_cast<T>(static_cast<int>(l) & static_cast<int>(r));
}

template <BitFlagEnum T> constexpr T &operator|=(T &l, T r) {
  l = l | r;
  return l;
}

template <BitFlagEnum T> constexpr bool operator!(T v) {
  return v == static_cast<T>(0);
}

template <BitFlagEnum T> std::ostream &operator<<(std::ostream &os, T v) {
  os << static_cast<int>(v);
  return os;
}

} /* namespace cheri */
