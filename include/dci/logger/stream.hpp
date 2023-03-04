/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <sstream>
#include <iostream>
#include <string_view>
#include <system_error>
#include "timeProvider.hpp"

namespace dci::logger
{
    class Stream
    {
        std::ostream& _out;
        std::stringstream _buf;

    public:
        Stream(std::ostream& out, std::string_view level, std::string_view identity)
            : _out{out}
        {
            auto putOne = [&](bool needSpace, std::string_view str) -> bool
            {
                if(str.empty())
                {
                    return false;
                }

                if(needSpace)
                    _buf << ' ';
                _buf << str;
                return true;
            };

            bool needSpace = false;
            needSpace |= putOne(needSpace, timeProvidedAsString());
            needSpace |= putOne(needSpace, level);
            needSpace |= putOne(needSpace, identity);
            _buf << ": ";
        }

        ~Stream()
        {
            _buf << '\n';
            std::string_view str{_buf.view()};
            _out.write(str.data(), str.size());
            _out.flush();
        }

        Stream& operator<<(const std::error_code& ec)
        {
            _buf << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
            return *this;
        }

        Stream& operator<<(const std::error_condition& ec)
        {
            _buf << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
            return *this;
        }

        template <class T> Stream& operator<<(const T& v)
        {
            _buf << v;
            return *this;
        }
    };

}
