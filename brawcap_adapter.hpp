/**
 * @file brawcap_adapter.hpp
 * @authors johannes.fellinger@b-plus.com
 * @brief bRAWcap CPP Wrapper Adapter.
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
#ifndef BRAWCAP_ADAPTER_HPP
#define BRAWCAP_ADAPTER_HPP

#if 1 // INCLUDES
// STD
// C
#include <cstdint>
#include <cassert>
// CPP
#include <string>
#include <sstream>
#include <vector>

// bRAWcap
#include "libbrawcap.h"
#include "brawcap_handle.hpp"
#endif // INCLUDES

class BRAWcapAdapter : virtual public BRAWcapHandle
{
public:
  inline static std::vector<std::string> AdapterList()
  {
    std::vector<std::string> adapterList = {};
    
    if(!BRAWCAP_SUCCESS(brawcap_adapter_list_update()))
      assert(false);
    
    brawcap_adapter_count_t count = 0;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_list_count(&count)))
      assert(false);
    
    for(brawcap_adapter_count_t index = 0; index < count; ++index)
    {
      brawcap_adapter_name_t adapterName = { '\0' };
      if(!BRAWCAP_SUCCESS(brawcap_adapter_list_at(index, adapterName)))
        assert(false);
      
      adapterList.push_back(adapterName);
    }
    
    return adapterList;
  }
  
  inline static void AdapterChangeNotificationRegister(brawcap_adapter_notify_callback_t callback, void* user)
  {
    if(!BRAWCAP_SUCCESS(brawcap_adapter_notify_register(callback, user)))
      assert(false);
  }
  
  inline static void AdapterChangeNotificationUnregister()
  {
    if(!BRAWCAP_SUCCESS(brawcap_adapter_notify_unregister()))
      assert(false);
  }
  
public:
  inline BRAWcapAdapter(const std::string& name)
    : BRAWcapHandle(name)
  { }
  
  inline ~BRAWcapAdapter()
  { }
  
  inline std::string AdapterFriendlyName() const
  {
    UINT32 length = 0;
    char* friendlyName = nullptr;
    std::string strFriendlyName = "";
    
    if(brawcap_adapter_friendly_name_by_handle(BRAWcapHandle::Native().get(), nullptr, &length)
      == BRAWCAP_STATUS_ERROR_OVERRUN)
    {
      friendlyName = (char*) _alloca(length);
      if(!BRAWCAP_SUCCESS(brawcap_adapter_friendly_name_by_handle(BRAWcapHandle::Native().get(), friendlyName, &length)))
        assert(false);
    }
    if(friendlyName)
      strFriendlyName = friendlyName;
    return strFriendlyName;
  }
  
  inline std::string AdapterName() const
  {
    brawcap_adapter_name_t name = { '\0' };
    if(!BRAWCAP_SUCCESS(brawcap_adapter_name_by_handle(BRAWcapHandle::Native().get(), name)))
      assert(false);
    
    return name;
  }
  
  inline std::string AdapterDesc() const
  {
    UINT32 length = 0;
    char* description = nullptr;
    std::string strDescription = "";
    
    if(brawcap_adapter_description_by_handle(BRAWcapHandle::Native().get(), nullptr, &length)
      == BRAWCAP_STATUS_ERROR_OVERRUN)
    {
      description = (char*) _alloca(length);
      if(!BRAWCAP_SUCCESS(brawcap_adapter_description_by_handle(BRAWcapHandle::Native().get(), description, &length)))
        assert(false);
    }
    if(description)
      strDescription = description;
    return strDescription;
  }
  
  inline brawcap_adapter_ipv4_t AdapterIpv4() const
  {
    brawcap_adapter_ipv4_t ipv4;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_ipv4_by_handle(BRAWcapHandle::Native().get(), &ipv4)))
      assert(false);
    return ipv4;
  }
  
  inline std::string AdapterIpv4String() const
  {
    std::stringstream strIpv4;
    brawcap_adapter_ipv4_t ipv4 = AdapterIpv4();
    for(auto i = 0; i < 4; ++i)
    {
      strIpv4 << std::dec << ipv4.bytes[i];
      if(i != 3)
        strIpv4 << ".";
    }
    return strIpv4.str();
  }
  
  inline brawcap_adapter_ipv6_t AdapterIpv6() const
  {
    brawcap_adapter_ipv6_t ipv6;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_ipv6_by_handle(BRAWcapHandle::Native().get(), &ipv6)))
      assert(false);
    return ipv6;
  }
  
  inline std::string AdapterIpv6String() const
  {
    std::stringstream strIpv6;
    brawcap_adapter_ipv6_t ipv6 = AdapterIpv6();
    for(auto i = 0; i < 8; ++i)
    {
      strIpv6 << std::hex << ipv6.bytes[i];
      if(i != 7)
        strIpv6 << ":";
    }
    return strIpv6.str();
  }
  
  inline void AdapterMac(brawcap_adapter_mac_t mac) const
  {
    if(!BRAWCAP_SUCCESS(brawcap_adapter_mac_by_handle(BRAWcapHandle::Native().get(), mac)))
      assert(false);
  }
  
  inline std::string AdapterMac() const
  {
    brawcap_adapter_mac_t mac = { 0 };
    AdapterMac(mac);
    char cMac[18] = { '\0' };
    snprintf(cMac, sizeof(cMac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(cMac);
  }
  
  inline brawcap_adapter_speed_t AdapterRxSpeed() const
  {
    brawcap_adapter_speed_t rxSpeed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_speed_rx_by_handle(BRAWcapHandle::Native().get(), &rxSpeed)))
      assert(false);
    return rxSpeed;
  }
  
  inline brawcap_adapter_speed_t AdapterTxSpeed() const
  {
    brawcap_adapter_speed_t txSpeed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_speed_tx_by_handle(BRAWcapHandle::Native().get(), &txSpeed)))
      assert(false);
    return txSpeed;
  }
  
  inline brawcap_adapter_connection_state_t AdapterConnection() const
  {
    brawcap_adapter_connection_state_t connectionState = BRAWCAP_ADAPTER_CONNECTION_UNKNOWN;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_connection_by_handle(BRAWcapHandle::Native().get(), &connectionState)))
      assert(false);
    return connectionState;
  }
  
  inline brawcap_adapter_operation_state_t AdapterOperation() const
  {
    brawcap_adapter_operation_state_t operationState = BRAWCAP_ADAPTER_OPERATION_UNKNOWN;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_operation_by_handle(BRAWcapHandle::Native().get(), &operationState)))
      assert(false);
    return operationState;
  }
  
  inline brawcap_adapter_mtu_t AdapterMtu() const
  {
    brawcap_adapter_mtu_t mtu = 0;
    if(!BRAWCAP_SUCCESS(brawcap_adapter_mtu_by_handle(BRAWcapHandle::Native().get(), &mtu)))
      assert(false);
    return mtu;
  }
  
  inline void NotificationRegister(brawcap_adapter_notify_handle_callback_t callback, void* user)
  {
    if(!BRAWCAP_SUCCESS(brawcap_adapter_notify_handle_register(BRAWcapHandle::Native().get(), callback, user)))
      assert(false);
  }
  
  inline void NotificationUnregister()
  {
    if(!BRAWCAP_SUCCESS(brawcap_adapter_notify_handle_unregister(BRAWcapHandle::Native().get())))
      assert(false);
  }
};

#endif // BRAWCAP_ADAPTER_HPP
