/**
 * @file 06_filter_firewall.c
 *
 * @brief bRAWcap Example - Demonstrates how to throw away specific received packets.
 *
 * It shows how to:
 *   - Open a handle to the first available adapter
 *   - Create and apply a receive filter
 *   - Read the receive statistics
 *
 * This example makes use of C only.
 *
 * @version 1.0
 *
 * @date 2023-03-28
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
#include <stdlib.h> // for strtol
#include <stdio.h> // for printf
#include <time.h> // for printing time info

// Check command line parameters if a custom execution time is specified
// To configure a custom execution time use "-t numberOfSeconds"
#if 1
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
#endif

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Firewall for SSDP Packets");

    // Some generic helper locals...
    int retVal = 0;
    int exec_time = 300; // 5 minutes
    int runtime_sec = 0;

    // Parse command line argument, to check for user specified execution time
    ParseArgs(argc, argv, &exec_time);

    // Here we will store the status of any bRAWcap function
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // Here we store the number of available bRAWcap adapters
    brawcap_adapter_count_t numberAdapters = 0;
    // This will contain our bRAWcap handle to the adapter we want to receive from
    brawcap_handle_t* pHandle = 0;
    // Our local filter to configure which packets should be dropped
    brawcap_filter_t* pFilter = 0;

    do
    {
        // First check if we have any bRAWcap adapters available.
        // --> This function can only fail if we did something wrong.
        //     But we are sure that we did everything right...
        //     therefore we do not check it´s status.
        brawcap_adapter_list_count(&numberAdapters);

        // No adapters... nothing do receive from... lets exit ...
        if (!numberAdapters)
        {
            printf("[WARNING] No bRAWcap adapter available... Will stop now.");
            break;
        }

        // Get the name of the first adapter.
        // To not overload the example with complex stuff,
        // we always go for the first available adapter.
        brawcap_adapter_name_t name = { '\0' };
        if (!BRAWCAP_SUCCESS(brawcap_adapter_list_at(0, name)))
        {
            printf("[ERROR] Unexpected status while retrieving adapter name: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Create a filter object where to store our filter settings
        if(!BRAWCAP_SUCCESS(brawcap_filter_create(&pFilter, BRAWCAP_FILTER_TYPE_BYTE_MASK)))
        {
            printf("[ERROR] Unexpected status while creating the filter: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Local filter attributes
        brawcap_filter_byte_length_t filterOffset = 0;
        brawcap_filter_byte_length_t filterLength = 0;
        brawcap_filter_mask_array_t filterMask = { 0 };
        brawcap_filter_ignore_bits_array_t filterBitMask = { 0 };

        /* Lets specify our (byte) filter.
         *
         * In this example we want to filter out all SSDP (Simple Service Discover Protocol) packets.
         *
         * -- If you need those packets, change the filter to another packet type. --
         *
         * SSDP packets can be identified by their 1900 port and
         * typically they are transmitted via UDP - so we filter for UDP destination port 1900.
         *
         * To make the filter more precise we also check if the transport protocol is UDP.
         * UDP is identified by the IP protocol type 17 (we always assume IPv4 for this example).
         */

        // Lets start our filter at the IP protocol type which is byte 23 (for IPv4):
        filterOffset = 23;

        // Because we also need to check for UDP destination port at byte 36-37 (also for IPv4)
        // our filter length must be 37 - 23 = 14 bytes
        filterLength = 14;

        // Now specify the UDP protocol type and the UDP destination port in the filter mask
        // IPv4 Protocol type = UDP (17 <=> 0x11)
        filterMask[0] = 0x11;
        // UDP destination port = SSDP (1900 <=> 0x076C)
        filterMask[13] = 0x07;
        filterMask[14] = 0x6C;

        // Ignore all bits of the bytes between IPv4 protocol type and UDP destination port.
        // Therefore we must set each bit in this range to 1.
        memset(&filterBitMask[1], 0xFF, 12);

        // Now set our filter attributes to the previous created filter.
        brawcap_filter_mask_set(pFilter, filterOffset, filterLength, filterMask, filterBitMask);

        // Because we want to discard matched packets - do not forward to network stack -
        // we set the filter indication to false
        brawcap_filter_indicate_set(pFilter, 0);

        // Set the filter to active
        brawcap_filter_activate(pFilter);

        // Open a handle to the adapter by using the retrieved name.
        if (!BRAWCAP_SUCCESS(brawcap_open(name, &pHandle)))
        {
            printf("[ERROR] Unexpected status while opening handle: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Now lets apply the filter to our opened handle
        if(!BRAWCAP_SUCCESS(brawcap_rx_filter_set(pHandle, pFilter)))
        {
            printf("[ERROR] Unexpected status while applying the filter: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Because we do not want to process the packets somehow and just filter them.
        // we can also reduce the drivers receive queue size for this handle to zero.
        // With this setting the driver doesn´t even try to buffer those packets.
        if(!BRAWCAP_SUCCESS(brawcap_rx_driver_queue_size_set(pHandle, 0)))
        {
            printf("[ERROR] Unexpected status while reducing driver queue size: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Local receive stats for some user feedback
        brawcap_stats_rx_t rxStats;
        rxStats.header.type = BRAWCAP_STATS_TYPE_RX;
        rxStats.header.revision = BRAWCAP_STATS_RX_REVISION_1;
        rxStats.header.size = BRAWCAP_STATS_RX_SIZEOF_REVISION_1;

        // After applying the filter we would have finished here...
        // Because we do not want to receive the filtered packets and only drop them.
        // The bRAWcap driver does everything for us and we just need to keep the handle open.
        do
        {
            // To give the user some feedback request the statistic counters from bRAWcap and
            // print how many packets have been blocked by our "firewall" since applying it.
            if (!BRAWCAP_SUCCESS(brawcap_stats_rx(pHandle, &rxStats)))
                printf("[ERROR] Unexpected status while requesting receive stats: %d", brawcap_last_status());
            else
            {
                time_t curTime = time(0);
                char strTime[30];
                strftime(strTime, 30, "<%F %X>", localtime(&curTime));
                // We could also use the rxStats.handleDroppedPacketsQueue here.
                // Both values should be equal as long as we do not process any of the matched packet
                // by calling a bRAWcap receive function.
                printf("%s Number of blocked packets: %llu\n", strTime, rxStats.handleReceivedPacketsMatched);
            }

            Sleep(1000);
        } while (++runtime_sec < exec_time);
    }while(0);

    // We do not want to produce any memory leaks...
    // ... so always cleanup what we have created
    // --> The functions here can only fail if we did something wrong.
    //     But we are sure that we did everything right...
    //     therefore we do not check their status.
    if (pHandle)
        brawcap_close(pHandle);
    if(pFilter)
        brawcap_filter_free(pFilter);

    return retVal;
}