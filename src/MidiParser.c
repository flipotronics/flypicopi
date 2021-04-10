#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "MidiParser.h"

// Allow logging functionality to be excluded, because devices with memory limitations may not
// have the capacity to include stdarg.h
#ifndef LOGGING_DISABLED
    #include <stdarg.h>
    #include <stdio.h>
#endif

#define MIDI_MEMORY_CATEGORY "m" // The category name emitted by this module for memory tracking
#define MAXIMUM_LOG_MESSAGE_SIZE 300

/**
* @brief Array containing the constants representing the valid MIDI command types.
*/
const uint8_t MIDI_COMMAND_TYPES[] = {
    MIDI_CMD_NOTE_OFF,
    MIDI_CMD_NOTE_ON,
    MIDI_CMD_NOTE_PRESSURE,
    MIDI_CMD_CONTROL,
    MIDI_CMD_PGM_CHANGE,
    MIDI_CMD_CHANNEL_PRESSURE,
    MIDI_CMD_BENDER,
    MIDI_CMD_COMMON_SYSEX,
    MIDI_CMD_COMMON_MTC_QUARTER,
    MIDI_CMD_COMMON_SONG_POS,
    MIDI_CMD_COMMON_SONG_SELECT,
    MIDI_CMD_COMMON_TUNE_REQUEST,
    // MIDI_CMD_COMMON_SYSEX_END is not included, since no messages start with it.
    MIDI_CMD_COMMON_CLOCK,
    MIDI_CMD_COMMON_START,
    MIDI_CMD_COMMON_CONTINUE,
    MIDI_CMD_COMMON_STOP,
    MIDI_CMD_COMMON_SENSING,
    MIDI_CMD_COMMON_RESET
};

/**
* @brief Enum for the possible states of a pending message being parsed.
*/
typedef enum midi_message_state {
    VALID,   /**< Indicates a complete and valid message that is ready to be used. */
    PENDING, /**< Indicates that the message is not fully received yet. */
    INVALID  /**< Indicates that the message has been fully received but is invalid. */
} midi_message_state_t;

#ifndef LOGGING_DISABLED

static logging_function_t m_logging_function = NULL;
static on_malloc_hook_function_t m_on_malloc_hook_function = NULL;
static on_free_hook_function_t m_on_free_hook_function = NULL;
static on_realloc_hook_function_t m_on_realloc_hook_function = NULL;

/**
* @brief Allows the user to register a logging function to enable logging.
*
* @param[in] logging_function
*/
void midi_register_logging_function(logging_function_t logging_function)
{
    m_logging_function = logging_function;
}

#endif // LOGGING_DISABLED

/**
* @brief Helper function to indicate whether a given byte is a MIDI command byte
*        (i.e. a byte with bit 7 set).
*/
bool is_command_byte(uint8_t byte)
{
    return (0xF0 & byte) >> 7;
}

/**
* @brief A printf-like function that logs a given message.
*
* @param[in] message - Format string
* @param[in] ...     - Variable number of arguments for format specifiers.
*/
void midi_log_info(const char *message, ...)
{
    if (m_logging_function == NULL)
    {
        return;
    }

    #ifndef LOGGING_DISABLED
        va_list args;
        va_start(args, message);
        char formatted_message[MAXIMUM_LOG_MESSAGE_SIZE];
        vsnprintf(formatted_message, MAXIMUM_LOG_MESSAGE_SIZE, message, args);
        m_logging_function(formatted_message);
        va_end(args);
    #endif
}

/**
* @brief Logs the given bytes.
*
* @param[in] bytes
* @param[in] length - The number of bytes.
*/
void midi_log_bytes(uint8_t *bytes, uint16_t length)
{
    if (length == 0) {
        midi_log_info("(no bytes to log)");
        return;
    }

    midi_log_info("logging %d bytes:", length);

    for (uint8_t i = 0; i < length; i++)
    {
        midi_log_info("\tbyte %d: %x", i, bytes[i]);
    }
}

/**
* @brief Allows the user to register a function that will get called when the module calls malloc().
*/
void midi_register_on_malloc_hook(on_malloc_hook_function_t on_malloc_hook_function)
{
    m_on_malloc_hook_function = on_malloc_hook_function;
}

/**
* @brief Allows the user to register a function that will get called when the module calls free().
*/
void midi_register_on_free_hook(on_free_hook_function_t on_free_hook_function)
{
    m_on_free_hook_function = on_free_hook_function;
}

/**
* @brief Allows the user to register a function that will get called when the module calls realloc().
*/
void midi_register_on_realloc_hook(on_realloc_hook_function_t on_realloc_hook_function)
{
    m_on_realloc_hook_function = on_realloc_hook_function;
}

/**
* @brief Invokes malloc() and calls the optional hook function.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] size     - number of bytes
* @return         - Pointer to the allocated memory
*/
void * midi_malloc_with_hook(char *category, size_t size)
{
    if (m_on_malloc_hook_function != NULL)
    {
        m_on_malloc_hook_function(category, size);
    }
    void * memory = malloc(size);

    if (memory == NULL)
    {
        midi_log_info("WARNING: malloc() returned NULL");
    }
    return memory;
}

/**
* @brief Invokes free() and calls the optional hook function.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] ptr
* @param[in] size
*/
void midi_free_with_hook(char *category, void * ptr, uint32_t size)
{
    if (ptr == NULL)
    {
        return;
    }

    if (m_on_free_hook_function != NULL)
    {
        m_on_free_hook_function(category, size);
    }
    return free(ptr);
}

/**
* @brief Invokes realloc() and calls the optional hook function.
*
* @param[in] category  - Null-terminated string with a category name for the memory operation
* @param[in] ptr
* @param[in] new_size
* @param[in] prev_size
* @return              - Pointer to the reallocated memory
*/
void * midi_realloc_with_hook(char *category, void * ptr, size_t new_size, size_t prev_size)
{
    if (m_on_realloc_hook_function != NULL)
    {
        m_on_realloc_hook_function(category, new_size, prev_size);
    }

    void * new_ptr = realloc(ptr, new_size);

    // Add some logging to see if this every happens.
    if (new_ptr == NULL)
    {
        midi_log_info("realloc() returned NULL");
    }

    return new_ptr;
}

// Forward declarations
static midi_message_t * _parse_message_from_status_byte(uint8_t status_byte);
static midi_message_state_t validate_midi_message(midi_message_t *message);

/**
* @brief MIDI message parser constructor
*/
midi_message_parser_t * new_midi_message_parser()
{
    midi_message_parser_t *parser = (midi_message_parser_t *) midi_malloc_with_hook(MIDI_MEMORY_CATEGORY, sizeof(midi_message_parser_t));
    parser->_pending_message = NULL;
    return parser;
}

/**
* @brief MIDI message parser destructor
*/
void free_midi_message_parser(midi_message_parser_t *parser)
{
    if (!parser)
    {
        return;
    }

    if (parser->_pending_message)
    {
        free_midi_message(parser->_pending_message);
    }
    midi_free_with_hook(MIDI_MEMORY_CATEGORY, parser, sizeof(midi_message_parser_t));
}

/**
* @brief MIDI message constructor
*/
midi_message_t * new_midi_message()
{
    midi_message_t *message = (midi_message_t *) midi_malloc_with_hook(MIDI_MEMORY_CATEGORY, sizeof(midi_message_t));
    message->command_type = 0;
    message->channel = 0;
    message->bytes_length = 0;
    return message;
}

/**
* @brief Given an array of bytes that may contain one or more MIDI messages
*        it will construct the midi_message_t instance from the first MIDI message in the array it
*        encounters. If it doesn't encounter any MIDI messages in the array, it returns NULL.
*
* @param[in]  parser                 - Message parser that keeps track of unfinished messages for which the final bites have yet to be received.
* @param[in]  bytes                  - The message bytes to parse
* @param[in]  length                 - Length of the message byte buffer
* @param[out] number_of_bytes_parsed - The number of bytes parsed
*/
midi_message_t * parse_midi_message(midi_message_parser_t *parser, uint8_t *bytes, uint16_t length, uint16_t *number_of_bytes_parsed)
{
    for (uint16_t byte_index = 0; byte_index < length; byte_index++)
    {
        uint8_t byte = bytes[byte_index];

        if (is_command_byte(byte) && byte != MIDI_CMD_COMMON_SYSEX_END)
        {
            if (parser->_pending_message != NULL)
            {
                free_midi_message(parser->_pending_message);
            }

            parser->_pending_message = _parse_message_from_status_byte(byte);
            continue;
        }

        // `byte` is a data byte or a MIDI SysEx End command byte
        if (parser->_pending_message == NULL) {

            midi_log_info("Ignoring data byte %x, because there is no pending MIDI message.", byte);
            continue;
        }

        add_byte_to_midi_message(&(parser->_pending_message), byte);

        midi_message_state_t state = validate_midi_message(parser->_pending_message);

        switch (state)
        {
            case VALID:
            {   // Brackets needed to create a scope for the local var.
                midi_message_t *complete_message = parser->_pending_message;
                parser->_pending_message = NULL;
                *number_of_bytes_parsed = byte_index + 1;
                return complete_message;
            }
            case INVALID:
                free_midi_message(parser->_pending_message);
                parser->_pending_message = NULL;
                break;
            case PENDING:
                // In the case where the message is PENDING, just continue processing bytes.
                break;
        }
    }
    // A complete, valid message wasn't parsed, so return null.
    *number_of_bytes_parsed = length;
    return NULL;
}

/**
* @brief Deallocates the memory for a midi message.
*/
void free_midi_message(midi_message_t * message)
{
    if (message == NULL)
    {
        return;
    }

    uint32_t size = (uint32_t) sizeof(midi_message_t) + message->bytes_length * sizeof(uint8_t);
    midi_free_with_hook(MIDI_MEMORY_CATEGORY, message, size);
}

/**
* Indicates whether the given MIDI message is valid, invalid, or pending.
*/
static midi_message_state_t validate_midi_message(midi_message_t *message)
{
    if (message == NULL)
    {
        return INVALID;
    }

    if (message->bytes_length < 3)
    {
        return PENDING;
    }

    if (message->command_type == MIDI_CMD_COMMON_SYSEX)
    {
        return message->bytes[message->bytes_length - 1] == MIDI_CMD_COMMON_SYSEX_END ? VALID : PENDING;
    }

    return INVALID;
}

/**
* @brief Used by the MIDI message constructor to detect whether a given byte is a valid MIDI status byte
*        and, if so, to allocate memory for a new MIDI message instance.
*
* @param[in] status_byte The first byte of the MIDI message, which should be a command byte.
* @return                The new MIDI message if status_byte is a valid MIDI command byte, otherwise NULL.
*/
static midi_message_t * _parse_message_from_status_byte(uint8_t status_byte)
{

    if (!is_command_byte(status_byte))
    {
        return NULL;
    }

    uint8_t number_of_command_types = sizeof(MIDI_COMMAND_TYPES) / sizeof(MIDI_COMMAND_TYPES[0]);
    uint8_t matching_command_type = 0; // invalid, falsy command type

    for (uint8_t i = 0; i < number_of_command_types; i++)
    {
        uint8_t command_type = MIDI_COMMAND_TYPES[i];

        if (command_type & 0x0F)
        {
            // This command type is one of the 0xf_ system status types for which the lower nibble does not indicate a channel number.
            // So, let's match the entire command_type code.
            if (command_type == status_byte)
            {
                matching_command_type = command_type;
                break;
            }
        }
        else
        {
            // This command type has a dynamic lower nibble that indicates its channel number, so let's just check that the upper nibble matches.
            if (command_type == (status_byte & 0xF0))
            {
                matching_command_type = command_type;
                break;
            }
        }
    }

    if (matching_command_type)
    {
        // Allocate memory for midi_message_t plus the first byte in the bytes array, which is the status byte.
        midi_message_t * message = (midi_message_t *) midi_malloc_with_hook(MIDI_MEMORY_CATEGORY, sizeof(midi_message_t) + sizeof(uint8_t));

        message->bytes_length = 1;
        // The lower nibble indicates the channel number except on the 0xF_ system status commands.
        message->bytes[0] = status_byte;
        message->command_type = matching_command_type;

        // Only set the channel for non-system messages.
        bool is_system_message = (0xF0 & status_byte) == 0xF0;
        message->channel = is_system_message ? 0 : (status_byte & 0x0F);

        return message;
    }

    return NULL;
}

/**
* @brief Adds a byte of data to a MIDI message's `bytes` array.
*
* @param[in,out] message_ptr - Pointer to the message into which the byte should be pushed
* @param[in]     byte        - The byte to push into the message's `bytes` array
*/
void add_byte_to_midi_message(midi_message_t **message_ptr, uint8_t byte)
{

    if (message_ptr == NULL || *message_ptr == NULL)
    {
        // This shouldn't happen.
        return;
    }

    midi_message_t *message = *message_ptr;

    size_t prev_size = sizeof(midi_message_t) + message->bytes_length * sizeof(uint8_t);
    message->bytes_length++;
    size_t new_size = prev_size + sizeof(uint8_t);

    // Resize the array of bytes to to have room for the new byte.
    // In order to do this for a struct with a flexible array in standard c, the entire struct needs to be resized.
    *message_ptr = midi_realloc_with_hook(MIDI_MEMORY_CATEGORY,message, new_size, prev_size);
    (*message_ptr)->bytes[(*message_ptr)->bytes_length - 1] = byte;
}

/**
* @brief Appends multiple bytes of data to a MIDI message's `bytes` array.
*
* @param[in,out] message_ptr  - Pointer to the message into which the byte should be pushed
* @param[in]     bytes        - The bytes to push into the message's `bytes` array
* @param[in]     bytes_length - The length of the bytes array.
*/
void add_bytes_to_midi_message(midi_message_t **message_ptr, uint8_t *bytes, uint16_t bytes_length)
{

    if (message_ptr == NULL || *message_ptr == NULL)
    {
        // This shouldn't happen.
        return;
    }

    midi_message_t *message = *message_ptr;

    size_t prev_size = sizeof(midi_message_t) + message->bytes_length;
    size_t new_size = prev_size + bytes_length;

    // Resize the array of bytes to to have room for the new byte.
    // In order to do this for a struct with a flexible array in standard c, the entire struct needs to be resized.
    *message_ptr = midi_realloc_with_hook(MIDI_MEMORY_CATEGORY, message, new_size, prev_size);
    message = *message_ptr;
    memcpy(&message->bytes[message->bytes_length], bytes, bytes_length);
    message->bytes_length += bytes_length;
}

/**
* @brief MIDI message queue constructor.
*/
midi_message_queue_t * new_midi_message_queue()
{
    midi_message_queue_t * message_queue = (midi_message_queue_t *) midi_malloc_with_hook(MIDI_MEMORY_CATEGORY, sizeof(midi_message_queue_t));
    message_queue->length = 0;
    return message_queue;
}

/**
* @brief Deallocates the memory for the message queue and the messages it contains.
*/
void free_midi_message_queue(midi_message_queue_t * message_queue)
{
    // First, free the messages in the queue
    for (uint16_t i = 0; i < message_queue->length; i++)
    {
        free_midi_message(message_queue->messages[i]);
    }
    uint32_t size = sizeof(midi_message_queue_t) + sizeof(midi_message_t *) * message_queue->length;
    midi_log_info("Freeing %u bytes for MIDI message queue", size);
    midi_free_with_hook(MIDI_MEMORY_CATEGORY, message_queue, size);
}

/**
* @brief Pushes a MIDI message into a MIDI message queue.
*
* @param[in,out] message_queue_ptr - Pointer to the message queue into which the message should be pushed
* @param[in] message           - The message to push into the queue
* @return                      - The updated message queue pointer after reallocating the memory to store the new message
*/
void add_midi_message_to_queue(midi_message_queue_t **message_queue_ptr, midi_message_t *message)
{
    if (!(message_queue_ptr && *message_queue_ptr && message))
    {
        // This shouldn't happen.
        return;
    }

    size_t prev_size = sizeof(midi_message_queue_t) + (*message_queue_ptr)->length * sizeof(midi_message_t *);
    (*message_queue_ptr)->length++;
    size_t new_size = prev_size + sizeof(midi_message_t *);

    // Resize the array of pointers to to have room for the new message pointer.
    // In order to do this for a struct with a flexible array in standard c, the entire struct needs to be resized.
    *message_queue_ptr = midi_realloc_with_hook(MIDI_MEMORY_CATEGORY, *message_queue_ptr, new_size, prev_size);
    (*message_queue_ptr)->messages[(*message_queue_ptr)->length - 1] = message;
}

/**
* @brief Revmoes a MIDI message at a given index from a MIDI message queue.
*
* @param[in,out] message_queue_ptr - Pointer to the message queue from which the message should be removed
* @param[in] index             - The index of the message to be removed
* @return                      - The updated message queue pointer after reallocating the memory for the message queue.
*/
void remove_midi_message_from_queue(midi_message_queue_t **message_queue_ptr, uint16_t index)
{
    if (!(message_queue_ptr && *message_queue_ptr))
    {
        return;
    }

    if (index > (*message_queue_ptr)->length - 1)
    {
        return;
    }

    size_t prev_size = sizeof(midi_message_queue_t) + (*message_queue_ptr)->length * sizeof(midi_message_t *);
    size_t new_size = prev_size - sizeof(midi_message_t *);


    // Unless the message removed is at the end of the queue, we need to shift the messages after it
    // up 1 spot.
    if (index != (*message_queue_ptr)->length - 1)
    {
        memcpy((*message_queue_ptr)->messages[index], (*message_queue_ptr)->messages[index + 1], (*message_queue_ptr)->length - (index + 1));
    }

    // Resize the array of pointers to to have room for the new message pointer.
    // In order to do this for a struct with a flexible array in standard c, the entire struct needs to be resized.
    *message_queue_ptr = midi_realloc_with_hook(MIDI_MEMORY_CATEGORY, *message_queue_ptr, new_size, prev_size);
    (*message_queue_ptr)->length--;
}

/**
* Transfers all of the MIDI messages from one queue to another.
*
* @param[in,out] destination_queue_ptr
* @param[in,out] source_queue_ptr
*/
void transfer_midi_messages(midi_message_queue_t **destination_queue_ptr, midi_message_queue_t **source_queue_ptr)
{
    if (!(destination_queue_ptr && *destination_queue_ptr && source_queue_ptr && *source_queue_ptr))
    {
        return;
    }

    for (uint16_t i = (*source_queue_ptr)->length - 1; i >= 0 ; i--)
    {
        midi_message_t *message = (*source_queue_ptr)->messages[i];
        add_midi_message_to_queue(destination_queue_ptr, message);
        remove_midi_message_from_queue(source_queue_ptr, i);
    }
}

/**
* @brief Converts a raw array of bytes (presumably received from another MIDI host) to a queue of MIDI messages.
*
* @param[in] parser - Keeps track of partially received messages for which we're still waiting for the remaining bytes.
* @param[in] bytes  - The array of raw bytes receive that presumably contains one or more MIDI messages.
* @return           - A queue containing all of the MIDI messages parsed from the bytes array.
*/
midi_message_queue_t * parse_midi_messages(midi_message_parser_t *parser, uint8_t *bytes, uint16_t bytes_length)
{
    midi_log_info("Parsing raw bytes into MIDI messages...");

    midi_message_queue_t * message_queue = new_midi_message_queue();
    uint16_t byte_index = 0;

    // Keep feeding bytes into the MIDI message constructor, each time offsetting the array
    // by the amount that it had bitten off to create the previous message.
    while (byte_index < bytes_length)
    {
        uint8_t * offset_bytes = &bytes[byte_index];
        uint16_t offset_bytes_length = bytes_length - byte_index;
        uint16_t number_of_bytes_parsed;

        midi_message_t * message = parse_midi_message(parser, offset_bytes, offset_bytes_length, &number_of_bytes_parsed);

        if (!message)
        {
            // No more valid messages left in the bytes array, so our work here is already done.
            break;
        }

        midi_log_info("Pushing new MIDI message into queue: ");
        midi_log_bytes(message->bytes, message->bytes_length);

        add_midi_message_to_queue(&message_queue, message);
        byte_index += number_of_bytes_parsed;
    }

    midi_log_info("Finished parsing raw bytes into MIDI messages...");

    return message_queue;
}
