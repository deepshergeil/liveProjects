//
// Created by Deepak Shergeil on 1/25/2018.
//

#ifndef CSCI402_CODE_MY402SORT_H
#define CSCI402_CODE_MY402SORT_H




typedef struct banktransaction
{
    char type;
    time_t time;
    unsigned long amount;
    char* description;
}transaction;
extern char* preprocess(char*);

#endif //CSCI402_CODE_MY402SORT_H
