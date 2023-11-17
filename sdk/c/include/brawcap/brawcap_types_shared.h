/**
 * @file brawcap_types_shared.h
 *
 * @brief bRAWcap API - Shared types.
 *
 * The file includes all (data)types and defines used for bRAWcap (kernel and user space).
 *
 * @copyright
 * <b> © 2021 - b-plus technologies GmbH. All rights reserved!</b>
 *
 * All rights exclusively reserved for b-plus GmbH, unless expressly otherwise agreed.
 *
 * Redistribution in source or any other form, with or without modification, is not permitted.
 *
 *
 * You may use this code under the according license terms of b-plus.
 * Please contact b-plus at services@b-plus.com to get the appropriate terms and conditions.
 */
#ifndef BRAWCAP_TYPES_SHARED_H
#define BRAWCAP_TYPES_SHARED_H

// Include required Windows headers for datatypes.
#if 1
#if !defined(_WIN64)
  #error Architecture not defined (please define "_WIN64").
#else
  #ifdef WINNT
    #include <sdkddkver.h>
    #include <basetsd.h>
    #include <minwindef.h>
  #else
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
  #endif
#endif
#endif

// Define required BPF structs if the BPF header is not included.
#if 1
#ifndef BRAWCAP_BPF_INCLUDED
  #define _BRAWCAP_DEFINE_BPF_STRUCTS_INTERNAL
  #ifndef _BRAWCAP_EXPORT
    #pragma message("<bRAWcap> Please define 'BRAWCAP_BPF_INCLUDED' if you get any redefinition errors on because of BPF.")
  #endif
#endif
#endif

/************************************************ GENERIC ************************************************************/
#if 1
/**
 * @example{lineno} 03_generic_handle_opener.c
 *
 * This example shows how to open and close bRAWcap handles to each available adapter on the machine.
 * If any unexpected status is returned, it will close all handles and print the unexpected status to console.
 * Therefore @ref brawcap_last_status is used.
 */

/**
 * @defgroup brawcap_generics Generics
 * @brief Contains generic types and functions of bRAWcap.
 *
 * Everything in here is not part of any specific module, instead those types and functions are relevant for many
 * other modules. It contains for example functions to @ref brawcap_open "open/create" and @ref brawcap_close "close"
 * a @ref brawcap_handle_t "bRAWcap handle".
 *
 * List of examples:
 *  1. @ref 03_generic_handle_opener.c "Handle Opener"
 * @{
 */

/**
 * @brief Specifies the maximum supported bRAWcap handles per adapter.
 *
 * @attention If this number of handles is reached (for a specific adapter) each additional @ref brawcap_open "open"
 * will fail.
 */
#define BRAWCAP_HANDLES_PER_ADAPTER_MAX 5

/**
 * @brief The number of packets which can be (temporary) buffered by the bRAWcap driver queue.
 *
 * The total number of packets which can be buffered by the driver queue is limited to:
 *  - receive:  @ref BRAWCAP_RX_DRIVER_QUEUE_SIZE_MAX
 *  - transmit: @ref BRAWCAP_TX_DRIVER_QUEUE_SIZE_MAX
 *
 * @note Each bRAWcap handle has it´s own driver queue. Therefore the total memory consumption by the driver
 * depends on the number of adapters and handles to each adapter, and also on how large the driver queue is specified.
 *
 * @attention Larger driver queues can minimize packet drops due to more resources. Especially in cases where the user
 * application requires longer than expected to pick up (call receive) on the handle. But it has to be said that
 * large queues shouldn´t be required in optimized configurations and that they can consume a lot of memory if they
 * are not configured with care.
 */
typedef UINT32 brawcap_queue_size_t;

/**@}*/
#endif

/************************************************** STATUS ***********************************************************/
#if 1
/**
 * @defgroup brawcap_status Status
 * @brief Contains all status/return codes of bRAWcap.
 *
 * If a bRAWcap function completed successfully and with no additional information it will indicate
 * @ref BRAWCAP_STATUS_SUCCESS.
 *
 * If an error occurs the status code will always be negative.
 *
 * A status which indicates a warning, will always have a positive value (range 1 to 1999).
 *
 * While a status with additional information is always represented by high positive value (range 2000 to 4000).
 *
 * It is also possible to request the last "non successful" status for the current thread again with
 * @ref brawcap_last_status. This function works similar to the one defined by the Windows API (GetLastError()).
 * @{
 */

/**
 * @brief Checks if the returned status indicates a success with no additional info.
 */
#define BRAWCAP_SUCCESS(status) (status == 0)

/**
 * @brief Checks if the returned status is a error.
 */
#define BRAWCAP_ERROR(status) (status < 0)

/**
 * @brief Checks if the returned status is a warning.
 */
#define BRAWCAP_WARNING(status) (status > 0 && status < 2000)

/**
 * @brief Checks if the returned status is a info.
 */
#define BRAWCAP_INFO(status) (status >= 2000 && status < 4000)

/**
 * @brief bRAWcap status/return codes.
 *
 * Each status is defined with a descriptive name, which indicates it´s reason.
 * To check for the status code type use one of the defined macros:
 *  - Status indicates @ref BRAWCAP_SUCCESS "success"
 *  - Status indicates @ref BRAWCAP_ERROR "error"
 *  - Status indicates @ref BRAWCAP_WARNING "warning"
 *  - Status indicates @ref BRAWCAP_INFO "info"
 */
typedef enum _brawcap_status
{
  // SUCCESS (== 0)
    BRAWCAP_STATUS_SUCCESS = 0, /*!< Executed successfully, with no additional information. */

  // ERROR (< 0)
  #if 1
    /**
     * @brief Detected a - not further specified - failure.
     *
     * This shouldn´t happen at all and mostly indicates a issue in OS abstraction.
     * If it happens in a specific situation, please check the system resources (CPU load, memory usage...).
     * If the issue can not be located, feel free to contact our support.
     */
    BRAWCAP_STATUS_ERROR_FAILED = -1,
    /**
     * @brief Function is not available.
     *
     * This may be returned due to several conditions:
     *    - bRAWcap license does not include the requested feature.
     *    - The current machine does not support the feature.
     *    - Higher API registration privilege is required.
     */
    BRAWCAP_STATUS_ERROR_NOT_AVAILABLE = -2,
    /**
     * @brief At least one specified pointer parameter was invalid.
     *
     * This is a caller issue.
     * Please check the provided pointers to solve this issue.
     */
    BRAWCAP_STATUS_ERROR_INVALID_POINTER = -3,
    /**
     * @brief At least one parameter content is invalid.
     *
     * This is a caller issue.
     * Please check the provided parameters and their contents to solve this issue.
     */
    BRAWCAP_STATUS_ERROR_INVALID_PARAM = -4,
    /**
     * @brief At least one parameter is out of bounds.
     *
     * This is a caller issue.
     * Please check if the provided parameters fit to the given parameter limitations.
     */
    BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE = -5,
    /**
     * @brief Given parameter not found.
     *
     * This is in general a caller issue.
     * For example it can occur if the function expects a local adapter name and the
     * provided name did not match any of the currently available adapters.
     */
    BRAWCAP_STATUS_ERROR_NOT_FOUND = -6,
    /**
     * @brief Indicates that operation failed because the provided object is still in use.
     *
     * This may happen if another thread uses the same object or another operation for the object is still outstanding.
     */
    BRAWCAP_STATUS_ERROR_IN_USE = -7,
    /**
     * @brief The packet size is beyond adapter MTU.
     *
     * This status is especially for transmitted packets.
     * It will be applied if the packet could not be transmitted because
     * it´s length is beyond the configured adapter MTU.
     */
    BRAWCAP_STATUS_ERROR_BEYOND_MTU = -8,
    /**
     * @brief Communication with the driver failed.
     *
     * This indicates an error between library and driver communication.
     *
     * To fix such issues, make sure that the loaded library version matches with the driver version.
     *
     * In general it is not allowed to mix up library and driver versions.
     * Applications shall always use the deployed bRAWcap library from the system
     * and not deploy it´s own copy of it.
     *
     * If driver and library version matches and this error still occurs, please contact our support.
     */
    BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED = -9,
    /**
     * @brief bRAWcap is not activated (on a specific adapter).
     *
     * This will be returned if bRAWcap driver is not available (anymore)
     * for the specified bRAWcap handle.
     * For example if the driver was deactivated meanwhile.
     */
    BRAWCAP_STATUS_ERROR_DRIVER_NOT_AVAILABLE = -10,
    /**
     * @brief A buffer overrun was detected.
     *
     * This is the case if:
     *    - The provided buffer length was to short for the content.
     *    - A bRAWcap packet buffer has reached it´s "packet" capacity.
     */
    BRAWCAP_STATUS_ERROR_OVERRUN = -11,
  #endif

  // WARNING (1 to 1999)
  #if 1
    /**
     * @brief The operation was not executed due to demo mode limitations.
     *
     * This should only be returned if there is no valid license for the requested feature.
     * For more information about demo limitation see chapter @ref demo-mode "Demo Mode" in the manual.
     */
    BRAWCAP_STATUS_WARNING_DEMO_MODE = 1,
    /**
     * @brief Will be returned if another operation for the same resource is pending.
     *
     * This could be the case for functions which will initiate a receive/transmit while another
     * receive/transmit is already pending on the specified handle.
     * In general it is not allowed to use receive/transmits in parallel on the same handle.
     * For example if a receive is @ref brawcap_rx_start "started", @ref brawcap_rx_packet cannot be used until the
     * receive is @ref brawcap_rx_stop "stopped". Same applies to transmissions.
     */
    BRAWCAP_STATUS_WARNING_PENDING = 2,
    /**
     * @brief Indicates that a bRAWcap limitation is reached.
     *
     * This could be the case if there are already max supported number of handles to the same adapter
     * and the caller tries to open another one.
     */
    BRAWCAP_STATUS_WARNING_LIMIT_REACHED = 3,
    /**
     * @brief Indicates that a operation was canceled.
     *
     * For example if a packet transmission was initiated and it is somehow canceled.
     * This could be the case due to bRAWcap driver deactivation.
     * If so this status will be applied for each packet which was canceled (not transmitted).
     */
    BRAWCAP_STATUS_WARNING_CANCELED = 4,
    /**
     * @brief Indicates that not all of the specified content/payload was processed.
     *
     * It will be returned for example if not all packets of a bRAWcap packet buffer was transmitted.
     * If so the user should check the status of each packet inside the buffer for further information.
     */
    BRAWCAP_STATUS_WARNING_NOT_ALL_PROCESSED = 5,
    /**
     * @brief Indicates that there is currently no uplink on the given adapter.
     *
     * It will be returned if the user tries to send packets to a adapter which has currently no uplink.
     */
    BRAWCAP_STATUS_WARNING_NO_UPLINK = 6,
  #endif

  // INFO (2000 - 3999)
  #if 1
    /**
     * @brief No operation running.
     *
     * This will be returned if the user tries to stop something which is currently not running.
     * For example when trying to stop receiving while it is not running.
     */
    BRAWCAP_STATUS_INFO_NOT_RUNNING = 2000,
    /**
     * @brief Indicates that the given object is not attached.
     *
     * Will be returned for example if the user tries to detach a packet buffer
     * which is currently not attached to a handle.
     */
    BRAWCAP_STATUS_INFO_NOT_ATTACHED = 2001,
    /**
     * @brief Indicates that the given object is not registered.
     *
     * Will be returned if user tries to unregister from a bRAWcap notification
     * which was not registered before.
     */
    BRAWCAP_STATUS_INFO_NOT_REGISTERED = 2002,
    /**
     * @brief Indicates that the given object was already registered.
     *
     * Will be returned if user tries to register for something again
     * which is already registered.
     */
    BRAWCAP_STATUS_INFO_ALREADY_REGISTERED = 2003,
    /**
     * @brief Indicates that a function returns without any data.
     *
     * This could be the case if a receive function returns without any received packet.
     * For example if the configured receive timeout is reached.
     */
    BRAWCAP_STATUS_INFO_NO_DATA = 2004,
    /**
     * @brief Indicates that a function returned because of a timeout.
     *
     * This could be the case if a function has not finished it´s processing until a configured timeout is reached.
     */
    BRAWCAP_STATUS_INFO_TIMEOUT = 2005,
  #endif
} brawcap_status_t;

/**@}*/
#endif

/************************************************* VERSION ***********************************************************/
#if 1
/**
 * @example{lineno} 04_version_comparator.c
 *
 * This example shows how to read library (/API) and driver version of bRAWcap.
 * After reading the versions it will check if both versions match.
 */

/**
 * @defgroup brawcap_version Version
 * @brief Types and functions for operating with bRAWcap version(s).
 *
 * Here our bRAWcap version structure is defined. This structure allows easy access to each part of the version
 * and due to it´s union the complete value can be used for easy version compares.
 *
 * There are currently two functions to retrieve a version.
 * One for the @ref brawcap_version_api "version of the loaded library" and another one for the
 * @ref brawcap_version_driver "version of the loaded bRAWcap driver" on the machine.
 *
 * @attention In general both versions shall be equal.
 * This means it is not allowed to deploy copys of the bRAWcap library with any application.
 * Instead always the bRAWcap library from system path shall be used (which will be deployed by the bRAWcap setup).
 * This avoids communication issues between the driver and the library.
 * If there are any issues with driver communication, please check that the driver and library version matches.
 *
 * List of examples:
 *  1. @ref 04_version_comparator.c "Version Comparator"
 * @{
 */

/**
 * @brief Struct containing the separated version parts.
 */
typedef struct _brawcap_version_fragments
{
  /**
   * @brief Build version.
   *
   * Different for each build.
   * Releases will always have different version by the upper three values.
   * Versions with different build versions only should not appear.
   * API and ABI always stays compatible between different build versions.
   */
  UINT16 build;
  /**
   * @brief Patch version.
   *
   * Different for each bugfix release.
   * Will be increased if the release contains bugfixes only.
   * API and ABI always stays compatible between different patch versions.
   *
   */
  UINT16 patch;
  /**
   * @brief Minor version.
   *
   * Different for each minor release.
   * Will be increased if the release contains new features (and maybe some bugfixes).
   * API always stays compatible between different minor versions.
   */
  UINT16 minor;
  /**
   * @brief Major version.
   *
   * Different for each major release.
   * Will be increased if for example some features have removed or changed in a way which would
   * break the API compatibility.
   * API compatibility between different major versions is not guaranteed.
   */
  UINT16 major;
}brawcap_version_fragments_t;

/**
 * @brief bRAWcap version.
 *
 * The version is implemented as a union. The complete member allows easy compare between versions.
 * While the fragments allow easy access to each version part (major, minor, patch, build).
 */
typedef union _brawcap_version
{
  brawcap_version_fragments_t fragments; /*!< Separated parts of the version.*/
  UINT64 complete; /*!< Complete version as integer. Use this for comparing versions. */
} brawcap_version_t;

/**@}*/
#endif

/*********************************************** TIMESTAMP ***********************************************************/
#if 1
/**
 * @defgroup brawcap_timestamp Timestamp
 * @brief Types and functions for operating with bRAWcap timestamps.
 *
 * At the moment it is not possible to create a standalone @ref brawcap_timestamp_t "timestamp object".
 * Instead a timestamp is always part of a @ref brawcap_packet "packet".
 * To get the timestamp object use @ref brawcap_packet_timestamp_get on a specific packet.
 * This object can be used than for all functions in this module.
 *
 * @attention The user application shall never store a timestamp, instead it should always request it from
 * a packet and only use it as long as the packet itself is valid.
 *
 * List of examples:
 *  1. @ref 08_receive_timestamp_inspector.c "Receive Timestamp Inspector"
 *  2. @ref 05_receive_simple_packet_receiver.c "Simple Packet Receiver"
 * @{
 */

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC "system low precision" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_SYSL_AVAILABLE(_capabilities) (_capabilities & 0x01)

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_SYSTEM_HIGHPREC "system high precision" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_SYSH_AVAILABLE(_capabilities) (_capabilities & 0x02)

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_SOFTWARE "software" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_SW_AVAILABLE(_capabilities) (_capabilities & 0x04)

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYSTEM "adapter system" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYS_AVAILABLE(_capabilities) (_capabilities & 0x08)

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_ADAPTER_SOFTWARE "adapter software" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_ADAPTER_SW_AVAILABLE(_capabilities) (_capabilities & 0x10)

/**
 * @brief Checks if timestamp mode @ref BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE "adapter hardware" is supported.
 */
#define BRAWCAP_TIMESTAMP_MODE_ADAPTER_HW_AVAILABLE(_capabilities) (_capabilities & 0x20)

/**
 * @brief If a timestamp resolution is set to this value,
 * it´s resolution is unknown - could not be resolved.
 *
 * For example this can be the case for adapter hardware timestamps,
 * if the adapter does not report it´s clock frequency.
 */
#define BRAWCAP_TIMESTAMP_RESOLUTION_UNKNOWN 0

/**
 * @brief Number of nanoseconds per second.
 */
#define BRAWCAP_TIMESTAMP_NS_PER_SEC 1000000000

/**
 * @brief Number of microseconds per second.
 */
#define BRAWCAP_TIMESTAMP_US_PER_SEC 1000000

/**
 * @brief Number of milliseconds per second.
 */
#define BRAWCAP_TIMESTAMP_MS_PER_SEC 1000

/**
 * @brief Number of nanoseconds per microsecond.
 */
#define BRAWCAP_TIMESTAMP_NS_PER_US 1000

/**
 * @brief Number of nanoseconds per millisecond.
 */
#define BRAWCAP_TIMESTAMP_NS_PER_MS 1000000


/**
 * @brief Type used for indicating the supported timestamp modes.
 *
 * It has to be interpreted as bitfield.
 */
typedef UINT32 brawcap_timestamp_capabilities_t;

/**
 * @brief Represents the timestamp resolution in nanoseconds.
 *
 * The resolution can be used to check how accurate a timestamp is.
 * For example if it is set to 1.000.000 ns the timestamp has a accuracy of "only" one millisecond.
 * If it is set to 1 ns instead, it would indicate a very precise timestamp with a resolution of one nanosecond.
 */
typedef UINT32 brawcap_timestamp_resolution_ns_t;

/**
 * @brief List of different timestamp modes.
 *
 * The mode of a timestamp can be used to check when the timestamp is generated (time of trigger).
 * It can also indicate how accurate a timestamp is.
 *
 * Description for the different timestamp types:
 *  - System timestamps:
 *    - Less accurate
 *    - Jumps if the system time itself is jumping.
 *    - Source is the system time
 *  - Software timestamps:
 *    - Good accuracy
 *    - Guaranteed to not jump.
 *    - Source is almost ever the local TSC (if the CPU supports it).
 *      If not Windows will take the next accurate source for it.
 *      For more information on QPC, please see the official
 *      <a href=https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps> Microsoft documentation </a>.
 *    - They will always start from zero and increase with system up time.
 *  - Hardware timestamps:
 *    - Highest accuracy because they are not influenced by the system.
 *    - Only jumps if the adapter hardware clock jumps.
 *    - Source is the adapter hardware clock.
 *
 * @note Adapter timestamps are features provided by the underlying adapter and not part of bRAWcap.
 * bRAWcap only provides those timestamps if the underlying adapter supports it.
 * To check if the underlying adapter supports them use
 *    - @ref brawcap_rx_timestamp_capabilities for receive timestamps
 *    - @ref brawcap_tx_timestamp_capabilities for transmit timestamps
 *
 * @note More accurate system or software timestamps can increase the overall CPU load a bit.
 * These differences can be neglected in most cases, but if the user is not interested in high-precision timestamps,
 * always timestamps that require less effort should be preferred.
 *
 * @note Trigger position for timestamps created by bRAWcap driver:
 *  - Receive: When the packet is recognized by the bRAWcap driver (forwarded from the underlying driver)
 *  - Transmit: When the transmission complete event is recognized by the bRAWcap driver
 */
typedef enum _brawcap_timestamp_mode
{
  /**
   * @brief No timestamp.
   */
  BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP = 0x00,
  /**
   * @brief A low precision system timestamp created by bRAWcap driver.
   */
  BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC = 0x01,
  /**
   * @brief A high precision system timestamp created by bRAWcap driver.
   */
  BRAWCAP_TIMESTAMP_MODE_SYSTEM_HIGHPREC = 0x02,
  /**
   * @brief A software timestamp created by bRAWcap driver.
   */
  BRAWCAP_TIMESTAMP_MODE_SOFTWARE = 0x04,
  /**
   * @brief A system timestamp created by the network adapter driver.
   */
  BRAWCAP_TIMESTAMP_MODE_ADAPTER_SYSTEM = 0x08,
  /**
   * @brief A software timestamp created by the network adapter driver.
   */
  BRAWCAP_TIMESTAMP_MODE_ADAPTER_SOFTWARE = 0x10,
  /**
   * @brief A hardware timestamp created by the network adapter hardware/firmware.
   */
  BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE = 0x20
}brawcap_timestamp_mode_t;

/**
 * @brief bRAWcap timestamp object.
 *
 * The timestamp object is part of each @ref brawcap_packet "packet" and
 * has to be used for each timestamp module function.
 * To get a timestamp object from a packet use @ref brawcap_packet_timestamp_get.
 */
typedef struct _brawcap_timestamp brawcap_timestamp_t;

/**@}*/
#endif

/************************************************ PACKET *************************************************************/
#if 1
/**
 * @defgroup brawcap_packet Packet
 * @brief Types and functions for operating with bRAWcap packets.
 *
 * To create a packet use @ref brawcap_packet_create.
 * If a packet is not required anymore it shall be deleted with @ref brawcap_packet_free.
 *
 * @note It is also possible to use "buffered" packets. Those packets must not be created and freed on their one.
 * Instead they are part of a @ref brawcap_buffer "packet buffer". To access buffered packets use
 * @ref brawcap_buffer_at_index to get a packet on a specific position of the buffer or
 * @ref brawcap_buffer_iterator_eval on a @ref brawcap_buffer_iterator_t "packet buffer iterator".
 * The second one is especially useful while iterating the buffer.
 *
 * @attention The user application shall never store buffered packets separately.
 * Instead it should always request packets directly from the packet buffer and only use them as long as the packet
 * buffer itself is valid.
 *
 * List of examples:
 *  1. @ref 05_receive_simple_packet_receiver.c "Simple Packet Receiver"
 *  2. @ref 07_receive_buffered_receiver.c "Buffered Receiver"
 *  3. @ref 08_receive_timestamp_inspector.c "Receive Timestamp Inspector"
 *
 * @{
 */

/**
 * @brief The maximum supported (byte) size for a single packet payload.
 *
 * This value corresponds to jumbo frame size (9000 Bytes) + Ethernet header (14 Bytes) + single VLAN header (4 Bytes).
 */
#define BRAWCAP_PACKET_SIZE_MAX 9018

/**
 * @brief The minimum required (byte) size for a single packet payload.
 *
 * This corresponds to Ethernet header size.
 *
 * @note The Ethernet standard defines a minimum packet length of 64 Bytes (including 4 Byte for Ethernet FCS).
 * If smaller packets are sent the network adapter will automatically add some padding at the end of the payload.
 * The padding contains zeros only and after the last padding/payload byte the Ethernet FCS will be added.
 */
#define BRAWCAP_PACKET_SIZE_MIN 14

/**
 * @brief Type for handling the number of payload bytes per packet.
 *
 * Typically the packet payload size is set to the adapters max transmission unit (MTU) size + Ethernet header size
 * and maybe a single VLAN header.
 * Packet payload sizes have to be between @ref BRAWCAP_PACKET_SIZE_MIN and @ref BRAWCAP_PACKET_SIZE_MAX,
 * values below/beyond are not accepted.
 */
typedef UINT16 brawcap_packet_size_t;

/**
 * @brief bRAWcap packet object.
 *
 * The packet object has to be used for each packet module function.
 * Packet objects can be @ref brawcap_packet_create "created" and @ref brawcap_packet_free "freed"
 * or retrieved from a @ref brawcap_buffer "packet buffer".
 */
typedef struct _brawcap_packet brawcap_packet_t;

/**@}*/
#endif

/************************************************* BUFFER *************************************************************/
#if 1
/**
 * @defgroup brawcap_buffer Packet Buffer
 * @brief Types and functions for operating with bRAWcap packet buffers.
 *
 * To create a new packet buffer use @ref brawcap_buffer_create.
 * If a packet buffer is not required anymore it shall be deleted with @ref brawcap_buffer_free.
 *
 * Packet buffers can be used for
 * - Receiving:
 *   To use a buffer for receiving on a bRAWcap handle it has to be @ref brawcap_rx_buffer_attach "attached" to it.
 *   If it shall not be used anymore for receiving it should be @ref brawcap_rx_buffer_detach "detached".
 *
 * - Transmitting:
 *   For transmitting a packet buffer, fill up the buffer with packets and than @ref brawcap_tx_buffer_send "send"
 *   it on a handle. This will place the buffer into the transmission queue of the specified handle.
 *
 * @attention The packet buffer will be locked down after attaching/sending to a specific handle.
 * This means that it cannot be accessed by user application as long as it is pending for transmission or attached for
 * receiving. If the user application tries to access a locked buffer a error will be returned by the specific
 * packet buffer function.
 *
 * A packet buffer can be accessed if:
 *  - It is not attached for receiving.
 *  - No transmission is pending.
 *  - While receiving - from inside the @ref brawcap_rx_callback_t "receive callback".
 *  - While transmitting - from inside the  @ref brawcap_tx_callback_t "transmit callback".
 *
 * List of examples:
 *  1. @ref 07_receive_buffered_receiver.c "Buffered Receiver"
 * @{
 */

/**
 * @brief The maximum amount of packets, which can be stored in a packet buffer.
 */
#define BRAWCAP_BUFFER_PACKETS_MAX 100000

/**
 * @brief The minimum amount of packets, which a packet buffer must have.
 */
#define BRAWCAP_BUFFER_PACKETS_MIN 1

/**
 * @brief Type for handling the number of packets which can be stored in a packet buffer.
 *
 * The total amount of packets per packet buffer has to be between @ref BRAWCAP_BUFFER_PACKETS_MIN -
 * @ref BRAWCAP_BUFFER_PACKETS_MAX.
 */
typedef UINT32 brawcap_buffer_packet_count_t;

/**
 * @brief bRAWcap packet buffer object.
 *
 * The packet buffer object has to be used for each packet buffer module function.
 * Packet buffers can be @ref brawcap_buffer_create "created" and @ref brawcap_buffer_free "freed".
 */
typedef struct _brawcap_buffer brawcap_buffer_t;

/********************************************* BUFFER ITERATOR ********************************************************/
#if 1
/**
 * @defgroup brawcap_buffer_iterator Packet Buffer Iterator
 * @brief bRAWcap packet buffer iterators.
 *
 * To create a new packet buffer iterator use @ref brawcap_buffer_iterator_create.
 * If a packet buffer iterator is not required anymore it shall be deleted with @ref brawcap_buffer_iterator_free.
 *
 * @note Iterators should only be used as long as the packet buffer for which it was created is still available.
 *
 * @attention Packet buffer iterators are not synchronized. This means that if the packet buffer for which the iterator
 * was created is modified meanwhile, the iterator is not updated.
 *
 * List of examples:
 *  1. @ref 07_receive_buffered_receiver.c "Buffered Receiver"
 * @{
 */

/**
 * @brief bRAWcap packet buffer iterator object.
 *
 * The packet buffer object has to be used for each packet buffer iterator module function.
 * Packet buffer iterators can be @ref brawcap_buffer_iterator_create "created" and @ref brawcap_buffer_iterator_free
 * "freed".
 */
typedef struct _brawcap_buffer_iterator brawcap_buffer_iterator_t;

/**@}*/
#endif

/**@}*/
#endif

/************************************************** ADAPTER **********************************************************/
#if 1
/**
 * @example{lineno} 01_adapter_property_reader.c
 *
 * This example shows how to get all currently available adapters on the machine.
 * And how to get the properties for each adapter.
 * The adapters and their properties will be printed to command line output.
 *
 * @attention This example requires a valid bRAWcap license.
 * Reading adapter properties is not part of the demo.
 */

/**
 * @example{lineno} 02_adapter_change_notifier.c
 *
 * This example shows how to register and handle adapter change notifications.
 * It will print each received notification to command line.
 * Each output will also include the change reason and the new adapter property value.
 *
 * @attention This example requires a valid bRAWcap license.
 * Reading adapter properties is not part of the demo.
 */

/**
 * @defgroup brawcap_adapter Adapter
 * @brief Types and functions for handling bRAWcap (Ethernet) adapters.
 *
 * This module provides many features that facilitate the detection of bRAWcap (Ethernet) adapters.
 * It allows to scan the local machine for available and supported adapters,
 * to resolve many useful adapter properties for each adapter and also an adapter change notification.
 *
 * Adapter change notifications inform your application directly when any of the available properties have changed,
 * or when a new adapter has been found or an existing one has been removed (e.g. by disabling the interface).
 *
 * When you use these functions, they make all calls to the Windows API - related to adapter handling - obsolete.
 *
 * List of examples:
 *  1. @ref 01_adapter_property_reader.c "Adapter Property Reader"
 *  2. @ref 02_adapter_change_notifier.c "Adapter Change Notifier"
 * @{
 */

/**@}*/
#endif

/************************************************** RECEIVE **********************************************************/
#if 1
/**
 * @example{lineno} 05_receive_simple_packet_receiver.c
 *
 * This example shows how to use the bRAWcap single packet receive feature.
 * It opens a handle to the first available bRAWcap adapter, creates a bRAWcap packet
 * to store received data to. For each received packet it will print the packet information to console.
 */

/**
 * @example{lineno} 07_receive_buffered_receiver.c
 *
 * This example shows how to use the bRAWcap single packet receive feature.
 * It opens a handle to the first available bRAWcap adapter, creates a bRAWcap packet
 * to store received data to. For each received packet it will print the packet information to console.
 */

/**
 * @example{lineno} 08_receive_timestamp_inspector.c
 *
 * This example shows how to configure the different timestamp modes and read timestamp information from single
 * received packets.
 * It opens a handle to the first available bRAWcap adapter, creates a bRAWcap packet
 * to store received data to and applies the configured timestamp mode. For each received packet it will print
 * it´s applied timestamp info.
 */

/**
 * @defgroup brawcap_receive Receive
 * @brief Types and functions for bRAWcap receiving.
 *
 * List of examples:
 *  1. @ref 05_receive_simple_packet_receiver.c "Simple Packet Receiver"
 *  2. @ref 07_receive_buffered_receiver.c "Buffered Receiver"
 *  3. @ref 08_receive_timestamp_inspector.c "Timestamp Inspector"
 * @{
 */

/********************************************** RECEIVE FILTER *******************************************************/
#if 1
/**
 * @example{lineno} 06_filter_firewall.c
 *
 * This example shows how bRAWcap can be used to filter (discard) specific packets by using a byte filter.
 */

/**
 * @defgroup brawcap_filter Receive Filter
 * @brief Types and functions for operating with bRAWcap receive filters.
 *
 * To create a new filter use @ref brawcap_filter_create.
 * If a filter is not required anymore it shall be deleted with @ref brawcap_filter_free.
 *
 * @note Applying filters to handles:<br>
 * Each bRAWcap handle has it´s own filter instance.
 * Therefore after applying the filter to a bRAWcap handle the filter may be reused to apply the
 * same (or a different) filter to another handle or freed if not required anymore.
 *
 * List of examples:
 *  1. @ref 06_filter_firewall.c "Firewall"
 * @{
 */

/**
 * @brief The maximum supported filter byte mask length in bytes.
 */
#define BRAWCAP_FILTER_BYTE_MAX_LENGTH 64U

/**
 * @brief Fixed size array for storing a filter byte mask.
 *
 * @note It is not necessary to set all bytes.
 * If the used filter does not require the complete mask length.
 * bRAWcap evaluates only the bytes up to the specified filter length.
 */
typedef UINT8 brawcap_filter_mask_array_t [BRAWCAP_FILTER_BYTE_MAX_LENGTH];

/**
 * @brief Fixed size array for storing a byte wise bitfield.
 *
 * This allows to ignore specific bits in the specified filter byte mask.
 * Each bit which is set to one is ignored in the filter byte mask.
 *
 * @note It is not necessary to set all bytes.
 * If the used filter does not require the complete mask length.
 * bRAWcap evaluates only the bytes up to the specified filter length.
 */
typedef UINT8 brawcap_filter_ignore_bits_array_t [BRAWCAP_FILTER_BYTE_MAX_LENGTH];

/**
 * @brief Type for specifying the byte filter length.
 */
typedef brawcap_packet_size_t brawcap_filter_byte_length_t;

/**
 * @brief List of filter types.
 *
 * @note Currently only the byte filter is supported.
 * A BPF (Berkley Packet Filter) filter, which is equivalent to the one which libpcap uses,
 * may be added in the future.
 */
typedef enum _brawcap_filter_type
{
  /**
   * @brief A bRAWcap byte filter.
   */
  BRAWCAP_FILTER_TYPE_BYTE_MASK = 0,
  /**
   * @brief A Berkley Packet Filter.
   */
  BRAWCAP_FILTER_TYPE_BPF = 1
}brawcap_filter_type_t;
  typedef struct bpf_insn* brawcap_bpf_insn_t;

  typedef struct bpf_program* brawcap_bpf_program_t;
#ifdef _BRAWCAP_DEFINE_BPF_STRUCTS_INTERNAL
  /**
   * @brief A single BPF pseudo-instruction.
   *
   * Contains a single instruction for the BPF.
   */
  struct bpf_insn
  {
    USHORT code; /*!< Instruction type and addressing mode. */
    UCHAR jt; /*!< Jump if true. */
    UCHAR jf; /*!< Jump if false. */
    UINT32 k; /*!< Generic field used for various purposes. */
  };

  /**
   * @brief A BPF pseudo-assembly program.
   *
   * The BPF program can be used to filter incoming packets.
   * To get a BPF program it has to be generated/compiled from a BPF filter string, first.
   */
  struct bpf_program
  {
    UINT bf_len; /*!< Indicates the number of instructions of the program. */
    struct bpf_insn* bf_insns; /*!< A pointer to the first instruction of the program. */
  };
#endif

/**
 * @brief bRAWcap filter object.
 *
 * The filter object can be accessed with the bRAWcap filter functions.
 * Filters can be @ref brawcap_filter_create "created" and @ref brawcap_filter_free "freed".
 */
typedef struct _brawcap_filter brawcap_filter_t;

/**@}*/
#endif

/**
 * @brief Default minimum packets to copy value which has to be received before any receive function return
 * even if configured timeout exceeds.
 *
 * @note bRAWcap waits at least up to 10 times the configured timeout before finally returning if
 * the minimum number of packets has not yet been received.
 *
 * @warning If a timeout of zero which is equivalent to infinite wait is configured it blocks until
 * the minimum amount of packets are received and will not return earlier.
 */
#define BRAWCAP_RX_COPY_PACKETS_DEFAULT 0

/**
 * @brief Default timeout in milliseconds after which a bRAWcap receive returns,
 * even if buffer is not yet full but the configured receive min packets to copy must be reached.
 */
#define BRAWCAP_RX_TIMEOUT_MS_DEFAULT 10

/**
 * @brief Default receive timestamp mode which will be used.
 */
#define BRAWCAP_RX_TIMESTAMP_MODE_DEFAULT BRAWCAP_TIMESTAMP_MODE_SYSTEM_LOWPREC

/**
 * @brief Default VLAN tagging mode which will be used.
 */
#define BRAWCAP_RX_VLAN_TAGGING_DEFAULT FALSE

/**
 * @brief Default size for driver receive queues in number of packets.
 */
#define BRAWCAP_RX_DRIVER_QUEUE_SIZE_DEFAULT 4096

/**
 * @brief Maximum supported receive timeout in number of milliseconds.
 */
#define BRAWCAP_RX_TIMEOUT_MS_MAX 5000

/**
 * @brief Maximum size (in number of packets) supported for driver receive queues.
 */
#define BRAWCAP_RX_DRIVER_QUEUE_SIZE_MAX 40000

/**
 * @brief Default receive direction which will be used.
 */
#define BRAWCAP_RX_DIRECTION_DEFAULT BRAWCAP_RX_DIRECTION_BOTH

/**
 * @brief Type for handling receive timeouts in number of milliseconds.
 * The timeout specifies the maximum blocking time in milliseconds for pending bRAWcap receives.
 * If a packet is received or the receive buffer is full before the timeout is reached, receive will return earlier.
 *
 * @attention A receive timeout value of zero corresponds to an infinite wait.
 * This means that bRAWcap receive will only return if
 *  - @ref brawcap_rx_packet - a packet is received,
 *  - @ref brawcap_rx_start - the minimum amount of packets to copy is reached (see @ref brawcap_rx_min_packets_t)
 *                            or the attached buffer is full - whichever comes first.
 */
typedef UINT16 brawcap_rx_timeout_t;

/**
 * @brief Type for handling the minimum amount of packets to copy before a bRAWcap receive will return.
 * It specifies how many packets should be received before a pending receive returns.
 * Reading more packets in a single call can improve the performance but leads to higher latency.
 *
 * @note This setting is only used with looped receive (@ref brawcap_rx_start) and has no impact on single packet
 * reception (@ref brawcap_rx_packet).
 */
typedef UINT32 brawcap_rx_min_packets_t;

/**
 * @brief List of receive modes.
 */
typedef enum _brawcap_rx_mode
{
  /**
   * @brief Received packets are stored in a user created bRAWcap packet (buffer) and provided to the
   * calling application for processing.
   */
  BRAWCAP_RX_MODE_LIVE = 1,
  /**
   * @brief Received packets are directly written to disk.
   *
   * @note Dumping mode is not yet supported and may be added to later releases.
   */
  BRAWCAP_RX_MODE_DUMP = 2
}brawcap_rx_mode_t;

/**
 * @brief List of supported receive directions.
 *
 * The receive direction allows the select for which direction (transmit/receive) the packets shall be captured.
 * This setting can be seen as some kind of generic receive filter.
 */
typedef enum _brawcap_rx_direction
{
  /**
   * @brief Stops to receive packets of any direction.
   */
  BRAWCAP_RX_DIRECTION_UNKNOWN = 0x00,
  /**
   * @brief Only received packets by the network adapter will be received.
   */
  BRAWCAP_RX_DIRECTION_IN = 0x01,
  /**
   * @brief Only transmitted packets by bRAWcap or upper network stack drivers will be received.
   * This allows some kind of loopback for packets which are transmitted to the network.
   *
   * @note This currently does not include packets sent to localhost (IP 127.0.0.1), because they are
   * not transmitted/received via a real network adapter. Instead Windows creates a separate "virtual" adapter
   * for localhost communication. This adapter is currently not supported by bRAWcap.
   */
  BRAWCAP_RX_DIRECTION_OUT = 0x02,
  /**
   * @brief Both transmitted packets by bRAWcap or upper network stack drivers and received packets from the
   * underlying network adapter will be received.
   */
  BRAWCAP_RX_DIRECTION_BOTH = 0x03
}brawcap_rx_direction_t;

/**@}*/
#endif

/************************************************* TRANSMIT **********************************************************/
#if 1
/**
 * @defgroup brawcap_transmit Transmit
 * @brief Types and functions for bRAWcap transmitting.
 * @{
 */

/**
 * @brief Default size in number of packets for driver transmit queues.
 */
#define BRAWCAP_TX_DRIVER_QUEUE_SIZE_DEFAULT 512

/**
 * @brief Minimum size in number of packets supported for driver transmit queues.
 */
#define BRAWCAP_TX_DRIVER_QUEUE_SIZE_MIN 256

/**
 * @brief Maximum size in number of packets supported for driver transmit queues.
 */
#define BRAWCAP_TX_DRIVER_QUEUE_SIZE_MAX 4096

/**
 * @brief Default timestamp mode for transmitted packets.
 */
#define BRAWCAP_TX_TIMESTAMP_MODE_DEFAULT BRAWCAP_TIMESTAMP_MODE_NO_TIMESTAMP

/**@}*/
#endif

/*************************************************** STATS ***********************************************************/
#if 1
/**
 * @defgroup brawcap_stats Statistics
 * @brief Types and functions for bRAWcap statistics.
 * @{
 */

/**
 * @internal
 * @brief Returns the offset from the first member to the last member in number of bytes.
*/
#define _BRAWCAP_LAST_M_OFFSET(type, lastMember) ((LONG)(LONG_PTR)&(((type *)0)->lastMember))
/**
 * @internal
 * @brief Returns the size of the last member in number of bytes.
 */
#define _BRAWCAP_LAST_M_SIZE(type, lastMember) (sizeof(((type *)0)->lastMember))
/**
 * @internal
 * @brief Returns the total byte size of the structure, depending on the structure and its last member.
 * @warning It is very important to always update the lastMember if the struct is going to be extended.
 */
#define _BRAWCAP_SIZEOF_STRUCT(type, lastMember) \
  (_BRAWCAP_LAST_M_OFFSET(type, lastMember) + _BRAWCAP_LAST_M_SIZE(type, lastMember))

/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterReceivedPacketsTotal member of the receive stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_RX_ADAPTER_RECEIVED_PACKETS_TOTAL_VALID 0x00000001
/**
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterReceivedPacketsTotal member of the given receive stats is
 * valid. This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_RX_ADAPTER_RECEIVED_PACKETS_TOTAL_VALID(__stats_rx) \
  (__stats_rx.adapterValid & _BRAWCAP_STATS_RX_ADAPTER_RECEIVED_PACKETS_TOTAL_VALID)
/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterReceivedBytesTotal member of the receive stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_RX_ADAPTER_RECEIVED_BYTES_TOTAL_VALID 0x00000002
/**
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterReceivedBytesTotal member of the given receive stats is valid.
 * This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_RX_ADAPTER_RECEIVED_BYTES_TOTAL_VALID(__stats_rx) \
  (__stats_rx.adapterValid &  _BRAWCAP_STATS_RX_ADAPTER_RECEIVED_BYTES_TOTAL_VALID)
/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterDroppedPacketsTotal member of the receive stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_RX_ADAPTER_DROPPED_PACKETS_TOTAL_VALID 0x00000004
/**
 * @brief Indicates if the @ref brawcap_stats_rx_t.adapterDroppedPacketsTotal member of the given receive stats is
 * valid. This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_RX_ADAPTER_DROPPED_PACKETS_TOTAL_VALID(__stats_rx) \
  (__stats_rx.adapterValid & _BRAWCAP_STATS_RX_ADAPTER_DROPPED_PACKETS_TOTAL_VALID)

/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCompletedPacketsTotal member of the transmit stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_TX_ADAPTER_COMPLETED_PACKETS_TOTAL_VALID 0x00000001
/**
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCompletedPacketsTotal member of the given transmit stats is
 * valid. This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_TX_ADAPTER_COMPLETED_PACKETS_TOTAL_VALID(__stats_tx) \
  (__stats_tx.adapterValid & _BRAWCAP_STATS_TX_ADAPTER_COMPLETED_PACKETS_TOTAL_VALID)
/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCompletedBytesTotal member of the transmit stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_TX_ADAPTER_COMPLETED_BYTES_TOTAL_VALID 0x00000002
/**
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCompletedBytesTotal member of the given transmit stats is
 * valid. This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_TX_ADAPTER_COMPLETED_BYTES_TOTAL_VALID(__stats_tx) \
  (__stats_tx.adapterValid & _BRAWCAP_STATS_TX_ADAPTER_COMPLETED_BYTES_TOTAL_VALID)
/**
 * @internal
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCanceledPacketsTotal member of the transmit stats is valid.
 * @endinternal
 */
#define _BRAWCAP_STATS_TX_ADAPTER_CANCELED_PACKETS_TOTAL_VALID 0x00000004
/**
 * @brief Indicates if the @ref brawcap_stats_tx_t.adapterCanceledPacketsTotal member of the given transmit stats is
 * valid. This statistic is provided from the underlying network adapter driver and may be supported or not.
 */
#define BRAWCAP_STATS_TX_ADAPTER_CANCELED_PACKETS_TOTAL_VALID(__stats_tx) \
  (__stats_tx.adapterValid & _BRAWCAP_STATS_TX_ADAPTER_CANCELED_PACKETS_TOTAL_VALID )

/**
 * @brief Header describing given bRAWcap stats structure.
 *
 * @attention If your create a new bRAWcap stats structure you must always initialize
 * the header first before make use of it.
 * This is required to allow backward compatibility for applications using older versions.
 */
typedef struct _brawcap_stats_header
{
  /**
   * @brief Indicates which type of stats it is.
   */
  BYTE type;
  /**
   * @brief Indicates the revision of the stats.
   * @note It is preferred to always use the latest version available in your bRAWcap header file.
   * But you can also use older versions if you dont want to access members added in the newer versions.
   */
  BYTE revision;
  /**
   * @brief Should be set to the size of the structure depending on it´s type and revision.
   * You should use the BRAWCAP_STATS_..._SIZEOF_REVISION_X macros for initializing this member.
   */
  UINT16 size;
}brawcap_stats_header_t;

/**
 * @brief List of all available statistic types.
 */
typedef enum _brawcap_stats_type
{
  /**
   * @brief Type for receive stats.
   */
  BRAWCAP_STATS_TYPE_RX = 1,
  /**
   * @brief Type for transmit stats.
   */
  BRAWCAP_STATS_TYPE_TX = 2
}brawcap_stats_type_t;

/**
 * @brief List of all available receive statistic revisions.
 */
typedef enum _brawcap_stats_rx_revision
{
  /**
   * @brief Revision 1 for receive stats.
   */
  BRAWCAP_STATS_RX_REVISION_1 = 1
}brawcap_stats_rx_revision_t;

/**
 * @brief The bRAWcap receive statistics.
 *
 * This can be used to retrieve all available bRAWcap statistics and counters related to receive path.
 * After creating the structure, its header has to be initialized.
 *
 * The @ref brawcap_stats_header_t.type shall be set to @ref BRAWCAP_STATS_TYPE_RX.
 *
 * The @ref brawcap_stats_header_t.revision shall be set to one of the following values:
 *    - @ref BRAWCAP_STATS_RX_REVISION_1
 *
 * The @ref brawcap_stats_header_t.size shall be set depending on your selected version by using the macro:
 *    - @ref BRAWCAP_STATS_RX_SIZEOF_REVISION_1 (if selected revision is @ref BRAWCAP_STATS_RX_REVISION_1)
 *
 * @attention User is responsible for initializing the structure header before using it.
 * Otherwise it may lead to undefined behaviour.
 */
typedef struct _brawcap_stats_rx
{
  /**
   * @brief Header for receive statistics.
   * This has to be always initialized by the user after creation before using it.
   */
  brawcap_stats_header_t header;
  /**
   * @brief Bitfield indicating which counters provided by the underlying network adapter driver are valid.
   */
  UINT32 adapterValid;
  /**
   * @brief Total number of received packets by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_RX_ADAPTER_RECEIVED_PACKETS_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterReceivedPacketsTotal;
  /**
   * @brief Total number of received bytes by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_RX_ADAPTER_RECEIVED_BYTES_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterReceivedBytesTotal;
  /**
   * @brief Total number of received bytes by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_RX_ADAPTER_DROPPED_PACKETS_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterDroppedPacketsTotal;
  /**
   * @brief The total number of indicated packets to the upper network stack by bRAWcap on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverIndicatedPacketsTotal;
  /**
   * @brief The total number of received packets by bRAWcap on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter..
   */
  UINT64 driverReceivedPacketsTotal;
  /**
   * @brief The total number of received packets by the given bRAWcap handle.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleReceivedPacketsTotal;
  /**
   * @brief The total number of packets which have matched the given bRAWcap handle filter.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleReceivedPacketsMatched;
  /**
   * @brief The total number of packets which was queued to the handles driver queue.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleReceivedPacketsQueued;
  /**
   * @brief The total number of packets which could be directly delivered to user space memory.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleReceivedPacketsDirect;
  /**
   * @brief The total number of received bytes by the given bRAWcap handle.
   * @note This counter relates to a handle and will be deleted when the handle is closed
   */
  UINT64 handleReceivedBytesTotal;
  /**
   * @brief The total number of packets which was dropped for the handle (sum of all reasons).
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleDroppedPacketsTotal;
  /**
   * @brief The total number of packets which was dropped for the handle because of resources (driver queue overrun).
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleDroppedPacketsQueue;
  /**
   * @brief The total number of packets which was dropped for the handle their length exceeds the handles configured max
   * packet size.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleDroppedPacketsToLong;
}brawcap_stats_rx_t;

/**
 * @brief Returns the size of receive statistics revision 1.
 */
#define BRAWCAP_STATS_RX_SIZEOF_REVISION_1 _BRAWCAP_SIZEOF_STRUCT(brawcap_stats_rx_t, handleDroppedPacketsToLong)

/**
 * @brief List of all available transmit statistic revisions.
 */
typedef enum _brawcap_stats_tx_revision
{
  /**
   * @brief Revision 1 for transmit stats.
   */
  BRAWCAP_STATS_TX_REVISION_1 = 1
}brawcap_stats_tx_revision_t;

/**
 * @brief The bRAWcap transmit statistics.
 *
 * This can be used to retrieve all available bRAWcap statistics and counters related to transmit path.
 * After creating the structure, its header has to be initialized.
 *
 * The @ref brawcap_stats_header_t.type shall be set to @ref BRAWCAP_STATS_TYPE_TX.
 *
 * The @ref brawcap_stats_header_t.revision shall be set to one of the following values:
 *    - @ref BRAWCAP_STATS_TX_REVISION_1
 *
 * The @ref brawcap_stats_header_t.size shall be set depending on your selected version by using the macro:
 *    - @ref BRAWCAP_STATS_TX_SIZEOF_REVISION_1 (if selected revision is @ref BRAWCAP_STATS_TX_REVISION_1)
 *
 * @attention User is responsible for initializing the structure header before using it.
 * Otherwise it may lead to undefined behaviour.
 */
typedef struct _brawcap_stats_tx
{
  /**
   * @brief Header for transmit statistics.
   * This has to be always initialized by the user after creation before using it.
   */
  brawcap_stats_header_t header;
  /**
   * @brief Bitfield indicating which counters provided by the underlying network adapter driver are valid.
   *
   */
  UINT32 adapterValid;
  /**
   * @brief Total number of (completed) transmitted packets by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_TX_ADAPTER_COMPLETED_PACKETS_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterCompletedPacketsTotal;
  /**
   * @brief Total number of (completed) transmitted bytes by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_TX_ADAPTER_COMPLETED_BYTES_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterCompletedBytesTotal;
  /**
   * @brief Total number of canceled transmitted packets by the underlying network adapter.
   * @note You should check if the network adapter supports this counter with
   * @ref BRAWCAP_STATS_TX_ADAPTER_CANCELED_PACKETS_TOTAL_VALID.
   * If the counter is not supported it will be always zero.
   */
  UINT64 adapterCanceledPacketsTotal;
  /**
   * @brief Total number of initiated packets to transmit by the bRAWcap driver on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverInitiatedPacketsTotal;
  /**
   * @brief Initiated packets to transmit from upper network stack detected by bRAWcap driver on
   * the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverInitiatedPacketsStack;
  /**
   * @brief Initiated packets to transmit from bRAWcap driver itself on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverInitiatedPacketsHandles;
  /**
   * @brief Total number of canceled packets to transmit by the bRAWcap driver on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCanceledPacketsTotal;
  /**
   * @brief Canceled packets to transmit from the upper network stack detected by bRAWcap driver on the
   * corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCanceledPacketsStack;
  /**
   * @brief Canceled packets to transmit from bRAWcap driver itself on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCanceledPacketsHandles;
  /**
   * @brief Total number of completed transmitted packets detected by bRAWcap driver on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCompletedPacketsTotal;
  /**
   * @brief Completed packets to transmit from the upper network stack detected by bRAWcap driver on the corresponding
   * adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCompletedPacketsStack;
  /**
   * @brief Completed packets to transmit from the bRAWcap driver itself on the corresponding adapter.
   * @note This counter will reset if bRAWcap driver is restarted, or it is deactivated on the adapter.
   */
  UINT64 driverCompletedPacketsHandles;
  /**
   * @brief Total number of initiated packets to transmit for the handle.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleInitiatedPacketsTotal;
  /**
   * @brief Total number of canceled packets to transmit for the handle.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleCanceledPacketsTotal;
  /**
   * @brief Total number of completed transmitted packets for the handle.
   * @note This counter relates to a handle and will be deleted when the handle is closed.
   */
  UINT64 handleCompletedPacketsTotal;
  
  UINT64 handleCompletedBytesTotal;
}brawcap_stats_tx_t;

/**
 * @brief Returns the size of transmit statistics revision 1.
 */
#define BRAWCAP_STATS_TX_SIZEOF_REVISION_1 _BRAWCAP_SIZEOF_STRUCT(brawcap_stats_tx_t, handleCompletedBytesTotal)

/**@}*/
#endif

#endif // BRAWCAP_TYPES_SHARED_H
