/**
 * @file 01_adapter_property_reader.c
 *
 * @brief bRAWcap Example - Demonstrates the usage of the adapter handling module.
 *
 * It shows how to:
 *   - Update and use the adapter list
 *   - Read adapter properties
 *
 * This example makes use of C only.
 *
 * @attention This example requires a valid bRAWcap license.
 * Reading adapter properties is not part of the demo.
 *
 * @version 1.0
 *
 * @date 2023-03-24
 *
 * @copyright
 * <b> © 2021 - b-plus technologies GmbH. All rights reserved!</b>
 *
 * All rights exclusively reserved for b-plus GmbH, unless expressly otherwise agreed.
 *
 * Redistribution in source or any other form, with or without modification, is not permitted.
 *
 * You may use this code under the according license terms of b-plus.
 * Please contact b-plus at services@b-plus.com to get the appropriate terms and conditions.
 */
// Include bRAWcap
#include "libbrawcap.h"

// C STD
#include <stdlib.h> // for malloc & free
#include <stdio.h>  // for printf

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Adapter Property Reader");

    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // Encapsulate all in a single loop.
    // This allow breaking execution if something unexpected happens.
    do
    {
#if 1   // Update list of available adapters
        // NOTE: The list is initialized during loading/initializing bRAWcap.
        status = brawcap_adapter_list_update();
        if(!BRAWCAP_SUCCESS(status))
            break;
#endif
#if 1   // Get number of currently available adapters
        brawcap_adapter_count_t adapterCount = 0;
        status = brawcap_adapter_list_count(&adapterCount);
        if(!BRAWCAP_SUCCESS(status))
            break;
#endif
#if 1   // Now loop through each entry of the list.
        // Here we retrieve all it´s available adapter properties and print them to console.
        for(unsigned int index = 0; index < adapterCount; ++index)
        {
#if 1       // Get adapter name from list
            brawcap_adapter_name_t name = {'\0'};
            status = brawcap_adapter_list_at(index, name);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("%02u. Adapter - Name: %s ", index + 1, name);
#endif
#if 1       // Resolve adapter friendly name by using it´s name
            UINT32 friendlyNameLength = 0;
            char* friendlyName = NULL;
            status = brawcap_adapter_friendly_name(name, friendlyName, &friendlyNameLength);
            // We expect this return value because our buffer is not yet initialized (zero).
            // If we use the function like this, it will inform us of the required this by setting friendlyNameLength.
            if(status == BRAWCAP_STATUS_ERROR_OVERRUN)
            {
                // friendlyNameLength is set to the required length, so lets use it to create the buffer
                friendlyName = malloc(friendlyNameLength);
                // Now call the function again with a valid buffer
                status = brawcap_adapter_friendly_name(name, friendlyName, &friendlyNameLength);
                if(!BRAWCAP_SUCCESS(status))
                    break;
            }
            else
                break;
            printf("Friendly Name: %s ", friendlyName);
#endif
#if 1       // Resolve adapter description by using it´s name
            UINT32 descriptionLength = 0;
            char* description = NULL;
            status = brawcap_adapter_description(name, description, &descriptionLength);
            // Here we have the same handling like we had for the friendly name...
            if(status == BRAWCAP_STATUS_ERROR_OVERRUN)
            {
                // friendlyNameLength is set to the required length, so lets use it to create the buffer
                description = malloc(descriptionLength);
                // Now call the function again with a valid buffer
                status = brawcap_adapter_description(name, description, &descriptionLength);
                if(!BRAWCAP_SUCCESS(status))
                    break;
            }
            else
                break;
            printf("Description: %s ", description);
#endif
#if 1       // Resolve adapter MAC address by using it´s name
            brawcap_adapter_mac_t mac = {0};
            status = brawcap_adapter_mac(name, mac);
            if (!BRAWCAP_SUCCESS(status))
                break;
            printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
#if 1       // Resolve adapter operation state by using it´s name
            brawcap_adapter_operation_state_t operationState = BRAWCAP_ADAPTER_OPERATION_UNKNOWN;
            status = brawcap_adapter_operation(name, &operationState);
            if(!BRAWCAP_SUCCESS(status))
                break;
            switch (operationState)
            {
            case BRAWCAP_ADAPTER_OPERATION_UNKNOWN: printf("Operation: %s ", "Unknown"); break;
            case BRAWCAP_ADAPTER_OPERATION_DOWN:    printf("Operation: %s ", "Down");    break;
            case BRAWCAP_ADAPTER_OPERATION_TESTING: printf("Operation: %s ", "Testing"); break;
            case BRAWCAP_ADAPTER_OPERATION_UP:      printf("Operation: %s ", "Up");      break;
            }
#endif
#if 1       // Resolve adapter connection state by using it´s name
            brawcap_adapter_connection_state_t connectionState = BRAWCAP_ADAPTER_CONNECTION_UNKNOWN;
            status = brawcap_adapter_connection(name, &connectionState);
            if(!BRAWCAP_SUCCESS(status))
                break;
            switch (connectionState)
            {
            case BRAWCAP_ADAPTER_CONNECTION_UNKNOWN: printf("Connection: %s ", "Unknown"); break;
            case BRAWCAP_ADAPTER_CONNECTION_DOWN:    printf("Connection: %s ", "Down");    break;
            case BRAWCAP_ADAPTER_CONNECTION_UP:      printf("Connection: %s ", "Up");      break;
            }
#endif
#if 1       // Resolve adapter IPv4 address by using it´s name
            brawcap_adapter_ipv4_t ipv4 = {0};
            status = brawcap_adapter_ipv4(name, &ipv4);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("IPv4: %u.%u.%u.%u ", ipv4.bytes[0], ipv4.bytes[1], ipv4.bytes[2], ipv4.bytes[3]);
#endif
#if 1       // Resolve adapter IPv6 address by using it´s name
            brawcap_adapter_ipv6_t ipv6 = {0};
            status = brawcap_adapter_ipv6(name, &ipv6);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("IPv6: %x:%x:%x:%x:%x:%x:%x:%x ", ipv6.words[0], ipv6.words[1], ipv6.words[2], ipv6.words[3],
                                                     ipv6.words[4], ipv6.words[5], ipv6.words[6], ipv6.words[7]);
#endif
#if 1       // Resolve adapter max transmission unit (MTU) size by using it´s name
            brawcap_adapter_mtu_t mtu = 0;
            status = brawcap_adapter_mtu(name, &mtu);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("MTU: %llu ", mtu);
#endif
#if 1       // Resolve adapter receive speed (uplink speed) by using it´s name
            brawcap_adapter_speed_t rxSpeed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
            status = brawcap_adapter_speed_rx(name, &rxSpeed);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("RX Speed: %d MBit/s ", rxSpeed);
#endif
#if 1       // Resolve adapter transmit speed (uplink speed) by using it´s name
            brawcap_adapter_speed_t txSpeed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
            status = brawcap_adapter_speed_tx(name, &txSpeed);
            if(!BRAWCAP_SUCCESS(status))
                break;
            printf("TX Speed: %d MBit/s\n\n", txSpeed);
#endif
            // We have allocated memory above...
            // So we should not forget to release it.
            //
            // NOTE: Here we have still a memory leak if
            //       loop exits due to a break after allocation.
            //       But it is only in case of a failure and we ignore it
            //       because of simplicity of the example...
            free(friendlyName);
            free(description);
        }
#endif
    }while(0);

    // We do not handle anything unexpected...
    // Instead just print the last unexpected status to console and leave.
    if(!BRAWCAP_SUCCESS(status))
    {
       printf("Unexpected status returned: %d\n", status);
       return -1;
    }
    return 0;
}