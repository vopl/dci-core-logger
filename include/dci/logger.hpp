/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "logger/stream.hpp"
#include "logger/timeProvider.hpp"

#if !defined(dciLoggerIdentity)
#   if defined(dciModuleName)
#       define dciLoggerIdentity dciModuleName
#   elif defined(dciUnitName)
#       define dciLoggerIdentity dciUnitName
#   else
#       define dciLoggerIdentity ""
#   endif
#endif

#   define LOGF(...) dci::logger::Stream{std::cout, "FTL", dciLoggerIdentity} << __VA_ARGS__
#   define LOGE(...) dci::logger::Stream{std::cout, "ERR", dciLoggerIdentity} << __VA_ARGS__
#   define LOGW(...) dci::logger::Stream{std::cout, "WRN", dciLoggerIdentity} << __VA_ARGS__
#   define LOGI(...) dci::logger::Stream{std::cout, "INF", dciLoggerIdentity} << __VA_ARGS__
#   define LOGD(...) dci::logger::Stream{std::cout, "DBG", dciLoggerIdentity} << __VA_ARGS__
#   define LOGT(...) dci::logger::Stream{std::cout, "TRC", dciLoggerIdentity} << __VA_ARGS__
