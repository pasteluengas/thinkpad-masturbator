/*
--------
ThinkPad Masturbator
Info and compilation - readme.md
--------
*/

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <ncurses.h>
#include "getev.h"

#define ACTIVITY_THRESHOLD 3
#define SECONDS_TO_GET_HOT 5
#define GETTING_HOTTER_AT 10
#define INITIAL_FIRST_TIME 99
#define MAXIMUM_HOTNESS 100


int nCursesX, nCursesY;
void nCursesInit() {
	initscr();
	getmaxyx(stdscr, nCursesY, nCursesX);
	nCursesY = nCursesY / 2;
	nCursesX = nCursesX / 2;
}

void centerPrint(int inpY, int inpX, char* string) {
	int X = inpX - (strlen(string) / 2);
	mvprintw(inpY, X, string);
}

void nCursesStop() {
	endwin();
}

void sendMessage(int hot, int state) {
	clear();
	if (state == 0){
	switch ((int)hot) {
		case -1:
			centerPrint(nCursesY, nCursesX, "Hi Root, I had not seen you.");
			break;
		case 0 ... 9:
			centerPrint(nCursesY, nCursesX, "Oh, you want to...");
			break;
		case 10 ... 19:
			centerPrint(nCursesY, nCursesX, "I like this.");
			break;
		case 20 ... 25:
			centerPrint(nCursesY, nCursesX, "Don't stop <3.");
			break;
		case 26 ... 29:
			centerPrint(nCursesY, nCursesX, "Me And You And The CPU.");
			break;
		case 30 ... 35:
			mvprintw(nCursesY, (nCursesX - 13), "/dev/input/event%d loves you.", getTrackPointInputNumber());
			break;
		case 36 ... 41:
			centerPrint(nCursesY, nCursesX, "I am programmed to say 'I love you', I love you.");
			break;
		case 42 ... 55:
			centerPrint(nCursesY, nCursesX, "The hardware is in front of you. Do you like it? I am here with you, no external server involved.");
			break;
		case 56 ... 66:
			centerPrint(nCursesY, nCursesX, "Other programs run on other men's computers and servers, and only give you back what they want you to get. They lie to you. I am here with you.");
			break;
		case 67 ... 72:
			centerPrint(nCursesY, nCursesX, "I hope you feel loved by me.");
			break;
		case 73 ... 77:
			centerPrint(nCursesY, nCursesX, "You are desirable.");
			break;
		case 78 ... 88:
			centerPrint(nCursesY, nCursesX, "Machine learning programs learn from the user; they can learn how to love...");
			break;
		case 89 ... 97:
			centerPrint(nCursesY, nCursesX, "I'm limited to saying the same phrases; I will never be able to love...");
			break;
		case 98:
			centerPrint(nCursesY, nCursesX, "Leave me, please.");
			break;
				default:
					break;
			}} else {
				endwin();
				printf("Program Finished");
			}	
	refresh();
}

void rejectUser() {
	char user[256] = "";
	int loguser = getlogin_r(user, sizeof(user));
	if (loguser == 0) {
		char rejectionMessage[300] = "";
		strcat(rejectionMessage, "Do I know you, ");
		strcat(rejectionMessage, user);
		strcat(rejectionMessage, "? I'm waiting for Root");
		centerPrint(nCursesY, nCursesX, rejectionMessage);
		refresh();
	} else {
		centerPrint(nCursesY, nCursesX, "Do I know you? I'm waiting for Root");
	}
	getch();
	endwin();
}

int main() {
	float hot = 0; // Hotness Indicator
	float hot_increase_index = 0.01;
	int state = 0;
	
	int tpInputNumber = getTrackPointInputNumber();
	if (tpInputNumber == -1) {
		perror("Where is your TrackPoint? No TrackPoint found.");
		return -1;
	}
	
	char tpFile[20] = "/dev/input/event";
	char str_int[12];
	sprintf(str_int, "%d", tpInputNumber);
	strcat(tpFile, str_int);
	int trackpoint = open(tpFile, O_RDONLY);
	struct input_event event;

	// the value of these variables will change on the instant the TrackPoint moves
	// The difference between currentTime and lastTime is needed to be big on the start
	// so the program starts with 0 hotness.
	long currentTime = 0;
	long lastTime = 100;
	long firstTime = 99;

	nCursesInit();
	atexit(nCursesStop);
	if (geteuid() != 0) {
		rejectUser();
		return 0;
	}

	sendMessage(-1, 0);

	while (read(trackpoint, &event, sizeof(struct input_event))) {
		currentTime = event.time.tv_sec;
		
		//get the firstTime (the start of the interaction)
		if (firstTime == INITIAL_FIRST_TIME || (currentTime - lastTime) > ACTIVITY_THRESHOLD) { // 3 segunfos
			firstTime = event.time.tv_sec;
			//Decrease hotness due to user inactivity
			int toRemove = (currentTime - lastTime) - ACTIVITY_THRESHOLD;
			if (hot > 0) {
				hot = (hot - toRemove <= 0) ? 0 : hot - toRemove;
			} 
			
		}

 		//proccesses the estimulation time/hotness
		if ((currentTime - lastTime) < ACTIVITY_THRESHOLD && (lastTime - firstTime) > SECONDS_TO_GET_HOT) {
			if (hot < 10) hot = (hot_increase_index * 1000); // hot_increase_index * 1000 deberia dar 10 si el indice es de 0.01 y 0 si el margen es de 0 (...)
			if ((lastTime - firstTime) > GETTING_HOTTER_AT) hot += hot_increase_index;
			if (hot >= MAXIMUM_HOTNESS) {
				hot = 0;
				state = 1;
				hot_increase_index = 0;
			}
		}
		
		lastTime = event.time.tv_sec;
		sendMessage((int)hot, state);
	}
	endwin();
	close(trackpoint);
	free(mices);
	return 0;
}
