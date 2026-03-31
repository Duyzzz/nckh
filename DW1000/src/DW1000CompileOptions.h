/**
 * Copyright (c) 2016 by Ludwig Grill (www.rotzbua.de)
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file DW1000CompileOptions.h
 * Here are some options to optimize code and save some ram and rom
 * 
 */

#ifndef DW1000COMPILEOPTIONS_H
#define DW1000COMPILEOPTIONS_H

/**
 * To override these options from your sketch, define them BEFORE including DW1000Ranging.h:
 *
 *   #define DW1000TIME_H_PRINTABLE false
 *   #include "DW1000Ranging.h"
 *
 * Or create a "DW1000UserConfig.h" in your sketch folder and define options there.
 */

/**
 * Printable DW1000Time object costs about: rom: 490 byte ; ram: 58 byte
 * This option is needed because compiler can not optimize unused codes from inheritanced methods
 * Some examples or debug code use this
 * Set false if you do not need it and have to save some space
 */
#ifndef DW1000TIME_H_PRINTABLE
#define DW1000TIME_H_PRINTABLE true
#endif

#endif // DW1000COMPILEOPTIONS_H
