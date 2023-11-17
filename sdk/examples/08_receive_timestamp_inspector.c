/**
 * @file 08_receive_timestamp_inspector.c
 *
 * @brief bRAWcap Example - Demonstrates how to use bRAWcap to use different receive timestamp modes.
 *
 * It shows how to:
 *   - Open a handle to the first available adapter
 *   - Configure the receive timestamp mode
 *   - Create a bRAWcap packet object
 *   - Receive single packets from the adapter
 *   - Access received packet timestamp.
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
#include <time.h> // for converting to readable time (gmtime)

/* Check command line parameters if a custom timestamp mode or accuracy is specified
 * The timestamp mode can be configured with the "-m" parameter.
 * Possible values are:
 * - No: Create no timestamp
 * - DriverSysLow: Timestamp by driver from system time (low precision)
 * - DriverSysHigh: Timestamp by driver from system time (high precision)
 * - DriverSw: Timestamp by driver from software (QPC)
 * - AdapterSys: Timestamp by adapter driver from system time
 * - AdapterSw: Timestamp by adapter from software (QPC)
 * - AdapterHw: Timestamp by adapter from hardware clock
 *
 * The timestamp accuracy can be configured with the "-r" parameter.
 * Possible values are:
 * - ns: Nanoseconds
 * - us: Microseconds
 * - ms: Miliseconds
 */
void ParseArgs(int argc, char** argv, brawcap_timestamp_mode_t* pMode, int* pAccuracy)
{
    for(int index = 0; index < argc; ++index)
    {
        if(!memcmp(argv[index], "-m", 2))
        {
            if(!strcmp(argv[index + 1], "No"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP;
            else if(!strcmp(argv[index + 1], "DriverSysLow"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC;
            else if(!strcmp(argv[index + 1], "DriverSysHigh"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_SYSTEM_HIGHPREC;
            else if(!strcmp(argv[index + 1], "DriverSw"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_SOFTWARE;
            else if(!strcmp(argv[index + 1], "AdapterSys"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYSTEM;
            else if(!strcmp(argv[index + 1], "AdapterSw"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_ADAPTER_SOFTWARE;
            else if(!strcmp(argv[index + 1], "AdapterHw"))
              *pMode = BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE;
            else
              printf("[WARNING] Unknown timestamp mode - will use default mode.\n");
        }
        else if(!memcmp(argv[index], "-r", 2))
        {
            if(!strcmp(argv[index + 1], "ns"))
              *pAccuracy = 1;
            else if(!strcmp(argv[index + 1], "us"))
              *pAccuracy = 2;
            else if(!strcmp(argv[index + 1], "ms"))
              *pAccuracy = 3;
            else
                printf("[WARNING] Unknown accuracy - will use default accuracy.\n");
        }
    }
}

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Receive Timestamp Inspector");

    // Some generic helper locals...
    int retVal = 0;
    unsigned char demoModeLogged = 0;
    unsigned long long packetCounter = 0;
    int accuracy = 1;

    // Set the default timestamp mode to system low precision
    brawcap_timestamp_mode_t timestampMode = BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC;

    // Here we will store the status of any bRAWcap function
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    // Here we store the number of available bRAWcap adapters
    brawcap_adapter_count_t numberAdapters = 0;
    // This will contain our bRAWcap handle to the adapter we want to receive from
    brawcap_handle_t* pHandle = 0;
    // Our local packet for storing received data
    brawcap_packet_t* pPacket = 0;

    // Parse command line argument for selected timestamp mode
    ParseArgs(argc, argv, &timestampMode, &accuracy);

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
            printf("[WARNING] No bRAWcap adapter available... Will stop now.\n");
            break;
        }

        // Get the name of the first adapter.
        // To not overload the example with complex stuff,
        // we always go for the first available adapter.
        brawcap_adapter_name_t name = { '\0' };
        if (!BRAWCAP_SUCCESS(brawcap_adapter_list_at(0, name)))
        {
            printf("[ERROR] Unexpected status while retrieving adapter name: %d\n", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Open a handle to the adapter by using the retrieved name.
        if (!BRAWCAP_SUCCESS(brawcap_open(name, &pHandle)))
        {
            printf("[ERROR] Unexpected status while opening handle: %d\n", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Lets see what timestamp modes we have on the specified adapter
        brawcap_timestamp_capabilities_t timestampCapabilities;
        if(!BRAWCAP_SUCCESS(brawcap_rx_timestamp_capabilities(pHandle, &timestampCapabilities)))
        {
            printf("[ERROR] Unexpected status while retrieving timestamp capabilities: %d\n", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Print available adapter timestamp capabilities to console before we start reading packets
        // We only check for adapter modes explicitly because all other modes are always available.
        printf("ADAPTER TIMESTAMP CAPABILITIES: \n");
        printf("  - System:   %s\n", timestampCapabilities & BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYSTEM   ? "yes" : "no");
        printf("  - Software: %s\n", timestampCapabilities & BRAWCAP_TIMESTAMP_MODE_ADAPTER_SOFTWARE ? "yes" : "no");
        printf("  - Hardware: %s\n", timestampCapabilities & BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE ? "yes" : "no");
        printf("\n");

        // Check if configured timestamp mode is available... if not exit now...
        if(timestampMode && !(timestampMode & timestampCapabilities))
        {
          printf("[ERROR] Specified timestamp mode is not available on the adapter... will exit now.\n");
          retVal = -1;
          break;
        }

        // Apply the configured timestamp mode to the previously opened handle.
        if(!BRAWCAP_SUCCESS(brawcap_rx_timestamp_mode_set(pHandle, timestampMode)))
        {
            printf("[ERROR] Unexpected status while applying configured timestamp mode: %d\n", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Create a packet object
        // For simplicity and to make sure that we get all packets,
        // we set the payload size to the max supported value.
        if(!BRAWCAP_SUCCESS(brawcap_packet_create(&pPacket, BRAWCAP_PACKET_SIZE_MAX)))
        {
            printf("[ERROR] Unexpected status while creating packet: %d\n", brawcap_last_status());
            retVal = -1;
            break;
        }

        // Some local variables to buffer the timestamp info, later on.
        brawcap_timestamp_t* pTimestamp = 0;
        brawcap_timestamp_mode_t source = BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP;
        brawcap_timestamp_resolution_ns_t resolution = 0;
        UINT64 sec = 0;
        UINT32 ns = 0;

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
                brawcap_packet_timestamp_get(pPacket, &pTimestamp);
                brawcap_timestamp_mode_get(pTimestamp, &source);
                brawcap_timestamp_resolution_ns_get(pTimestamp, &resolution);
                switch(accuracy)
                {
                  case 1:
                    brawcap_timestamp_value_ns_get(pTimestamp, &sec, &ns);
                    break;
                  case 2:
                    brawcap_timestamp_value_us_get(pTimestamp, &sec, &ns);
                    ns *= BRAWCAP_TIMESTAMP_NS_PER_US;
                    break;
                  case 3:
                    brawcap_timestamp_value_ms_get(pTimestamp, &sec, &ns);
                    ns *= BRAWCAP_TIMESTAMP_NS_PER_MS;
                    break;
                }
                struct tm *tmp = gmtime((time_t*)&sec);

                // Print the packet timestamp info to console
                printf("%llu. Received Packet Timestamp:\n", ++packetCounter);
                printf(" - Source/Mode: ");
                switch(source)
                {
                  case BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP: printf("No Timestamp\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC: printf("Driver System Low Precision\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_SYSTEM_HIGHPREC: printf("Driver System High Precision\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_SOFTWARE: printf("Driver Software (QPC)\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYSTEM: printf("Adapter System\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_ADAPTER_SOFTWARE: printf("Adapter Software (QPC)\n"); break;
                  case BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE: printf("Adapter Hardware\n"); break;
                }
                printf(" - Resolution: ");
                resolution != BRAWCAP_TIMESTAMP_RESOLUTION_UNKNOWN
                  ? printf("%09u ns\n", resolution)
                  : printf(" UNKNOWN\n");
                printf(" - Timestamp: %llu %09u ns (<=> %02u.%02u.%04u %02u:%02u:%02u.%09u)\n\n", sec, ns,
                        tmp->tm_mday, tmp->tm_mon+1, tmp->tm_year+1900,
                        tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ns);
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
                printf("[ERROR] Unexpected status while receiving packet: %d\n", brawcap_last_status());
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