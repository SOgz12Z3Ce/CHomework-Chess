/**
 * @file mallocer.h
 * @brief macros for `malloc()`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#define new_array(type, n) ((type *)malloc(sizeof(type) * (n)))
#define new(type) ((type *)malloc(sizeof(type)))
