#ifndef MIDIPARSER_H__
#define MIDIPARSER_H__

#include <stdint.h>
#include <stddef.h>

/**
* @file Constants for the MIDI command bytes.
*/
#define MIDI_CMD_NOTE_OFF               0x80
#define MIDI_CMD_NOTE_ON                0x90
#define MIDI_CMD_NOTE_PRESSURE          0xa0
#define MIDI_CMD_CONTROL                0xb0
#define MIDI_CMD_PGM_CHANGE             0xc0
#define MIDI_CMD_CHANNEL_PRESSURE       0xd0
#define MIDI_CMD_BENDER                 0xe0
#define MIDI_CMD_COMMON_SYSEX           0xf0
#define MIDI_CMD_COMMON_MTC_QUARTER     0xf1
#define MIDI_CMD_COMMON_SONG_POS        0xf2
#define MIDI_CMD_COMMON_SONG_SELECT     0xf3
#define MIDI_CMD_COMMON_TUNE_REQUEST    0xf6
#define MIDI_CMD_COMMON_SYSEX_END       0xf7
#define MIDI_CMD_COMMON_CLOCK           0xf8
#define MIDI_CMD_COMMON_START           0xfa
#define MIDI_CMD_COMMON_CONTINUE        0xfb
#define MIDI_CMD_COMMON_STOP            0xfc
#define MIDI_CMD_COMMON_SENSING         0xfe
#define MIDI_CMD_COMMON_RESET           0xff

#ifndef NULL
    #define NULL 0
#endif

/**@brief A MIDI message. */
typedef struct
{
    uint8_t command_type;  /**< One of the command type constants defined above. */
    uint8_t channel;       /**< Channel number for commands that apply to a specific channel (e.g. note on, note off) */
    uint32_t bytes_length; /**< The total number of bytes that constitute the message. */
    uint8_t  bytes[];      /**< All the bytes that constitute the message (one status byte plus zero or more data bytes). */
} midi_message_t;

/**@brief A queue that holds MIDI messages. */
typedef struct {
    uint16_t       length;
    midi_message_t *messages[]; /**< Array of pointers to messages */
} midi_message_queue_t;

/**@brief Keeps track of pending MIDI messages for which we're still waiting for the remaining bytes. */
typedef struct {
    midi_message_t *_pending_message; /**< Saves a partial message for which the end has not yet been received. */
} midi_message_parser_t;

/**
* @brief MIDI message parser constructor
*/
midi_message_parser_t * new_midi_message_parser();

/**
* @brief MIDI message parser destructor
*/
void free_midi_message_parser(midi_message_parser_t *parser);

/**
* @brief Converts a raw array of bytes (presumably received from another MIDI host) to a queue of MIDI messages.
*
* @param[in] parser - Keeps track of partially received messages for which we're still waiting for the remaining bytes.
* @param[in] bytes  - The array of raw bytes receive that presumably contains one or more MIDI messages.
* @return           - A queue containing all of the MIDI messages parsed from the bytes array.
*/
midi_message_queue_t * parse_midi_messages(midi_message_parser_t *parser, uint8_t *bytes, uint16_t bytes_length);

/**
* @brief MIDI message queue constructor.
*/
midi_message_queue_t * new_midi_message_queue();

/**
* @brief Deallocates the memory for the message queue and the messages it contains.
*/
void free_midi_message_queue(midi_message_queue_t * message_queue);

/**
* @brief MIDI message constructor
*/
midi_message_t * new_midi_message();

/**
* @brief Deallocates the memory for a midi message.
*/
void free_midi_message(midi_message_t * message);

/**
* @brief Adds a byte of data to a MIDI message's `bytes` array.
*
* @param[in,out] message_ptr - Pointer to the message into which the byte should be pushed
* @param[in]     byte        - The byte to push into the message's `bytes` array
*/
void add_byte_to_midi_message(midi_message_t **message_ptr, uint8_t byte);

/**
* @brief Appends multiple bytes of data to a MIDI message's `bytes` array.
*
* @param[in,out] message_ptr  - Pointer to the message into which the byte should be pushed
* @param[in]     bytes        - The bytes to push into the message's `bytes` array
* @param[in]     bytes_length - The length of the bytes array.
*/
void add_bytes_to_midi_message(midi_message_t **message_ptr, uint8_t *bytes, uint16_t bytes_length);

/**
* @brief Pushes a MIDI message into a MIDI message queue.
*
* @param[in] message_queue_ptr - Pointer to the message queue into which the message should be pushed
* @param[in] message           - The message to push into the queue
* @return                      - The updated message queue pointer after reallocating the memory to store the new message
*/
void add_midi_message_to_queue(midi_message_queue_t **message_queue_ptr, midi_message_t *message);

/**
* @brief Revmoes a MIDI message at a given index from a MIDI message queue.
*
* @param[in] message_queue_ptr - Pointer to the message queue from which the message should be removed
* @param[in] index             - The index of the message to be removed
* @return                      - The updated message queue pointer after reallocating the memory for the message queue.
*/
void remove_midi_message_from_queue(midi_message_queue_t **message_queue_ptr, uint16_t index);

/**
* Transfers all of the MIDI messages from one queue to another.
*
* @param[in,out] destination_queue_ptr
* @param[in,out] source_queue_ptr
*/
void transfer_midi_messages(midi_message_queue_t **destination_queue_ptr, midi_message_queue_t **source_queue_ptr);

/*
* Hooks that allow the library's consumer to receive log messages and notifications of heap memory usage.
*/

#ifndef LOGGING_DISABLED

/**
* @brief Function for logging.
*
* @param[in] format - Message to log.
*/
typedef void (*logging_function_t) (const char *message);

/**
* @brief Allows the user to register a logging function to enable logging.
*
* @param[in] logging_function
*/
void midi_register_logging_function(logging_function_t logging_function);

#endif // LOGGING_DISABLED

/**
* @brief Hook called when the module calls malloc() to allocate memory.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] size     - number of bytes
* @return         - Pointer to the allocated memory
*/
typedef void (*on_malloc_hook_function_t) (char *category, size_t size);

/**
* @brief Hook called when the module calls free() to free memory.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] size
*/
typedef void (*on_free_hook_function_t) (char *category, uint32_t size);

/**
* @brief Hook called when the module calls realloc() to reallocate memory.
*
* @param[in] category  - Null-terminated string with a category name for the memory operation
* @param[in] new_size
* @param[in] prev_size
* @return              - Pointer to the reallocated memory
*/
typedef void (*on_realloc_hook_function_t) (char *category, size_t new_size, size_t prev_size);

/**
* @brief Allows the user to register a function that will get called when the module calls malloc().
*/
void midi_register_on_malloc_hook(on_malloc_hook_function_t on_malloc_hook_function);

/**
* @brief Allows the user to register a function that will get called when the module calls free().
*/
void midi_register_on_free_hook(on_free_hook_function_t on_free_hook_function);

/**
* @brief Allows the user to register a function that will get called when the module calls realloc().
*/
void midi_register_on_realloc_hook(on_realloc_hook_function_t on_realloc_hook_function);


/**
* Internal functions that are exposed so they can be used by modules that extend the this MIDI module.
*/

/**
* @brief A printf-like function that logs a given message.
*
* @param[in] message - Format string
* @param[in] ...     - Variable number of arguments for format specifiers.
*/
void midi_log_info(const char *message, ...);

/**
* @brief Logs the given bytes.
*
* @param[in] bytes
* @param[in] length - The number of bytes.
*/
void midi_log_bytes(uint8_t *bytes, uint16_t length);

/**
* @brief Invokes malloc() and calls the optional hook function.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] size     - number of bytes
* @return         - Pointer to the allocated memory
*/
void * midi_malloc_with_hook(char *category, size_t size);

/**
* @brief Invokes free() and calls the optional hook function.
*
* @param[in] category - Null-terminated string with a category name for the memory operation
* @param[in] ptr
* @param[in] size
*/
void midi_free_with_hook(char *category, void * ptr, uint32_t size);

/**
* @brief Invokes realloc() and calls the optional hook function.
*
* @param[in] category  - Null-terminated string with a category name for the memory operation
* @param[in] ptr
* @param[in] new_size
* @param[in] prev_size
* @return              - Pointer to the reallocated memory
*/
void * midi_realloc_with_hook(char *category, void * ptr, size_t new_size, size_t prev_size);

#endif // MIDI_H__
