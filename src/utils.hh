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
#include <filesystem>
#include <numeric>
#include <string>

namespace cheri {

QDebug operator<<(QDebug debug, const std::filesystem::path &p);

template <typename T, char S = ','>
std::string Join(const std::vector<T> &&vec) {
  std::string acc;
  if (vec.size() == 0)
    return acc;
  acc = vec[0];

  auto JoinFn = [](std::string left, const T &right) {
    if constexpr (std::constructible_from<std::string, decltype(right)>) {
      return std::move(left) + S + right;
    } else {
      return std::move(left) + S + std::to_string(right);
    }
  };
  return std::accumulate(vec.begin() + 1, vec.end(), acc, JoinFn);
}

} /* namespace cheri */
