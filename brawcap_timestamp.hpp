/**
 * @file brawcap_timestamp.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Timestamp.
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
#ifndef BRAWCAP_TIMESTAMP_HPP
#define BRAWCAP_TIMESTAMP_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdint>
#include <cassert>

// bRAWcap
#include "libbrawcap.h"
#endif // INCLUDES

class BRAWcapTimestamp
{
protected:
  inline brawcap_timestamp_mode_t Mode()
  {
    assert(m_pTimestamp);
    brawcap_timestamp_mode_t mode;
    brawcap_status_t status = brawcap_timestamp_mode_get(m_pTimestamp, &mode);
    assert(!BRAWCAP_ERROR(status));
    return mode;
  }
  
  inline brawcap_timestamp_resolution_ns_t ResolutionNs()
  {
    assert(m_pTimestamp);
    brawcap_timestamp_resolution_ns_t resolution;
    brawcap_status_t status = brawcap_timestamp_resolution_ns_get(m_pTimestamp, &resolution);
    assert(!BRAWCAP_ERROR(status));
    return resolution;
  }
  
  inline void Ns(uint64_t& seconds, uint32_t& nanoseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_ns_get(m_pTimestamp, &seconds, &nanoseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void NsSet(const uint64_t seconds, const uint32_t nanoseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_ns_set(m_pTimestamp, seconds, nanoseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void Us(uint64_t& seconds, uint32_t& microseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_us_get(m_pTimestamp, &seconds, &microseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void UsSet(const uint64_t seconds, const uint32_t microseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_us_set(m_pTimestamp, seconds, microseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void Ms(uint64_t& seconds, uint32_t& milliseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_ms_get(m_pTimestamp, &seconds, &milliseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void MsSet(const uint64_t seconds, const uint32_t milliseconds)
  {
    assert(m_pTimestamp);
    brawcap_status_t status = brawcap_timestamp_value_ms_set(m_pTimestamp, seconds, milliseconds);
    assert(!BRAWCAP_ERROR(status));
  }
  
protected:
  inline BRAWcapTimestamp()
  { }
  
  inline ~BRAWcapTimestamp()
  { }
  
  inline void Init(brawcap_timestamp_t* pTimestamp)
  {
    m_pTimestamp = pTimestamp;
  }
  
private:
  brawcap_timestamp_t* m_pTimestamp;
};

#endif // BRAWCAP_TIMESTAMP_HPP
