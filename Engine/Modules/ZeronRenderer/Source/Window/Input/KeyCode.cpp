// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "KeyCode.h"

namespace Zeron
{
    KeyCode::KeyCode()
        : mType(Unknown)
    {
    }

    KeyCode::KeyCode(Type key)
        : mType(key)
    {
    }

    KeyCode::KeyCode(int key)
        : mType(static_cast<Type>(key))
    {
    }

    const char* KeyCode::ToString() const
    {
        switch (mType) {
            case N0:			return "Number 0";
            case N1:			return "Number 1";
            case N2:			return "Number 2";
            case N3:			return "Number 3";
            case N4: 			return "Number 4";
            case N5: 			return "Number 5";
            case N6: 			return "Number 6";
            case N7: 			return "Number 7";
            case N8: 			return "Number 8";
            case N9: 			return "Number 9";
            case A:				return "A";
            case B:				return "B";
            case C:				return "C";
            case D:				return "D";
            case E:				return "E";
            case F:				return "F";
            case G:				return "G";
            case H:				return "H";
            case I:				return "I";
            case J:				return "J";
            case K:				return "K";
            case L:				return "L";
            case M:				return "M";
            case N:				return "N";
            case O:				return "O";
            case P:				return "P";
            case Q:				return "Q";
            case R:				return "R";
            case S:				return "S";
            case T:				return "T";
            case U:				return "U";
            case V:				return "V";
            case W:				return "W";
            case X:				return "X";
            case Y:				return "Y";
            case Z:				return "Z";
            case Escape:		return "Escape";
            case Enter:			return "Enter";
            case Tab:			return "Tab";
            case Backspace:		return "Backspace";
            case Insert:		return "Insert";
            case Delete:		return "Delete";
            case LeftShift:		return "Left Shift";
            case LeftControl:	return "Left Control";
            case LeftAlt:		return "Left Alt";
            case LeftSuper:		return "Left Super";
            case RightShift:	return "Right Shift";
            case RightControl:	return "Right Control";
            case RightAlt:		return "Right Alt";
            case RightSuper:	return "Right Super";
            case Right:			return "Right";
            case Left:			return "Left";
            case Down:			return "Down";	
            case Up:			return "Up";
            case PageUp:		return "Page Up";
            case PageDown:		return "Page Down";
            case Home: 			return "Home";
            case End: 			return "End";
            case CapsLock: 		return "Caps Lock";
            case ScrollLock: 	return "Scroll Lock";
            case NumLock:  		return "Num Lock";
            case PrintScreen:  	return "Print Screen";
            case Pause:  		return "Pause";
            case F1:  			return "F1";
            case F2:  			return "F2";
            case F3:  			return "F3";
            case F4:  			return "F4";
            case F5:  			return "F5";
            case F6:  			return "F6";
            case F7:   			return "F7";
            case F8:   			return "F8";
            case F9:   			return "F9";
            case F10:  			return "F10";
            case F11:  			return "F11";
            case F12:  			return "F12";
            case F13:  			return "F13";
            case F14:  			return "F14";
            case F15:  			return "F15";
            case F16:   		return "F16";
            case F17:   		return "F17";
            case F18:   		return "F18";
            case F19:  			return "F19";
            case F20:  			return "F20";
            case F21:  			return "F21";
            case F22:  			return "F22";
            case F23:  			return "F23";
            case F24:  			return "F24";
            case F25:   		return "F25";
            case KP0:   		return "KeyPad 0";
            case KP1:   		return "KeyPad 1";
            case KP2:  			return "KeyPad 2";
            case KP3:  			return "KeyPad 3";
            case KP4:  			return "KeyPad 4";
            case KP5:  			return "KeyPad 5";
            case KP6:  			return "KeyPad 6";
            case KP7:   		return "KeyPad 7";
            case KP8:   		return "KeyPad 8";
            case KP9:   		return "KeyPad 9";
            case KPDecimal:   	return "KeyPad Decimal";
            case KPDivide:   	return "KeyPad Divide";
            case KPMultiply:   	return "KeyPad Multiply";
            case KPSubtract:   	return "KeyPad Subtract";
            case KPAdd:   		return "KeyPad Add";
            case KPEnter:   	return "KeyPad Enter";
            case KPEqual:   	return "KeyPad Equal";
            case Space:   		return "Space";
            case Apostrophe:   	return "Apostrophe";
            case Comma:			return "Comma";
            case Minus:			return "Minus";
            case Period:		return "Period";
            case Slash:			return "Slash";
            case Semicolon:		return "Semicolon";
            case Equal:			return "Equal";
            case LeftBracket:	return "Left Bracket";
            case RightBracket:  return "Right Bracket";
            case Backslash:     return "Backslash";
            case Tilde:	        return "Tilde";
            case Menu:			return "Menu";
            case Unknown:		return "Unknown Key";
            default:			return "Invalid Key";
        }
    }

}