#ifndef PB_ERROR_H
#define PB_ERROR_H

#define PB_OK 0             // Succees
#define PB_FAILED -1        // General fail
#define PB_INVAL -2        // Invalid arguments
#define PB_WRONG_STATE -3   // Object is in invalid state for the operation
#define PB_NOT_INITIALIZED -4 // Object is not initialized
#define PB_BUSY -5         // Busy on another operation
#define PB_NOMEM -6        // Not enough space (memory, disc, container, etc.)
#define PB_NOT_IMPEMENTED -7 // Functionality not implemented
#define PB_NOT_FOUND -8     // Not found

#endif // PB_ERROR_H
