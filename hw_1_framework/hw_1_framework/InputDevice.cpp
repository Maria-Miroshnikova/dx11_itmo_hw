#include "InputDevice.h"

InputDevice::InputDevice(Game* game): game(game) {

}

void InputDevice::AddPressedKey(int key) {
	keys.insert(key);
}

void InputDevice::RemovePressed(int key) {
	keys.erase(key);
}

bool InputDevice::IsKeyDown(int key) {
	return keys.find(key) != keys.end();
}

// undefined
void InputDevice::OnKeyDown() {

}

// undefined
void InputDevice::OnMouseMove() {

}