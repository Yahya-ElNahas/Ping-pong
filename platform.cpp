struct ButtonState {
	bool isPressed;
	bool isChanged;
};

enum {
	BTN_UP,
	BTN_DOWN,
	BTN_W,
	BTN_S,
	BTN_COUNT
};

struct Input {
	ButtonState buttons[BTN_COUNT];
};