/**
 * @file 03_generic_handle_opener.c
 *
 * @brief bRAWcap Example - Demonstrates the usage of the generic bRAWcap functions.
 *
 * It shows how to:
 *   - Open a bRAWcap handle
 *   - Close a previously opened handle
 *   - Check for unexpected status by using @ref brawcap_last_status.
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
#include <stdlib.h> // for malloc & free
#include <stdio.h>  // for printf

int main(int argc, char** argv)
{
    // Set console title
    SetConsoleTitleA("bRAWcap Example - Handle Opener");

    brawcap_handle_t** pHandleArray = 0;
    brawcap_adapter_count_t numberAdapters = 0;
    brawcap_status_t status = BRAWCAP_STATUS_SUCCESS;

    do
    {
#if 1 // Get number of available adapters
      status = brawcap_adapter_list_count(&numberAdapters);
      if(!BRAWCAP_SUCCESS(status))
          break;
#endif

#if 1 // Allocate array for storing opened bRAWcap handles
      pHandleArray = (brawcap_handle_t**) calloc(numberAdapters, sizeof(brawcap_handle_t*));
      memset(pHandleArray, 0, numberAdapters);
#endif

#if 1 // Get name for each available adapter and try to open a bRAWcap handle
      for(int index = 0; index < numberAdapters; ++index)
      {
          brawcap_adapter_name_t name = { '\0'};
          status = brawcap_adapter_list_at(index, name);
          if(!BRAWCAP_SUCCESS(status))
              break;

          status = brawcap_open(name, &pHandleArray[index]);
          if(!BRAWCAP_SUCCESS(status))
              break;

          printf("%02d. bRAWcap handle (%p) opened to %s.\n", index + 1, pHandleArray[index], name);
      }
 #endif
    }while(0);

    printf("\n");

    // Close all opened handles
    for(int index = numberAdapters - 1; index >= 0; --index)
    {
        if (pHandleArray[index])
        {
            status = brawcap_close(pHandleArray[index]);
            printf("%02d. bRAWcap handle (%p) closed.\n", index + 1, pHandleArray[index]);
        }
    }

    // Free alloacted handle array
    free(pHandleArray);

    // If anything unexpected was returned,
    // request the status again and print it to console.
#if 1
    if(!BRAWCAP_SUCCESS(status))
    {
        printf("Unexpected status occurred: %d", brawcap_last_status());
        return -1;
    }
#endif

    return 0;
}