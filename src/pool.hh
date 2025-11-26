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
#include <future>

#include <QDebug>
#include <QThreadPool>
#include <QtLogging>

#include "scraper.hh"

namespace cheri {

/**
 * Thread pool interface.
 *
 * An simple thread pool that supports graceful shutdown.
 */
class ThreadPool {
public:
  explicit ThreadPool(unsigned long workers) {
    pool_.setMaxThreadCount(workers);
  }

  std::future<ScraperResult> schedule(std::unique_ptr<DwarfScraper> scraper) {
    std::promise<ScraperResult> promise;
    auto result = promise.get_future();
    auto token = stop_state_.get_token();

    pool_.start([s = std::move(scraper), p = std::move(promise),
                 token]() mutable {
      try {
        s->initSchema();
        s->run(token);
        qInfo() << "Scraper" << s->name() << "completed job for"
                << s->source().getPath().string();
        p.set_value(s->result());
      } catch (std::exception &ex) {
        qCritical() << "DWARF scraper failed for"
                    << s->source().getPath().string() << "reason " << ex.what();
        p.set_exception(std::current_exception());
      }
    });
    return result;
  }

  void wait() { pool_.waitForDone(); }

  void cancel() {
    pool_.clear();
    stop_state_.request_stop();
  }

private:
  std::stop_source stop_state_;
  QThreadPool pool_;
};

} /* namespace cheri */
