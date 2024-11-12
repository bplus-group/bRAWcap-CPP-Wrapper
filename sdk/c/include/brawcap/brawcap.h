/**
 * @file brawcap.h
 *
 * @brief bRAWcap API - Functions.
 *
 * This file contains generic parts of the API and includes all specific bRAWcap parts.
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
#ifndef BRAWCAP_H
#define BRAWCAP_H

#include "brawcap_types_um.h"

#if defined(__cplusplus)
extern "C"
{
#endif //(__cplusplus)

/************************************************ LOGGING ************************************************************/
#if 1
/**
 * @addtogroup brawcap_logging
 * @{
 */

/**
 * @brief Sets the current log level for the given log type to the specified log level.
 *
 * @param[in] type Log type for which to configure the log level.
 *
 * @param[in] level The log level to be applied.
 *
 * @param[in] pKey Key required to activate debug logging.
 * For any other log level than @ref BRAWCAP_LOG_LEVEL_DEBUG this parameter is not required
 * and should be set to NULL.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Attempted to enable debug logging with wrong key.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | None of the specified log types is available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_log_level_set(const brawcap_log_type_t type, const brawcap_log_level_t level, const char* pKey);

/**
 * @brief Sets the current log levels for the given log types to the specified log levels.
 *
 * @param[in] type Log types for which to configure the log level(s).
 * Value has to be interpreted as bitfield.
 * This allows to (de)activate levels for several types with only one call.
 *
 * @param[in] level The log levels to be applied.
 * Value has to be interpreted as bitfield.
 * This allows to (de)activate several levels with only one call.
 *
 * @param[in] pKey Key required to activate debug logging.
 * For any other log level than @ref BRAWCAP_LOG_LEVEL_DEBUG this parameter is not required
 * and should be set to NULL.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Attempted to enable debug logging with wrong key.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | None of the specified log types is available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_log_levels_set(const UINT32 type, const UINT32 level, const char* pKey);

/**
 * @brief Reads the level(s) for the given log type.
 *
 * @param[in] type Log type for which to read the log level.
 * It is only possible to retrieve the log level for one type.
 * Call this function for each type to resolve all log levels.
 *
 * @param[out] pLevel Will contain the currently configured log level(s), afterwards.
 * This value shall be interpreted as bitfield, which allows to resolve all log levels for the type with only one call.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The specified log types is invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_log_level_get(const brawcap_log_type_t type, brawcap_log_level_t* const pLevel);

/**@}*/
#endif

/************************************************ GENERIC ************************************************************/
#if 1
/**
 * @addtogroup brawcap_generics
 * @{
 */

/**
 * @brief Opens a new bRAWcap handle on the adapter, specified by it´s name.
 *
 * The created/opened bRAWcap handle should be saved by the user application and used for calling further bRAWcap
 * functions. Each opened handle shall be @ref brawcap_close "closed", if it is not required anymore.
 *
 * @note On Windows a adapter name is always represented by a GUID.
 *
 * @note bRAWcap supports multiple handles to the same adapter.
 * But the total number of open handles on the same adapter is limited to @ref BRAWCAP_HANDLES_PER_ADAPTER_MAX.
 *
 * @warning Opening multiple handles to the same adapter, with the same receive filter leads to redundancy and
 * performance degradation and therefore should be avoided, if possible.
 *
 * @param[in] name The name of the adapter to open a bRAWcap handle.
 *
 * @param[out] pHandle Will contain the opened bRAWcap handle, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The specified name does not represent a adapter name (GUID).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Specified adapter name not found/not available.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_NOT_AVAILABLE | bRAWcap driver is not running on specified adapter.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_open(const brawcap_adapter_name_t name, brawcap_handle_t** const pHandle);

/**
 * @brief Closes the specified bRAWcap handle.
 *
 * The user application should not use the specified handle anymore, after calling this function.
 *
 * @warning Closing bRAWcap handles is not thread safe and the user is responsible for synchronization.
 *
 * @param[in] pHandle bRAWcap handle to be closed.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_FAILED               | A system error occurred.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_close(brawcap_handle_t* pHandle);

/**@}*/
#endif

/************************************************** STATUS ***********************************************************/
#if 1
/**
 * @addtogroup brawcap_status
 * @{
 */

/**
 * @brief Reads the last status appeared in bRAWcap, for the calling thread.
 *
 * The last status is only updated if it does not equal @ref BRAWCAP_STATUS_SUCCESS.
 * Which means that this function shall never be used to check if a bRAWcap function succeeded.
 * Instead this should always be checked on each bRAWcap function return value directly
 * and only if something different than @ref BRAWCAP_STATUS_SUCCESS is returned,
 * this function can be used to get the last status again.
 *
 * The last status may be overwritten when the calling thread calls another bRAWcap function.
 * It is therefore recommended to call this function directly after a bRAWcap function to get the status of that
 * function again.
 *
 * @note If yet, no other status than @ref BRAWCAP_STATUS_SUCCESS occurred for the calling thread,
 * @ref BRAWCAP_STATUS_SUCCESS will be returned.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | If no other status than success happened for the calling thread.
 * Any of @ref brawcap_status_t                   | Please see return of last called function for detailed info.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_last_status();

/**@}*/
#endif

/************************************************ VERSION ************************************************************/
#if 1
/**
 * @addtogroup brawcap_version
 * @{
 */

/**
 * @brief Returns loaded bRAWcap library version.
 *
 * @param[out] pVersion Will contain the loaded library version, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_version_api(brawcap_version_t* const pVersion);

/**
 * @brief Reads current version of loaded bRAWcap driver.
 *
 * @note The driver version is the same for each adapter.
 * Therefore, it doesn´t matter which handle/adapter is used to read the driver version.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pVersion Will contain the bRAWcap driver version, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_version_driver(brawcap_handle_t* const pHandle, brawcap_version_t* const pVersion);

/**@}*/
#endif

/*********************************************** TIMESTAMP ***********************************************************/
#if 1
/**
 * @addtogroup brawcap_timestamp
 * @{
 */

/**
 * @brief Reads out the mode of the specified timestamp.
 *
 * @param[in] pTimestamp The timestamp to read from.
 *
 * @param[out] pMode Will contain the timestamp mode, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_mode_get(brawcap_timestamp_t* const pTimestamp, brawcap_timestamp_mode_t* const pMode);

/**
 * @brief Reads out the timestamp resolution of the specified timestamp.
 *
 * @note If the resolution is unknown it will be set to zero (@ref BRAWCAP_TIMESTAMP_RESOLUTION_UNKNOWN)
 *
 * @param[in] pTimestamp The timestamp to read from.
 *
 * @param[out] pResolution_ns Will contain the timestamp resolution in nanoseconds, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_resolution_ns_get(brawcap_timestamp_t* const pTimestamp,
  brawcap_timestamp_resolution_ns_t* const pResolution_ns);

/**
 * @brief Reads out the timestamp value in seconds and nanoseconds.
 *
 * The values are based on 00:00 01.01.1970.
 *
 * The seconds part contains only whole seconds of the timestamp,
 * while the nanoseconds part contains only the fractional part.
 *
 * @param[in] pTimestamp The timestamp to read from.
 *
 * @param[out] pSeconds Will contain the whole seconds of the timestamp, afterwards.
 *
 * @param[out] pNanoseconds Will contain the fractional part in nanoseconds, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_ns_get(brawcap_timestamp_t* const pTimestamp, UINT64* const pSeconds,
  UINT32* const pNanoseconds);

/**
 * @brief Sets the timestamp value in seconds and nanoseconds.
 *
 * The values has to be based on 00:00 01.01.1970.
 *
 * @param[in] pTimestamp The timestamp to be modified.
 *
 * @param[in] seconds Whole seconds since 00:00 01.01.1970.
 *
 * @param[in] nanoseconds The fractional part in nanoseconds.
 * This parameter should only contain the fractional timestamp part.
 * That means that it can never exceed 999.999.999 nanoseconds.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Nanosecond value to high (only fraction of second).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_ns_set(brawcap_timestamp_t* const pTimestamp, const UINT64 seconds, const UINT32 nanoseconds);

/**
 * @brief Reads out the timestamp value in seconds and microseconds.
 *
 * The values are based on 00:00 01.01.1970.
 *
 * The seconds part contains only whole seconds of the timestamp,
 * while the microseconds contains only the fractional part.
 *
 * @param[in] pTimestamp The timestamp to read from.
 *
 * @param[out] pSeconds Will contain the whole seconds of the timestamp, afterwards.
 *
 * @param[out] pMicroseconds Will contain the fractional part in microseconds, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_us_get(brawcap_timestamp_t* const pTimestamp, UINT64* const pSeconds,
  UINT32* const pMicroseconds);

/**
 * @brief Sets the timestamp value in seconds and microseconds.
 *
 * The values has to be based on 00:00 01.01.1970.
 *
 * @param[in] pTimestamp The timestamp to be modified.
 *
 * @param[in] seconds Whole seconds since 00:00 01.01.1970.
 *
 * @param[in] microseconds The fractional part in microseconds.
 * This parameter should only contain the fractional timestamp part.
 * That means that it can never exceed 999.999 microseconds.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Nanosecond value to high (only fraction of second).
 *
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_us_set(brawcap_timestamp_t* const pTimestamp, const UINT64 seconds, const UINT32 microseconds);

/**
 * @brief Reads out the timestamp value in seconds and milliseconds.
 *
 * The values are based on 00:00 01.01.1970.
 *
 * The seconds part contains only whole seconds of the timestamp,
 * while the milliseconds part contains only the fractional part.
 *
 * @param[in] pTimestamp The timestamp to read from.
 *
 * @param[out] pSeconds Will contain the whole seconds of the timestamp, afterwards.
 *
 * @param[out] pMilliseconds Will contain the fractional part in milliseconds, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_ms_get(brawcap_timestamp_t* const pTimestamp, UINT64* const pSeconds,
  UINT32* const pMilliseconds);

/**
 * @brief Sets the timestamp value in seconds and milliseconds.
 *
 * The values has to be based on 00:00 01.01.1970.
 *
 * @param[in] pTimestamp The timestamp to be modified.
 *
 * @param[in] seconds Whole seconds since 00:00 01.01.1970.
 *
 * @param[in] milliseconds The fractional part in milliseconds.
 * This parameter should only contain the fractional timestamp part.
 * That means that it can never exceed 999 milliseconds.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Nanosecond value to high (only fraction of second).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_timestamp_value_ms_set(brawcap_timestamp_t* const pTimestamp, const UINT64 seconds, const UINT32 milliseconds);

/**@}*/
#endif

/************************************************ PACKET *************************************************************/
#if 1
/**
 * @addtogroup brawcap_packet
 * @{
 */

/**
 * @brief Creates a new packet.
 *
 * A packet can be used for @ref brawcap_rx_packet "receiving" (single packet based) or
 * @ref brawcap_tx_packet "transmitting"
 * (also single packet based).
 *
 * If a packet is not required anymore it should be @ref brawcap_packet_free "freed".
 *
 * @param[out] pPacket Will contain the new created packet, afterwards.
 *
 * @param[in] maxSize The maximum supported byte size for the packet payload.
 * This value cannot be changed after packet creation and determines how much memory is required for the packet.
 * The packet can only contain payloads up to this value.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Packet payload byte size to low/high.
 * @ref BRAWCAP_STATUS_ERROR_FAILED               | System error occurred (e.g. out of memory).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_create(brawcap_packet_t** const pPacket, const brawcap_packet_size_t maxSize);

/**
 * @brief Frees the specified packet.
 * When this function is called the specified packet becomes invalid and it´s memory is released.
 *
 * @warning Freeing a packet is not thread safe and the user is responsible for synchronization.
 *
 * @param[in] pPacket The packet to be freed.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_free(brawcap_packet_t* pPacket);

/**
 * @brief Reads out the status for the specified packet.
 * The status will be set updated during reception/transmission.
 * It indicates the status of the packet depending on the operation.
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pStatus Will contain the packet status, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_status_get(brawcap_packet_t* const pPacket, brawcap_status_t* const pStatus);

/**
 * @brief Reads out the @ref brawcap_timestamp "timestamp" object for the specified packet.
 *
 * @attention The timestamp is part of the packet itself.
 * Therefore the returned timestamp is only valid as long as the packet itself is valid.
 * The user application shall not store the timestamp object separately and only used it as long as the packet is valid.
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pTimestamp Will contain the timestamp object of the packet, afterwards.
 * This object can be used by the @ref brawcap_timestamp module.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_timestamp_get(brawcap_packet_t* const pPacket, brawcap_timestamp_t** const pTimestamp);

/**
 * @brief Reads out the length on wire of the specified packet.
 * This value is especially useful for received packets.
 * It indicates the total packet payload byte size of the packet on wire.
 * This may not equal to the @ref brawcap_packet_payload_size_get "captured payload byte size".
 * Because the payload byte size depends on the configured filter and it´s
 * @ref brawcap_filter_bytes_to_capture_set "bytes to capture" parameters.
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pLengthOnWire Will contain the payload byte length on wire, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_length_on_wire_get(brawcap_packet_t* const pPacket, brawcap_packet_size_t* const pLengthOnWire);

/**
 * @brief Reads out the max supported payload byte size of the specified packet.
 * This value was set during packet @ref brawcap_packet_create "creation"
 * (or @ref brawcap_buffer_create "buffer creation" if it´s a buffered packet).
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pMaxPayloadSize Will contain the max packet payload byte size, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_payload_max_size_get(brawcap_packet_t* const pPacket, brawcap_packet_size_t* const pMaxPayloadSize);

/**
 * @brief Reads out the captured payload byte size of the specified packet.
 * This indicates the total byte size of the (captured - if received) packet payload
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pLength Will contain the captured payload byte size, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_payload_size_get(brawcap_packet_t* const pPacket, brawcap_packet_size_t* const pLength);

/**
 * @brief Reads out the payload of the specified packet.
 *
 * The function returns a pointer to the beginning of the packet payload.
 *
 * @note The returned payload pointer should be only accessed as long as the packet is not freed.
 * If post processing is required a copy of the required payload should be done.
 *
 * @attention Accessing the packet payload is not thread safe and user is responsible for synchronization.
 *
 * @param[in] pPacket The packet to read from.
 *
 * @param[out] pPayload Will contain a pointer to the beginning of the packet payload, afterwards.
 *
 * @param[out] pLength Will contain the total length of the packet payload, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_payload_get(brawcap_packet_t* const pPacket, const char** const pPayload,
  brawcap_packet_size_t* const pLength);

/**
 * @brief Sets the packet payload of the specified packet.
 *
 * @note It is not allowed to set a larger payload than the max byte payload size
 * (specified during @ref brawcap_packet_create "packet creation" or
 * @ref brawcap_buffer_create "buffer creation").
 * If a larger payload is specified the function will fail and return an error.
 *
 * @param[in] pPacket The packet to be modified.
 *
 * @param[in] pPayload Pointer to the beginning of the payload to set.
 *
 * @param[in] length Total byte size of the payload to set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The specified payload size exceeds the packet max payload size.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_packet_payload_set(brawcap_packet_t* const pPacket, const char* const pPayload,
  const brawcap_packet_size_t length);

/**@}*/
#endif

/************************************************* BUFFER ************************************************************/
#if 1
/**
 * @addtogroup brawcap_buffer
 * @{
 */

/**
 * @brief Creates a new packet buffer.
 *
 * The created buffer can be used for:
 *  - Receiving by @ref brawcap_rx_buffer_attach "attaching" it.
 *  - Transmitting by @ref brawcap_tx_buffer_send "sending" it.
 *
 * If a buffer is not required anymore it shall be @ref brawcap_buffer_free "freed".
 *
 * @param[out] pBuffer Will contain the new created packet buffer, afterwards.
 *
 * @param[in] maxPacketPayloadSize Specifies the max payload byte size for each packet in the buffer.
 * This cannot be changed after buffer creation and controls the memory size required for each packet in the buffer.
 * When trying to set the packet payload of any buffered packet which exceeds this value, it will fail.
 * The value has to be between @ref BRAWCAP_PACKET_SIZE_MIN and @ref BRAWCAP_PACKET_SIZE_MAX.
 *
 * @param[in] numPackets Specifies how many packets can be stored in the buffer.
 * This value, together with the max packet payload size, will control the total amount of memory required for
 * the packet buffer and cannot be changed after buffer creation.
 * The value has to be between @ref BRAWCAP_BUFFER_PACKETS_MIN and @ref BRAWCAP_BUFFER_PACKETS_MAX.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The specified packet payload or number of packets is invalid.
 * @ref BRAWCAP_STATUS_ERROR_FAILED               | System error occurred (e.g. out of memory).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_create(brawcap_buffer_t** const pBuffer, const brawcap_packet_size_t maxPacketPayloadSize,
  const brawcap_buffer_packet_count_t numPackets);

/**
 * @brief Frees the specified packet buffer.
 * When this function is called the specified packet buffer becomes invalid and it´s memory will be released.
 *
 * @warning Freeing a buffer is not thread safe and the user is responsible for synchronization.
 *
 * @param[in] pBuffer The packet buffer to be freed.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_free(brawcap_buffer_t* pBuffer);

/**
 * @brief Clears the specified packet buffer.
 *
 * All buffered packets will be cleared.
 * It may be used to reuse a already created packet buffer.
 * For example this can be useful after transmission of the packet buffer has finished.
 * Reusing a buffer improves performance and should be preferred instead of always creating and freeing packet
 * buffers.
 *
 * @warning After calling this function all buffered packets in the packet buffer become invalid and shall not be used
 * anymore.
 *
 * @param[in] pBuffer The packet buffer to be cleared.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_clear(brawcap_buffer_t* const pBuffer);

/**
 * @brief Adds the specified packet at the end of the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to be modified.
 *
 * @param[in] pPacket The packet to insert into the buffer.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | The max number of packets for the buffer is reached.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The packet payload length is to long for the buffer.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_add_back(brawcap_buffer_t* const pBuffer, const brawcap_packet_t* const pPacket);

/**
 * @brief Adds the specified packet at the front of the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to be modified.
 *
 * @param[in] pPacket The packet to insert into the buffer.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | The max number of packets for the buffer is reached.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The packet payload length is to long for the buffer.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_add_front(brawcap_buffer_t* const pBuffer, const brawcap_packet_t* const pPacket);

/**
 * @brief Inserts the specified packet at index/position into the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to be modified.
 *
 * @param[in] pPacket The packet to insert into the buffer.
 *
 * @param[in] index Index at which the packet should be inserted.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | The max number of packets for the buffer is reached.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The packet payload length is to long for the buffer.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The given index is invalid (exceeds buffer).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_add_at_index(brawcap_buffer_t* const pBuffer, const brawcap_packet_t* const pPacket,
  const brawcap_buffer_packet_count_t index);

/**
 * @brief Reads out a buffered packet at the index of the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to read from.
 *
 * @param[in] index The index of the packet.
 *
 * @param[out] pPacket Will contain the buffered packet, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The given index is invalid (exceeds buffer).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_at_index(brawcap_buffer_t* const pBuffer, const brawcap_buffer_packet_count_t index,
  brawcap_packet_t** const pPacket);

/**
 * @brief Reads out the first buffered packet of the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to read from.
 *
 * @param[out] pPacket Will contain the buffered packet, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_front(brawcap_buffer_t* const pBuffer, brawcap_packet_t** const pPacket);

/**
 * @brief Reads out the last buffered packet of the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to read from.
 *
 * @param[out] pPacket Will contain the buffered packet, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_back(brawcap_buffer_t* const pBuffer, brawcap_packet_t** const pPacket);

/**
 * @brief Reads out the number of currently buffered packets in the specified packet buffer.
 *
 * @param[in] pBuffer The packet buffer to read from.
 *
 * @param[out] pPacketCount Will contain the number of buffered packets, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_count(brawcap_buffer_t* const pBuffer, brawcap_buffer_packet_count_t* const pPacketCount);

/**
 * @brief Reads out the total number of packets which can be buffered by the specified packet buffer.
 * This value represents the packet buffer capacity, which is set during buffer creation.
 *
 * @param[in] pBuffer The packet buffer to read from.
 *
 * @param[out] pPacketCapacity Will contain the total number of packets which can be buffered, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_capacity(brawcap_buffer_t* const pBuffer, brawcap_buffer_packet_count_t* const pPacketCapacity);

/********************************************* BUFFER ITERATOR ********************************************************/
#if 1
/**
 * @addtogroup brawcap_buffer_iterator
 * @{
 */

/**
 * @brief Creates a new iterator for the specified buffer.
 *
 * If a iterator is not required anymore it shall be @ref brawcap_buffer_iterator_free "freed".
 *
 * @param[out] pIterator Will contain the new created iterator.
 *
 * @param[in] pBuffer The buffer for which to create the iterator.
 *
 * @param[in] startPosition The start position of the iterator (packet buffer index/position).
 * The specified position must be below the number of currently buffered packets,
 * which can be resolved with @ref brawcap_buffer_count.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | Buffer is still in use/pending (receive/transmit).
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | Start position exceeds number of buffered packets.
 * @ref BRAWCAP_STATUS_ERROR_FAILED               | System error occurred (e.g. out of memory).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_iterator_create(brawcap_buffer_iterator_t** const pIterator, brawcap_buffer_t* const pBuffer,
  brawcap_buffer_packet_count_t startPosition);

/**
 * @brief Frees the specified iterator.
 * When this function is called the specified iterator becomes invalid and it´s memory will be released.
 *
 * @warning Freeing a iterator is not thread safe and the user is responsible for synchronization.
 *
 * @param[in] pIterator The iterator to be freed.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_iterator_free(brawcap_buffer_iterator_t* const pIterator);

/**
 * @brief Increments the iterator to the next buffered packet.
 *
 * @param[in] pIterator The iterator to be incremented.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_iterator_next(brawcap_buffer_iterator_t* const pIterator);

/**
 * @brief Decreases the iterator to the previous buffered packet.
 *
 * @param[in] pIterator The iterator to be decremented.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_buffer_iterator_prev(brawcap_buffer_iterator_t* const pIterator);

/**
 * @brief Returns the buffered packet for the specified iterator.
 *
 * @param[in] pIterator The iterator to read from.
 *
 * @return
 * Pointer                                        | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref brawcap_packet_t                          | On success a valid bRAWcap packet object pointer.
 * NULL                                           | Invalid iterator (e.g. beyond the last or first packet).
 */
_BRAWCAP_API_FUNC(brawcap_packet_t*)
brawcap_buffer_iterator_eval(brawcap_buffer_iterator_t* const pIterator);

/**@}*/
#endif

/**@}*/
#endif

/************************************************ ADAPTER ************************************************************/
#if 1
/**
 * @addtogroup brawcap_adapter
 * @{
 */

/**
 * @brief Updates the list of supported adapters.
 * This function searches the current machine for supported adapters and add them to the adapter list.
 *
 * @note The list will be initialized when calling the first bRAWcap function and not update automatically after that.
 * To update the list later on use this function.
 * A alternative to detect new/removed adapters is
 * @ref brawcap_adapter_notify_register "registering for change notifications".
 *
 * @attention The adapter list is not thread safe.
 * Therefore parallel calls to this function, @ref brawcap_adapter_list_count and @ref brawcap_adapter_list_at
 * should be avoided or at least synchronized.
 * If the list is changed by this function and another thread is currently retrieving information from the list
 * it may lead to undefined behavior or even a crash if the number of entries is reduced.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_list_update();

/**
 * @brief Reads out the current number of supported adapters in the adapter list.
 *
 * @note The count is only updated after a call to @ref brawcap_adapter_list_update.
 *
 * @attention Updating the list and retrieving information from it is not thread safe.
 * Therefore accessing it from different threads shall be avoided or synchronized.
 *
 * @param[out] pCount Will contain the current number of adapters in the list, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_list_count(brawcap_adapter_count_t* const pCount);

/**
 * @brief Reads out the adapter name of the adapter at the adapter list index.
 *
 * @note The adapter list is only updated after a call to @ref brawcap_adapter_list_update.
 *
 * @attention Updating the list and retrieving information from it is not thread safe.
 * Therefore accessing it from different threads shall be avoided or synchronized.
 *
 * @param[in] index The index of the adapter in the adapter list..
 *
 * @param[out] name Will contain the name of the adapter, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Specified index exceeds adapter list length.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_list_at(const brawcap_adapter_count_t index, brawcap_adapter_name_t name);

/**
 * @brief Registers the calling process for receiving adapter change notifications.
 *
 * The given callback function will be called each time a adapter change is recognized by bRAWcap for any of the
 * supported adapters on the current machine.
 *
 * @attention Blocking for long periods within the callback function can lead to incorrect behavior
 * and should be avoided.
 * If you need to process a lot on adapter changes it is recommended to do the processing in
 * another thread rather than directly within the callback.
 *
 * @note Calls to the registered callback function are serialized.
 * This means that the callback can not be called in parallel and
 * one call has to be finished before another one is received.
 *
 * @note Each process can only register for notifications once.
 *
 * @param[in] callback Function pointer to the callback function which should be executed each time
 * a adapter change is registered.
 *
 * @param[in] pUser Pointer to a user defined context.
 * This allows access to user specific data within the callback.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_INFO_ALREADY_REGISTERED    | Calling process has already registered for this notification.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_notify_register(brawcap_adapter_notify_callback_t const callback, void* const pUser);

/**
 * @brief Unregisters the calling process from receiving adapter change notifications.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_INFO_NOT_REGISTERED        | Calling process was not registered for this notification.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_notify_unregister();

/**
 * @brief Registers the specified handle for receiving adapter change notifications.
 *
 * This registration only receives adapter change notifications related to the underlying adapter of the given
 * bRAWcap handle.
 * Whereas @ref brawcap_adapter_notify_register notifies about adapter changes for all supported adapters on the
 * current machine.
 *
 * Each bRAWcap handle can register for this notification once.
 *
 * @attention Blocking for long periods within the callback function can lead to incorrect behavior
 * and should be avoided. If you need to process a lot on adapter changes it is recommended to do the processing in
 * another thread rather than directly within the callback.
 *
 * @note Calls to the registered callback function are serialized.
 * This means that the callback can not be called in parallel and
 * one call has to be finished before another one is received.
 *
 * @param[in] pHandle Handle to register for adapter notifications.
 *
 * @param[in] pUser Pointer to a user defined context.
 * This allows access to user specific data within the callback.
 *
 * @param[in] callback Function pointer to the callback function which should be executed each time
 * a adapter change is registered.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_INFO_ALREADY_REGISTERED    | Specified handle has already registered for this notification.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_notify_handle_register(brawcap_handle_t* const pHandle,
  brawcap_adapter_notify_handle_callback_t const callback, void* const pUser);

/**
 * @brief Unregisters the specified handle from receiving adapter change notifications.
 *
 * @param[in] pHandle Handle to unregister for adapter notifications.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_INFO_NOT_REGISTERED        | Specified handle was not registered for this notification.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_notify_handle_unregister(brawcap_handle_t* const pHandle);

/**
 * @brief Reads out the adapter name of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] name Will contain the adapter name, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_name_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_name_t name);

/**
 * @brief Reads out the currently set adapter friendly name for the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] friendlyName Pointer to a buffer which will contain the currently set adapter friendly name, afterwards.
 * This parameter can be NULL.
 * If it is NULL the function will set the required buffer length in pLength and
 * return @ref BRAWCAP_STATUS_ERROR_OVERRUN.
 *
 * @param[inout] pLength  Length of the provided buffer.
 * If the specified length is to short,
 * it will be set to the required length and @ref BRAWCAP_STATUS_ERROR_OVERRUN is returned.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | Given buffer length was to short.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_friendly_name(const brawcap_adapter_name_t name, char* const friendlyName, UINT32* const pLength);

/**
 * @brief Reads out the currently set friendly name of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] friendlyName Pointer to a buffer which will contain the currently set friendly name, afterwards.
 * This parameter can be NULL.
 * If it is NULL the function will set the required buffer length in pLength and
 * return @ref BRAWCAP_STATUS_ERROR_OVERRUN.
 *
 * @param[inout] pLength Length of the provided buffer.
 * If the specified length is to short,
 * it will be set to the required length and @ref BRAWCAP_STATUS_ERROR_OVERRUN is returned.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | Given buffer length was to short.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_friendly_name_by_handle(brawcap_handle_t* const pHandle, char* const friendlyName,
  UINT32* const pLength);

/**
 * @brief Reads out the description of the specified adapter.
 *
 * @note Adapter descriptions are set depending on the loaded network adapter driver.
 * This means it will not change as long as the network driver is not changed.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] description Pointer to a buffer which will contain the description, afterwards.
 * This parameter can be NULL.
 * If it is NULL the function will set the required buffer length in pLength and
 * return @ref BRAWCAP_STATUS_ERROR_OVERRUN.
 *
 * @param[inout] pLength Length of the provided buffer.
 * If the specified length is to short,
 * it will be set to the required length and @ref BRAWCAP_STATUS_ERROR_OVERRUN is returned.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | Given buffer length was to short.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_description(const brawcap_adapter_name_t name, char* const description, UINT32* const pLength);

/**
 * @brief Reads out the description of a network adapter based on the specified adapter.
 *
 * @note Adapter descriptions are set depending on the loaded network adapter driver.
 * This means it will not change as long as the network driver is not changed.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] description Pointer to a buffer which will contain the description, afterwards.
 * This parameter can be NULL.
 * If it is NULL the required buffer length will be set for pLength and the
 * function will return @ref BRAWCAP_STATUS_ERROR_OVERRUN.
 *
 * @param[inout] pLength Length of the provided buffer.
 * If the specified length is to short,
 * it will be set to the required length and @ref BRAWCAP_STATUS_ERROR_OVERRUN is returned.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_OVERRUN              | Given buffer length was to short.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_description_by_handle(brawcap_handle_t* const pHandle, char* const description, UINT32* const pLength);

/**
 * @brief Reads out the currently set IPv4 address of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pIpv4 Will contain the currently set IPv4 address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_ipv4(const brawcap_adapter_name_t name, brawcap_adapter_ipv4_t* const pIpv4);

/**
 * @brief Reads out the currently set IPv4 address of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pIpv4 Will contain the currently set IPv4 address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_ipv4_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_ipv4_t* const pIpv4);

/**
 * @brief Reads out the currently set IPv6 address of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pIpv6 Will contain the currently set IPv6 address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_ipv6(const brawcap_adapter_name_t name, brawcap_adapter_ipv6_t* const pIpv6);

/**
 * @brief Reads out the currently set IPv6 address of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pIpv6 Will contain the currently set IPv6 address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_ipv6_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_ipv6_t* const pIpv6);

/**
 * @brief Reads out the MAC address of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] mac Will contain the MAC address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_mac(const brawcap_adapter_name_t name, brawcap_adapter_mac_t mac);

/**
 * @brief Reads out the MAC address of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] mac Will contain the MAC address, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_mac_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_mac_t mac);

/**
 * @brief Reads out the current receive speed of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pSpeed Will contain the current receive speed, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_speed_rx(const brawcap_adapter_name_t name, brawcap_adapter_speed_t* const pSpeed);

/**
 * @brief Reads out the current receive speed of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pSpeed Will contain the current receive speed, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_speed_rx_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_speed_t* const pSpeed);

/**
 * @brief Reads out the current transmit speed of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pSpeed Will contain the current transmit speed, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_speed_tx(const brawcap_adapter_name_t name, brawcap_adapter_speed_t* const pSpeed);

/**
 * @brief Reads out the current transmit speed of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pSpeed Will contain the current transmit speed, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_speed_tx_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_speed_t* const pSpeed);

/**
 * @brief Reads out the current connection state of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pConnection Will contain the current connection state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_connection(const brawcap_adapter_name_t name, brawcap_adapter_connection_state_t* const pConnection);

/**
 * @brief Reads out the current connection state of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pConnection Will contain the current connection state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_connection_by_handle(brawcap_handle_t* const pHandle,
  brawcap_adapter_connection_state_t* const pConnection);

/**
 * @brief Reads out the current operation state of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pOperation Will contain the current operation state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_operation(const brawcap_adapter_name_t name, brawcap_adapter_operation_state_t* const pOperation);

/**
 * @brief Reads out the current operation state of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pOperation Will contain the current operation state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_operation_by_handle(brawcap_handle_t* const pHandle,
  brawcap_adapter_operation_state_t* const pOperation);

/**
 * @brief Reads out the configured MTU of the specified adapter.
 *
 * @param[in] name Name of the adapter.
 *
 * @param[out] pMtu Will contain the configured MTU, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_NOT_FOUND            | Adapter name not found/not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_mtu(const brawcap_adapter_name_t name, brawcap_adapter_mtu_t* const pMtu);

/**
 * @brief Reads out the configured MTU of the specified adapter.
 *
 * @param[in] pHandle The bRAWcap handle to read from.
 *
 * @param[out] pMtu Will contain the configured MTU, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Not available in demo mode (no license).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_adapter_mtu_by_handle(brawcap_handle_t* const pHandle, brawcap_adapter_mtu_t* const pMtu);

/**@}*/
#endif

/************************************************ RECEIVE ************************************************************/
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

/**
 * @brief Creates a new filter.
 *
 * The created filter can be used for filtering received packets by applying it to a bRAWcap handle with
 * @ref brawcap_rx_filter_set.
 *
 * If a created filter is not required anymore it should be freed with @ref brawcap_filter_free.
 *
 * @note Currently only the @ref BRAWCAP_FILTER_TYPE_BYTE_MASK filter type is supported.
 * Support for @ref BRAWCAP_FILTER_TYPE_BPF may be added later on.
 *
 * @param[in] pFilter Will contain the new created filter.
 *
 * @param[in] type Specifies the type of the created filter.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Specified filter type is not supported.
 * @ref BRAWCAP_STATUS_ERROR_FAILED               | System error occurred (e.g. out of memory).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_create(brawcap_filter_t** const pFilter, const brawcap_filter_type_t type);

/**
 * @brief Frees the specified filter.
 * When this function is called the given filter becomes invalid and it´s memory will be released.
 *
 * @warning Freeing a filter is not thread safe and the user is responsible for synchronization.
 *
 * @param[in] pFilter The filter to be freed.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_free(brawcap_filter_t* pFilter);

/**
 * @brief Reads out the type for the specified filter.
 *
 * @param[in] pFilter The filter to read from.
 *
 * @param[out] pType Will contain the filter type, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_type_get(brawcap_filter_t* const pFilter, brawcap_filter_type_t* const pType);

/**
 * @brief Sets the filter parameters for the specified filter.
 *
 * @note Changes will only effect after applying the modified filter to a handle with @ref brawcap_rx_filter_set.
 *
 * @param[in] pFilter The filter to be modified.
 *
 * @param[in] offset Byte offset of the filter.
 * This value specifies at which packet byte offset the first filter byte shall be applied.
 *
 * @param[in] length Byte length of the filter.
 * Specifies the number of bytes used by the filter.
 * This value is limited to @ref BRAWCAP_FILTER_BYTE_MAX_LENGTH.
 *
 * @param[in] mask The byte mask for the filter.
 * This is a fixed size byte array.
 * Set the byte values to the filter you want to apply.
 * Only bytes up to the specified filter length are used and following bytes will be ignored.
 *
 * @param[in] ignoreBits A fixed size bitfield byte array.
 * The bytes in this array has to be interpreted/used as bitfields.
 * This allows to select if a single bit at the corresponding mask position shall be ignored.
 * To ignore the value of a bit set it´s value to 1.
 * Like the mask, only bytes up to the specified filter length are used and following bytes will be ignored.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | Specified filter is not of type @ref BRAWCAP_FILTER_TYPE_BYTE_MASK.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Specified offset+length exceeds @ref BRAWCAP_FILTER_BYTE_MAX_LENGTH.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_mask_set(brawcap_filter_t* const pFilter, const brawcap_filter_byte_length_t offset,
  const brawcap_filter_byte_length_t length, const brawcap_filter_mask_array_t mask,
  const brawcap_filter_ignore_bits_array_t ignoreBits);

/**
 * @brief Reads out the filter mask parameters for the specified filter.
 *
 * @param[in] pFilter The filter to read from.
 *
 * @param[out] pOffset Will contain the filter byte offset, afterwards.
 *
 * @param[out] pLength Will contain the filter byte length, afterwards.
 *
 * @param[out] pMask Will contain the filter mask bytes, afterwards.
 *
 * @param[out] pIgnoreBits Will contain the filter bits to ignore, afterwards
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | Specified filter is not of type @ref BRAWCAP_FILTER_TYPE_BYTE_MASK.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_mask_get(brawcap_filter_t* const pFilter, brawcap_filter_byte_length_t* const pOffset,
  brawcap_filter_byte_length_t* const pLength, brawcap_filter_mask_array_t* const pMask,
  brawcap_filter_ignore_bits_array_t* const pIgnoreBits);

/**
 * @brief Sets the specified filter to active.
 *
 * @note Changes will only effect after applying the modified filter to a handle with @ref brawcap_rx_filter_set.
 *
 * @param[in] pFilter The filter to be modified.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_activate(brawcap_filter_t* const pFilter);

/**
 * @brief Sets the specified filter to inactive.
 *
 * @note Changes will only effect after applying the modified filter to a handle with @ref brawcap_rx_filter_set.
 *
 * @param[in] pFilter The filter to be modified.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_deactivate(brawcap_filter_t* const pFilter);

/**
 * @brief Reads out the current activation state of the specified filter.
 *
 * @param[in] pFilter The filter to read from.
 *
 * @param[out] pActive Will contain the activation state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_is_activated(brawcap_filter_t* const pFilter, BOOLEAN* const pActive);

/**
 * @brief Sets the bytes to capture of the specified filter.
 *
 * @note Changes will only effect after applying the modified filter to a handle with @ref brawcap_rx_filter_set.
 *
 * @param[in] pFilter The filter to be modified.
 *
 * @param[in] offset The byte offset, from packet payload beginning, to begin capturing the payload bytes.
 *
 * @param[in] length The number of bytes to be captured - beginning at offset.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Specified offset+length exceeds @ref BRAWCAP_PACKET_SIZE_MAX.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_bytes_to_capture_set(brawcap_filter_t* const pFilter, const brawcap_packet_size_t offset,
  const brawcap_packet_size_t length);

/**
 * @brief Reads out the configured bytes to capture of the specified filter.
 *
 * @param[in] pFilter The filter to read from.
 *
 * @param[out] pOffset Will contain the byte offset of the bytes to capture, afterwards.
 *
 * @param[out] pLength Will contain the number of bytes to capture, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_bytes_to_capture_get(brawcap_filter_t* const pFilter, brawcap_packet_size_t* const pOffset,
  brawcap_packet_size_t* const pLength);

/**
 * @brief Specifies if packets which matched the specified filter should be indicated to the network stack.
 *
 * @note Changes will only effect after applying the modified filter to a handle with @ref brawcap_rx_filter_set.
 *
 * @param[in] pFilter The filter to be modified.
 *
 * @param[in] indicate Flag specifying the network stack indication.
 * If true filtered packets are forwarded to the network stack.
 * If false filtered packets are not forwarded to the network stack, they are only visible for bRAWcap handles.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_indicate_set(brawcap_filter_t* const pFilter, const BOOLEAN indicate);

/**
 * @brief Reads out the current network stack indication state of the specified filter.
 *
 * @param[in] pFilter The filter to read from.
 *
 * @param[out] pIndicate Will contain the current network stack indication, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | ------------------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_filter_indicate_get(brawcap_filter_t* const pFilter, BOOLEAN* const pIndicate);

/**@}*/
#endif

/**
 * @brief Receives a single packet from the specified handle.
 *
 * The caller must provide a valid packet were bRAWcap will store the received data.
 * To create a packet use @ref brawcap_packet_create or use a buffered packet which was returned by a
 * valid buffer with @ref brawcap_buffer_at_index.
 *
 * @note Parallel receives per handle are not allowed. This means that if there is any other receive pending on the
 * handle each further initiated receive will fail until the pending operation has finished.
 *
 * @param[in] pHandle bRAWcap handle to receive from.
 *
 * @param[out] pPacket Packet were the received data will be stored.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_WARNING_PENDING            | Another receive is still pending/active.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_NOT_AVAILABLE | bRAWcap driver is not active (anymore) on the adapter.
 * @ref BRAWCAP_STATUS_WARNING_DEMO_MODE          | Running in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 * @ref BRAWCAP_STATUS_INFO_NO_DATA               | Returned without any data received (due to timeout).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_packet(brawcap_handle_t* const pHandle, brawcap_packet_t* const pPacket);

/**
 * @brief Starts the internal receive loop.
 *
 * To receive data using this method it is necessary to attach memory to the
 * bRAWcap handle were received data can be stored. This can be done by attaching (with @ref brawcap_rx_buffer_attach)
 * a previously created bRAWcap packet buffer (with @ref brawcap_buffer_create).
 *
 * When a reception has finished (due to timeout, min packets or full buffer) the receive callback will be called
 * for this handle (@ref brawcap_rx_callback_t). In this callback the user application can do it´s post
 * processing on the received data. If the user application does not detach the buffer
 * (with @ref brawcap_rx_buffer_detach) it will be reused for receiving after callback has completed.
 *
 * The user application should do it´s post processing anyway, as fast as possible. Depending on the current
 * bandwidth the attached buffers may be filled up very fast. If the user application takes to long it
 * will lead to a bottleneck in the reception and the driver runs out of resources. This may result in performance
 * degradation or packet loss.
 *
 * It is possible to attach more than only one buffer to a single handle. This ensures that the bRAWcap driver can
 * toggle the buffers during the reception. This reduces the time gap the driver has to buffer data internally (because
 * there is no user buffer available). With more than one buffer the driver can still fill up another buffer while the
 * user application does it´s post processing. Therefore in general, using more than one buffer increases the
 * performance.
 *
 * @note Due to the focus on optimized throughput with minimal CPU load bRAWcap does not fill up the packets in the
 * buffer with the same sequence they were received by the adapter.
 * Here we want to give some additional information on the topic of "packet sequence/timestamp":
 *  - First of all it should be known that the receive sequence may not be equal to the sending sequence.
 *    Due to several senders or some network managers in between (switches and so on...).
 *    For any protocols which require a ordered packet stream, there will be at least one field in the packet
 *    payload which allows to sort the packets on the receiver side.
 *    This is a "higher" layer topic and not part of bRAWcap.
 *  - With the bRAWcap supported timestamp mechanisms it is possible to sort the received packets corresponding to
 *    their receive timestamps. But keep in mind that only the @ref BRAWCAP_TIMESTAMP_MODE_ADAPTER_HARDWARE guarantees
 *    that the receive timestamps really match the received packet sequence on the adapter. To use the named timestamp
 *    mechanism, the underlying network adapter must support it. Any other timestamp is generated by software. Windows
 *    is not a realtime operation system, and therefore due to multithreading, scheduling (...) it could happen
 *    that some packets are processed before other packets which was received earlier.
 *    Especially the Windows network stack feature called "receive side scaling" can also increase the issue
 *    of incorrect packet sequences.
 *
 * @invariant The user parameter allows access to user defined context.
 * The given pointer is not used and validated by bRAWcap. Therefore the user is responsible for validating
 * the context in the callback before accessing it.
 *
 * @attention After starting receive @ref brawcap_rx_packet can not be used until you stop the receive
 * with @ref brawcap_rx_stop, because parallel receives are not allowed.
 *
 * @param[in] pHandle bRAWcap handle to receive from.
 *
 * @param[in] callback Function pointer to user application receive callback.
 *
 * @param[in] pUser Pointer to a user defined context. Can be NULL if no user context is required.
 *
 * @param[in] indicateNoPackets Specifies if callback is called even without any received data.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_WARNING_PENDING            | Another receive is pending or reception is already started.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_start(brawcap_handle_t* const pHandle, brawcap_rx_callback_t const callback, void* const pUser,
  const BOOLEAN indicateNoPackets);

/**
 * @brief Stops the internal receive loop.
 *
 * @param[in] pHandle bRAWcap handle to stop reception.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_INFO_NOT_RUNNING           | There was no receive running.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_stop(brawcap_handle_t* const pHandle);

/**
 * @brief Attaches the specified bRAWcap packet buffer to the specified handle.
 *
 * After attaching a packet buffer to a handle it will be used for receiving data on that handle.
 * Data is only received if the internal receive loop was started/is running (with @ref brawcap_rx_start).
 *
 * The buffer is locked until:
 *  - It is detached from the handle with @ref brawcap_rx_buffer_detach.
 *  - A receive has finished and the receive callback is called (buffer is not locked during callback execution).
 *
 * @note A buffer can be attached only once and is than locked down by the handle it is attached to.
 * This means that it has to be detached before it can be used again for another handle.
 *
 * @param[in] pHandle bRAWcap handle for attaching the buffer.
 *
 * @param[in] pBuffer Packet buffer to be attached.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | The buffer is already in use / locked down by another operation.
 * @ref BRAWCAP_STATUS_WARNING_LIMIT_REACHED      | Limitation of buffer per handle reached (@ref BRAWCAP_RX_BUFFERS_PER_HANDLE_MAX)
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_buffer_attach(brawcap_handle_t* const pHandle, brawcap_buffer_t* const pBuffer);

/**
 * @brief Detaches the specified bRAWcap packet buffer from the specified handle.
 *
 * After detaching a buffer it is unlocked and can be accessed/modified.
 * It can also be reused for another operation or freed with @ref brawcap_buffer_free if it is not required anymore.
 *
 * @param[in] pHandle bRAWcap handle from which the buffer is to be detached.
 *
 * @param[in] pBuffer Packet buffer to be detached.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_INFO_NOT_ATTACHED          | The buffer was not attached.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_buffer_detach(brawcap_handle_t* const pHandle, brawcap_buffer_t* const pBuffer);

/**
 * @brief Sets the receive direction for the specified handle.
 *
 * Possible directions are:
 *  - @ref BRAWCAP_RX_DIRECTION_UNKNOWN
 *  - @ref BRAWCAP_RX_DIRECTION_IN
 *  - @ref BRAWCAP_RX_DIRECTION_OUT
 *  - @ref BRAWCAP_RX_DIRECTION_BOTH
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] direction The direction to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_direction_set(brawcap_handle_t* const pHandle, const brawcap_rx_direction_t direction);

/**
 * @brief Reads out the currently configured receive direction of the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pDirection Will contain the currently configured receive direction, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_direction_get(brawcap_handle_t* const pHandle, brawcap_rx_direction_t* const pDirection);

/**
 * @brief Sets the minimum amount of packets to receive for the specified handle.
 *
 * This value influences how often the bRAWcap driver will return pending receives.
 * It forces the driver to stay in the request as long as not the minimum amount of packets were captured.
 * Setting a higher value here can reduce the overall system load but increases also the responsiveness/latency
 * when the packet was received by the adapter and the indication of the packet to the caller.
 * The initial/default value is set to @ref BRAWCAP_RX_COPY_PACKETS_DEFAULT.
 *
 * @invariant If high receive rates should be reached (and many packets coming in), a high value is preferred.
 * Otherwise if low latency is required the best effort would be to set this value to zero.
 *
 * @note Independent from this value the receive will at least return if it pends for 10 times the configured
 * receive timeout. This avoid infinite blocking (infinite blocking is only the possible if the receive timeout is zero).
 * If this value is set to zero it will always return after the first receive timeout.
 * And if the given buffer becomes filled up before the timeout is reached it will return when the buffer is full.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] minPackets The minimum amount of packets to receive.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_min_packets_set(brawcap_handle_t* const pHandle, const brawcap_rx_min_packets_t minPackets);

/**
 * @brief Reads out the currently configured number of packets to receive for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pMinPackets Will contain the configured minimum amount of packets per receive, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_min_packets_get(brawcap_handle_t* const pHandle, brawcap_rx_min_packets_t* const pMinPackets);

/**
 * @brief Configures the receive timeout for the specified handle.
 *
 * It´s guaranteed that receive functions will return at least after the specified timeout value,
 * if the configured amount of packets value @ref brawcap_rx_min_packets_t is reached before.
 * If the configured amount of packets are not reached until the timeout occurs, the driver will stay in the request
 * for up to 10 times of the specified timeout time until it final returns.
 * It is also possible to specify a timeout of zero. A timeout of zero is equivalent to "no timeout".
 *
 * @note Default timeout value is @ref BRAWCAP_RX_TIMEOUT_MS_DEFAULT.
 * And the max supported timeout is @ref BRAWCAP_RX_TIMEOUT_MS_MAX.
 *
 * @attention Be careful in using infinite timeouts.
 * It could lead to a endless blocking behaviour.
 * For example if there are no packets received which meet your configured filter on the underlying adapter.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] timeout_ms Receive timeout in milliseconds to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The specified timeout value is invalid/out of range.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_timeout_set(brawcap_handle_t* const pHandle, const brawcap_rx_timeout_t timeout_ms);

/**
 * @brief Reads out the currently configured receive timeout for the specified handle
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pTimeout_ms Will contain the configured receive timeout in milliseconds, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_timeout_get(brawcap_handle_t* const pHandle, brawcap_rx_timeout_t* const pTimeout_ms);

/**
 * @brief Sets the specified filter to the specified handle.
 *
 * For more information on filters see @ref brawcap_filter_t.
 *
 * @note Each handle can have exactly one filter applied.
 * If several filters shall be applied to the same adapter,
 * it is required to open several bRAWcap handles to the same adapter.
 *
 * @param[in] pHandle bRAWcap handle to be modified.
 *
 * @param[in] pFilter The filter to set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Specified filter type is not available.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_filter_set(brawcap_handle_t* const pHandle, const brawcap_filter_t* const pFilter);

/**
 * @brief Reads out the currently configured filter for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pFilter Will contain the currently configured filter, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_filter_get(brawcap_handle_t* const pHandle, brawcap_filter_t* const pFilter);

/**
 * @brief Sets the current promiscuous mode flag for the specified handle.
 *
 * If a adapter is set into promiscuous mode it receives not only packages which directly address the adapter.
 * This can be used for recording/sniffing purposes but increases the local system load.
 * Promiscuous mode is a network adapter feature and which can only be modified by bRAWcap.
 * Therefore it is not specific to a bRAWcap handle. Instead it is a per adapter setting which can also be
 * modified from another application.
 *
 * @note Promiscuous mode will be reset automatically upon deleting the corresponding handle.
 * Which means if it was activated due to the bRAWcap handle it will be disabled again when the handle is closed.
 *
 * @attention Because of the per adapter scope, promiscuous mode can not explicitly modified by bRAWcap.
 * Instead if at least one application or another bRAWcap handle activated it, it will stay enabled until the last
 * application/handle which has activated it, is closed or deactivates it again.
 *
 * @param[in] pHandle bRAWcap handle to be modified.
 *
 * @param[in] enable Promiscuous mode state to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_promiscuous_mode_set(brawcap_handle_t* const pHandle, const BOOLEAN enable);

/**
 * @brief Reads out the current promiscuous mode state for the specified handle.
 *
 * Additionally to the promiscuous state of the handle itself it also reads the current promiscuous state of to the
 * underlying adapter.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pEnabled Will contain the current promiscuous mode state of the handle, afterwards.
 *
 * @param[out] pEnabledAdapter Will contain the current promiscuous mode state of the underlying adapter, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_promiscuous_mode_get(brawcap_handle_t* const pHandle, BOOLEAN* const pEnabled,
  BOOLEAN* const pEnabledAdapter);

/**
 * @brief Reads out the available receive timestamp modes for the specified handle.
 *
 * Some of the timestamp capabilites depend on the features provided by the underlying adapter.
 * Those modes are named with "adapter".
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pCapabilities Will contain the available receive timestamp modes, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_timestamp_capabilities(brawcap_handle_t* const pHandle,
  brawcap_timestamp_capabilities_t* const pCapabilities);

/**
 * @brief Reads out the currently configured receive time stamping mode for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pMode Will contain the currently used receive time stamping mode, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_timestamp_mode_get(brawcap_handle_t* const pHandle, brawcap_timestamp_mode_t* const pMode);

/**
 * @brief Configures the used receive time stamping mode for the specified handle.
 *
 *
 * @note Only available timestamp modes are accepted. It is recommended to check if a timestamp mode is supported
 * before setting it. To check if a timestamp mode is available use @ref brawcap_rx_timestamp_capabilities.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[out] mode The receive timestamp mode to be configured.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Specified timestamp mode not available.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_timestamp_mode_set(brawcap_handle_t* const pHandle, const brawcap_timestamp_mode_t mode);

/**
 * @brief Sets the current VLAN tagging flag for the specified handle.
 *
 * If VLAN tagging is enabled, the VLAN Header (defined by IEEE 802.1Q) will be part of the packet payload.
 * By default, the VLAN header is not included in the packet payload.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] enable VLAN tagging state to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_vlan_tagging_set(brawcap_handle_t* const pHandle, const BOOLEAN enable);

/**
 * @brief Reads out the configured VLAN tagging state for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pEnabled Will contain the currently configured VLAN tagging state, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_vlan_tagging_get(brawcap_handle_t* const pHandle, BOOLEAN* const pEnabled);

/**
 * @brief Sets the driver queue size (in number of packets) for the specified handle.
 *
 * A larger driver queue can avoid packet drops for time gaps were no packet buffer is available.
 * But setting large driver queues should only be done if really necessary, because it can consume a lot of memory.
 * Because driver queues exist per handle, the total amount of consumed memory has to summed up for all open handles
 * and their current configuration.
 *
 * The driver queue size can be adjusted at any time.
 *
 * @note The default queue size is @see BRAWCAP_RX_DRIVER_QUEUE_SIZE_DEFAULT (in number of packets).
 * And its max limitation is @see BRAWCAP_RX_DRIVER_QUEUE_SIZE_MAX (in number of packets).
 *
 * @attention Reducing the queue size during runtime may result in packet loss.
 * This may be the case if there are more packets stored in the queue than the new size can accommodate.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] size Driver receive queue size to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Specified queue size is out of range/invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_driver_queue_size_set(brawcap_handle_t* const pHandle, const brawcap_queue_size_t size);

/**
 * @brief Reads out the currently configured driver receive queue size for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pSize Will contain the currently configured driver receive queue size, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_rx_driver_queue_size_get(brawcap_handle_t* const pHandle, brawcap_queue_size_t* const pSize);

/**@}*/
#endif

/*********************************************** TRANSMIT ************************************************************/
#if 1
/**
 * @addtogroup brawcap_transmit
 * @{
 */

/**
 * @brief Transmits a single packet on the specified handle.
 *
 * The caller must provide a valid packet filled up with the payload to transmit.
 *
 * For creating a packet use @ref brawcap_packet_create. After creating the packet set all the required packet fields
 * and than call this function to initiate a transmission.
 *
 * When transmission has finished (the function returns) the packet fields are updated.
 *
 * To check if the transmission was successful check for the function return status or the packet status itself
 * with @ref brawcap_packet_status_get. If the packet was transmitted and transmit time stamping was enabled the packet
 * timestamp will contain the transmission time, which be accessed with @ref brawcap_packet_timestamp_get.
 *
 * @param[in] pHandle bRAWcap handle to transmit on.
 *
 * @param[inout] pPacket Packet to be transmitted.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_NOT_AVAILABLE | bRAWcap driver is not active (anymore) on the adapter.
 * @ref BRAWCAP_STATUS_WARNING_DEMO_MODE          | Running in demo mode (no license).
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 * @ref BRAWCAP_STATUS_WARNING_NOT_ALL_PROCESSED  | The transmission failed (or payload was not fully transmitted).
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_packet(brawcap_handle_t* const pHandle, brawcap_packet_t* const pPacket);

/**
 * @brief Starts the internal transmit loop.
 *
 * To transmit data using this method push previously filled packet buffers to the handle by using
 * @ref brawcap_tx_buffer_send. After that the buffer will be locked down by the handle and added to it´s transmission
 * queue.
 *
 * For creating a new packet buffer which can be used for transmission use @ref brawcap_buffer_create.
 *
 * When a packet buffer transmission has completed the @ref brawcap_tx_callback_t "transmit callback" will be
 * called for this handle and with the corresponding packet buffer. From within the callback the buffer can be
 * accessed for post processing (it is unlocked before entering the callback). It is also possible to do
 * post processing from a different thread and after the callback.
 *
 * In general executing to long in the callback can lead to a bottleneck in the transmission.
 * It´s not as critical as it is for receiving but it should be still kept in mind.
 *
 * Like @ref brawcap_tx_packet does, also each packet in transmitted packet buffers will be updated (e.g. packet status,
 * transmission timestamp). The user application can access those updated information after transmission has completed.
 *
 * A completed packet buffer transmission will automatically detach the buffer from the handle.
 * It is not needed to detach it explicitly (which would be required for receive buffers).
 *
 * @note The buffers are transmitted in the same order they have been added by the user application with
 * @ref brawcap_tx_buffer_send.
 *
 * @invariant The user parameter allows access to user defined context.
 * The given pointer is not used and validated by bRAWcap. Therefore the user is responsible for validating
 * the context in the callback before accessing it.
 *
 * @attention After starting transmission, @ref brawcap_tx_packet can not be used - on this handle - until tranmission
 * is stopped with @ref brawcap_tx_stop, because parallel transmissions are not allowed
 *
 * @param[in] pHandle bRAWcap handle to transmit on.
 *
 * @param[in] callback Function pointer to user application transmit callback.
 *
 * @param[in] pUser Pointer to a user defined context. Can be NULL if no user context is required.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_WARNING_PENDING            | Another transmit is pending or transmission is already started.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_start(brawcap_handle_t* const pHandle, brawcap_tx_callback_t const callback, void* pUser);

/**
 * @brief Stops the internal transmit loop.
 *
 * @param[in] pHandle bRAWcap handle to stop transmission on.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_INFO_NOT_RUNNING           | There was no transmission running.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_stop( brawcap_handle_t* const pHandle);

/**
 * @brief Pushes the specified buffer to the specified handle transmit queue.
 *
 * After pushing a packet buffer to the handle for transmission it is locked down by the handle until transmission
 * has completed (and the @ref brawcap_tx_callback_t "transmit callback" is called).
 *
 * With the synchronized flag it is possible to activate a synchronized transmission of the specified packet buffer.
 * This means that the driver reads the time gap between each packet in the packet buffer and waits until it has been
 * elapsed before initiating transmission.
 *
 * @attention For synchronized transmission it is necessary to sort the packets in the buffer by rising timestamp.
 * Furthermore, it is currently not supported to specify a "start time" when the first packet should be transmitted.
 * Therefore if you want to start transmission depending on the first packet timestamp you should wait until the
 * time is reached and after this initiate the transmission by calling this function. The driver itself only
 * evaluates the time gaps between each packet in the buffer.
 *
 * @note Packet buffers are transmitted in the same order they was pushed to the handle.
 *
 * @invariant Keep in mind that the packet timestamp is updated after transmission has been completed by the
 * transmission timestamp (depending on the configured timestamp mode). Therefore if there was a packet timestamp set
 * for example to send synchronized, this timestamp will be overwritten.
 *
 * @param[in] pHandle bRAWcap handle for sending the buffer.
 *
 * @param[in] pBuffer Packet buffer to be sent.
 *
 * @param[in] synchronized Flag indicating if the packets in the packet buffer should be sent synchronized.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_IN_USE               | The buffer is already in use / locked down by another operation.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_buffer_send(brawcap_handle_t* const pHandle, brawcap_buffer_t* const pBuffer, const BOOLEAN synchronized);

/**
 * @brief Sets the driver queue size (in number of packets) for the specified handle.
 *
 * For transmission the driver queue size may be increased if high data rates should be reached (for example 10GBit/s).
 * With a larger transmission queue the driver can forward more packets to the network adapter at the same time.
 * Therefore if the transmission rate is below the expected value it may be solved by a larger driver queue size.
 *
 * The driver queue size can be adjusted at any time.
 *
 * @note The default queue size is @see BRAWCAP_TX_DRIVER_QUEUE_SIZE_DEFAULT (in number of packets).
 * And its max limitation is @see BRAWCAP_TX_DRIVER_QUEUE_SIZE_MAX (in number of packets).
 *
 * @attention Reducing the queue size during runtime may result in packet transmission cancellation.
 * This may be the case if there are more packets pending for transmission than the new size can accommodate.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] size Driver transmit queue size to be set.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | Specified queue size is out of range/invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_driver_queue_size_set(brawcap_handle_t* const pHandle, const brawcap_queue_size_t size);

/**
 * @brief Reads out the currently configured driver transmit queue size for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pSize Will contain the currently configured driver transmit queue size, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_driver_queue_size_get(brawcap_handle_t* const pHandle, brawcap_queue_size_t* const pSize);

/**
 * @brief Reads out the available transmit timestamp modes for the specified handle.
 *
 * Some of the timestamp capabilities depend on the features provided by the underlying adapter.
 * Those modes are named with "adapter".
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pCapabilities Will contain the available transmit timestamp modes, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_timestamp_capabilities(brawcap_handle_t* const pHandle,
  brawcap_timestamp_capabilities_t* const pCapabilities);

/**
 * @brief Configures the used transmit time stamping mode for the specified handle.
 *
 * @note Only available timestamp modes are accepted. It is recommended to check if a timestamp mode is supported
 * before setting it. To check if a timestamp mode is available use @ref brawcap_tx_timestamp_capabilities.
 *
 * @param[in] pHandle bRAWcap handle to be configured.
 *
 * @param[in] mode The transmit timestamp mode to be configured.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_NOT_AVAILABLE        | Specified timestamp mode not available.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_timestamp_mode_set(brawcap_handle_t* const pHandle, const brawcap_timestamp_mode_t mode);

/**
 * @brief Reads out the currently configured transmit time stamping mode for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[out] pMode Will contain the currently used transmit time stamping mode, afterwards.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_tx_timestamp_mode_get(brawcap_handle_t* const pHandle, brawcap_timestamp_mode_t* const pMode);

/**@}*/
#endif

/************************************************* STATS **************************************************************/
#if 1
/**
 * @addtogroup brawcap_stats
 * @{
 */

/**
 * @brief Reads out the available receive statistic counters for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[inout] pStats Will contain the current receive statistic counters, afterwards.
 * The header of the statistic struct hast to be filled by the caller, before calling this function.
 * If it is not filled or contains invalid data, the function will fail and return a error.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The header was invalid (type or revision).
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The specified size in the header was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_stats_rx(brawcap_handle_t* const pHandle, brawcap_stats_rx_t* const pStats);

/**
 * @brief Reads out the available transmit statistic counters for the specified handle.
 *
 * @param[in] pHandle bRAWcap handle to read from.
 *
 * @param[inout] pStats Will contain the current transmit statistic counters, afterwards.
 * The header of the statistic struct hast to be filled by the caller, before calling this function.
 * If it is not filled or contains invalid data, the function will fail and return a error.
 *
 * @return
 * Status                                         | Description
 * ---------------------------------------------- | -----------------------------------------------------------
 * @ref BRAWCAP_STATUS_SUCCESS                    | Success.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_POINTER      | At least one of the given pointer parameters was invalid.
 * @ref BRAWCAP_STATUS_ERROR_INVALID_PARAM        | The header was invalid (type or revision).
 * @ref BRAWCAP_STATUS_ERROR_PARAM_OUT_OF_RANGE   | The specified size in the header was invalid.
 * @ref BRAWCAP_STATUS_ERROR_DRIVER_IO_FAILED     | Communicating with bRAWcap driver failed.
 */
_BRAWCAP_API_FUNC(brawcap_status_t)
brawcap_stats_tx(brawcap_handle_t* const pHandle, brawcap_stats_tx_t* const pStats);

/**@}*/
#endif

#if defined(__cplusplus)
}
#endif //(__cplusplus)

#endif //BRAWCAP_H
