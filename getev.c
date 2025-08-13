/*
--------
ThinkPad Masturbator
Info and compilation - readme.md
--------
*/

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getev.h"

struct MouseDevice {
	char name[256];
	char handler[256];
	bool isTrackPoint;
};

struct MouseDevice *mices = NULL;
int micesCounter = 0;
void addMouse(const char* name, const char* handler) {
	//el propio infierno
	micesCounter++;
	mices = realloc(mices, (sizeof(struct MouseDevice) * micesCounter));
	strncpy(mices[micesCounter-1].name, name, 255);
	mices[micesCounter -1].name[sizeof(mices[micesCounter - 1].name) -1] = '\0';
	strncpy(mices[micesCounter-1].handler, handler, 255);
	mices[micesCounter -1].handler[sizeof(mices[micesCounter - 1].handler) - 1] = '\0';
	mices[micesCounter -1].isTrackPoint = true;
}

int getHandler(char* handler) {
	// parsing the H: line
	char handlerInt[3];
	char* le_punter = strstr(handler, "event");	
	handlerInt[0] = le_punter[5];
	if (le_punter[6] != ' ') {
		handlerInt[1] = le_punter[6];
		handlerInt[2] = (le_punter[7] != ' ') ? le_punter[7] : '\0';
	} else {
		handlerInt[1] = '\0';
		handlerInt[2] = '\0';
	}
	return atoi(handlerInt);
}

int getTrackPointInputNumber() {
	FILE *list;
	list = fopen("/proc/bus/input/devices", "r");	
	if (list == NULL) {
		return -1;
	}
	
	char nameBuffer[256];
	char handlerBuffer[256];
	char lineBuffer[256];
	while (fgets(lineBuffer, 256, list)){
		if (lineBuffer[0] == 'N') {
			strcpy(nameBuffer, lineBuffer);
		}
		if (lineBuffer[0] == 'H' && strstr(lineBuffer, "mouse")) {
			strcpy(handlerBuffer, lineBuffer);
			addMouse(nameBuffer, handlerBuffer);
		}	
	}

	//At this point every mouse is considered a trackpoint, this is a filtering process
	for (int i = 0; i < micesCounter; i++) {
		if (strcasestr(mices[i].name, "trackpoint")) {
			// si es
			return getHandler(mices[i].handler);
		}
		// "banned" words
		if (strcasestr(mices[i].name, "optical") || strcasestr(mices[i].name, "trackpad") || strcasestr(mices[i].name, "wired") || strcasestr(mices[i].name, "usb")) {
			mices[i].isTrackPoint = false; 
		}
	}

	for (int i = 0; i < micesCounter; i++) {
		if(mices[i].isTrackPoint) {
			return getHandler(mices[i].handler);
		}
	}
	
	return -1;
}
