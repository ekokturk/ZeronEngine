// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#define ZE_ENUM_OPERATIONS(Enum) \
	inline Enum		operator|	(Enum l,  Enum r)	{ return static_cast<Enum>(static_cast<uint32_t>(l)	| static_cast<uint32_t>(r)); } \
	inline Enum		operator&	(Enum l,  Enum r)	{ return static_cast<Enum>(static_cast<uint32_t>(l) & static_cast<uint32_t>(r)); } \
	inline Enum		operator^	(Enum l,  Enum r)	{ return static_cast<Enum>(static_cast<uint32_t>(l) ^ static_cast<uint32_t>(r)); } \
	inline Enum		operator~	(Enum l)			{ return static_cast<Enum>(~static_cast<uint32_t>(l)); } \
	inline Enum&	operator|=	(Enum& l, Enum r)	{ return l = l | r; } \
	inline Enum&	operator&=	(Enum& l, Enum r)	{ return l = l & r; } \
	inline Enum&	operator^=	(Enum& l, Enum r)	{ return l = l ^ r; } \
	inline bool		operator!	(Enum l)			{ return static_cast<uint32_t>(l) != 0; } 
	