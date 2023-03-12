#define _USE_MATH_DEFINES

#include "ReelEndurance.h"

#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
#include <list>

using namespace std;

bool running = false;

bool valid = false;
bool monst = false;
bool reset = false;
bool found = false;

int catches = 0;

bool ReelEndurance::HandleIncomingText(int32_t mode, bool indent, const char* message, int32_t* modifiedMode, bool* modifiedIndent, char* modifiedMessage, bool injected, bool blocked)
{
	UNREFERENCED_PARAMETER(mode);
	UNREFERENCED_PARAMETER(indent);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(modifiedMode);
	UNREFERENCED_PARAMETER(modifiedIndent);
	UNREFERENCED_PARAMETER(modifiedMessage);
	UNREFERENCED_PARAMETER(injected);
	UNREFERENCED_PARAMETER(blocked);

	if
	(
		string(message).find("Something caught the hook!") != string::npos ||
		string(message).find("Something caught the hook!!!") != string::npos ||
		string(message).find("You feel something pulling at your line.") != string::npos ||
		string(message).find("Your keen angler's senses tell you that this is the pull of a") != string::npos ||
		string(message).find("Something caught the hook!") != string::npos
	)
	{
		valid = true;
	}

	if
	(
		string(message).find("Something clamps onto your line ferociously!") != string::npos
	)
	{
		monst = true;
	}

	if
	(
		string(message).find("You didn't catch anything.") != string::npos ||
		string(message).find("You cannot use that command at this time.") != string::npos ||
		string(message).find("You must wait longer to perform that action.") != string::npos
	)
	{
		reset = true;
	}

	if
	(
		string(message).find("You don't know how much longer you can keep this one on the line...") != string::npos
	)
	{
		found = true;
	}

	return false;
}

uint32_t ReelEndurance::ThreadEntry(void)
{
	while (!this->IsTerminated())
	{
		if (InventoryFull())
		{
			running = false;
		}

		if (running) {
			if (valid && !monst && !reset && found) {
				Log("Pulling fish up!");

				Sleep(RandomA(1000));

				QueueCommand(-1, "/sendkey enter down");
				QueueCommand(-1, "/sendkey enter up");

				catches++;

				Sleep(RandomA(14000));

				if (!InventoryFull()) 
				{
					QueueCommand(2, "/fish");
				}
				

				valid = false;
				monst = false;
				reset = false;
				found = false;
			}

			if (!valid && !monst && reset && !found) {
				Log("Resetting!");

				Sleep(RandomA(14000));

				if (!InventoryFull())
				{
					QueueCommand(2, "/fish");
				}

				valid = false;
				monst = false;
				reset = false;
				found = false;
			}

			if (!valid && monst && !reset && !found) {
				Sleep(RandomA(1000));

				QueueCommand(-1, "/sendkey escape down");
				QueueCommand(-1, "/sendkey escape up");

				Sleep(RandomA(14000));

				if (!InventoryFull())
				{
					QueueCommand(2, "/fish");
				}

				valid = false;
				monst = false;
				reset = false;
				found = false;
			}
		}

		Sleep(RandomA(100));
	}

	return 0;
}

void ReelEndurance::Log(const char* format, ...) {
	if (debug) {
		char message[4096];
		va_list args;
		__crt_va_start(args, format);
		_vsprintf_s_l(message, 4096, format, NULL, args);
		__crt_va_end(args);
		m_AshitaCore->GetChatManager()->Write(0, true, message);
	}
}

void ReelEndurance::QueueCommand(int32_t mode, const char* format, ...) {
	char command[4096];
	va_list args;
	__crt_va_start(args, format);
	_vsprintf_s_l(command, 4096, format, NULL, args);
	__crt_va_end(args);
	Log("[COMMAND] %s", command);
	m_AshitaCore->GetChatManager()->QueueCommand(mode, command);
}

bool ReelEndurance::IsRunning()
{
	return running;
}

void ReelEndurance::SetRunning(bool _running)
{
	running = _running;
}

int ReelEndurance::RandomA(int factor) {
	int min = factor - (int)(factor * 0.2F);
	int max = factor + (int)(factor * 0.2F);
	return rand() % ((max - min) + 1) + min;
}

bool ReelEndurance::GetValid()
{
	return valid;
}

bool ReelEndurance::GetMonst()
{
	return monst;
}

bool ReelEndurance::GetReset()
{
	return reset;
}

bool ReelEndurance::GetFound()
{
	return found;
}

int ReelEndurance::GetCatches()
{
	return catches;
}

bool ReelEndurance::InventoryFull()
{
	IInventory* inventory = this->m_AshitaCore->GetMemoryManager()->GetInventory();

	if (inventory->GetContainerCount(0) < inventory->GetContainerCountMax(0))
	{
		return false;
	}

	return true;
}