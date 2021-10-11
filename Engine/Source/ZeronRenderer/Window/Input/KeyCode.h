// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	struct KeyCode {
		enum Type : uint16_t {
			
			N0 = 0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
			
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			
			Escape, Enter, Tab, Backspace, Insert, Delete,

			LeftShift,  LeftControl,  LeftAlt,  LeftSuper,
			RightShift, RightControl, RightAlt, RightSuper,
			
			Right, Left, Down, Up,
			
			PageUp, PageDown, Home, End, CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
			
			F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, F13, F14,
			F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

			KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
			KPDecimal, KPDivide, KPMultiply, KPSubtract, KPAdd, KPEnter, KPEqual,
			
			Space,				/*   */
			Apostrophe,			/* ' */
			Comma,				/* , */
			Minus,				/* - */
			Period,				/* . */
			Slash,				/* / */
			Semicolon,			/* ; */
			Equal,				/* = */
			LeftBracket,		/* [ */
			RightBracket,		/* \ */
			Backslash,			/* ] */
			Tilde,				/* ~ */

			Menu,
			COUNT,
			Unknown
		};

		KeyCode();
		KeyCode(Type key);
		KeyCode(int key);
		KeyCode(const KeyCode& other) = default;
		
		operator Type() const							{ return mType; }
		operator const int() const						{ return static_cast<int>(mType); }
		operator const bool() const						{ return mType != Unknown; }
		bool operator ==(const KeyCode& other) const	{ return mType == other.mType; }
		bool operator !=(const KeyCode& other) const	{ return mType != other.mType; }
		bool operator ==(const Type& other) const		{ return mType == other; }
		bool operator !=(const Type& other) const		{ return mType != other; }
		
		KeyCode& operator =(const KeyCode& other) = default;
		
		const char* ToString() const;

	private:
		Type mType;
	};
}
