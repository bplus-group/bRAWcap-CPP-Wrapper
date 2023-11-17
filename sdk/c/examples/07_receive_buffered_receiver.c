/**
 * @file 07_receive_buffered_receiver.c
 *
 * @brief bRAWcap Example - Demonstrates how to use bRAWcap to receive packets with packet buffers.
 *
 * It shows how to:
 *   - Open a handle to the first available adapter
 *   - Create a bRAWcap packet buffer objects
 *   - Start receiving on a handle
 *   - Using the receive callback
 *   - Iterating through a packet buffer
 *   - Accessing a buffered packet
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
#include <stdlib.h> // for strtol

typedef struct receive_counters
{
  unsigned long long packets;
  unsigned long long bytes;
}receive_counters_t;

typedef struct example_context
{
  unsigned char demoLogged;
  receive_counters_t counters;
}example_context_t;


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

void receiveCallback(brawcap_handle_t* const pHandle, const brawcap_status_t status, brawcap_buffer_t* const pBuffer,
                     void* pUser)
{
    example_context_t* pContext = (example_context_t*) pUser;
    if(status == BRAWCAP_STATUS_WARNING_DEMO_MODE)
    {
        if(!pContext->demoLogged)
        {
            printf("[WARNING] DEMO MODE: Receiving not available.\n");
            pContext->demoLogged = 1;
        }
    }
    else if(status == BRAWCAP_STATUS_SUCCESS)
    {
        if(pContext->demoLogged)
        {
          printf("[NOTICE] DEMO MODE: Limitation period elapsed, receiving is available again.\n");
          pContext->demoLogged = 0;
        }
        brawcap_buffer_iterator_t* pIterator = 0;
        brawcap_buffer_iterator_create(&pIterator, pBuffer, 0);

        brawcap_packet_t* pPacket = 0;
        brawcap_packet_size_t packetSize = 0;

        do
        {
          pPacket = brawcap_buffer_iterator_eval(pIterator);
          if(!pPacket)
            break;

          brawcap_packet_payload_size_get(pPacket, &packetSize);
          pContext->counters.bytes += packetSize;
          ++pContext->counters.packets;

          brawcap_buffer_iterator_next(pIterator);
        } while (1);

        brawcap_buffer_iterator_free(pIterator);

        printf("[NOTICE] Received new packets [Total Packets: %llu / Total Bytes: %llu].\n",
               pContext->counters.packets, pContext->counters.bytes);
    }
}

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Buffered Receiver");

    int exec_time = 300; // 5 minutes
    int runtime_sec = 0;
    ParseArgs(argc, argv, &exec_time);

    // Some generic helper locals...
    int retVal = 0;
    example_context_t context = {0};

    // Here we will store the status of any bRAWcap function
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // Here we store the number of available bRAWcap adapters
    brawcap_adapter_count_t numberAdapters = 0;
    // This will contain our bRAWcap handle to the adapter we want to receive from
    brawcap_handle_t* pHandle = 0;
    // Our local packet buffers for storing received data
    brawcap_buffer_t* pPacketBuffer1 = 0;
    brawcap_buffer_t* pPacketBuffer2 = 0;

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

        // Create the packet buffers
        // For simplicity and to make sure that we get all packets,
        // we set the packet payload size to the max supported value.
        if(!BRAWCAP_SUCCESS(brawcap_buffer_create(&pPacketBuffer1, BRAWCAP_PACKET_SIZE_MAX, 512)))
        {
            printf("[ERROR] Unexpected status while creating packet buffer: %d", brawcap_last_status());
            retVal = -1;
            break;
        }
        if(!BRAWCAP_SUCCESS(brawcap_buffer_create(&pPacketBuffer2, BRAWCAP_PACKET_SIZE_MAX, 512)))
        {
            printf("[ERROR] Unexpected status while creating packet buffer: %d", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Attach the created packet buffers to the handle for reception
        brawcap_rx_buffer_attach(pHandle, pPacketBuffer1);
        brawcap_rx_buffer_attach(pHandle, pPacketBuffer2);

        // Now start the the bRAWcap reception on the handle
        brawcap_rx_start(pHandle, receiveCallback, &context, 0);

        // After starting the reception our main thread has finished it´s work.
        // Everything else will be done from inside the receive callback until we should stop receiving.
        do
        {
            Sleep(1000);
        } while (++runtime_sec < exec_time);

        brawcap_rx_stop(pHandle);
        brawcap_rx_buffer_detach(pHandle, pPacketBuffer2);
        brawcap_rx_buffer_detach(pHandle, pPacketBuffer1);
    }while(0);

    // We do not want to produce any memory leaks...
    // ... so always cleanup what we have created
    // --> The functions here can only fail if we did something wrong.
    //     But we are sure that we did everything right...
    //     therefore we do not check their status.
    if (pPacketBuffer1)
        brawcap_buffer_free(pPacketBuffer1);
    if(pPacketBuffer2)
        brawcap_buffer_free(pPacketBuffer2);
    if (pHandle)
        brawcap_close(pHandle);

    return retVal;
}