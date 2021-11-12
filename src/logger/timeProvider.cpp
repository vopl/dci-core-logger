/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/logger/timeProvider.hpp>
#include <utility>
#include <array>

namespace dci::logger
{
    namespace
    {
        TimeProvider g_timeProvider{};

        char g_buf[64] {};

        struct LastState
        {
            std::chrono::system_clock::time_point _timeProvided {};
            std::chrono::seconds _secs {};

            std::string_view _strToSecs {};
            std::string_view _str {};
        } g_lastState {};
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    TimeProvider API_DCI_LOGGER setTimeProvider(TimeProvider tp)
    {
        return std::exchange(g_timeProvider, tp);
    }

    namespace
    {
        constexpr std::array<char, 1000*3> thousand = []
        {
            std::array<char, 1000*3> res;
            for(std::size_t i{}; i<res.size()/3; ++i)
            {
                res[i*3+0] = '0' + i/100%10;
                res[i*3+1] = '0' + i/10%10;
                res[i*3+2] = '0' + i/1%10;
            }
            return res;
        }();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::string_view API_DCI_LOGGER timeProvidedAsString()
    {
        if(!g_timeProvider)
        {
            return {};
        }

        std::chrono::system_clock::time_point now = g_timeProvider();
        if(now == g_lastState._timeProvided)
        {
            return g_lastState._str;
        }

        g_lastState._timeProvided = now;

        std::chrono::microseconds time_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

        std::size_t end;
        std::chrono::seconds secs = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch);
        if(secs == g_lastState._secs)
        {
            end = g_lastState._strToSecs.size();
        }
        else
        {
            std::time_t t = secs.count();
            end = strftime(g_buf, sizeof(g_buf), "%F %T", std::localtime(&t));
            g_lastState._strToSecs = std::string_view{g_buf, end};
        }

        std::size_t msecs = static_cast<std::size_t>((time_since_epoch - std::chrono::duration_cast<std::chrono::microseconds>(secs)).count());

        g_buf[end+0] = '.';

        auto d3 = [&](int mult, char* buf)
        {
            int idx = msecs/mult % 1000;
            buf[0] = thousand[idx*3+0];
            buf[1] = thousand[idx*3+1];
            buf[2] = thousand[idx*3+2];
        };
        d3(1000, g_buf+end+1);
        d3(1, g_buf+end+4);

        g_buf[end+7] = 0;
        end += 7;

        g_lastState._str = std::string_view{g_buf, end};
        return g_lastState._str;
    }
}
