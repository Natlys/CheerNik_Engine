#ifndef NW_DECL_HPP
#define NW_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	// --==<global>==--
	class NW_API CoreEngine;
	class NW_API CoreState;
	class NW_API AWindow;
	// --systems
	class NW_API IOSys;
	class NW_API TimeSys;
	class NW_API MemSys;
	class NW_API DataSys;
	class NW_API GuiSys;
	// --objects
	
	// --==<enums>==--
	enum KeyCodes : Char16 {
		KC_NONE = 0,
		KC_BELL = '\a', KC_BACKSPACE = NW_KEY_BACKSPACE_8, KC_HTAB = NW_KEY_HTAB_9, KC_ENTER = NW_KEY_ENTER_10, KC_VTAB = 11,
		KC_LSHIFT = NW_KEY_SHIFT_LEFT, KC_RSHIFT = NW_KEY_SHIFT_RIGHT,
		KC_LCTRL = NW_KEY_CONTROL_LEFT, KC_RCTRL = NW_KEY_CONTROL_RIGHT,
		KC_PAUSE = NW_KEY_PAUSE, KC_CAPS_LOCK = NW_KEY_CAPS_LOCK, KC_ESCAPE = NW_KEY_ESCAPE_27,
		KC_SPACE = NW_KEY_SPACE_32, KC_EXCLAIM = 33, KC_HASH = 35,
		KC_LEFT = NW_KEY_LEFT_37, KC_UP = NW_KEY_UP_38, KC_RIGHT = NW_KEY_RIGHT_39, KC_DOWN = NW_KEY_DOWN_40,
		KC_INSERT = NW_KEY_INS_45, KC_DEL = NW_KEY_DEL_46, KC_HOME = NW_KEY_HOME,
		KC_K0 = NW_KEY_KP_0, KC_K1 = NW_KEY_KP_1, KC_K2 = NW_KEY_KP_2, KC_K3 = NW_KEY_KP_3, KC_K4 = NW_KEY_KP_4,
		KC_K5 = NW_KEY_KP_5, KC_K6 = NW_KEY_KP_6, KC_K7 = NW_KEY_KP_7, KC_K8 = NW_KEY_KP_8, KC_K9 = NW_KEY_KP_9,
		KC_COLON = NW_KEY_KP_0, KC_SEMICOLON = NW_KEY_SEMICOLON_59, KC_LESSER = 60, KC_EQUAL = 61, KC_GREATER = 62,
		KC_A = NW_KEY_A_65, KC_B = NW_KEY_B_66, KC_C = NW_KEY_C_67, KC_D = NW_KEY_D_68,
		KC_E = NW_KEY_E_69, KC_F = NW_KEY_F_70, KC_G = NW_KEY_G_71, KC_H = NW_KEY_H_72, KC_I = NW_KEY_I_73,
		KC_J = NW_KEY_J_74, KC_K = NW_KEY_K_75, KC_L = NW_KEY_L_76, KC_M = NW_KEY_M_77, KC_N = NW_KEY_N_78,
		KC_O = NW_KEY_O_79, KC_P = NW_KEY_P_80, KC_Q = NW_KEY_Q_81, KC_R = NW_KEY_R_82, KC_S = NW_KEY_S_83,
		KC_T = NW_KEY_T_84, KC_U = NW_KEY_U_85, KC_V = NW_KEY_V_86, KC_W = NW_KEY_W_87, KC_X = NW_KEY_X_88,
		KC_Y = NW_KEY_Y_89, KC_Z = NW_KEY_Z_90,
		KC_a = 'a', KC_b, KC_c, KC_d, KC_e, KC_f, KC_g, KC_h, KC_i, KC_j, KC_k, KC_l, KC_m, KC_n, KC_o, KC_p = 112,
		KC_q = 113, KC_r, KC_s, KC_t, KC_u, KC_v, KC_w, KC_x, KC_y, KC_z = 'z',
		KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
		KC_RECT_DITHER = 177, KC_QUAD = 254,
		KC_PGUP, KC_PGDN,
		KC_RETURN, KC_SCROLL,
		KC_NP_MUL, KC_NP_DIV, KC_NP_ADD, KC_NP_SUB, KC_NP_DECIMAL, KC_PERIOD,
		KC_COUNT = NW_KEY_LAST
	};
	enum MouseButtons : UInt32 {
		MB_BUTTON_LEFT = NW_MS_BTN_0, MB_BUTTON_RIGHT = NW_MS_BTN_1,
		MB_BUTTON_0 = NW_MS_BTN_0, MB_BUTTON_1 = NW_MS_BTN_1, MB_BUTTON_2 = NW_MS_BTN_2,
		MB_BUTTON_3 = NW_MS_BTN_3, MB_BUTTON_4 = NW_MS_BTN_4, MB_BUTTON_5 = NW_MS_BTN_5,
		MB_BUTTON_6 = NW_MS_BTN_6, MB_BUTTON_7 = NW_MS_BTN_7,
		MB_BUTTON_COUNT = NW_MS_BTN_LAST
	};
	// --==<enums>==--
}

#endif // NW_DECL_H