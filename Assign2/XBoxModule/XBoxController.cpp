#include <iostream>
#include "XboxController.h"
#include "XInputWrapper.h"
#include <cmath>

//Could have made a global variable of XINPUT_STATE but decided to make a local for each individual function

GamePad::XBoxController::XBoxController(XInputWrapper* xinput, DWORD id) {

	this->xinput = xinput;
	this->id = id;
	
}

void GamePad::XBoxController::BatteryInformation()
{
	XINPUT_BATTERY_INFORMATION BatteryInformation;
	XInputGetBatteryInformation(id, BATTERY_DEVTYPE_GAMEPAD, &BatteryInformation);
	switch (BatteryInformation.BatteryLevel)
	{
	case BATTERY_LEVEL_EMPTY: std::cout << "Battery level empty " << std::endl;
		break;
	case BATTERY_LEVEL_LOW: std::cout << "Battery level low " << std::endl;
		break;
	case BATTERY_LEVEL_MEDIUM: std::cout << "Battery level medium " << std::endl;
		break;
	case BATTERY_LEVEL_FULL: std::cout << "Battery level full" << std::endl;
	}
	//Sleep(3000);
}

DWORD GamePad::XBoxController::GetControllerId()
{	
	return id;
}

bool GamePad::XBoxController::IsConnected() {

	XINPUT_STATE state;
	
	DWORD StateResult = xinput->XInputGetState(id, &state);
	
	return (StateResult);
}

bool GamePad::XBoxController::PressedA() {
	XINPUT_STATE state;
	

	ZeroMemory(&state, sizeof(state));
	
	xinput->XInputGetState(id, &state);
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
}

bool GamePad::XBoxController::PressedB() {

	XINPUT_STATE state;
	

	ZeroMemory(&state, sizeof(state));
	xinput->XInputGetState(id, &state);
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
		
}

bool GamePad::XBoxController::PressedX() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ;
}

bool GamePad::XBoxController::PressedY() {
	
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
}

bool GamePad::XBoxController::PressedLeftShoulder() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);

}

bool GamePad::XBoxController::PressedRightShoulder() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

bool GamePad::XBoxController::PressedLeftDpad() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
}

bool GamePad::XBoxController::PressedRightDpad() {
	
	XINPUT_STATE state;
	DWORD result;
	result = xinput->XInputGetState(id, &state);
	
	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
}

bool GamePad::XBoxController::PressedUpDpad() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	
	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
}

bool GamePad::XBoxController::PressedDownDpad() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
}

bool GamePad::XBoxController::PressedStart() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
}

bool GamePad::XBoxController::PressedBack() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
}

bool GamePad::XBoxController::PressedLeftThumb() {

	XINPUT_STATE state; 
	xinput->XInputGetState(id, &state);

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
}

bool GamePad::XBoxController::PressedRightThumb() {

	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
}

BYTE GamePad::XBoxController::LeftTriggerLocation()
{	
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	return (state.Gamepad.bLeftTrigger);
}

BYTE GamePad::XBoxController::RightTriggerLocation()
{
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	return (state.Gamepad.bRightTrigger);
}


GamePad::Coordinate GamePad::XBoxController::LeftThumbLocation()
{	
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY * LY);

	//determine the direction the controller is pushed
	float angle = atan2(LY , LX);

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude = (magnitude - INPUT_DEADZONE)*(32767) / (32767 - INPUT_DEADZONE); //mapping
																					 
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
	}

	//Set directions to the magnitude in X and Y
	float NewLX = magnitude * cos(angle); 
	float NewLY = magnitude * sin(angle);
	
	GamePad::Coordinate NewLThumblocation(NewLX, NewLY);


	return NewLThumblocation;
}

GamePad::Coordinate GamePad::XBoxController::RightThumbLocation()
{
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);

	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrt(RX*RX + RY * RY);

	//determine the direction the controller is pushed
	float angle = atan2(RY, RX);

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude = (magnitude - INPUT_DEADZONE)*(32767) / (32767 - INPUT_DEADZONE); //mapping

	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
	}

	//Set directions to the magnitude in X and Y
	float NewRX = magnitude * cos(angle);
	float NewRY = magnitude * sin(angle);

	GamePad::Coordinate NewRThumblocation(NewRX, NewRY);


	return NewRThumblocation;
}

void GamePad::XBoxController::Vibrate(WORD left, WORD right)
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(vibration));
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
	xinput->XInputSetState(id, &vibration);
}

void GamePad::XBoxController::SetDeadzone(unsigned int radius)
{
	
	XINPUT_STATE state;
	xinput->XInputGetState(id, &state);
	INPUT_DEADZONE = radius;

	std::cout << "Deadzone radius for P" << id + 1 << " = " << radius << std::endl;
	
	
}


//non member function
void GamePad::ButtonPressed(GamePad::XBoxController playerX) //takes in a XBoxController type variable
{
	DWORD id = playerX.GetControllerId();
	if (playerX.IsConnected() == ERROR_SUCCESS) {
		std::cout << "Player" << id + 1;

		//Buttons
		std::cout << " Pressed: ";
		
		if (playerX.PressedA()) {
			std::cout << "A ";
		}
		else
			std::cout << "  ";

		if (playerX.PressedB()) {
			std::cout << "B ";
		}
		else
			std::cout << "  ";

		if (playerX.PressedX()) {
			std::cout << "X ";
		}
		else
			std::cout << "  ";

		if (playerX.PressedY()) {
			std::cout << "Y ";
		}
		else
			std::cout << "  ";

		//if (playerX.PressedLeftShoulder()) {
		//	std::cout << "LS ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedRightShoulder()) {
		//	std::cout << "RS ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedLeftDpad()) {
		//	std::cout << " <= ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedRightDpad()) {
		//	std::cout << " => ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedUpDpad()) {
		//	std::cout << " ^ ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedDownDpad()) {
		//	std::cout << " v ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedStart()) {
		//	std::cout << "Start ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedBack()) {
		//	std::cout << "Back ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedLeftThumb()) {
		//	std::cout << "L3 ";
		//}
		//else std::cout << "  ";

		//if (playerX.PressedRightThumb()) {
		//	std::cout << "R3 ";
		//}
		//else std::cout << "  ";



		////Analogs
		////Triggers
		//std::cout << "LT " << (int)playerX.LeftTriggerLocation() << " " << "|";

		//std::cout << "RT " << (int)playerX.RightTriggerLocation() << " " << "|";

		////Thumbs
		std::cout << "LeftThumb: " << playerX.LeftThumbLocation().GetX() << " , ";
		std::cout << playerX.LeftThumbLocation().GetY() << " | ";
		std::cout << playerX.RightThumbLocation().GetY() << " | ";

		//std::cout << "RightThumb: " << playerX.RightThumbLocation().GetX() << " 


		//int Total = playerX.PressedA() + playerX.PressedB() + playerX.PressedX() + playerX.PressedY();
		////Vibration
		//if (Total > 2) { //if right & left trigger pressed = vibrate

		//	std::cout << "vibrating";
		//	playerX.Vibrate(50000, 30000); //Left,right motor speed
		//}
		// if(playerX.PressedLeftShoulder() & playerX.PressedRightThumb())
		//
		//{
		//	std::cout << "Not vibrating";
		//	playerX.Vibrate(0, 0);
		//}



		std::cout << std::endl;
		Sleep(100);
	}
	else 
		std::cout << "-----NOT FOUND-----" << std::endl;

		
}
//Non member functions that checks for player ids, battery info and deadzones
bool GamePad::PlayerConnection(GamePad::XBoxController &playerX) //takes in a XBoxController type variable
{
	DWORD id = playerX.GetControllerId();
	
	int playersconnected = (playerX.IsConnected() == ERROR_SUCCESS) ;
	if (playersconnected == 0) { //if no players connected
		

		std::cout << "Player" << id + 1 << " is not connected..." << std::endl;
		return 0;
	}
	else { 
		if (playerX.IsConnected() == ERROR_SUCCESS) {//if players connected

			std::cout << "Player" << id+1 <<" is connected" << std::endl;
			std::cout << "Player" << id + 1 << " ";
			playerX.BatteryInformation(); //prints out battery info
			playerX.SetDeadzone(0.3*32767); //comment out for multiple deadzones for different controllers
			
			
		}
		else {
			std::cout << "Player" << id+1  << " is not connected" << std::endl;
			
		}

		
	}
}

bool GamePad::EXIT(GamePad::XBoxController playerX) //takes in a XBoxController type variable
{
	return (playerX.PressedA() & playerX.PressedLeftShoulder());
}

