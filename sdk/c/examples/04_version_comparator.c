/**
 * @file 04_version_comparator.c
 *
 * @brief bRAWcap Example - Demonstrates how to use the bRAWcap version.
 *
 * It shows how to:
 *   - Read library and driver version
 *   - Print readable versions
 *   - Do a version compare
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
#include <stdio.h>  // for printf

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Version Comparator");

    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;
    brawcap_adapter_count_t numberAdapters = 0;
    brawcap_version_t apiVersion = {0};
    brawcap_version_t driverVersion = {0};

    status = brawcap_version_api(&apiVersion);
    if(!BRAWCAP_SUCCESS(status))
        printf("Unexpected status (%d) while reading API(/library) version.\n", status);
    else
        printf("Loaded library/API version is: %u.%u.%u.%u\n", apiVersion.fragments.major, apiVersion.fragments.minor,
               apiVersion.fragments.patch, apiVersion.fragments.build);

    status = brawcap_adapter_list_count(&numberAdapters);
    if(!BRAWCAP_SUCCESS(status))
        printf("Unexpected status (%d) while reading number of available adapters.\n", status);
    else if(!numberAdapters)
        printf("No adapter available to read driver version.\n");
    else
    {
        brawcap_adapter_name_t name = { '\0' };
        status = brawcap_adapter_list_at(0, name);
        if(!BRAWCAP_SUCCESS(status))
            printf("Unexpected status (%d) while reading adapter name.", status);
        else
        {
            brawcap_handle_t* handle = 0;
            status = brawcap_open(name, &handle);
            if(!BRAWCAP_SUCCESS(status))
              printf("Unexpected status (%d) while handle opening.", status);
            else
            {
                status = brawcap_version_driver(handle, &driverVersion);
                if(!BRAWCAP_SUCCESS(status))
                    printf("Unexpected status (%d) while reading driver version.\n", status);
                else
                {
                    printf("Loaded driver version is: %u.%u.%u.%u\n", driverVersion.fragments.major,
                           driverVersion.fragments.minor, driverVersion.fragments.patch, driverVersion.fragments.build);

                  if(driverVersion.complete == apiVersion.complete)
                      printf("Driver and API version MATCH. :-)\n");
                  else
                      printf("Driver and API version does NOT MATCH. :-(\n");
                }
                brawcap_close(handle);
            }
        }
    }
    return 0;
}