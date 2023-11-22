/**
 * @file brawcap_packet.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Packet.
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
#ifndef BRAWCAP_PACKET_HPP
#define BRAWCAP_PACKET_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdint>
#include <cassert>
// CPP
#include <memory>

// bRAWcap
#include "libbrawcap.h"
#include "brawcap_timestamp.hpp"
#endif // INCLUDES

class BRAWcapPacket : public BRAWcapTimestamp
{
  friend class BRAWcapBuffer;
  friend class BRAWcapReceive;
  friend class BRAWcapTransmit;
public:
  inline BRAWcapPacket(const brawcap_packet_size_t maxPayloadSize)
    : m_created(true)
  {
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;
    brawcap_packet_t* pPacket = nullptr;
    status = brawcap_packet_create(&pPacket, maxPayloadSize);
    assert(!BRAWCAP_ERROR(status) && pPacket);
    m_pCreatedPacket = std::shared_ptr<brawcap_packet_t>(pPacket, &brawcap_packet_free);
    
    brawcap_timestamp_t* pTimestamp = nullptr;
    status = brawcap_packet_timestamp_get(m_pCreatedPacket.get(), &pTimestamp);
    assert(!BRAWCAP_ERROR(status) && pTimestamp);
    BRAWcapTimestamp::Init(pTimestamp);
  }
  
  inline ~BRAWcapPacket()
  { }
  
  inline brawcap_status_t Status()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_status_t packetStatus = BRAWCAP_STATUS_ERROR_FAILED;
    brawcap_status_t status = brawcap_packet_status_get(pPacket, &packetStatus);
    assert(!BRAWCAP_ERROR(status));
    return packetStatus;
  }
  
  inline brawcap_packet_size_t LengthOnWire()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_packet_size_t lengthOnWire = 0;
    brawcap_status_t status = brawcap_packet_length_on_wire_get(pPacket, &lengthOnWire);
    assert(!BRAWCAP_ERROR(status));
    return lengthOnWire;
  }
  
  inline brawcap_packet_size_t MaxPayloadSize()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_packet_size_t maxPayloadSize = 0;
    brawcap_status_t status = brawcap_packet_payload_max_size_get(pPacket, &maxPayloadSize);
    assert(!BRAWCAP_ERROR(status));
    return maxPayloadSize;
  }
  
  inline brawcap_packet_size_t PayloadLength()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_packet_size_t length = 0;
    brawcap_status_t status = brawcap_packet_payload_size_get(pPacket, &length);
    assert(!BRAWCAP_ERROR(status));
    return length;
  }
  
  inline void PayloadRef(const char*& payload, brawcap_packet_size_t& length)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_status_t status = brawcap_packet_payload_get(pPacket, &payload, &length);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline bool PayloadCopy(char* payload, brawcap_packet_size_t& length)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    
    const char* tempPayload = nullptr;
    brawcap_packet_size_t tempLength = 0;
    brawcap_status_t status = brawcap_packet_payload_get(pPacket, &tempPayload, &tempLength);
    assert(!BRAWCAP_ERROR(status));
    
    if(tempLength > length)
    {
      length = tempLength;
      return false;
    }
    
    memcpy(payload, tempPayload, tempLength);
    return true;
  }
  
  inline bool PayloadSet(const char* pPayload, const brawcap_packet_size_t length)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    brawcap_status_t status = brawcap_packet_payload_set(pPacket, pPayload, length);
    if(!BRAWCAP_SUCCESS(status))
    {
      if(status == BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE)
        return false;
      
      assert(!BRAWCAP_ERROR(status));
    }
    return true;
  }
  
  inline brawcap_timestamp_mode_t TimestampMode()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    return BRAWcapTimestamp::Mode();
  }
  
  inline brawcap_timestamp_resolution_ns_t TimestampResolutionNs()
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    return BRAWcapTimestamp::ResolutionNanoseconds();
  }
  
  inline void TimestampNs(uint64_t& seconds, uint32_t& nanoseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::Nanoseconds(seconds, nanoseconds);
  }
  
  inline void TimestampNsSet(const uint64_t seconds, const uint32_t nanoseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::NanosecondsSet(seconds, nanoseconds);
  }
  
  inline void TimestampUs(uint64_t& seconds, uint32_t& microseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::Microseconds(seconds, microseconds);
  }
  
  inline void TimestampUsSet(const uint64_t seconds, const uint32_t microseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::MicrosecondsSet(seconds, microseconds);
  }
  
  inline void TimestampMs(uint64_t& seconds, uint32_t& milliseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::Milliseconds(seconds, milliseconds);
  }
  
  inline void TimestampMsSet(const uint64_t seconds, const uint32_t milliseconds)
  {
    brawcap_packet_t* pPacket = ResolvePacket();
    BRAWcapTimestamp::MillisecondsSet(seconds, milliseconds);
  }
  
private:
  inline BRAWcapPacket(std::shared_ptr<brawcap_buffer_t> pBuffer, brawcap_packet_t* pBufferedPacket)
    : m_created(false)
  {
    m_pBuffer = pBuffer;
    MapTo(pBufferedPacket);
  }
  
  inline brawcap_packet_t* ResolvePacketSafe(std::shared_ptr<brawcap_buffer_t>& pLockedBuffer) const
  {
    brawcap_packet_t* pPacket = nullptr;
    if(m_created)
      pPacket = m_pCreatedPacket.get();
    else
    {
      pLockedBuffer = m_pBuffer.lock();
      if(pLockedBuffer)
        pPacket = m_pBufferedPacket;
      else
        assert(false);
    }
    return pPacket;
  }
  
  inline brawcap_packet_t* ResolvePacket() const
  {
    brawcap_packet_t* pPacket = nullptr;
    if(m_created)
      pPacket = m_pCreatedPacket.get();
    else
    {
      //assert(!m_pBuffer.expired());
      pPacket = m_pBufferedPacket;
    }
    return pPacket;
  }
  
  inline void MapTo(brawcap_packet_t* pBufferedPacket)
  {
    m_pBufferedPacket = pBufferedPacket;
    if (pBufferedPacket)
    {
      brawcap_timestamp_t* pTimestamp = nullptr;
      brawcap_status_t status = brawcap_packet_timestamp_get(m_pBufferedPacket, &pTimestamp);
      assert(!BRAWCAP_ERROR(status) && pTimestamp);
      BRAWcapTimestamp::Init(pTimestamp);
    }
    else
      BRAWcapTimestamp::Init(nullptr);
  }
  
private:
  const bool m_created;
  std::shared_ptr<brawcap_packet_t> m_pCreatedPacket;
  
  std::weak_ptr<brawcap_buffer_t> m_pBuffer;
  brawcap_packet_t* m_pBufferedPacket;
};

#endif // BRAWCAP_PACKET_HPP
