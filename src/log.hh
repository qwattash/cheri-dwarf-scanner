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

#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <vector>

namespace cheri {

enum LogLevel {
  kError,
  kWarn,
  kInfo,
  kDebug,
  kTrace,
};

std::ostream &operator<<(std::ostream &os, LogLevel ll);

struct LogMessage {
  LogLevel level;
  std::string message;
};

class Logger;

/*
 * Abstract logging sink.
 */
class Sink {
public:
  virtual void Emit(LogMessage &msg) = 0;
};

/*
 * Console logger sink
 */
class ConsoleLogSink : public Sink {
public:
  void Emit(LogMessage &msg) override;
};

/*
 * Lightweight handle that builds a log message and dispatches
 * it when out of scope.
 */
class LogAs {
  friend Logger;

public:
  ~LogAs();
  std::ostream &Stream();
  operator bool();

private:
  LogAs(Logger &logger, LogLevel level);

  Logger &logger_;
  LogLevel level_;
  std::ostringstream stream_;
  bool consumed_flag_;
};

/*
 * Minimal logging interface.
 * Can't use Boost because it requires RTTI.
 */
class Logger {
  friend LogAs;

public:
  /**
   * Get the global logger instance.
   * If this was not initialized, create a blank logger.
   */
  static Logger &Get();

  /**
   * Create a default logger and return a reference to it.
   * The logger is then used as the global logger.
   */
  static Logger &Default();

  /**
   * Attach a sink to the logger.
   */
  void AddSink(std::unique_ptr<Sink> sink);

  /**
   * Change the log level filter.
   */
  void SetLevel(LogLevel level);

  /**
   * Create a new RAII log stream
   */
  LogAs NewStream(LogLevel level);

private:
  explicit Logger(LogLevel level);

  void Emit(LogMessage msg);

  std::mutex mutex_;
  LogLevel level_;
  std::vector<std::unique_ptr<Sink>> sinks_;
};

/**
 * The log handle stream supports the usual stream << operations with ostream.
 * Note that the for loop creates a scoped block that will release the handle
 * after the stream is done.
 *
 * This should be used as
 * LOG(kDebug) << "My message";
 */
#define LOG(level)                                                             \
  for (auto log_handle = ::cheri::Logger::Get().NewStream(level);              \
       !log_handle;)                                                           \
  log_handle.Stream()

} /* namespace cheri */
