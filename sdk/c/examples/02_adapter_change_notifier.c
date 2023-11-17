/**
 * @file 02_adapter_change_notifier.c
 *
 * @brief bRAWcap Example - Demonstrates the usage of the adapter handling module.
 *
 * It shows how to:
 *   - Receive adapter change notifications
 *   - Process change notifications
 *
 * This example makes use of C only.
 *
 * @attention This example requires a valid bRAWcap license.
 * Receiving adapter change notifications and reading their properties is not part of the demo.
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

// Check command line parameters if a custom execution time is specified
void ParseArgs(int argc, char** argv, int* exec_time)
{
    if(argc > 1)
    {
        if(!memcmp(argv[1], "-t", 2))
        {
            char* pEnd = 0;
            *exec_time = strtol(argv[2], &pEnd,0);
        }
    }
}

void change_notification_callback(const brawcap_adapter_name_t name, const brawcap_adapter_notify_reason_t reason,
                                  const UINT32 properties, void* pUser)
{
    // We have entered a user context which points to a notification counter.
    // To use it we should first of all check if it is valid and if so cast it to the correct type.
    if(pUser)
    {
        brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;
        int* pNotifyCounter = (int*) pUser;
        ++(*pNotifyCounter);

#if 1   // We always try to print the friendly name too (for readability...)
        // Therefore we must retrieve it from the adapter name.
        // If an adapter is not available anymore we can not resolve it´s friendly name.
        // In this case it will be "null" instead.
        char* friendlyName = 0;
        UINT32 friendlyNameLength = 0;
        if(brawcap_adapter_friendly_name(name, friendlyName, &friendlyNameLength) == BRAWCAP_STATUS_ERROR_OVERRUN)
        {
            friendlyName = malloc(friendlyNameLength);
            status = brawcap_adapter_friendly_name(name, friendlyName, &friendlyNameLength);
            if(status != BRAWCAP_STATUS_SUCCESS)
            {
                printf("Could not resolve adapter friendly name (unexpected status: %d)\n", status);
                free(friendlyName);
                return;
            }
        }
#endif
        printf("\n%03d. Notification\n", *pNotifyCounter);
#if 1   // Now handle the different notification reasons.
        switch (reason)
        {
#if 1   // The given reason is unknown
        case BRAWCAP_ADAPTER_NOTIFY_REASON_UNKNOWN:
        default:
            printf("Unknown notification reason.");
            break;
#endif
#if 1   // The specified adapter was added
        case BRAWCAP_ADAPTER_NOTIFY_REASON_ADD:
            printf("%s (%s) added\n", friendlyName, name);
            break;
#endif
#if 1   // The specified adapter was removed
        case BRAWCAP_ADAPTER_NOTIFY_REASON_REMOVE:
            printf("%s (%s) removed\n", friendlyName, name);
            break;
#endif
#if 1   // At least one property of the specified adapter has changed.
        case BRAWCAP_ADAPTER_NOTIFY_REASON_PROPERTY_CHANGE:
#if 1       // Connection state has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_CONNECTION_STATE)
            {
                brawcap_adapter_connection_state_t connectionState = BRAWCAP_ADAPTER_CONNECTION_UNKNOWN;
                brawcap_adapter_connection(name, &connectionState);
                printf("%s (%s) new connection state: ", friendlyName, name);
                switch (connectionState)
                {
                case BRAWCAP_ADAPTER_CONNECTION_UNKNOWN: printf("%s\n", "Unknown"); break;
                case BRAWCAP_ADAPTER_CONNECTION_DOWN:    printf("%s\n", "Down");    break;
                case BRAWCAP_ADAPTER_CONNECTION_UP:      printf("%s\n", "Up");      break;
                }
            }
#endif
#if 1       // Friendly name has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_FRIENDLY_NAME)
                printf("%s new friendly name: %s\n", name, friendlyName);
#endif
#if 1       // IPv4 has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_IPV4)
            {
                brawcap_adapter_ipv4_t ipv4 = {0};
                brawcap_adapter_ipv4(name, &ipv4);
                printf("%s (%s) new IPv4: %u.%u.%u.%u\n", friendlyName, name,
                       ipv4.bytes[0], ipv4.bytes[1], ipv4.bytes[2], ipv4.bytes[3]);
            }
#endif
#if 1       // IPv6 has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_IPV6)
            {
                brawcap_adapter_ipv6_t ipv6 = {0};
                brawcap_adapter_ipv6(name, &ipv6);
                printf("%s (%s) new IPv6: %x:%x:%x:%x:%x:%x:%x:%x\n", friendlyName, name,
                       ipv6.words[0], ipv6.words[1], ipv6.words[2], ipv6.words[3],
                       ipv6.words[4], ipv6.words[5], ipv6.words[6], ipv6.words[7]);
            }
#endif
#if 1       // Max transmission unit has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_MTU)
            {
                brawcap_adapter_mtu_t mtu = 0;
                brawcap_adapter_mtu(name, &mtu);
                printf("%s (%s) new max transmission unit: %llu\n", friendlyName, name, mtu);
            }
#endif
#if 1       // Operation state has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_OPERATION_STATE)
            {
                brawcap_adapter_operation_state_t operationState = BRAWCAP_ADAPTER_OPERATION_UNKNOWN;
                brawcap_adapter_operation(name, &operationState);
                printf("%s (%s) new operation state: ", friendlyName, name);
                switch (operationState)
                {
                case BRAWCAP_ADAPTER_OPERATION_UNKNOWN: printf("%s\n", "Unknown"); break;
                case BRAWCAP_ADAPTER_OPERATION_DOWN:    printf("%s\n", "Down");    break;
                case BRAWCAP_ADAPTER_OPERATION_TESTING: printf("%s\n", "Testing"); break;
                case BRAWCAP_ADAPTER_OPERATION_UP:      printf("%s\n", "Up");      break;
                }
            }
#endif
#if 1       // Receive speed has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_RX_SPEED)
            {
                brawcap_adapter_speed_t speed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
                brawcap_adapter_speed_rx(name, &speed);
                printf("%s (%s) new receive (uplink) speed: %u MBit/s\n", friendlyName, name, speed);
            }
#endif
#if 1       // Transmit speed has changed
            if(properties & BRAWCAP_ADAPTER_PROPERTY_TX_SPEED)
            {
                brawcap_adapter_speed_t speed = BRAWCAP_ADAPTER_SPEED_UNKNOWN;
                brawcap_adapter_speed_tx(name, &speed);
                printf("%s (%s) new transmit (uplink) speed: %u MBit/s\n", friendlyName, name, speed);
            }
#endif
            break;
#endif
        }
        // Don´t forget to free the allocated memory...
        free(friendlyName);
    }
#endif
}

// Use command line parameter -t to specify a custom execution time.
// If it is not given the execution time is per default 5 minutes.
int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Adapter Change Notifier");

    int exec_time = 300; // 5 minutes
    int runtime_sec = 0;
    ParseArgs(argc, argv, &exec_time);

    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // This variable will be entered during registration, to make it available from inside the callback.
    // ATTENTION: You are responsible for checking the pointer inside your callback, before accessing it.
    int notifyCounter = 0;

    // Register our process for bRAWcap adapter change notifications
    brawcap_adapter_notify_callback_t callback = change_notification_callback;
    status = brawcap_adapter_notify_register(callback, &notifyCounter);
    if(!BRAWCAP_SUCCESS(status))
    {
        printf("Registration for adapter change notification failed (Status: %d). Will stop now...", status);
        return -1;
    }

    // Our main thread does nothing than waiting until execution time is elapsed.
    // Everything else is done by the thread which is calling our registered callback.
    do
    {
        Sleep(1000);
    } while (++runtime_sec < exec_time);

    // Unregister bRAWcap adapter change notifications before exiting.
    // NOTE: bRAWcap would automatically unregister on unloading.
    //       Therefore unregistering on exiting isn´t required...
    status = brawcap_adapter_notify_unregister();
    if(!BRAWCAP_SUCCESS(status))
    {
        printf("Unregistration for adapter change noTification failed (Status: %d). Will stop now...", status);
        return -1;
    }

    return 0;
}
