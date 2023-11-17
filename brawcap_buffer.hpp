/**
 * @file brawcap_buffer.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Buffer.
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
#ifndef BRAWCAP_BUFFER_HPP
#define BRAWCAP_BUFFER_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdbool>
// CPP
#include <memory>

// bRAWcap
#include "libbrawcap.h"
#include "brawcap_packet.hpp"
#endif // INCLUDES

class BRAWcapBuffer
{
  friend class BRAWcapReceive;
  friend class BRAWcapTransmit;
public:
  class Iterator
  {
    public:
      inline Iterator(std::shared_ptr<brawcap_buffer_t> buffer, brawcap_buffer_packet_count_t startIndex = 0)
        : m_packet(buffer, nullptr)
      {
        m_buffer = buffer;
        brawcap_buffer_iterator_t* pIterator = nullptr;
        brawcap_status_t status = brawcap_buffer_iterator_create(&pIterator, buffer.get(), startIndex);
        m_packet.MapTo(brawcap_buffer_iterator_eval(pIterator));
        assert(!BRAWCAP_ERROR(status));
        m_iterator = std::shared_ptr<brawcap_buffer_iterator_t>(pIterator, &brawcap_buffer_iterator_free);
      }
      
      inline ~Iterator()
      { }
      
      inline Iterator& operator++()
      {
        brawcap_status_t status = brawcap_buffer_iterator_next(m_iterator.get());
        assert(!BRAWCAP_ERROR(status));
        m_packet.MapTo(brawcap_buffer_iterator_eval(m_iterator.get()));
        return *this;
      }
      
      inline Iterator operator++(int)
      {
        Iterator result(*this);
        ++(*this);
        return result;
      }
      
      inline Iterator& operator--()
      {
        brawcap_status_t status =  brawcap_buffer_iterator_prev(m_iterator.get());
        assert(!BRAWCAP_ERROR(status));
        m_packet.MapTo(brawcap_buffer_iterator_eval(m_iterator.get()));
        return *this;
      }
      
      inline Iterator operator--(int)
      {
        Iterator result(*this);
        --(*this);
        return result;
      }
      
      inline BRAWcapPacket& operator*()
      {
        return m_packet;
      }
      
      inline BRAWcapPacket* operator->()
      {
        return &m_packet;
      }
      
      inline bool operator== (const Iterator& b)
      {
        return m_packet.m_pBufferedPacket == b.m_packet.m_pBufferedPacket;
      }
      
      inline bool operator!= (const Iterator& b)
      {
        return m_packet.m_pBufferedPacket != b.m_packet.m_pBufferedPacket;
      }
    
    private:
      std::weak_ptr<brawcap_buffer_t> m_buffer;
      std::shared_ptr<brawcap_buffer_iterator_t> m_iterator;
      BRAWcapPacket m_packet;
  };
  
public:
  inline BRAWcapBuffer(const brawcap_packet_size_t packetMaxPayloadSize, const brawcap_buffer_packet_count_t numPackets)
  {
    assert(packetMaxPayloadSize && numPackets);
    brawcap_buffer_t* pBuffer = nullptr;
    brawcap_status_t status = brawcap_buffer_create(&pBuffer, packetMaxPayloadSize, numPackets);
    assert(!BRAWCAP_ERROR(status) && pBuffer);
    m_pBuffer = std::shared_ptr<brawcap_buffer_t>(pBuffer,&brawcap_buffer_free);
  }
  
  inline ~BRAWcapBuffer()
  { }
  
  inline bool Clear()
  {
    brawcap_status_t status = brawcap_buffer_clear(m_pBuffer.get());
    assert(!BRAWCAP_ERROR(status));
    return BRAWCAP_SUCCESS(status) || BRAWCAP_INFO(status);
  }
  
  inline bool PushBack(const BRAWcapPacket& packet)
  {
    brawcap_status_t status = brawcap_buffer_add_back(m_pBuffer.get(), packet.ResolvePacket());
    if(!BRAWCAP_SUCCESS(status))
    {
      if(status == BRAWCAP_STATUS_ERROR_OVERRUN)
        return false;
      else
        assert(false);
    }
    return true;
  }
  
  inline bool PushFront(const BRAWcapPacket& packet)
  {
    brawcap_status_t status = brawcap_buffer_add_front(m_pBuffer.get(), packet.ResolvePacket());
    if(!BRAWCAP_SUCCESS(status))
    {
      if(status == BRAWCAP_STATUS_ERROR_OVERRUN)
        return false;
      else
        assert(false);
    }
    return true;
  }
  
  inline bool Insert(const BRAWcapPacket& packet, const brawcap_buffer_packet_count_t index)
  {
    bool retVal = false;
    brawcap_status_t status = brawcap_buffer_add_at_index(m_pBuffer.get(), packet.ResolvePacket(), index);
    if(!BRAWCAP_SUCCESS(status))
    {
      if(status == BRAWCAP_STATUS_ERROR_OVERRUN)
        retVal = false;
      else
        assert(false);
    }
    return retVal;
  }
  
  inline BRAWcapPacket Front()
  {
    return At(0);
  }
  
  inline BRAWcapPacket Back()
  {
    return At(Count());
  }
  
  inline BRAWcapPacket At(const brawcap_buffer_packet_count_t index)
  {
    brawcap_packet_t* pPacket = nullptr;
    if(!BRAWCAP_SUCCESS(brawcap_buffer_at_index(m_pBuffer.get(), index, &pPacket)))
      assert(false);
    
    return BRAWcapPacket(m_pBuffer, pPacket);
  }
  
  inline brawcap_buffer_packet_count_t Count()
  {
    brawcap_buffer_packet_count_t count = 0;
    if(!BRAWCAP_SUCCESS(brawcap_buffer_count(m_pBuffer.get(), &count)))
      assert(false);
    return count;
  }
  
  inline bool Empty()
  {
    return !Count();
  }
  
  inline brawcap_buffer_packet_count_t Capacity()
  {
    brawcap_buffer_packet_count_t capacity = 0;
    if(!BRAWCAP_SUCCESS(brawcap_buffer_capacity(m_pBuffer.get(), &capacity)))
      assert(false);
    return capacity;
  }
  
  inline Iterator Begin()
  {
    return Iterator(m_pBuffer, 0);
  }
  
  inline Iterator End()
  {
    brawcap_buffer_packet_count_t count = 0;
    if(!BRAWCAP_SUCCESS(brawcap_buffer_count(m_pBuffer.get(), &count)))
      assert(false);
    return ++Iterator(m_pBuffer, count);
  }
  
private:
  std::shared_ptr<brawcap_buffer_t> m_pBuffer;
};

#endif // BRAWCAP_BUFFER_HPP
