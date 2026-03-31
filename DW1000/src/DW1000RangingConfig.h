/*
 * DW1000 Ranging Configuration
 *
 * To override from your sketch, define BEFORE including DW1000Ranging.h:
 *
 *   #define USING_6_8M_MODE      // For 6.8 Mbps mode (fast, short range)
 *   // or
 *   #define USING_110K_MODE      // For 110 kbps mode (default, long range)
 *
 *   #include "DW1000Ranging.h"
 */

#ifndef DW1000_RANGING_CONFIG_H
#define DW1000_RANGING_CONFIG_H

// If user didn't specify, default to 6.8M mode
#if !defined(USING_6_8M_MODE) && !defined(USING_110K_MODE)
#define USING_6_8M_MODE
#endif

#endif
