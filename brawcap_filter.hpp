/**
 * @file brawcap_filter.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Filter.
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
#ifndef BRAWCAP_FILTER_HPP
#define BRAWCAP_FILTER_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdbool>
#include <cstdint>
#include <cassert>
// CPP
#include <memory>

// bRAWcap
#include "libbrawcap.h"
#endif // INCLUDES

class BRAWcapFilter
{
  friend class BRAWcapReceive;
public:
  inline BRAWcapFilter(const brawcap_filter_type_t type = BRAWCAP_FILTER_TYPE_BYTE_MASK)
    : m_pFilter(nullptr), m_type(type), m_captureByteOffset(0), m_captureByteLength(0), m_byteOffset(0),
      m_byteLength(0)
  {
    memset(m_byteMask, 0, sizeof(m_byteMask));
    memset(m_byteIgnore, 0, sizeof(m_byteIgnore));
    
    brawcap_filter_t* pFilter = nullptr;
    brawcap_status_t status = brawcap_filter_create(&pFilter, type);
    assert(!BRAWCAP_ERROR(status));
    
    m_pFilter = std::shared_ptr<brawcap_filter_t>(pFilter, &brawcap_filter_free);
  }
  
  inline ~BRAWcapFilter()
  { }
  
  inline brawcap_filter_type_t Type()
  {
    brawcap_filter_type_t type;
    brawcap_status_t status = brawcap_filter_type_get(m_pFilter.get(), &type);
    assert(!BRAWCAP_ERROR(status));
    return type;
  }
  
  inline void Activate()
  {
    brawcap_status_t status = brawcap_filter_activate(m_pFilter.get());
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void Deactivate()
  {
    brawcap_status_t status = brawcap_filter_deactivate(m_pFilter.get());
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline bool IsActivated()
  {
    BOOLEAN activated;
    brawcap_status_t status = brawcap_filter_is_activated(m_pFilter.get(), &activated);
    assert(!BRAWCAP_ERROR(status));
    
    return static_cast<bool>(activated);
  }
  
  inline void Indicate(bool enable)
  {
    brawcap_status_t status = brawcap_filter_indicate_set(m_pFilter.get(), enable);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline bool Indication()
  {
    BOOLEAN indication;
    brawcap_status_t status = brawcap_filter_indicate_get(m_pFilter.get(), &indication);
    assert(!BRAWCAP_ERROR(status));
    return static_cast<bool>(indication);
  }
  
  inline void CaptureByteOffsetSet(const brawcap_packet_size_t offset)
  {
    brawcap_status_t status = brawcap_filter_bytes_to_capture_set(m_pFilter.get(), offset, m_captureByteLength);
    assert(!BRAWCAP_ERROR(status));
    m_captureByteOffset = offset;
  }
  
  inline brawcap_packet_size_t CaptureByteOffset()
  {
    brawcap_packet_size_t length = 0;
    brawcap_packet_size_t offset = 0;
    brawcap_status_t status = brawcap_filter_bytes_to_capture_get(m_pFilter.get(), &offset, &length);
    assert(!BRAWCAP_ERROR(status));
    return offset;
  }
  
  inline void CaptureByteLengthSet(const brawcap_packet_size_t length)
  {
    brawcap_status_t status = brawcap_filter_bytes_to_capture_set(m_pFilter.get(), m_captureByteOffset, length);
    assert(!BRAWCAP_ERROR(status));
    m_captureByteLength = length;
  }
  
  inline brawcap_packet_size_t CaptureByteLength()
  {
    brawcap_packet_size_t length = 0;
    brawcap_packet_size_t offset = 0;
    brawcap_status_t status = brawcap_filter_bytes_to_capture_get(m_pFilter.get(), &offset, &length);
    assert(!BRAWCAP_ERROR(status));
    return length;
  }
  
  inline void ByteFilterOffsetSet(const brawcap_packet_size_t offset)
  {
    brawcap_status_t status = brawcap_filter_mask_set(m_pFilter.get(), offset, m_byteLength, m_byteMask, m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    m_byteOffset = offset;
  }
  
  inline brawcap_packet_size_t ByteFilterOffset()
  {
    brawcap_status_t status = brawcap_filter_mask_get(m_pFilter.get(), &m_byteOffset, &m_byteLength, &m_byteMask,
                                                      &m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    return m_byteOffset;
  }
  
  inline void ByteFilterLengthSet(const brawcap_packet_size_t length)
  {
    brawcap_status_t status = brawcap_filter_mask_set(m_pFilter.get(), m_byteOffset, length, m_byteMask, m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    m_byteLength = length;
  }
  
  inline brawcap_packet_size_t ByteFilterLength()
  {
    brawcap_status_t status = brawcap_filter_mask_get(m_pFilter.get(), &m_byteOffset, &m_byteLength, &m_byteMask,
                                                      &m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    return m_byteLength;
  }
  
  inline void ByteFilterByteMaskSet(const brawcap_filter_mask_array_t mask)
  {
    brawcap_status_t status = brawcap_filter_mask_set(m_pFilter.get(), m_byteOffset, m_byteLength, mask, m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    memcpy(m_byteMask, mask, sizeof(brawcap_filter_mask_array_t));
  }
  
  inline void ByteFilterByteMaskGet(brawcap_filter_mask_array_t mask)
  {
    brawcap_status_t status = brawcap_filter_mask_get(m_pFilter.get(), &m_byteOffset, &m_byteLength, &m_byteMask,
                                                      &m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    memcpy(mask, m_byteMask, sizeof(brawcap_filter_mask_array_t));
  }
  
  inline void ByteFilterIgnoreBitsSet(const brawcap_filter_ignore_bits_array_t ignoreBits)
  {
    brawcap_status_t status = brawcap_filter_mask_set(m_pFilter.get(), m_byteOffset, m_byteLength, m_byteMask,
                                                      ignoreBits);
    assert(!BRAWCAP_ERROR(status));
    memcpy(m_byteIgnore, ignoreBits, sizeof(brawcap_filter_ignore_bits_array_t));
  }
  
  inline void ByteFilterIgnoreBitsGet(brawcap_filter_ignore_bits_array_t ignoreBits)
  {
    brawcap_status_t status = brawcap_filter_mask_get(m_pFilter.get(), &m_byteOffset, &m_byteLength, &m_byteMask,
                                                      &m_byteIgnore);
    assert(!BRAWCAP_ERROR(status));
    memcpy(ignoreBits, m_byteIgnore, sizeof(brawcap_filter_ignore_bits_array_t));
  }
  
private:
  std::shared_ptr<brawcap_filter_t> m_pFilter;
  
  brawcap_filter_type_t m_type;
  brawcap_packet_size_t m_captureByteOffset;
  brawcap_packet_size_t m_captureByteLength;
  
  // Byte filter
  brawcap_packet_size_t m_byteOffset;
  brawcap_packet_size_t m_byteLength;
  brawcap_filter_mask_array_t m_byteMask;
  brawcap_filter_ignore_bits_array_t m_byteIgnore;
};

#endif // BRAWCAP_FILTER_HPP
