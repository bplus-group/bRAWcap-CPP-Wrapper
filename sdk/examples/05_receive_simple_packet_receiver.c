/**
 * @file 05_receive_simple_packet_receiver.c
 *
 * @brief bRAWcap Example - Demonstrates how to use bRAWcap to receive separated packets.
 *
 * It shows how to:
 *   - Open a handle to the first available adapter
 *   - Create a bRAWcap packet object
 *   - Receive single packets from the adapter
 *   - Access received packet data.
 *
 * This example makes use of C only.
 *
 * @version 1.1
 *
 * @date 2023-03-30
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
#include <stdio.h> // for printf

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Simple Packet Receiver");

    // Some generic helper locals...
    int retVal = 0;
    unsigned char demoModeLogged = 0;
    unsigned long long packetCounter = 0;

    // Here we will store the status of any bRAWcap function
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // Here we store the number of available bRAWcap adapters
    brawcap_adapter_count_t numberAdapters = 0;
    // This will contain our bRAWcap handle to the adapter we want to receive from
    brawcap_handle_t* pHandle = 0;
    // Our local packet for storing received data
    brawcap_packet_t* pPacket = 0;

    do
    {
        // First check if we have any bRAWcap adapters available.
        // --> This function can only fail if we did something wrong.
        //     But we are sure that we did everything right...
        //     therefore we do not check it�s status.
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

        // Open a handle to the adapter by using the retrieved name.
        if (!BRAWCAP_SUCCESS(brawcap_open(name, &pHandle)))
        {
            printf("[ERROR] Unexpected status while opening handle: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Create a packet object
        // For simplicity and to make sure that we get all packets,
        // we set the payload size to the max supported value.
        if(!BRAWCAP_SUCCESS(brawcap_packet_create(&pPacket, BRAWCAP_PACKET_SIZE_MAX)))
        {
            printf("[ERROR] Unexpected status while creating packet: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Some local variables to buffer the packet data, later on.
        brawcap_packet_size_t captureLength = 0;
        brawcap_packet_size_t lengthOnWire = 0;
        brawcap_timestamp_t* pTimestamp = 0;
        UINT64 timestamp_sec = 0;
        UINT32 timestamp_ns = 0;
        const char* pPayload = 0;

        // Our receive loop, we stay in here until we should stop receiving.
        while(1)
        {
            // Now lets see if we have received a packet...
            status = brawcap_rx_packet(pHandle, pPacket);

            // Seems like we had success and got some packet data to look at. :-)
            if(BRAWCAP_SUCCESS(status))
            {
                // Check if we made it out of the demo limitation... :-D
                // ... notify the user and go ahead with receiving.
                if (demoModeLogged)
                {
                    printf("[NOTICE] DEMO MODE: Limitation period elapsed, receiving is available again.\n");
                    demoModeLogged = 0;
                }

                // --> The bRAWcap functions below can only fail if we did something wrong.
                //     But we are sure that we did everything right...
                //     therefore we do not check their status.
                brawcap_packet_length_on_wire_get(pPacket, &lengthOnWire);
                brawcap_packet_payload_get(pPacket, &pPayload, &captureLength);
                brawcap_packet_timestamp_get(pPacket, &pTimestamp);
                brawcap_timestamp_value_ns_get(pTimestamp, &timestamp_sec, &timestamp_ns);

                // Show the user what we received
                printf("%llu. Packet:\n", ++packetCounter);
                printf(" - Capture Length: %04u Bytes\n", captureLength);
                printf(" - Length On Wire: %04u Bytes\n", lengthOnWire);
                printf(" - Timestamp: %llu %09u ns\n", timestamp_sec, timestamp_ns);
                printf(" - Payload: ");
                // Do some nice formatting stuff, to improve readability...
                UINT32 byteCounter = 1;
                for (const char* pByte = pPayload; byteCounter <= captureLength; ++byteCounter)
                {
                    printf("%02X ", (unsigned char)*pByte++);
                    if (!(byteCounter % 16)) printf("\n            ");
                    else if(!(byteCounter % 8)) printf("   ");
                }
                printf("\n\n");
            }
            // Hm... we got a very silent connection...
            // ... lets try again...
            else if (status == BRAWCAP_STATUS_INFO_NO_DATA)
                continue;
            // Sad but we do not have a bRAWcap license for this feature... yet. :-(
            // Notify our user about this sad information and
            // wait some time to check if we are allowed to receive again...
            else if (status == BRAWCAP_STATUS_WARNING_DEMO_MODE)
            {
                if (!demoModeLogged)
                {
                    demoModeLogged = 1;
                    printf("[WARNING] DEMO MODE: Receiving not available.\n");
                }
                Sleep(1000);
            }
            // Ok... Something strange happened here... :-O
            // Better stop now if we do not know how to handle it.
            else
            {
                printf("[ERROR] Unexpected status while receiving packet: %d", brawcap_last_status());
                retVal = -1;
                break;
            }
        }
    }while(0);

    // We do not want to produce any memory leaks...
    // ... so always cleanup what we have created
    // --> The functions here can only fail if we did something wrong.
    //     But we are sure that we did everything right...
    //     therefore we do not check their status.
    if (pPacket)
        brawcap_packet_free(pPacket);
    if (pHandle)
        brawcap_close(pHandle);

    return retVal;
}