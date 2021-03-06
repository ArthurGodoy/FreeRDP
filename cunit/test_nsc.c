/**
 * FreeRDP: A Remote Desktop Protocol Client
 * NSCodec Library Unit Tests
 *
 * Copyright 2012 Vic Lee
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
 */

/**
 * The sample data comes from [MS-RDPRFX] 4.2.3, which is decoded into three
 * vertical bands in red (21x64), green (23x64) and blue(20x64) color.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freerdp/types.h>
#include <freerdp/constants.h>
#include <freerdp/utils/print.h>
#include <freerdp/utils/memory.h>
#include <freerdp/utils/hexdump.h>
#include <freerdp/codec/nsc.h>

#include "test_nsc.h"

static const uint8 nsc_data[] =
{
	0x71, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
	0x03, 0x01, 0x00, 0x00, 0x63, 0x63, 0x01, 0x64, 0x64, 0x00, 0x63, 0x63, 0x02, 0x64, 0x64, 0x00,
	0x63, 0x63, 0x00, 0x64, 0x64, 0x01, 0x63, 0x63, 0x01, 0x64, 0x64, 0x01, 0x63, 0x63, 0x01, 0x64,
	0x64, 0x00, 0x63, 0x63, 0x00, 0x64, 0x64, 0x01, 0x63, 0x63, 0x00, 0x64, 0x64, 0x0c, 0x63, 0x63,
	0x00, 0x64, 0x64, 0x0c, 0x63, 0x63, 0x00, 0x64, 0x64, 0x0c, 0x63, 0x63, 0x00, 0x64, 0x64, 0x0c,
	0x63, 0x64, 0x64, 0x04, 0x63, 0x64, 0x63, 0x63, 0x00, 0x64, 0x64, 0x03, 0x63, 0x64, 0x64, 0x03,
	0x63, 0x63, 0x00, 0x64, 0x63, 0x63, 0x00, 0x64, 0x64, 0x03, 0x65, 0x63, 0x64, 0x64, 0x01, 0x63,
	0x64, 0x64, 0x00, 0x65, 0x64, 0x64, 0x06, 0x63, 0x64, 0x64, 0x00, 0x63, 0x63, 0x00, 0x64, 0x64,
	0x04, 0x64, 0x65, 0x65, 0x65, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x37, 0x37, 0x19, 0x36,
	0x37, 0x37, 0x06, 0x37, 0x37, 0x37, 0x37, 0xff, 0xff, 0x90, 0xff, 0xff, 0xff, 0xff
};

static const uint8 nsc_stress_data[] =
{
	  0xec, 0x04, 0x00, 0x00, 0xfb, 0x04, 0x00, 0x00, 0x74, 0x03, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00
	, 0x03, 0x00, 0x00, 0x00, 0x94, 0x94, 0x0f, 0xa2, 0xb2, 0x8e, 0x61, 0x62, 0x82, 0x9b, 0xa8, 0xb0
	, 0xb3, 0xb3, 0x00, 0xb0, 0xa8, 0x9b, 0x82, 0x62, 0x61, 0x8e, 0xb2, 0xa3, 0x94, 0x94, 0x1e, 0x98
	, 0xae, 0x98, 0x61, 0x73, 0x93, 0x9b, 0xa0, 0xa5, 0xaa, 0xad, 0xaf, 0xaf, 0x00, 0xad, 0xaa, 0xa5
	, 0xa0, 0x9b, 0x93, 0x73, 0x61, 0x98, 0xae, 0x98, 0x94, 0x94, 0x1b, 0x9c, 0xaf, 0x72, 0x62, 0x8b
	, 0x89, 0x8a, 0x92, 0x9c, 0xa2, 0xa8, 0xab, 0xac, 0xac, 0x00, 0xab, 0xa8, 0xa2, 0x9c, 0x92, 0x8a
	, 0x89, 0x8b, 0x63, 0x72, 0xaf, 0x9c, 0x94, 0x94, 0x19, 0x9f, 0xab, 0x63, 0x73, 0x7d, 0x79, 0x82
	, 0x89, 0x8f, 0x98, 0x9f, 0xa4, 0xa6, 0xa7, 0xa7, 0x00, 0xa6, 0xa4, 0x9f, 0x98, 0x8f, 0x89, 0x82
	, 0x79, 0x7d, 0x73, 0x63, 0xab, 0x9f, 0x94, 0x94, 0x17, 0x9c, 0xad, 0x58, 0x71, 0x6a, 0x6f, 0x7a
	, 0x81, 0x86, 0x8b, 0x92, 0x99, 0x9e, 0xa1, 0xa2, 0xa2, 0x00, 0x9f, 0x9c, 0x97, 0x91, 0x8b, 0x86
	, 0x81, 0x7a, 0x6f, 0x6a, 0x72, 0x58, 0xad, 0x9c, 0x94, 0x94, 0x15, 0x98, 0xb0, 0x61, 0x68, 0x5a
	, 0x64, 0x70, 0x78, 0x7f, 0x84, 0x87, 0x8c, 0x92, 0x97, 0x97, 0x00, 0x91, 0x8a, 0x82, 0x7e, 0x7a
	, 0x79, 0x79, 0x00, 0x7c, 0x7d, 0x78, 0x70, 0x64, 0x5a, 0x68, 0x61, 0xb0, 0x98, 0x94, 0x94, 0x14
	, 0xae, 0x71, 0x5f, 0x51, 0x5a, 0x65, 0x6f, 0x78, 0x7e, 0x83, 0x86, 0x88, 0x8a, 0x88, 0x7d, 0x76
	, 0x80, 0x8a, 0x8d, 0x8a, 0x83, 0x77, 0x6c, 0x70, 0x75, 0x6f, 0x65, 0x5a, 0x51, 0x5f, 0x71, 0xae
	, 0x94, 0x94, 0x13, 0xa3, 0x98, 0x54, 0x4d, 0x4f, 0x5a, 0x65, 0x6f, 0x77, 0x7d, 0x81, 0x83, 0x82
	, 0x7f, 0x77, 0x87, 0x9f, 0xa1, 0x9e, 0x9b, 0x97, 0x94, 0x90, 0x8b, 0x7b, 0x6e, 0x6f, 0x65, 0x5b
	, 0x4f, 0x4d, 0x54, 0x98, 0xa3, 0x94, 0x94, 0x11, 0x96, 0xb2, 0x5c, 0x51, 0x44, 0x4d, 0x50, 0x59
	, 0x66, 0x70, 0x75, 0x76, 0x73, 0x6b, 0x6e, 0x70, 0xae, 0xac, 0xab, 0xa9, 0xa6, 0xa2, 0x9c, 0x96
	, 0x8f, 0x82, 0x67, 0x6e, 0x65, 0x5b, 0x50, 0x45, 0x51, 0x5b, 0xb2, 0x96, 0x94, 0x94, 0x10, 0xa3
	, 0x90, 0x50, 0x41, 0x47, 0x4f, 0x6e, 0x59, 0x58, 0x5c, 0x5f, 0x60, 0x62, 0x71, 0x85, 0x6a, 0x99
	, 0xb9, 0xb7, 0xb4, 0xb1, 0xac, 0xa6, 0x9f, 0x98, 0x89, 0x69, 0x6b, 0x64, 0x5a, 0x50, 0x47, 0x41
	, 0x50, 0x90, 0xa3, 0x94, 0x94, 0x10, 0xaf, 0x59, 0x49, 0x3e, 0x46, 0x44, 0x72, 0x60, 0x68, 0x6c
	, 0x71, 0x76, 0x7f, 0x88, 0x93, 0x71, 0x7e, 0xc4, 0xc0, 0xbe, 0xbb, 0xb6, 0xb0, 0xa8, 0xa0, 0x91
	, 0x74, 0x65, 0x62, 0x58, 0x4f, 0x47, 0x3e, 0x49, 0x59, 0xaf, 0x94, 0x94, 0x0f, 0x9a, 0xaa, 0x49
	, 0x3d, 0x3f, 0x46, 0x47, 0x62, 0x63, 0x69, 0x71, 0x78, 0x80, 0x8a, 0x97, 0xa3, 0x8a, 0x69, 0xc9
	, 0xc9, 0x00, 0xc7, 0xc3, 0xbf, 0xb8, 0xb0, 0xa7, 0x99, 0x80, 0x5d, 0x5f, 0x56, 0x4e, 0x46, 0x3f
	, 0x3d, 0x49, 0xaa, 0x9a, 0x94, 0x94, 0x0e, 0xa2, 0x88, 0x48, 0x38, 0x3f, 0x45, 0x49, 0x51, 0x6d
	, 0x6d, 0x00, 0x75, 0x7c, 0x87, 0x97, 0xa4, 0xb0, 0xae, 0x62, 0xb5, 0xd4, 0xcf, 0xcb, 0xc6, 0xc0
	, 0xb7, 0xae, 0xa3, 0x8a, 0x5c, 0x5b, 0x55, 0x4d, 0x45, 0x3f, 0x38, 0x48, 0x88, 0xa2, 0x94, 0x94
	, 0x0e, 0xa9, 0x66, 0x40, 0x37, 0x3d, 0x44, 0x4a, 0x48, 0x70, 0x70, 0x00, 0x78, 0x81, 0x92, 0xa2
	, 0xaf, 0xbd, 0xca, 0x6b, 0x8d, 0xdd, 0xd7, 0xd2, 0xcd, 0xc6, 0xbe, 0xb4, 0xaa, 0x95, 0x6a, 0x55
	, 0x53, 0x4b, 0x44, 0x3d, 0x37, 0x40, 0x67, 0xa9, 0x94, 0x94, 0x0e, 0xae, 0x52, 0x3b, 0x36, 0x3c
	, 0x42, 0x49, 0x48, 0x66, 0x73, 0x7b, 0x87, 0x9a, 0xaa, 0xb9, 0xc8, 0xd6, 0x8b, 0x6c, 0xdf, 0xde
	, 0xb1, 0x86, 0x6e, 0x6a, 0x72, 0x7e, 0x8f, 0x7e, 0x52, 0x51, 0x49, 0x42, 0x3c, 0x36, 0x3b, 0x52
	, 0xae, 0x94, 0x94, 0x0e, 0xb2, 0x47, 0x37, 0x35, 0x3b, 0x40, 0x47, 0x49, 0x55, 0x78, 0x7d, 0x8c
	, 0x9f, 0xaf, 0xc0, 0xd0, 0xdb, 0xbc, 0x5f, 0xb2, 0x69, 0x57, 0x5e, 0x6b, 0x6e, 0x63, 0x55, 0x50
	, 0x4d, 0x4e, 0x4e, 0x00, 0x47, 0x40, 0x3b, 0x35, 0x37, 0x47, 0xb2, 0x94, 0x94, 0x0e, 0xb3, 0x42
	, 0x33, 0x32, 0x37, 0x3d, 0x45, 0x4a, 0x49, 0x78, 0x7d, 0x8e, 0xa1, 0xb2, 0xc4, 0xd2, 0xdb, 0xe2
	, 0x60, 0x57, 0x68, 0xa8, 0xc4, 0xc1, 0xb6, 0xa9, 0x99, 0x80, 0x62, 0x49, 0x4a, 0x45, 0x3d, 0x37
	, 0x32, 0x33, 0x42, 0xb3, 0x94, 0x94, 0x0e, 0xb3, 0x40, 0x4c, 0x55, 0x47, 0x41, 0x43, 0x47, 0x46
	, 0x4f, 0x64, 0x82, 0xa1, 0xb1, 0xc4, 0xcb, 0xa9, 0x62, 0x53, 0x70, 0xd0, 0xd1, 0xc5, 0xbd, 0xb2
	, 0xa7, 0x9a, 0x8d, 0x83, 0x52, 0x42, 0x43, 0x41, 0x47, 0x55, 0x4c, 0x40, 0xb3, 0x94, 0x94, 0x0e
	, 0xb2, 0x4c, 0x81, 0x70, 0x64, 0x5c, 0x58, 0x56, 0x51, 0x51, 0x00, 0x54, 0x4c, 0x51, 0x58, 0x59
	, 0x4e, 0x4f, 0x67, 0x75, 0x56, 0xca, 0xc9, 0xc2, 0xba, 0xb0, 0xa5, 0x98, 0x8a, 0x7d, 0x61, 0x4c
	, 0x57, 0x5c, 0x64, 0x70, 0x81, 0x4c, 0xb2, 0x94, 0x94, 0x0e, 0xae, 0x58, 0x90, 0x6d, 0x62, 0x5e
	, 0x5f, 0x62, 0x60, 0x59, 0x6c, 0x71, 0x6f, 0x6d, 0x6d, 0x00, 0x77, 0x95, 0xaf, 0xa1, 0x54, 0xad
	, 0xc7, 0xbe, 0xb6, 0xab, 0xa1, 0x94, 0x86, 0x79, 0x6f, 0x54, 0x5d, 0x5e, 0x62, 0x6d, 0x90, 0x59
	, 0xae, 0x94, 0x94, 0x0e, 0xa9, 0x66, 0x91, 0x71, 0x65, 0x60, 0x62, 0x64, 0x65, 0x5a, 0x67, 0x72
	, 0x83, 0x90, 0x9a, 0xa4, 0xa8, 0xa9, 0xa9, 0x00, 0x66, 0x87, 0xc1, 0xb8, 0xb0, 0xa6, 0x9b, 0x8e
	, 0x81, 0x75, 0x70, 0x59, 0x5d, 0x60, 0x65, 0x71, 0x91, 0x66, 0xa9, 0x94, 0x94, 0x0e, 0xa2, 0x87
	, 0x8a, 0x7c, 0x6b, 0x65, 0x66, 0x69, 0x6c, 0x62, 0x62, 0x00, 0x6b, 0x7b, 0x86, 0x90, 0x99, 0x9f
	, 0xa2, 0xa5, 0x7a, 0x6d, 0xb8, 0xb1, 0xa9, 0x9f, 0x95, 0x88, 0x7c, 0x70, 0x69, 0x63, 0x5c, 0x65
	, 0x6b, 0x7c, 0x8b, 0x87, 0xa2, 0x94, 0x94, 0x0e, 0x9a, 0xaa, 0x68, 0x93, 0x74, 0x6d, 0x6b, 0x6e
	, 0x71, 0x6c, 0x61, 0x65, 0x71, 0x7e, 0x88, 0x90, 0x96, 0x99, 0x9b, 0x8c, 0x64, 0xaa, 0xa8, 0xa0
	, 0x97, 0x8b, 0x7f, 0x75, 0x6c, 0x62, 0x6c, 0x5c, 0x6b, 0x74, 0x93, 0x69, 0xaa, 0x9a, 0x94, 0x94
	, 0x0f, 0xaf, 0x59, 0xab, 0x83, 0x79, 0x73, 0x74, 0x75, 0x75, 0x00, 0x64, 0x61, 0x66, 0x73, 0x7d
	, 0x85, 0x8a, 0x8d, 0x8e, 0x8d, 0x68, 0x93, 0x9c, 0x92, 0x88, 0x7d, 0x72, 0x6c, 0x63, 0x5c, 0x6e
	, 0x64, 0x75, 0x83, 0xab, 0x59, 0xaf, 0x94, 0x94, 0x10, 0xa2, 0x8e, 0x82, 0xa1, 0x87, 0x80, 0x7d
	, 0x7d, 0x01, 0x6f, 0x60, 0x5a, 0x66, 0x6f, 0x77, 0x7c, 0x80, 0x80, 0x01, 0x74, 0x80, 0x94, 0x7e
	, 0x70, 0x70, 0x01, 0x6e, 0x6a, 0x64, 0x66, 0x76, 0x82, 0xa1, 0x82, 0x8e, 0xa2, 0x94, 0x94, 0x10
	, 0x96, 0xb2, 0x5a, 0xba, 0x9a, 0x8f, 0x89, 0x87, 0x87, 0x00, 0x7c, 0x65, 0x4e, 0x59, 0x61, 0x68
	, 0x6c, 0x70, 0x71, 0x6d, 0x75, 0x74, 0x7f, 0x7c, 0x85, 0x88, 0x88, 0x00, 0x87, 0x84, 0x7f, 0x7e
	, 0x87, 0x98, 0xba, 0x5a, 0xb2, 0x96, 0x94, 0x94, 0x11, 0xa2, 0x98, 0x6e, 0xbe, 0x9f, 0x97, 0x93
	, 0x91, 0x8b, 0x76, 0x54, 0x49, 0x4f, 0x57, 0x5b, 0x5d, 0x5c, 0x62, 0x76, 0x85, 0x8f, 0x90, 0x91
	, 0x91, 0x03, 0x93, 0x97, 0x9f, 0xbe, 0x6e, 0x98, 0xa2, 0x94, 0x94, 0x13, 0xae, 0x6e, 0xa2, 0xbf
	, 0xa7, 0xa1, 0x9c, 0x9a, 0x91, 0x82, 0x78, 0x6d, 0x62, 0x5d, 0x65, 0x71, 0x7f, 0x8d, 0x97, 0x9a
	, 0x9a, 0x03, 0x9b, 0x9c, 0xa1, 0xa7, 0xbf, 0xa2, 0x6e, 0xae, 0x94, 0x94, 0x09, 0xd9, 0xd9, 0x09
	, 0xda, 0xe1, 0x66, 0xb8, 0xc6, 0xb1, 0xab, 0xa7, 0xa4, 0xa0, 0x9a, 0x93, 0x8f, 0x8e, 0x91, 0x96
	, 0x9d, 0xa1, 0xa2, 0xa2, 0x01, 0xa3, 0xa3, 0x00, 0xa5, 0xa7, 0xab, 0xb1, 0xc6, 0xb8, 0x65, 0xe1
	, 0xda, 0xd9, 0xd9, 0x09, 0x4d, 0x4d, 0x0b, 0x4b, 0x44, 0xb5, 0xd3, 0xbd, 0xb7, 0xb3, 0xb0, 0xae
	, 0xac, 0xac, 0x00, 0xab, 0xab, 0x04, 0xac, 0xac, 0x00, 0xae, 0xb0, 0xb3, 0xb7, 0xbd, 0xd3, 0xb5
	, 0x44, 0x4b, 0x4d, 0x4d, 0x0b, 0x00, 0x00, 0x0c, 0x03, 0x30, 0x92, 0xdd, 0xcf, 0xc3, 0xbf, 0xbc
	, 0xb9, 0xb8, 0xb7, 0xb6, 0xb6, 0x02, 0xb7, 0xb8, 0xb9, 0xbc, 0xbf, 0xc3, 0xcf, 0xdd, 0x92, 0x30
	, 0x02, 0x00, 0x00, 0x1c, 0x24, 0x5d, 0xca, 0xe3, 0xd6, 0xcd, 0xc9, 0xc7, 0xc5, 0xc4, 0xc4, 0x02
	, 0xc5, 0xc7, 0xc9, 0xcd, 0xd6, 0xe2, 0xc9, 0x5d, 0x24, 0x00, 0x00, 0x1f, 0x0d, 0x35, 0x75, 0xc8
	, 0xe4, 0xe5, 0xdf, 0xda, 0xd7, 0xd6, 0xd6, 0x00, 0xd7, 0xda, 0xdf, 0xe4, 0xe2, 0xc5, 0x73, 0x35
	, 0x0d, 0x00, 0x00, 0x22, 0x14, 0x30, 0x51, 0x7e, 0xad, 0xcb, 0xd3, 0xd8, 0xd8, 0x00, 0xd3, 0xcb
	, 0xac, 0x7c, 0x50, 0x30, 0x14, 0x00, 0x00, 0x26, 0x04, 0x16, 0x26, 0x33, 0x36, 0x41, 0x41, 0x00
	, 0x36, 0x33, 0x26, 0x16, 0x04, 0x00, 0x00, 0xff, 0xf7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x3a, 0x3a, 0x0f, 0x3b, 0x3c, 0x39, 0x33, 0x2d, 0x28, 0x25, 0x23, 0x21, 0x20, 0x20, 0x00, 0x21
	, 0x23, 0x25, 0x28, 0x2c, 0x32, 0x39, 0x3c, 0x3b, 0x3a, 0x3a, 0x1e, 0x3b, 0x3c, 0x3a, 0x32, 0x2b
	, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 0x20, 0x00, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x2b
	, 0x32, 0x3a, 0x3c, 0x3b, 0x3a, 0x3a, 0x1b, 0x3b, 0x3c, 0x36, 0x2d, 0x27, 0x27, 0x01, 0x25, 0x24
	, 0x23, 0x22, 0x22, 0x00, 0x21, 0x21, 0x00, 0x22, 0x22, 0x00, 0x23, 0x24, 0x25, 0x27, 0x27, 0x01
	, 0x2d, 0x36, 0x3c, 0x3b, 0x3a, 0x3a, 0x19, 0x3b, 0x3b, 0x00, 0x33, 0x2b, 0x29, 0x28, 0x28, 0x00
	, 0x27, 0x25, 0x24, 0x23, 0x23, 0x00, 0x22, 0x22, 0x02, 0x23, 0x23, 0x00, 0x24, 0x25, 0x27, 0x28
	, 0x28, 0x00, 0x29, 0x2b, 0x33, 0x3b, 0x3b, 0x00, 0x3a, 0x3a, 0x17, 0x3b, 0x3b, 0x00, 0x32, 0x2b
	, 0x2c, 0x2a, 0x28, 0x28, 0x00, 0x27, 0x26, 0x25, 0x24, 0x23, 0x23, 0x03, 0x24, 0x24, 0x00, 0x25
	, 0x26, 0x27, 0x28, 0x28, 0x00, 0x2a, 0x2c, 0x2a, 0x32, 0x3b, 0x3b, 0x00, 0x3a, 0x3a, 0x15, 0x3b
	, 0x3c, 0x33, 0x2b, 0x2e, 0x2c, 0x2a, 0x28, 0x28, 0x00, 0x27, 0x26, 0x25, 0x24, 0x24, 0x01, 0x25
	, 0x27, 0x29, 0x2c, 0x2b, 0x29, 0x29, 0x01, 0x28, 0x29, 0x2a, 0x2c, 0x2e, 0x2b, 0x33, 0x3c, 0x3b
	, 0x3a, 0x3a, 0x14, 0x3c, 0x36, 0x2c, 0x2f, 0x2e, 0x2c, 0x2b, 0x29, 0x28, 0x27, 0x27, 0x00, 0x26
	, 0x25, 0x26, 0x29, 0x37, 0x07, 0x11, 0x14, 0x13, 0x0f, 0x05, 0x39, 0x2c, 0x29, 0x2b, 0x2c, 0x2e
	, 0x2f, 0x2b, 0x36, 0x3c, 0x3a, 0x3a, 0x13, 0x3b, 0x3a, 0x2e, 0x2f, 0x30, 0x2e, 0x2c, 0x2b, 0x29
	, 0x28, 0x27, 0x27, 0x02, 0x2f, 0x0d, 0x19, 0x1a, 0x1a, 0x04, 0x19, 0x3e, 0x2b, 0x2b, 0x00, 0x2c
	, 0x2e, 0x2f, 0x2f, 0x00, 0x2e, 0x3a, 0x3b, 0x3a, 0x3a, 0x11, 0x3b, 0x3c, 0x32, 0x2e, 0x31, 0x30
	, 0x30, 0x00, 0x2e, 0x2c, 0x2b, 0x29, 0x29, 0x00, 0x2a, 0x2b, 0x2c, 0x32, 0x1a, 0x1a, 0x04, 0x19
	, 0x1a, 0x1a, 0x00, 0x0b, 0x2d, 0x2b, 0x2c, 0x2d, 0x2f, 0x31, 0x2e, 0x32, 0x3c, 0x3b, 0x3a, 0x3a
	, 0x10, 0x3b, 0x39, 0x2e, 0x31, 0x31, 0x00, 0x32, 0x2e, 0x2d, 0x2e, 0x2e, 0x00, 0x2d, 0x2d, 0x00
	, 0x2b, 0x29, 0x28, 0x31, 0x0e, 0x17, 0x17, 0x00, 0x18, 0x19, 0x1a, 0x1a, 0x02, 0x15, 0x31, 0x2b
	, 0x2c, 0x2d, 0x2f, 0x30, 0x31, 0x2e, 0x39, 0x3b, 0x3a, 0x3a, 0x10, 0x3c, 0x33, 0x2f, 0x32, 0x30
	, 0x31, 0x30, 0x2a, 0x2a, 0x02, 0x29, 0x28, 0x29, 0x2b, 0x31, 0x01, 0x13, 0x13, 0x00, 0x14, 0x15
	, 0x18, 0x19, 0x1a, 0x1a, 0x01, 0x39, 0x2c, 0x2c, 0x00, 0x2d, 0x2f, 0x30, 0x32, 0x2f, 0x32, 0x3c
	, 0x3a, 0x3a, 0x0f, 0x3b, 0x3b, 0x00, 0x2f, 0x32, 0x32, 0x00, 0x31, 0x31, 0x00, 0x32, 0x2a, 0x2b
	, 0x2b, 0x00, 0x2a, 0x29, 0x2a, 0x2e, 0x30, 0x31, 0x35, 0x11, 0x0f, 0x10, 0x12, 0x14, 0x17, 0x19
	, 0x1a, 0x1a, 0x00, 0x04, 0x2d, 0x2c, 0x2d, 0x2f, 0x31, 0x32, 0x32, 0x00, 0x2f, 0x3b, 0x3b, 0x00
	, 0x3a, 0x3a, 0x0e, 0x3b, 0x38, 0x30, 0x33, 0x32, 0x31, 0x30, 0x31, 0x2b, 0x2b, 0x00, 0x2a, 0x29
	, 0x2a, 0x2e, 0x31, 0x33, 0x32, 0x32, 0x00, 0x0e, 0x0b, 0x0d, 0x0f, 0x11, 0x13, 0x17, 0x1a, 0x1a
	, 0x00, 0x11, 0x30, 0x2d, 0x2e, 0x2f, 0x31, 0x32, 0x33, 0x30, 0x38, 0x3b, 0x3a, 0x3a, 0x0e, 0x3b
	, 0x35, 0x31, 0x33, 0x32, 0x31, 0x30, 0x31, 0x2e, 0x2b, 0x29, 0x29, 0x00, 0x2c, 0x30, 0x32, 0x34
	, 0x35, 0x33, 0x06, 0x09, 0x0a, 0x0c, 0x0f, 0x12, 0x15, 0x19, 0x1b, 0x1a, 0x36, 0x2e, 0x2e, 0x00
	, 0x30, 0x31, 0x32, 0x33, 0x31, 0x35, 0x3b, 0x3a, 0x3a, 0x0e, 0x3c, 0x33, 0x32, 0x34, 0x32, 0x31
	, 0x30, 0x30, 0x01, 0x2a, 0x29, 0x2a, 0x2e, 0x32, 0x34, 0x36, 0x38, 0x34, 0x3a, 0x0a, 0x0a, 0x00
	, 0x05, 0x3e, 0x39, 0x3a, 0x3f, 0x07, 0x14, 0x02, 0x2e, 0x2e, 0x00, 0x30, 0x31, 0x32, 0x34, 0x32
	, 0x33, 0x3c, 0x3a, 0x3a, 0x0e, 0x3c, 0x31, 0x33, 0x34, 0x33, 0x32, 0x30, 0x30, 0x00, 0x31, 0x2a
	, 0x29, 0x2b, 0x30, 0x32, 0x35, 0x37, 0x39, 0x35, 0x33, 0x05, 0x35, 0x32, 0x36, 0x3a, 0x3b, 0x3a
	, 0x37, 0x36, 0x34, 0x2f, 0x2f, 0x00, 0x30, 0x32, 0x33, 0x34, 0x33, 0x31, 0x3c, 0x3a, 0x3a, 0x0e
	, 0x3c, 0x31, 0x34, 0x34, 0x00, 0x33, 0x32, 0x31, 0x30, 0x31, 0x2a, 0x28, 0x2b, 0x30, 0x33, 0x36
	, 0x38, 0x39, 0x38, 0x30, 0x31, 0x39, 0x03, 0x06, 0x07, 0x08, 0x09, 0x08, 0x04, 0x3e, 0x33, 0x30
	, 0x31, 0x32, 0x33, 0x34, 0x34, 0x00, 0x31, 0x3c, 0x3a, 0x3a, 0x0e, 0x3c, 0x32, 0x35, 0x36, 0x34
	, 0x33, 0x31, 0x30, 0x30, 0x00, 0x31, 0x2e, 0x2a, 0x2d, 0x31, 0x33, 0x34, 0x34, 0x00, 0x33, 0x30
	, 0x3a, 0x06, 0x06, 0x01, 0x07, 0x08, 0x09, 0x09, 0x00, 0x08, 0x05, 0x38, 0x31, 0x31, 0x00, 0x33
	, 0x34, 0x36, 0x35, 0x32, 0x3c, 0x3a, 0x3a, 0x0e, 0x3c, 0x33, 0x38, 0x38, 0x00, 0x36, 0x35, 0x33
	, 0x32, 0x31, 0x3a, 0x3e, 0x36, 0x34, 0x33, 0x33, 0x01, 0x35, 0x09, 0x0a, 0x35, 0x05, 0x06, 0x07
	, 0x07, 0x00, 0x08, 0x09, 0x09, 0x00, 0x07, 0x04, 0x3c, 0x33, 0x33, 0x00, 0x35, 0x36, 0x38, 0x38
	, 0x00, 0x33, 0x3c, 0x3a, 0x3a, 0x0e, 0x3c, 0x34, 0x39, 0x38, 0x37, 0x36, 0x34, 0x33, 0x33, 0x00
	, 0x3e, 0x16, 0x17, 0x10, 0x0b, 0x0b, 0x00, 0x11, 0x19, 0x19, 0x00, 0x15, 0x35, 0x03, 0x06, 0x07
	, 0x07, 0x00, 0x08, 0x09, 0x09, 0x00, 0x06, 0x04, 0x00, 0x36, 0x35, 0x36, 0x37, 0x38, 0x39, 0x34
	, 0x3c, 0x3a, 0x3a, 0x0e, 0x3b, 0x36, 0x3a, 0x39, 0x37, 0x36, 0x35, 0x34, 0x34, 0x00, 0x36, 0x11
	, 0x15, 0x16, 0x17, 0x17, 0x00, 0x18, 0x18, 0x02, 0x3d, 0x3f, 0x07, 0x07, 0x00, 0x08, 0x09, 0x09
	, 0x00, 0x08, 0x05, 0x03, 0x01, 0x39, 0x36, 0x36, 0x00, 0x37, 0x39, 0x39, 0x00, 0x36, 0x3b, 0x3a
	, 0x3a, 0x0e, 0x3b, 0x38, 0x39, 0x39, 0x00, 0x38, 0x37, 0x36, 0x35, 0x35, 0x01, 0x09, 0x15, 0x16
	, 0x16, 0x01, 0x17, 0x17, 0x00, 0x18, 0x18, 0x00, 0x07, 0x3b, 0x07, 0x08, 0x08, 0x00, 0x09, 0x09
	, 0x00, 0x07, 0x04, 0x02, 0x00, 0x3c, 0x37, 0x37, 0x00, 0x38, 0x39, 0x39, 0x00, 0x38, 0x3b, 0x3a
	, 0x3a, 0x0e, 0x3b, 0x3b, 0x00, 0x36, 0x3a, 0x39, 0x38, 0x37, 0x36, 0x36, 0x01, 0x01, 0x14, 0x15
	, 0x16, 0x16, 0x01, 0x17, 0x17, 0x01, 0x11, 0x39, 0x06, 0x09, 0x09, 0x01, 0x07, 0x05, 0x02, 0x01
	, 0x3f, 0x3f, 0x00, 0x38, 0x38, 0x00, 0x39, 0x3a, 0x36, 0x3b, 0x3b, 0x00, 0x3a, 0x3a, 0x0f, 0x3c
	, 0x35, 0x3b, 0x39, 0x39, 0x00, 0x38, 0x37, 0x37, 0x00, 0x36, 0x3a, 0x13, 0x15, 0x15, 0x00, 0x16
	, 0x16, 0x02, 0x17, 0x16, 0x3b, 0x03, 0x09, 0x09, 0x00, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x3e
	, 0x00, 0x3b, 0x39, 0x39, 0x00, 0x3b, 0x35, 0x3c, 0x3a, 0x3a, 0x10, 0x3b, 0x39, 0x38, 0x3b, 0x3a
	, 0x39, 0x38, 0x38, 0x00, 0x37, 0x38, 0x0d, 0x14, 0x15, 0x15, 0x00, 0x16, 0x16, 0x03, 0x03, 0x3f
	, 0x07, 0x3f, 0x3a, 0x38, 0x38, 0x01, 0x39, 0x3c, 0x3f, 0x3d, 0x3a, 0x3b, 0x38, 0x39, 0x3b, 0x3a
	, 0x3a, 0x10, 0x3b, 0x3c, 0x35, 0x3c, 0x3a, 0x39, 0x39, 0x00, 0x38, 0x38, 0x01, 0x06, 0x13, 0x14
	, 0x15, 0x15, 0x03, 0x16, 0x0e, 0x3a, 0x3a, 0x00, 0x38, 0x38, 0x04, 0x39, 0x39, 0x00, 0x3a, 0x3b
	, 0x3c, 0x35, 0x3c, 0x3b, 0x3a, 0x3a, 0x11, 0x3b, 0x3a, 0x36, 0x3c, 0x3a, 0x3a, 0x00, 0x39, 0x39
	, 0x01, 0x3e, 0x10, 0x14, 0x14, 0x01, 0x15, 0x15, 0x01, 0x11, 0x04, 0x39, 0x38, 0x38, 0x03, 0x39
	, 0x39, 0x01, 0x3a, 0x3a, 0x00, 0x3c, 0x36, 0x3a, 0x3b, 0x3a, 0x3a, 0x13, 0x3c, 0x36, 0x3a, 0x3c
	, 0x3b, 0x3a, 0x3a, 0x00, 0x39, 0x3a, 0x3b, 0x02, 0x08, 0x0c, 0x0c, 0x00, 0x0a, 0x04, 0x3c, 0x3a
	, 0x39, 0x39, 0x05, 0x3a, 0x3a, 0x00, 0x3b, 0x3c, 0x3a, 0x36, 0x3c, 0x3a, 0x3a, 0x09, 0x3c, 0x3c
	, 0x0a, 0x3d, 0x35, 0x3b, 0x3d, 0x3b, 0x3a, 0x3a, 0x09, 0x39, 0x39, 0x01, 0x3a, 0x3a, 0x04, 0x3b
	, 0x3d, 0x3b, 0x35, 0x3d, 0x3c, 0x3c, 0x0a, 0x3d, 0x3d, 0x0b, 0x3c, 0x34, 0x3b, 0x3d, 0x3c, 0x3b
	, 0x3b, 0x00, 0x3a, 0x3a, 0x0c, 0x3b, 0x3b, 0x00, 0x3c, 0x3d, 0x3b, 0x34, 0x3c, 0x3d, 0x3d, 0x0b
	, 0x00, 0x00, 0x0c, 0x3f, 0x35, 0x38, 0x3d, 0x3d, 0x00, 0x3c, 0x3b, 0x3b, 0x0c, 0x3c, 0x3d, 0x3d
	, 0x00, 0x38, 0x35, 0x3f, 0x00, 0x00, 0x1b, 0x3f, 0x38, 0x35, 0x3b, 0x3e, 0x3d, 0x3c, 0x3c, 0x0a
	, 0x3d, 0x3e, 0x3b, 0x35, 0x38, 0x3f, 0x00, 0x00, 0x1e, 0x3d, 0x35, 0x35, 0x00, 0x3b, 0x3d, 0x3e
	, 0x3e, 0x00, 0x3d, 0x3d, 0x04, 0x3e, 0x3e, 0x00, 0x3d, 0x3a, 0x35, 0x35, 0x00, 0x3d, 0x00, 0x00
	, 0x22, 0x3b, 0x35, 0x33, 0x35, 0x38, 0x3b, 0x3b, 0x04, 0x38, 0x35, 0x33, 0x35, 0x3b, 0x00, 0x00
	, 0x26, 0x3f, 0x3b, 0x37, 0x34, 0x34, 0x00, 0x32, 0x32, 0x00, 0x34, 0x34, 0x00, 0x37, 0x3b, 0x3f
	, 0x00, 0x00, 0xff, 0xf7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x11, 0x00, 0x01
	, 0x03, 0x06, 0x09, 0x0a, 0x0c, 0x0c, 0x02, 0x0a, 0x09, 0x06, 0x03, 0x01, 0x00, 0x3f, 0x3f, 0x22
	, 0x00, 0x01, 0x04, 0x07, 0x07, 0x00, 0x08, 0x09, 0x0a, 0x0b, 0x0b, 0x02, 0x0a, 0x09, 0x08, 0x07
	, 0x07, 0x00, 0x04, 0x01, 0x00, 0x3f, 0x3f, 0x1f, 0x00, 0x03, 0x06, 0x05, 0x05, 0x00, 0x06, 0x08
	, 0x09, 0x0a, 0x0a, 0x00, 0x0b, 0x0b, 0x00, 0x0a, 0x0a, 0x00, 0x09, 0x08, 0x06, 0x05, 0x05, 0x00
	, 0x06, 0x03, 0x00, 0x3f, 0x3f, 0x1c, 0x00, 0x01, 0x04, 0x05, 0x04, 0x04, 0x00, 0x05, 0x06, 0x07
	, 0x08, 0x09, 0x09, 0x00, 0x0a, 0x0a, 0x00, 0x09, 0x09, 0x00, 0x08, 0x07, 0x06, 0x05, 0x04, 0x04
	, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x19, 0x00, 0x01, 0x03, 0x03, 0x01, 0x04, 0x04
	, 0x01, 0x05, 0x06, 0x07, 0x08, 0x09, 0x09, 0x01, 0x08, 0x08, 0x00, 0x07, 0x06, 0x05, 0x04, 0x04
	, 0x01, 0x03, 0x03, 0x00, 0x04, 0x01, 0x00, 0x3f, 0x3f, 0x18, 0x01, 0x03, 0x02, 0x02, 0x00, 0x03
	, 0x04, 0x04, 0x01, 0x05, 0x06, 0x07, 0x07, 0x03, 0x06, 0x06, 0x00, 0x05, 0x05, 0x00, 0x04, 0x04
	, 0x02, 0x03, 0x02, 0x02, 0x00, 0x03, 0x00, 0x3f, 0x3f, 0x17, 0x00, 0x02, 0x02, 0x02, 0x03, 0x04
	, 0x04, 0x02, 0x05, 0x06, 0x06, 0x00, 0x05, 0x04, 0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x03, 0x03
	, 0x01, 0x02, 0x02, 0x02, 0x00, 0x3f, 0x3f, 0x15, 0x00, 0x02, 0x02, 0x03, 0x03, 0x03, 0x00, 0x04
	, 0x04, 0x02, 0x05, 0x04, 0x03, 0x04, 0x04, 0x01, 0x03, 0x03, 0x00, 0x02, 0x02, 0x01, 0x03, 0x03
	, 0x01, 0x02, 0x02, 0x03, 0x00, 0x3f, 0x3f, 0x14, 0x01, 0x02, 0x01, 0x01, 0x00, 0x02, 0x02, 0x01
	, 0x03, 0x03, 0x00, 0x04, 0x04, 0x00, 0x03, 0x03, 0x00, 0x04, 0x06, 0x06, 0x02, 0x05, 0x04, 0x04
	, 0x00, 0x03, 0x02, 0x03, 0x02, 0x02, 0x03, 0x01, 0x02, 0x01, 0x3f, 0x3f, 0x13, 0x00, 0x01, 0x01
	, 0x02, 0x02, 0x02, 0x04, 0x03, 0x03, 0x02, 0x05, 0x06, 0x06, 0x03, 0x05, 0x04, 0x03, 0x03, 0x00
	, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01, 0x00, 0x3f, 0x3f, 0x12, 0x01, 0x01, 0x04, 0x02, 0x02, 0x01
	, 0x03, 0x03, 0x00, 0x04, 0x04, 0x00, 0x03, 0x02, 0x04, 0x04, 0x00, 0x05, 0x05, 0x01, 0x06, 0x06
	, 0x00, 0x05, 0x04, 0x03, 0x02, 0x02, 0x03, 0x01, 0x01, 0x02, 0x3f, 0x3f, 0x11, 0x00, 0x01, 0x01
	, 0x04, 0x02, 0x02, 0x01, 0x03, 0x04, 0x04, 0x00, 0x02, 0x01, 0x01, 0x00, 0x03, 0x04, 0x03, 0x04
	, 0x04, 0x00, 0x05, 0x06, 0x06, 0x00, 0x05, 0x04, 0x02, 0x02, 0x03, 0x01, 0x01, 0x02, 0x00, 0x3f
	, 0x3f, 0x10, 0x00, 0x01, 0x01, 0x04, 0x02, 0x02, 0x00, 0x03, 0x04, 0x04, 0x00, 0x02, 0x01, 0x01
	, 0x00, 0x00, 0x02, 0x04, 0x03, 0x03, 0x01, 0x04, 0x05, 0x06, 0x06, 0x00, 0x05, 0x03, 0x02, 0x02
	, 0x02, 0x01, 0x01, 0x02, 0x00, 0x3f, 0x3f, 0x10, 0x00, 0x01, 0x01, 0x03, 0x02, 0x02, 0x01, 0x03
	, 0x04, 0x03, 0x01, 0x01, 0x01, 0x00, 0x02, 0x03, 0x02, 0x02, 0x00, 0x03, 0x03, 0x00, 0x04, 0x05
	, 0x06, 0x06, 0x00, 0x04, 0x02, 0x02, 0x01, 0x01, 0x01, 0x03, 0x00, 0x3f, 0x3f, 0x10, 0x00, 0x01
	, 0x01, 0x05, 0x03, 0x04, 0x04, 0x00, 0x02, 0x01, 0x01, 0x01, 0x00, 0x01, 0x03, 0x02, 0x02, 0x0a
	, 0x01, 0x01, 0x03, 0x00, 0x3f, 0x3f, 0x10, 0x01, 0x01, 0x06, 0x03, 0x04, 0x03, 0x02, 0x01, 0x01
	, 0x01, 0x00, 0x01, 0x02, 0x02, 0x02, 0x03, 0x04, 0x05, 0x05, 0x00, 0x03, 0x02, 0x02, 0x02, 0x01
	, 0x01, 0x04, 0x3f, 0x3f, 0x10, 0x01, 0x01, 0x06, 0x03, 0x04, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00
	, 0x00, 0x01, 0x02, 0x02, 0x00, 0x04, 0x06, 0x05, 0x06, 0x07, 0x08, 0x09, 0x09, 0x00, 0x06, 0x02
	, 0x01, 0x01, 0x05, 0x3f, 0x3f, 0x10, 0x00, 0x01, 0x00, 0x01, 0x01, 0x04, 0x02, 0x03, 0x02, 0x01
	, 0x01, 0x03, 0x02, 0x04, 0x05, 0x05, 0x01, 0x06, 0x07, 0x08, 0x09, 0x09, 0x01, 0x03, 0x01, 0x01
	, 0x02, 0x00, 0x01, 0x00, 0x3f, 0x3f, 0x10, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x00, 0x3f
	, 0x00, 0x01, 0x01, 0x02, 0x00, 0x00, 0x01, 0x02, 0x05, 0x05, 0x01, 0x06, 0x07, 0x08, 0x09, 0x09
	, 0x01, 0x05, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x3f, 0x3f, 0x10, 0x00, 0x00, 0x03, 0x01
	, 0x01, 0x01, 0x3f, 0x3b, 0x3c, 0x3e, 0x3e, 0x00, 0x3f, 0x00, 0x03, 0x08, 0x05, 0x01, 0x05, 0x05
	, 0x00, 0x06, 0x06, 0x00, 0x07, 0x09, 0x09, 0x02, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x3f
	, 0x3f, 0x10, 0x00, 0x00, 0x03, 0x01, 0x01, 0x01, 0x00, 0x3b, 0x3c, 0x3f, 0x01, 0x03, 0x05, 0x06
	, 0x07, 0x06, 0x00, 0x03, 0x06, 0x06, 0x00, 0x07, 0x08, 0x09, 0x09, 0x02, 0x08, 0x02, 0x01, 0x00
	, 0x00, 0x03, 0x3f, 0x3f, 0x10, 0x00, 0x00, 0x04, 0x01, 0x01, 0x01, 0x3d, 0x3c, 0x3e, 0x00, 0x01
	, 0x03, 0x04, 0x05, 0x06, 0x00, 0x02, 0x06, 0x07, 0x08, 0x09, 0x09, 0x04, 0x04, 0x00, 0x00, 0x04
	, 0x3f, 0x3f, 0x10, 0x00, 0x00, 0x05, 0x01, 0x01, 0x00, 0x3e, 0x3b, 0x3c, 0x3e, 0x00, 0x01, 0x02
	, 0x03, 0x04, 0x00, 0x01, 0x07, 0x08, 0x09, 0x09, 0x05, 0x06, 0x00, 0x00, 0x04, 0x3f, 0x3f, 0x11
	, 0x00, 0x00, 0x07, 0x3a, 0x3b, 0x3c, 0x3e, 0x3f, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05
	, 0x09, 0x09, 0x00, 0x08, 0x06, 0x05, 0x06, 0x07, 0x09, 0x09, 0x00, 0x01, 0x00, 0x00, 0x02, 0x3f
	, 0x3f, 0x12, 0x00, 0x00, 0x07, 0x3b, 0x39, 0x3b, 0x3c, 0x3d, 0x3e, 0x3e, 0x00, 0x3f, 0x3e, 0x3f
	, 0x03, 0x08, 0x03, 0x01, 0x00, 0x00, 0x01, 0x01, 0x02, 0x04, 0x02, 0x00, 0x00, 0x02, 0x3f, 0x3f
	, 0x13, 0x00, 0x00, 0x06, 0x3d, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3c, 0x03, 0x00, 0x01, 0x00, 0x00
	, 0x0a, 0x3f, 0x3f, 0x14, 0x00, 0x00, 0x06, 0x3f, 0x38, 0x37, 0x38, 0x39, 0x3a, 0x3a, 0x01, 0x3b
	, 0x3e, 0x00, 0x00, 0x0c, 0x3f, 0x3f, 0x15, 0x00, 0x00, 0x07, 0x3e, 0x3d, 0x3c, 0x3b, 0x3c, 0x3e
	, 0x00, 0x00, 0x0d, 0x3f, 0x3f, 0x17, 0x00, 0x00, 0x1a, 0x3f, 0x3f, 0x0b, 0x00, 0x00, 0x0b, 0x3f
	, 0x00, 0x00, 0x18, 0x3f, 0x00, 0x00, 0xff, 0xfd, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff
	, 0xff, 0xff, 0x54, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0f, 0xc5, 0xff, 0xff, 0x14, 0xc5, 0x0c
	, 0x00, 0x00, 0x1b, 0x03, 0x97, 0xff, 0xff, 0x12, 0x97, 0x03, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00
	, 0x18, 0x36, 0xd4, 0xff, 0xff, 0x0e, 0xd3, 0x36, 0x00, 0x00, 0x04, 0x01, 0x01, 0x00, 0x00, 0x00
	, 0x1a, 0x51, 0xc8, 0xff, 0xff, 0x0a, 0xc6, 0x51, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x15, 0x01
	, 0x00, 0x00, 0x06, 0x12, 0x5a, 0x9f, 0xd3, 0xd9, 0xfd, 0xfd, 0x00, 0xd9, 0xd3, 0x9f, 0x5a, 0x12
	, 0x00, 0x00, 0xff, 0xf7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8 rgb_scanline_data[] =
{
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,

	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF
};

int init_nsc_suite(void)
{
	return 0;
}

int clean_nsc_suite(void)
{
	return 0;
}

int add_nsc_suite(void)
{
	add_test_suite(nsc);

	add_test_function(nsc_decode);
	add_test_function(nsc_encode);

	return 0;
}

void test_nsc_decode(void)
{
	int i;
	NSC_CONTEXT* context;

	context = nsc_context_new();
	nsc_context_set_cpu_opt(context, CPU_SSE2);
	nsc_process_message(context, 32, 15, 10, (uint8*) nsc_data, sizeof(nsc_data));
	/*freerdp_hexdump(context->bmpdata, 15 * 10 * 4);*/
	for (i = 0; i < 30000; i++)
	{
		nsc_process_message(context, 32, 54, 44, (uint8*) nsc_stress_data, sizeof(nsc_stress_data));
	}
	nsc_context_free(context);
}

void test_nsc_encode(void)
{
	int i;
	uint8* rgb_data;
	STREAM* enc_stream;
	NSC_CONTEXT* context;

	rgb_data = (uint8 *) malloc(64 * 64 * 3);
	for (i = 0; i < 64; i++)
		memcpy(rgb_data + i * 64 * 3, rgb_scanline_data, 64 * 3);

	context = nsc_context_new();
	nsc_context_set_cpu_opt(context, CPU_SSE2);
	nsc_context_set_pixel_format(context, RDP_PIXEL_FORMAT_R8G8B8);

	enc_stream = stream_new(65536);
	stream_clear(enc_stream);

	for (i = 0; i < 30000; i++)
	{
		stream_set_pos(enc_stream, 0);
		nsc_compose_message(context, enc_stream, rgb_data, 64, 64, 64 * 3);
	}
	/*freerdp_hexdump(stream_get_head(enc_stream), stream_get_length(enc_stream));*/
	nsc_process_message(context, 32, 64, 64, stream_get_head(enc_stream), stream_get_length(enc_stream));
	/*freerdp_hexdump(context->bmpdata, 64 * 64 * 4);*/
	stream_free(enc_stream);

	nsc_context_free(context);
}
