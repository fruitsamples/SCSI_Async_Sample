/*								DisplayTestResults.c							*//* * DisplayTestResults.c * Copyright � 93 Apple Computer Inc. All Rights Reserved. * * Run the dialog until the user has had enough. */#include "SCSIAsyncSample.h"#include "SCSIDefinitions.h"#include <math.h>#define ALWAYS		1#define NEVER		1#define INFO		(*infoPtr)voidDisplayOneResult(		register InfoPtr				infoPtr	);voidDisplayTestResults(void){		InfoPtr							currentInfoPtr;				if (infoPtrQueue.qHead == NULL)			DisplayLogString(gLogListHandle, "\pNo devices to test");		for (currentInfoPtr = (InfoPtr) infoPtrQueue.qHead;				currentInfoPtr != NULL;				currentInfoPtr = (InfoPtr) currentInfoPtr->link) {			DisplayOneResult(currentInfoPtr);		}}voidDisplayOneResult(		register InfoPtr				infoPtr	){		Str255							work;		Str255							temp;		register short					i;		register RequestMemoryPtr		requestMemoryPtr;		UnsignedWide					deltaTime;		double							overallAverageTime;		double							mean;		double							sdNumerator;		double							sdDenomenator;		double							standardDeviation;				DisplayDeviceInfo(infoPtr);		if (INFO.validDevice) {			work[0] = 0;			AppendPascalString(work, "\p Final status: ");			AppendSigned(work, INFO.finalStatus);			AppendPascalString(work, "\p. Transfers ");			AppendUnsigned(work, INFO.transfersAttempted);			AppendPascalString(work, "\p attempted, ");			AppendUnsigned(work, INFO.transfersCompleted);			AppendPascalString(work, "\p completed, ");			AppendUnsigned(work, INFO.asynchRequests);			AppendPascalString(work, "\p asynch");			DisplayLogString(gLogListHandle, work);			LogStatus(gLogListHandle, INFO.finalStatus, "\p Status error");			/*			 * This is ugly but I have a plane to catch.			 */			if (NEVER == FALSE) {				for (i = 0; i < kRequestMemory; i++) {					requestMemoryPtr = &INFO.requestMemory[i];					if (ALWAYS || i < 10) {						MicrosecondDelta(							&requestMemoryPtr->startTime,							&requestMemoryPtr->endTime,							&deltaTime						);						work[0] = 0;						AppendUnsignedInField(temp, i, 2);						AppendPascalString(work, temp);						AppendPascalString(work, "\p: ");						AppendUnsignedInField(temp, requestMemoryPtr->blockNumber, 8);						AppendPascalString(work, temp);						AppendChar(work, ' ');						AppendDouble(temp, MicrosecondToDouble(&deltaTime) / 1000000.0, 6);						AppendPascalString(work, temp);						DisplayLogString(gLogListHandle, work);					}				}			}			/*			 * Convert from microseconds to seconds.			 */			MicrosecondDelta(				&INFO.testStartTime,				&INFO.testEndTime,				&deltaTime			);			if (INFO.transfersCompleted > 0) {				mean = INFO.sampleSum / (double) INFO.transfersCompleted;				if (INFO.transfersCompleted > 1) {					sdNumerator = INFO.sampleSumSquare						- (INFO.sampleSum * INFO.sampleSum) / (double) INFO.transfersCompleted;					sdDenomenator = (double) INFO.transfersCompleted - 1;					standardDeviation = sqrt(sdNumerator / sdDenomenator);				}			}			if (INFO.transfersCompleted > 0) {				overallAverageTime = MicrosecondToDouble(&deltaTime);				overallAverageTime /= ((double) INFO.transfersCompleted);				overallAverageTime /= 1000000.0;				work[0] = 0;				AppendPascalString(work, "\p Mean ");				AppendDouble(work, mean, 6);				if (INFO.transfersCompleted > 1) {					AppendPascalString(work, "\p, Standard Deviation ");					AppendDouble(work, standardDeviation, 6);				}				DisplayLogString(gLogListHandle, work);				work[0] = 0;				AppendPascalString(work, "\p Overall average ");				AppendDouble(work, overallAverageTime, 6);				AppendPascalString(work, "\p, Average overhead ");				AppendDouble(work, overallAverageTime - mean, 6);				DisplayLogString(gLogListHandle, work);			}		}}