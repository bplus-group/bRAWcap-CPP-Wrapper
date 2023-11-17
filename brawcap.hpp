/**
 * @file brawcap.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Main.
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
#ifndef BRAWCAP_HPP
#define BRAWCAP_HPP

#if 1 // INCLUDES
// STD
// C
#include <cassert>
// CPP
#include <string>

// bRAWcap
#include "brawcap_handle.hpp"
#include "brawcap_receive.hpp"
#include "brawcap_transmit.hpp"
#endif // INCLUDES

class BRAWcap : public BRAWcapReceive, public BRAWcapTransmit, virtual public BRAWcapHandle
{
public:
  using BRAWcapAdapter::AdapterList;
  
  using BRAWcapAdapter::AdapterChangeNotificationRegister;
  
  using BRAWcapAdapter::AdapterChangeNotificationUnregister;
  
  inline static const brawcap_version_t VersionLib()
  {
    brawcap_version_t apiVersion = {0,0,0,0};
    brawcap_status_t status = brawcap_version_api(&apiVersion);
    assert(!BRAWCAP_ERROR(status));
    
    return apiVersion;
  }
  
  inline static void LoglevelSet(const brawcap_log_type_t type, const brawcap_log_level_t level, const char* key)
  {
    brawcap_status_t status = brawcap_log_level_set(type, level, key);
    assert(!BRAWCAP_ERROR(status));
  }
  
  inline static brawcap_log_level_t LogLevel(const brawcap_log_type_t type)
  {
    brawcap_log_level_t logLevel;
    brawcap_status_t status = brawcap_log_level_get(type, &logLevel);
    assert(!BRAWCAP_ERROR(status));
    return logLevel;
  }
  
public:
  inline BRAWcap(const std::string& name)
    : BRAWcapHandle(name), BRAWcapAdapter(name), BRAWcapReceive(name), BRAWcapTransmit(name)
  { }
  
  ~BRAWcap()
  { }
};

#endif // BRAWCAP_HPP
