/**
 * @file brawcap_handle.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Handle.
 *
 * @copyright
 * <b> The MIT License (MIT)
 * Copyright © 2021 b-plus technologies GmbH. </b>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the “Software”), to deal in the  *Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef BRAWCAP_HANDLE_HPP
#define BRAWCAP_HANDLE_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdint>
#include <cassert>
// CPP
#include <string>

// bRAWcap
#include "libbrawcap.h"
#endif // INCLUDES

class BRAWcapHandle
{
public:
  inline BRAWcapHandle(const std::string& name)
  {
    brawcap_handle_t* pHandle = nullptr;
    brawcap_status_t status = brawcap_open(name.c_str(), &pHandle);
    assert(!BRAWCAP_ERROR(status)&& pHandle);
    m_pHandle = std::shared_ptr<brawcap_handle_t>(pHandle, &brawcap_close);
  }
  
  inline ~BRAWcapHandle()
  { }
  
  inline const brawcap_version_t VersionDriver() const
  {
    brawcap_version_t driverVersion = {0,0,0,0};
    brawcap_status_t status = brawcap_version_driver(m_pHandle.get(), &driverVersion);
    assert(!BRAWCAP_ERROR(status));
    return driverVersion;
  }
  
  inline void StatsReceive(brawcap_stats_rx_t& stats) const
  {
    brawcap_status_t status = brawcap_stats_rx(m_pHandle.get(), &stats);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void StatsTransmit(brawcap_stats_tx_t& stats) const
  {
    brawcap_status_t status = brawcap_stats_tx(m_pHandle.get(), &stats);
    assert(!BRAWCAP_ERROR(status));
  }
  
protected:
    inline std::shared_ptr<brawcap_handle_t> const Native() const
    {
      return m_pHandle;
    }

private:
    std::shared_ptr<brawcap_handle_t> m_pHandle;
};

#endif // BRAWCAP_HANDLE_HPP
