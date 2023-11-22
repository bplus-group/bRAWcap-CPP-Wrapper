/**
 * @file brawcap_receive.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Receive.
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
#ifndef BRAWCAP_RECEIVE_HPP
#define BRAWCAP_RECEIVE_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdbool>
#include <cassert>
// CPP
#include <string>
#include <vector>
#include <mutex>

// bRAWcap
#include "brawcap_handle.hpp"
#include "brawcap_adapter.hpp"
#include "brawcap_buffer.hpp"
#include "brawcap_filter.hpp"
#endif // INCLUDES

class BRAWcapReceive : virtual public BRAWcapAdapter, virtual public BRAWcapHandle
{
public:
  using RxBufferCompleteCallback = void(*)(BRAWcapBuffer& buffer, brawcap_status_t status, void* pUser);
  
public:
  inline BRAWcapReceive(const std::string& name)
    : BRAWcapAdapter(name), BRAWcapHandle(name)
  { }
  
  inline ~BRAWcapReceive()
  {
    while (!m_buffers.empty())
      ReceiveBufferRemove();
  }
  
  inline bool ReceiveSinglePacket(BRAWcapPacket& packet)
  {
    brawcap_status_t status = brawcap_rx_packet(BRAWcapHandle::Native().get(), packet.ResolvePacket());
    assert(!BRAWCAP_ERROR(status));
    return !BRAWCAP_ERROR(status);
  }
  
  inline void ReceiveStart(RxBufferCompleteCallback callback, void* pUser)
  {
    assert(callback);
    m_callback = callback;
    m_pUser = pUser;
    if(!BRAWCAP_SUCCESS(brawcap_rx_start(BRAWcapHandle::Native().get(), ReceiveBufferCompleteInternal, this, true)))
      assert(false);
  }
  
  inline void ReceiveStop()
  {
    if(!BRAWCAP_SUCCESS(brawcap_rx_stop(BRAWcapHandle::Native().get())))
      assert(false);
    
    m_callback = nullptr;
    m_pUser = nullptr;
  }
  
  inline void ReceiveBufferAdd(const brawcap_packet_size_t maxPacketPayloadSize,
    const brawcap_buffer_packet_count_t numPackets)
  {
    std::lock_guard<std::mutex> localLock(m_bufferLock);
    BRAWcapBuffer buffer(maxPacketPayloadSize, numPackets);
    m_buffers.push_back(buffer);
    brawcap_status_t status = brawcap_rx_buffer_attach(BRAWcapHandle::Native().get(), buffer.m_pBuffer.get());
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline void ReceiveBufferRemove()
  {
    std::lock_guard<std::mutex> localLock(m_bufferLock);
    brawcap_status_t status = brawcap_rx_buffer_detach(BRAWcapHandle::Native().get(), m_buffers.back().m_pBuffer.get());
    assert(!BRAWCAP_ERROR(status));
    m_buffers.pop_back();
  }
  
  inline void ReceiveDirectionSet(const brawcap_rx_direction_t direction)
  {
    brawcap_status_t status = brawcap_rx_direction_set(BRAWcapHandle::Native().get(), direction);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_rx_direction_t ReceiveDirection()
  {
    brawcap_rx_direction_t direction = BRAWCAP_RX_DIRECTION_DEFAULT;
    brawcap_status_t status = brawcap_rx_direction_get(BRAWcapHandle::Native().get(), &direction);
    assert(!BRAWCAP_ERROR(status));
    return direction;
  }
  
  inline void ReceiveMinPacketsSet(const brawcap_rx_min_packets_t minPackets)
  {
    brawcap_status_t status = brawcap_rx_min_packets_set(BRAWcapHandle::Native().get(), minPackets);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_rx_min_packets_t ReceiveMinPackets()
  {
    brawcap_rx_min_packets_t minPackets = 0;
    brawcap_status_t status = brawcap_rx_min_packets_get(BRAWcapHandle::Native().get(), &minPackets);
    assert(!BRAWCAP_ERROR(status));
    return minPackets;
  }
  
  inline void ReceiveTimeoutMillisecondsSet(const brawcap_rx_timeout_t timeout_ms)
  {
    brawcap_status_t status = brawcap_rx_timeout_set(BRAWcapHandle::Native().get(), timeout_ms);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_rx_timeout_t ReceiveTimeoutMilliseconds()
  {
    brawcap_rx_timeout_t timeout_ms = 0;
    brawcap_status_t status = brawcap_rx_timeout_get(BRAWcapHandle::Native().get(), &timeout_ms);
    assert(!BRAWCAP_ERROR(status));
    return timeout_ms;
  }
  
  inline void ReceiveFilterSet(const BRAWcapFilter& filter)
  {
    brawcap_status_t status = brawcap_rx_filter_set(BRAWcapHandle::Native().get(), filter.m_pFilter.get());
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline BRAWcapFilter ReceiveFilter()
  {
    BRAWcapFilter filter(BRAWCAP_FILTER_TYPE_BYTE_MASK);
    brawcap_status_t status = brawcap_rx_filter_get(BRAWcapHandle::Native().get(), filter.m_pFilter.get());
    assert(!BRAWCAP_ERROR(status));
    return filter;
  }
  
  inline void ReceivePromiscuousSet(const bool enable)
  {
    brawcap_status_t status = brawcap_rx_promiscuous_mode_set(BRAWcapHandle::Native().get(), enable);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline bool ReceivePromiscuousDriver()
  {
    BOOLEAN driver = FALSE;
    BOOLEAN adapter = FALSE;
    brawcap_status_t status = brawcap_rx_promiscuous_mode_get(BRAWcapHandle::Native().get(), &driver, &adapter);
    assert(!BRAWCAP_ERROR(status));
    return static_cast<bool>(driver);
  }
  
  inline bool ReceivePromiscuousAdapter()
  {
    BOOLEAN driver = FALSE;
    BOOLEAN adapter = FALSE;
    brawcap_status_t status = brawcap_rx_promiscuous_mode_get(BRAWcapHandle::Native().get(), &driver, &adapter);
    assert(!BRAWCAP_ERROR(status));
    return static_cast<bool>(adapter);
  }
  
  inline brawcap_timestamp_capabilities_t ReceiveTimestampCapabilities()
  {
    brawcap_timestamp_capabilities_t caps = 0;
    brawcap_status_t status = brawcap_rx_timestamp_capabilities(BRAWcapHandle::Native().get(), &caps);
    assert(!BRAWCAP_ERROR(status));
    return caps;
  }
  
  inline void ReceiveTimestampModeSet(const brawcap_timestamp_mode_t mode)
  {
    brawcap_status_t status = brawcap_rx_timestamp_mode_set(BRAWcapHandle::Native().get(), mode);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_timestamp_mode_t ReceiveTimestampMode()
  {
    brawcap_timestamp_mode_t mode = BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP;
    brawcap_status_t status = brawcap_rx_timestamp_mode_get(BRAWcapHandle::Native().get(), &mode);
    assert(!BRAWCAP_ERROR(status));
    return mode;
  }
  
  inline void ReceiveVlanTaggingSet(const bool enable)
  {
    brawcap_status_t status = brawcap_rx_vlan_tagging_set(BRAWcapHandle::Native().get(), enable);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline bool ReceiveVlanTagging()
  {
    BOOLEAN enabled = FALSE;
    brawcap_status_t status = brawcap_rx_vlan_tagging_get(BRAWcapHandle::Native().get(), &enabled);
    assert(!BRAWCAP_ERROR(status));
    return static_cast<bool>(enabled);
  }
  
  inline void ReceiveDriverQueueSizeSet(const brawcap_queue_size_t size)
  {
    brawcap_status_t status = brawcap_rx_driver_queue_size_set(BRAWcapHandle::Native().get(), size);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_queue_size_t ReceiveDriverQueueSize()
  {
    brawcap_queue_size_t size = 0;
    brawcap_status_t status = brawcap_rx_driver_queue_size_get(BRAWcapHandle::Native().get(), &size);
    assert(!BRAWCAP_ERROR(status));
    return size;
  }
  
private:
  inline static void ReceiveBufferCompleteInternal(brawcap_handle_t* const pHandle, const brawcap_status_t status,
    brawcap_buffer_t* const pBuffer, void* pUser)
  {
    BRAWcapReceive* pReceive = reinterpret_cast<BRAWcapReceive*>(pUser);
    for(auto buffer : pReceive->m_buffers)
    {
      if (!pReceive->m_callback)
        break;
      if(buffer.m_pBuffer.get() == pBuffer)
      {
        pReceive->m_callback(buffer, status, pReceive->m_pUser);
        break;
      }
    }
  }
  
private:
  std::vector<BRAWcapBuffer> m_buffers;
  std::mutex m_bufferLock;
  RxBufferCompleteCallback m_callback;
  void* m_pUser;
};

#endif // BRAWCAP_RECEIVE_HPP
