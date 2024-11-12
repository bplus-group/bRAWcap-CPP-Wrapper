/**
 * @file brawcap_types_um.h
 *
 * @brief bRAWcap API - User mode types.
 *
 * The file includes all (data)types and defines used for bRAWcap (user space only).
 *
 * @copyright
 * <b> © 2021 - b-plus technologies GmbH. All rights reserved!</b>
 *
 * All rights exclusively reserved for b-plus GmbH, unless expressly otherwise agreed.
 *
 * Redistribution in source or any other form, with or without modification, is not permitted.
 *
 *
 * You may use this code under the according license terms of b-plus. Please contact b-plus at services@b-plus.com
 * to get the appropriate terms and conditions.
 */
#ifndef BRAWCAP_TYPES_UM_H
#define BRAWCAP_TYPES_UM_H

#include "brawcap_types_shared.h"

/**************************************************** LIB ************************************************************/
#if 1
/**
 * @defgroup brawcap_lib Importing
 * @brief Defines required for automatic/dynamic bRAWcap API function importing.
 * @{
 */

// You should never define _BRAWCAP_EXPORT when including bRAWcap.
#ifdef _BRAWCAP_EXPORT
  /**
   * @internal
   * @brief Dynamic/automatic function export when compiling bRAWcap itself.
   * @warning Only required when building the bRAWcap library.
   * Should never be defined if you include bRAWcap.
  */
  #define _BRAWCAP_API_FUNC(returnType) __declspec(dllexport) returnType __cdecl
#else
  /**
   * @brief Dynamic/automatic function import for loading/including.
   */
  #define _BRAWCAP_API_FUNC(returnType) __declspec(dllimport) returnType __cdecl
#endif

/**@}*/
#endif

/************************************************ LOGGING ************************************************************/
#if 1
/**
 * @defgroup brawcap_logging Logging
 * @brief Types and functions for handling/modifying the bRAWcap logging.
 *
 * This module provides some functions for configuring the bRAWcap logging behavior.
 * The bRAWcap logger supports different log levels and types, which allows to configure the logging for specific
 * types independent to each other.
 * Increasing the log level may help during development of the user application which shall use bRAWcap.
 *
 * @note If you think there is a problem with bRAWcap, please enable detailed logging,
 * run your application until the problem occurs and then send the created bRAWcap log files to b-plus.
 *
 * @attention For release builds log levels such as verbose shall be avoided, because they can lead to performance
 * degradation.
 * @{
 */

/**
 * @brief List of bRAWcap log levels.
 *
 * The log levels allows to (de)activate additional log entries.
 * This can be especially useful during development, to get a more detailed log from bRAWcap.
 */
typedef enum _brawcap_log_level
{
  /**
   * @brief Debug log level is for debugging bRAWcap itself only.
   * Therefore it can only be enabled if the application is registered with higher privilege.
   *
   * @note Default: Deactivated
   */
  BRAWCAP_LOG_LEVEL_DEBUG = 0x01,
  /**
   * @brief Verbose log level will include many informative loggings.
   * Those loggings gives more info why some API calls may fail.
   * This will especially contain those cases in which the caller has provided invalid parameters.
   * It should be activated only for development, testing or error research.
   *
   * @note Default: Deactivated
   */
  BRAWCAP_LOG_LEVEL_VERBOSE = 0x02,
  /**
   * @brief Notice log level will include some loggings which gives information of the current bRAWcap state.
   * This may be disabled to decrease logging to a minimum.
   *
   * @note Default: Activated
   */
  BRAWCAP_LOG_LEVEL_NOTICE = 0x04,
  /**
   * @brief Warning log level will include loggings for unexpected states.
   * This level should never be disabled.
   *
   * @note Default: Activated
   */
  BRAWCAP_LOG_LEVEL_WARNING = 0x08,
  /**
   * @brief Error log level will include loggings if something went completely wrong and bRAWcap could not solve
   * the issue.
   * Those loggings should never appear, if so bRAWcap may be in a invalid state.
   * If loggings contain such a logentry please try to reproduce it again with all log levels enabled
   * and send the logfile (with a short description of how to reproduce it) to b-plus.
   * This should really never be disabled.
   *
   * @note Default: Activated
   */
  BRAWCAP_LOG_LEVEL_ERROR = 0x10
}brawcap_log_level_t;

/**
 * @brief List of bRAWcap log types.
 *
 * The log types allows to adjust log levels for each type independent.
 * To distinguish between different types each logentry will be prefixed with it´s type, this increases the readability
 * of the logfile.
 */
typedef enum _brawcap_log_type
{
  /**
   * @brief Generic loggings will be used for everything which has no specific type.
   */
  BRAWCAP_LOG_TYPE_GENERIC = 0x01,
  /**
   * @brief Adapter loggings will be used for anything which relates to the local adapter handling.
   */
  BRAWCAP_LOG_TYPE_ADAPTER = 0x02,
  /**
   * @brief Receive loggings will be used for anything which relates to receiving.
   */
  BRAWCAP_LOG_TYPE_RECEIVE = 0x04,
  /**
   * @brief Transmit loggings will be used for anything which relates to transmitting.
   */
  BRAWCAP_LOG_TYPE_TRANSMIT = 0x08,
  /**
   * @brief License loggings will be used for anything which relates to licensing.
   */
  BRAWCAP_LOG_TYPE_LICENSE = 0x10,
  
  BRAWCAP_LOG_TYPE_ALL = 0xFFFF
}brawcap_log_type_t;

/**@}*/
#endif

/************************************************ GENERIC ************************************************************/
#if 1
/**
 * @addtogroup brawcap_generics
 * @{
 */

/**
 * @brief A bRAWcap handle.
 *
 * It can be @ref brawcap_open "created/opened" and  @ref brawcap_close "deleted/closed".
 * In general every function which communicates with the bRAWcap driver requires a open bRAWcap handle.
 *
 * @attention The number of bRAWcap handles per adapter is limited to @ref BRAWCAP_HANDLES_PER_ADAPTER_MAX.
 * If this limit is reached each additional open on that adapter will fail.
 */
typedef struct _brawcap_handle brawcap_handle_t;

/**@}*/
#endif

/*********************************************** TIMESTAMP ***********************************************************/
#if 1
/**
 * @addtogroup brawcap_timestamp
 * @{
 */

/**@}*/
#endif

/************************************************ PACKET *************************************************************/
#if 1
/**
 * @addtogroup brawcap_packet
 * @{
 */

/**@}*/
#endif

/************************************************* BUFFER *************************************************************/
#if 1
/**
 * @addtogroup brawcap_buffer
 * @{
 */

/********************************************* BUFFER ITERATOR ********************************************************/
#if 1
/**
 * @addtogroup brawcap_buffer_iterator
 * @{
 */

/**@}*/
#endif

/**@}*/
#endif

/************************************************** ADAPTER **********************************************************/
#if 1
/**
 * @addtogroup brawcap_adapter
 * @{
 */

/**
 * @brief Required number of bytes for describing a MAC address.
 * The defined length does not containing any delimiters, it represents the number of MAC address digits.
 */
#define BRAWCAP_ADAPTER_MAC_LENGTH 6

/**
 * @brief The max supported length for an adapter name.
 *
 * @note On Windows a adapter name always corresponds to it´s GUID.
 * Those GUIDs have a fixed length.
 */
#define BRAWCAP_ADAPTER_NAME_LENGTH 40

/**
 * @brief Type used for counting the available/supported adapters on a machine.
 */
typedef UINT8 brawcap_adapter_count_t;

/**
 * @brief Fixed size array describing a MAC address.
 *
 * The array only contains the MAC address digits without any delimiters.
 */
typedef UINT8 brawcap_adapter_mac_t[BRAWCAP_ADAPTER_MAC_LENGTH];

/**
 * @brief Fixed size array containing a adapter name.
 *
 * @note On Windows a adapter name corresponds always to it´s GUID.
 * Textual GUIDs always have the format "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}".
 */
typedef char brawcap_adapter_name_t[BRAWCAP_ADAPTER_NAME_LENGTH];

/**
 * @brief IPv4 address.
 */
typedef union _brawcap_adapter_ipv4
{
  UINT8 bytes[4]; /*!< IPv4 address as byte array. */
  UINT16 words[2]; /*!< IPv4 address as word array (each element has two Bytes). */
  UINT32 addr; /*!< IPv4 address as single integer value. */
} brawcap_adapter_ipv4_t;

/**
 * @brief IPv6 address.
 */
typedef union _brawcap_adapter_ipv6
{
  UINT8 bytes[16]; /*!< IPv6 address as byte array. */
  UINT16 words[8]; /*!< IPv6 address as word array (each element has two  Bytes). */
  UINT64 qwords[2]; /*!< IPv6 address as large word array (each element has eight Bytes). */
} brawcap_adapter_ipv6_t;

/**
 * @brief List of bRAWcap adapter speeds.
 * Values indicating the current adapter uplink speed.
 *
 * @note The speed should be always equal for rx and tx,
 * but both can be requested independent of each other.
 */
typedef enum _brawcap_adapter_speed
{
  BRAWCAP_ADAPTER_SPEED_UNKNOWN =      0ULL, /*!< Speed unknown */
  BRAWCAP_ADAPTER_SPEED_10M     =     10ULL, /*!< 10 MBit/s */
  BRAWCAP_ADAPTER_SPEED_100M    =    100ULL, /*!< 100 MBit/s */
  BRAWCAP_ADAPTER_SPEED_1G      =   1000ULL, /*!< 1 GBit/s */
  BRAWCAP_ADAPTER_SPEED_2_5G    =   2500ULL, /*!< 2,5 GBit/s */
  BRAWCAP_ADAPTER_SPEED_5G      =   5000ULL, /*!< 5 GBit/s */
  BRAWCAP_ADAPTER_SPEED_10G     =  10000ULL, /*!< 10 GBit/s*/
  BRAWCAP_ADAPTER_SPEED_25G     =  25000ULL, /*!< 25 GBit/s */
  BRAWCAP_ADAPTER_SPEED_40G     =  40000ULL, /*!< 40 GBit/s */
  BRAWCAP_ADAPTER_SPEED_50G     =  50000ULL, /*!< 50 GBit/s */
  BRAWCAP_ADAPTER_SPEED_100G    = 100000ULL  /*!< 100 GBit/s */
} brawcap_adapter_speed_t;

/**
 * @brief List of adapter connection states.
 * Values indicating the current adapter connection state (uplink/cable connected).
 */
typedef enum _brawcap_adapter_connection_state
{
  BRAWCAP_ADAPTER_CONNECTION_UNKNOWN = 0, /*!< Connection state unknown. */
  BRAWCAP_ADAPTER_CONNECTION_UP      = 1, /*!< Connection is up (uplink/cable connected). */
  BRAWCAP_ADAPTER_CONNECTION_DOWN    = 2  /*!< Connection is down (cable not connected). */
}brawcap_adapter_connection_state_t;

/**
 * @brief List of adapter operation states.
 * Values indicating the current adapter operation state (adapter driver status).
 */
typedef enum _brawcap_adapter_operation_state
{
  BRAWCAP_ADAPTER_OPERATION_UNKNOWN = 0, /*!< Operation state unknown. */
  BRAWCAP_ADAPTER_OPERATION_UP      = 1, /*!< Adapter is running (driver activated). */
  BRAWCAP_ADAPTER_OPERATION_DOWN    = 2, /*!< Adapter is down/disabled (driver deactivated). */
  BRAWCAP_ADAPTER_OPERATION_TESTING = 3  /*!< Adapter is in probing state. */
}brawcap_adapter_operation_state_t;

/**
 * @brief Type for handling the adapter maximum transmission unit (MTU).
 *
 * It describes the maximum bytes per packet which can be received/transmitted on the adapter.
 * Typical values are 1500 bytes for default sized Ethernet and 9000 bytes for jumbo frames.
 *
 * @note In general the MTU does not contain the Ethernet header (14 Bytes).
 * If something like 1514/9014 is displayed by the adapter driver it contains the Ethernet header.
 * If VLAN tagging is activated, VLAN packets require 4 additional bytes for each VLAN tag.
 * There is also a 4 Byte FCS - defined by Ethernet - added to each packet (done by the network adapter).
 * For example a full sized Jumbo Packet with one VLAN Tag can have:
 * 14 Bytes Ethernet header + 4 Bytes VLAN Tag + 9000 Bytes Payload + 4 Bytes FCS = 9022 Bytes.
 */
typedef UINT64 brawcap_adapter_mtu_t;

/**
 * @brief List of reasons for adapter change notifications.
 * This values indicate why a adapter change notification is received.
 */
typedef enum _brawcap_adapter_notify_reason
{
  /**
   * @brief The notification reason is unknown.
   */
  BRAWCAP_ADAPTER_NOTIFY_REASON_UNKNOWN = 0,
  /**
   * @brief A new bRAWcap adapter was detected.
   * E.g. due to enabling bRAWcap driver on an adapter where it was disabled before.
   */
  BRAWCAP_ADAPTER_NOTIFY_REASON_ADD = 1,
  /**
   * @brief A bRAWcap adapter was removed.
   * E.g. due to disabling bRAWcap driver on an adapter where it was enabled before.
   */
  BRAWCAP_ADAPTER_NOTIFY_REASON_REMOVE = 2,
  /**
   * @brief A adapter property has changed.
   * This can have several reasons, for a list of all properties see @ref brawcap_adapter_properties_t.
   */
  BRAWCAP_ADAPTER_NOTIFY_REASON_PROPERTY_CHANGE = 3
}brawcap_adapter_notify_reason_t;

/**
 * @brief List of adapter properties.
 * They are used to identify adapter change notifications with a property change reason.
 *
 * @note The property values allow to store several properties in a single bitfield.
 * This makes it possible to indicate several property changes with only one notification.
 */
typedef enum _brawcap_adapter_properties
{
  BRAWCAP_ADAPTER_PROPERTY_FRIENDLY_NAME    = 0x0001, /*!< Adapter friendly name changed. */
  BRAWCAP_ADAPTER_PROPERTY_RX_SPEED         = 0x0002, /*!< Adapter receive speed changed. */
  BRAWCAP_ADAPTER_PROPERTY_TX_SPEED         = 0x0004, /*!< Adapter transmit speed changed. */
  BRAWCAP_ADAPTER_PROPERTY_MTU              = 0x0008, /*!< Adapter max transmission unit changed. */
  BRAWCAP_ADAPTER_PROPERTY_OPERATION_STATE = 0x0010, /*!< Adapter operation state changed. */
  BRAWCAP_ADAPTER_PROPERTY_CONNECTION_STATE = 0x0020, /*!< Adapter connection state changed. */
  BRAWCAP_ADAPTER_PROPERTY_IPV4             = 0x0040, /*!< Adapter IPv4 address changed. */
  BRAWCAP_ADAPTER_PROPERTY_IPV6             = 0x0080  /*!< Adapter IPv6 address changed. */
}brawcap_adapter_properties_t;

/**
 * @brief The general callback for adapter change notifications.
 * It will be called each time a available and supported adapter change is detected.
 *
 * @attention Each application can only register for this callback once.
 *
 * @param[in] name The adapter name on which the change happened.
 *
 * @param[in] reason The reason/type of change (see @ref brawcap_adapter_notify_reason_t).
 *
 * @param[in] properties A bit field with the properties that have changed (see @ref brawcap_adapter_properties_t).
 *
 * @param[in] pUser Pointer to a user defined context.
 * It allows access to user specific data within the callback.
 */
typedef void (*brawcap_adapter_notify_callback_t)(const brawcap_adapter_name_t name,
  const brawcap_adapter_notify_reason_t reason, const UINT32 properties, void* pUser);

/**
 * @brief The per handle callback for adapter change notifications.
 * It will be called each time a change for the handle underlying adapter (and only for this adapter) is detected.
 *
 * @attention Each handle can register for this callback once.
 *
 * @param[in] pHandle The handle for which the adapter change notification was registered.
 *
 * @param[in] reason The reason/type of change (see @ref brawcap_adapter_notify_reason_t).
 *
 * @param[in] properties A bit field with the properties that have changed (see @ref brawcap_adapter_properties_t).
 *
 * @param[in] pUser Pointer to a user defined context.
 * It allows access to user specific data within the callback.
 */
typedef void (*brawcap_adapter_notify_handle_callback_t)(brawcap_handle_t* const pHandle,
  const brawcap_adapter_notify_reason_t reason, const UINT32 properties, void* pUser);

/**@}*/
#endif

/************************************************** RECEIVE **********************************************************/
#if 1
/**
 * @addtogroup brawcap_receive
 * @{
 */

/********************************************** RECEIVE FILTER *******************************************************/
#if 1
/**
 * @addtogroup brawcap_filter
 * @{
 */

/**@}*/
#endif

/**
 * @brief Max number of supported packet (receive) buffers per bRAWcap handle.
 *
 * Limits the number of buffers which can be attached to a single handle with @ref brawcap_rx_buffer_attach.
 */
#define BRAWCAP_RX_BUFFERS_PER_HANDLE_MAX 2

/**
 * @brief Receive callback which is called each time a receive has finished.
 * Receiving can be started with @ref brawcap_rx_start and has to be stopped with @ref brawcap_rx_stop.
 * Before anything can be received it is necessary to attach packet buffers to the bRAWcap handle.
 * This can be done by creating new bRAWcap buffers (@ref brawcap_buffer_create) and attach it with
 * @ref brawcap_rx_buffer_attach.
 *
 * @note If a handle has multiple buffers attached, the callback may be called in parallel (by different threads).
 *
 * @warning Executing to long in the callback can lead to a bottleneck in reception!
 * It is recommended that the driver always has at least one (free) buffer were it can store the packets to.
 *
 * @param[in] pHandle Handle on which the receive returned.
 *
 * @param[in] status The overall receive status see @ref brawcap_status_t for possible return values.
 *
 * @param[in] pBuffer Pointer to the buffer which has finished receiving.
 * It can be used to access all packets in the buffer and allows to retrieve information
 * for each received packet (e.g. payload, length, timestamp, ...).
 *
 * @param[in] pUser Pointer to a user defined context.
 * It allows access to user specific data within the callback.
 */
typedef void (*brawcap_rx_callback_t)(brawcap_handle_t* const pHandle, const brawcap_status_t status,
  brawcap_buffer_t* const pBuffer, void* pUser);

/**@}*/
#endif

/************************************************* TRANSMIT **********************************************************/
#if 1
/**
 * @addtogroup brawcap_transmit
 * @{
 */

/**
 * @brief Transmit callback which is called each time a transmission initiated with @ref brawcap_tx_buffer_send has
 * finished.
 *
 * @note Before any buffer can be transmitted the tranmission has to be started with @ref brawcap_tx_start and
 * stopped with @ref brawcap_tx_stop.
 *
 * @param[in] pHandle Handle on which the transmission has finished.
 *
 * @param[in] status The overall transmission status see @ref brawcap_status_t for possible return values.
 *
 * @param[in] pBuffer Pointer to the buffer which has finished transmission (initiated with
 * @ref brawcap_tx_buffer_send). It can be used to access all packets in the buffer - after transmission - and allows
 * to retrieve information about each packet (e.g. (transmission) status, timestamp, ...).
 *
 * @param[in] pUser Pointer to a user defined context (allows access to user specific data within the callback).
 */
typedef void (*brawcap_tx_callback_t)(brawcap_handle_t* const pHandle, const brawcap_status_t status,
  brawcap_buffer_t* const pBuffer, void* pUser);

/**@}*/
#endif

/*************************************************** STATS ***********************************************************/
#if 1
/**
 * @addtogroup brawcap_stats
 * @{
 */

/**@}*/
#endif

#endif // BRAWCAP_TYPES_UM_H
