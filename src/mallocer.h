/**
 * @file mallocer.h
 * @brief macros for `malloc()`
 * @author SOgz12Z3Ce
 * @date 2025-10-12
 * @version 1.0
 */

#ifndef MALLOCER_H_
#define MALLOCER_H_

#include <stdlib.h>

#define new_array(type, n) ((type *)malloc(sizeof(type) * (n)))
#define new(type) ((type *)malloc(sizeof(type)))

#endif /* MALLOCER_H_ */
