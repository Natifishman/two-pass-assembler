#ifndef MAMAN14V10_SECONDPASSHELPER_H
#define MAMAN14V10_SECONDPASSHELPER_H

#include "assemblerHelper.h"
#include "strHandle.h"

int errorsScanner(line *, int, int);

void updateLabelDC(label *, int, int *);

void handleEntry(line *, int, label *, int, int);

void scanForEntrys(line *, int, label *, int, int);

void codeMissedLabel(line *, int c, label *, int, int, int);

void codeExternMissed(line *, int, int, int *);

void searchMissedLabel(line *, int, label *, int, int);

void secondPass(line *, int, label *, int, int);

int is_definedLabel(label *, int , const char *);

#endif
