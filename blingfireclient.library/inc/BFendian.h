/**
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */


#ifndef _BF_ENDIAN_H_
#define _BF_ENDIAN_H_

#include <stdint.h>

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#include <byteswap.h>
#define from_le16(x) bswap_16(x)
#define from_le32(x) bswap_32(x)
#define from_le64(x) bswap_64(x)
#else  // __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define from_le16(x) (x)
#define from_le32(x) (x)
#define from_le64(x) (x)
#endif // __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

template <typename T>
struct byteswap_helper
{
	static inline T from_le(T value)
	{
		return value;
	}
};

template <>
struct byteswap_helper<uint16_t>
{
	static inline uint16_t from_le(uint16_t value)
	{
		return from_le16(value);
	}
};

template <>
struct byteswap_helper<int16_t>
{
	static inline int16_t from_le(int16_t value)
	{
		return from_le16(value);
	}
};

template <>
struct byteswap_helper<uint32_t>
{
	static inline uint32_t from_le(uint32_t value)
	{
		return from_le32(value);
	}
};

template <>
struct byteswap_helper<int32_t>
{
	static inline int32_t from_le(int32_t value)
	{
		return from_le32(value);
	}
};

template <>
struct byteswap_helper<uint64_t>
{
	static inline uint64_t from_le(uint64_t value)
	{
		return from_le64(value);
	}
};

template <>
struct byteswap_helper<int64_t>
{
	static inline int64_t from_le(int64_t value)
	{
		return from_le64(value);
	}
};

#endif
