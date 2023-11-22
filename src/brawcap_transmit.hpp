/**
 * @file brawcap_transmit.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Transmit.
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
#ifndef BRAWCAP_TRANSMIT_HPP
#define BRAWCAP_TRANSMIT_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdbool>
#include <cassert>
// CPP
#include <vector>
#include <mutex>

// bRAWcap
#include "brawcap_handle.hpp"
#include "brawcap_adapter.hpp"
#include "brawcap_buffer.hpp"
#endif // INCLUDES

class BRAWcapTransmit : virtual public BRAWcapAdapter, virtual public BRAWcapHandle
{
  using TransmitBufferCompleteCallback = void(*)(BRAWcapBuffer& buffer, brawcap_status_t status, void* pUser);

public:
  inline BRAWcapTransmit(const std::string& name)
    : BRAWcapHandle(name), BRAWcapAdapter(name)
  { }
  
  inline ~BRAWcapTransmit()
  { }
  
  inline bool TransmitSinglePacket(const BRAWcapPacket& packet)
  {
    brawcap_status_t status = brawcap_tx_packet(BRAWcapHandle::Native().get(), packet.ResolvePacket());
    assert(!BRAWCAP_ERROR(status));
    return BRAWCAP_SUCCESS(status) || BRAWCAP_INFO(status);
  }
  
  inline bool TransmitStart(TransmitBufferCompleteCallback callback, void* pUser)
  {
    assert(callback);
    m_callback = callback;
    m_pUser = pUser;
    brawcap_status_t status = brawcap_tx_start(BRAWcapHandle::Native().get(), TransmitBufferCompleteInternal, this);
    assert(!BRAWCAP_ERROR(status));
    return BRAWCAP_SUCCESS(status) || BRAWCAP_INFO(status);
  }
  
  inline bool TransmitStop()
  {
    brawcap_status_t status = brawcap_tx_stop(BRAWcapHandle::Native().get());
    assert(!BRAWCAP_ERROR(status));
    return BRAWCAP_SUCCESS(status) || BRAWCAP_INFO(status);
  }
  
  inline bool TransmitBufferSend(BRAWcapBuffer& buffer, const bool synchronized)
  {
    m_bufferLock.lock();
    m_buffers.push_back(buffer);
    m_bufferLock.unlock();
    brawcap_status_t status = brawcap_tx_buffer_send(BRAWcapHandle::Native().get(), buffer.m_pBuffer.get(), synchronized);
    assert(!BRAWCAP_ERROR(status));
    return BRAWCAP_SUCCESS(status) || BRAWCAP_INFO(status);
  }
  
  inline void TransmitDriverQueueSizeSet(const brawcap_queue_size_t size)
  {
    brawcap_status_t status = brawcap_tx_driver_queue_size_set(BRAWcapHandle::Native().get(), size);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_queue_size_t TransmitDriverQueueSize()
  {
    brawcap_queue_size_t size = 0;
    brawcap_status_t status = brawcap_tx_driver_queue_size_get(BRAWcapHandle::Native().get(), &size);
    assert(!BRAWCAP_ERROR(status));
    return size;
  }
  
  inline brawcap_timestamp_capabilities_t TransmitTimestampCapabilities()
  {
    brawcap_timestamp_capabilities_t caps = 0;
    brawcap_status_t status = brawcap_tx_timestamp_capabilities(BRAWcapHandle::Native().get(), &caps);
    assert(!BRAWCAP_ERROR(status));
    return caps;
  }
  
  inline void TransmitTimestampModeSet(const brawcap_timestamp_mode_t mode)
  {
    brawcap_status_t status = brawcap_tx_timestamp_mode_set(BRAWcapHandle::Native().get(), mode);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline brawcap_timestamp_mode_t TransmitTimestampMode()
  {
    brawcap_timestamp_mode_t mode = BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP;
    brawcap_status_t status = brawcap_tx_timestamp_mode_get(BRAWcapHandle::Native().get(), &mode);
    assert(!BRAWCAP_ERROR(status));
    return mode;
  }
  
private:
  inline static void TransmitBufferCompleteInternal(brawcap_handle_t* const pHandle, const brawcap_status_t status,
    brawcap_buffer_t* const pBuffer, void* pUser)
  {
    BRAWcapTransmit* pTransmit = reinterpret_cast<BRAWcapTransmit*>(pUser);
    pTransmit->m_bufferLock.lock();
    for(std::vector<BRAWcapBuffer>::iterator it = pTransmit->m_buffers.begin(); it != pTransmit->m_buffers.end(); ++it)
    {
      if(it->m_pBuffer.get() == pBuffer)
      {
        BRAWcapBuffer buffer = *it;
        pTransmit->m_buffers.erase(it);
        pTransmit->m_bufferLock.unlock();
        pTransmit->m_callback(buffer, status, pTransmit->m_pUser);
        return;
      }
    }
    pTransmit->m_bufferLock.unlock();
  }
  
private:
  std::mutex m_bufferLock;
  std::vector<BRAWcapBuffer> m_buffers;
  TransmitBufferCompleteCallback m_callback;
  void* m_pUser;
};

#endif // BRAWCAP_TRANSMIT_HPP
