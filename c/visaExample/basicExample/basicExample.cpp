// basicExample.cpp
//
// 
// Additional Dependency: C:\Program Files\IVI Foundation\VISA\Win64\lib\msc;
//

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "visa.h"

#define MAX_CNT 200

int bclear(ViChar*, int);

int main()
{
	ViStatus    status;					/* For checking errors         */
	ViSession   defaultRM, instr;		/* Communication channels      */
	ViUInt32    retCount = 0;			/* Return count from string I/O */
	const int	bufferSz = 256;
	ViChar		buffer[bufferSz];			/* Buffer for string I/O       */
	ViUInt32	numInstrs = 0;
	ViFindList	fList = 0;
	ViChar		desc[VI_FIND_BUFLEN];
	ViChar		errorText[256];
	ViUInt16	iManf;
	ViString	iModelStr[256];
	int i, j, k, l; //counter ints
	char*		deviceIDs[256];
	int			maxChoice = 0;
	int			instNum = 999;
	ViPUInt32	retCnt = 0;
	char		cmd[128];

	

	for (i = 0; i < 256; i++) {
		deviceIDs[i] = (char*)malloc(256 * (sizeof(deviceIDs)));
	}

	

	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS) {
		/* Error Initializing VISA...exiting*/
		return -1;
	}

	status = viFindRsrc(defaultRM, "?*", &fList, &numInstrs, desc);
	strcpy_s(deviceIDs[0], 256, desc);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf(errorText);
		printf("viFindRsrc: %d", status);
		return -1;
	}

	printf("Found %d instruments\n", numInstrs);
	printf("Description desc: %s\n", desc);
	for (int i = numInstrs - 1; i > 0; i--) {
		viFindNext(fList, desc);
		strcpy_s(deviceIDs[i], 256, desc);
		maxChoice++;
	}
	for (i = 0; i < numInstrs; i++) {
		printf("inst %d: %s\n", i, deviceIDs[i]);
	}
	printf("Choose which instrument(999 to quit) : ");
	scanf_s("%d", &instNum);
	if (0 <= instNum && instNum <= maxChoice) {
		// maybe I can do this in a better way. :) - Ryan
	}
	else if (instNum == 999) {
		printf("Quitting\n");
		return 0;
	}
	else {
		printf("Invalid choice!! Seriously?  sigh....");
		return 1;
	}
	printf("Device String: %s\n", deviceIDs[instNum]);
	viOpen(defaultRM, deviceIDs[instNum], VI_NULL, VI_NULL, &instr);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf(errorText);
		printf("viFindRsrc: %d", status);
		return -1;
	}
	printf("Device connection opened");
	status = viSetAttribute(instr, VI_ATTR_TERMCHAR, (ViChar)'\n');
	status = viSetAttribute(instr, VI_ATTR_TERMCHAR_EN, VI_TRUE);

	bclear(buffer, bufferSz);
	sprintf_s(cmd, "*idn?\n");
	status = viQueryf(instr, (ViConstString)cmd, "%64c", buffer);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	printf("ID: %s", buffer);
	
	sprintf_s(cmd, "syst:rem\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	sprintf_s(cmd, "*rst\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	sprintf_s(cmd, "*cls\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	sprintf_s(cmd, ":source:volt:level:ampl 120\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: cmd = %s -- Errortext = %s\n", status, cmd, errorText);
		return -1;
	}
	sprintf_s(cmd, "output:state on\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	Sleep(3000);
	sprintf_s(cmd, "output:state off\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	status = viClose(defaultRM);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	return 0;
}

int bclear(ViChar* buff, int length) {
	int i;
	for (i = 0; i < length; i++) {
		buff[i] = 0;
	}
	return 0;
}


