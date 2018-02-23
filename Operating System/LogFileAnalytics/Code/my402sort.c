#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"my402list.h"
#include"my402sort.h"
#include"cs402.h"
#include<ctype.h>
#include <errno.h>
#include <sys/stat.h>
FILE *fp=NULL;


void processTime(char *transtime,char *pT)
{
    int i=0;
    int j=0;
    for(i=0; i<15; i++){
        if(i > 10)
        { j = i+9; }
        pT[i] = transtime[j];
        j++;
    }
    pT[15] = '\0';

}

void processDesc(char *desc)
{
    int i =0 ;
    int j=0;
    while(desc[i] != '\0')
    {
        i++;
        if(i>24)
            break;

    }
    for(j = i; j<24; j++) {
        desc[j] = ' ';
    }
    desc[24]='\0';

}
void processBalance(long int bal,char *balance)
{

    int i;
    if(bal >= 0 ){

        if (bal >= 1000000000)
            fprintf(stdout," \?,\?\?\?,\?\?\?.\?\? |\n");
        else {
            balance[0] = ' '; balance[13] = ' ';
            for(i = 12; i > 0; i--) {
                if(i == 10){
                    balance[i]='.'; continue;
                }
                if(i == 6 || i == 2){
                    balance[i]=','; continue;
                }
                balance[i] = (char)(((int)'0')+(bal%10));
                bal= bal/10;
                if(bal == 0) {i--;break;}
            }
            if(i == 10){
                balance[i]='.'; i--;
                balance[i]='0'; i--;
            }
            while(i>0) {
                balance[i] = ' ';
                i--;
            }
            balance[14]='\0';
            fprintf(stdout,"%s |\n",balance);
        }
    } else {
        bal = bal * (-1);
        if (bal >= 1000000000)
            fprintf(stdout,"(\?,\?\?\?,\?\?\?.\?\?) |\n");
        else {
            balance[0] = '('; balance[13] = ')';
            for(i = 12; i > 0; i--) {
                if(i == 10){
                    balance[i]='.'; continue;
                }
                if(i == 6 || i == 2){
                    balance[i]=','; continue;
                }
                balance[i] = (char)(((int)'0')+(bal%10));
                bal= bal/10;
                if(bal == 0) {i--;break;}
            }
            if(i == 10){
                balance[i]='.'; i--;
                balance[i]='0'; i--;
            }
            while(i>0) {
                balance[i] = ' ';
                i--;
            }
            balance[14]='\0';
            fprintf(stdout,"%s |\n",balance);
        }
    }

}
void processAmt( char type,char *amt,long int amount)
{

    int i;
    if(type == '-')
    {
        if (amount >= 1000000000)
            fprintf(stdout,"(\?,\?\?\?,\?\?\?.\?\?) |");
        else {
            amt[0] = '('; amt[13] = ')';
            for(i = 12; i > 0; i--) {
                if(i == 10){
                    amt[i]='.'; continue;
                }
                if(i == 6 || i == 2){
                    amt[i]=','; continue;
                }
                amt[i] = (char)(((int)'0')+(amount%10));
                amount= amount/10;
                if(amount == 0) {i--;break;}
            }
            if(i == 10){
                amt[i]='.'; i--;
                amt[i]='0'; i--;
            }
            while(i>0) {
                amt[i] = ' ';
                i--;
            }
            amt[14]='\0';
            fprintf(stdout,"%s | ",amt);
        }

    }
    else
    {
        if (amount >= 1000000000)
            fprintf(stdout," \?,\?\?\?,\?\?\?.\?\? | ");
        else {
            amt[0] = ' ';
            amt[13] = ' ';
            for(i = 12; i > 0; i--)
            {
                if(i == 10){
                    amt[i]='.';
                    continue;
                }
                if(i == 6 || i == 2)
                {
                    amt[i]=',';
                    continue;
                }
                amt[i] = (char)(((int)'0')+(amount%10));
                amount= amount/10;
                if(amount == 0)
                {
                    i--;
                    break;}
            }
            if(i == 10){
                amt[i]='.'; i--;
                amt[i]='0'; i--;
            }
            while(i>0) {
                amt[i] = ' ';
                i--;
            }
            amt[14]='\0';
            fprintf(stdout,"%s | ",amt);
        }

    }

}
void PrintList(My402List* mylist)
{
    long int tBalance = 0;
    My402ListElem* elem=NULL;

    transaction *temp=NULL;
    fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
    fprintf(stdout,"|       Date      | Description              |         Amount |        Balance |\n");
    fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");


    for(elem=My402ListFirst(mylist);elem!=NULL;elem=My402ListNext(mylist,elem))
    {
        temp= (transaction*)elem->obj;
        time_t time=temp->time;
        char amt[15];
        char transtime[26];
        char pT[16];
        char desc[25];
        char balance[15];
        char type=temp->type;
        long int amount=temp->amount;

        /* Time MANIPULATION*/
        strncpy(transtime, ctime(&time), sizeof(transtime));
        processTime(transtime,pT);
        /* Time MANIPULATION Ends*/
        fprintf(stdout,"| %s | ",pT);

        /*desc manipulation*/

        strncpy(desc,temp->description,24);

        processDesc(desc);

        fprintf(stdout,"%s | ",desc);

        /*desc manipulation ends*/


        /*amount manipulation starts*/
        processAmt(type,amt,amount);

        amount=temp->amount;

        /*amount manipulation ends*/

        /*balance starts*/


        if(type == '+')
            tBalance = tBalance + amount;
        else
            tBalance = tBalance - amount;

        long int bal = tBalance;

        processBalance(bal,balance);

        /*balance ends*/

    }
    fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
}

char *Matchup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}


void handleAmount(char *field[],transaction *trans)
{
    char ch1;
    int k = 0;
    trans->amount = 0;
    while((field[2])[k] != '.')
    {
        ch1 = (field[2])[k];
        if(!isdigit(ch1)){
            fprintf(stderr,"Transaction amount not numeric\n");
            fprintf(stderr,"Malformed Input\n");
            exit(0);
        }
        trans->amount = trans->amount*10 + (unsigned long) atoi(&ch1);
        ++k;
    }
    if(trans->amount >= 10000000){
        fprintf(stderr,"Transaction amount too large\n");
        fprintf(stderr,"Malformed Input\n");
        exit(0);
    }
    k++;
    int checkLen = 0;
    while((field[2])[k]!= '\0')
    {
        ch1 = (field[2])[k];
        if(!isdigit(ch1)){
            fprintf(stderr,"Transaction amount is not numeric\n");
            fprintf(stderr,"Malformed Input\n");
            exit(0);
        }
        checkLen++;
        trans->amount = trans->amount*10 + (unsigned long) atoi(&ch1);
        ++k;
    }
    if(checkLen != 2){
        fprintf(stderr,"Transaction amount is too long\n");
        exit(0);
    }
}

void errorHandler(char *field[],int field_count)
{
    if(strlen(field[0])>1|| (strcmp(field[0],"+")&&strcmp(field[0],"-")))
    {
        fprintf(stderr,"Transaction type should be 1 character long and should be  + or - only\n");
        fprintf(stderr,"Malformed Input\n");
        exit(0);
    }

    if(field_count!=4)
    {
        fprintf(stderr,"All Transaction must have 4 fields\n");
        fprintf(stderr,"Malformed Input\n");
        exit(0);
    }
    if(strlen(field[1])>10)
    {
        fprintf(stderr,"Transaction timestamp is bad\n");
        fprintf(stderr,"Malformed Input\n");
        exit(0);
    }
    if(strlen(field[3])==0)
    {
        fprintf(stderr,"Transaction description cannot be empty\n");
        fprintf(stderr,"Malformed Input\n");
        exit(0);
    }

}

int parseData(My402List* mylist)
{

    char* field[4];
    char buf[1026];
    char* tok=NULL;
    int field_count=0;
    /*read line by line from the file*/
    while(fgets(buf, sizeof(buf),fp)!=NULL)
    {
        if(strcmp(buf,"\n")!=0)
        {
            int i=0;
            for(i=0;i<4;i++){
                field[i]=NULL;
            }
            transaction* trans=(transaction*)malloc(sizeof(transaction));
            field_count=0;
            /* check for 1024 transaction length */
            if(strlen(buf)>1024)
            {
                fprintf(stderr,"\nTransaction length cannot be more than 1024\n");
                exit(0);
            }

            char* tempstr = Matchup(buf);
            tempstr = preprocess(tempstr);
            if(tempstr == NULL)
            {
                return FALSE;
            }
            strcpy(buf,tempstr);
            free(tempstr);
            /*Tokenize the string*/
            char* temp=Matchup(buf);
            tok=strtok(temp,"\t");
            while(tok!=NULL && field_count<=4)
            {
                field[field_count]=Matchup(tok);
                tok=strtok(NULL,"\t");
                field_count++;
            }
            free(temp);

             errorHandler(field,field_count);

            if(!strcmp(field[0],"+"))
            {
                trans->type='+';

            }
            else
            {
                trans->type='-';
            }

            unsigned long transtime=(unsigned long)strtoul(field[1],NULL,0);
            if(!(transtime<=(unsigned long)time(NULL))&& 0<=transtime)
            {
                fprintf(stderr,"Timestamp should be between 0 and current time\n");
                exit(0);
            }
            trans->time=(time_t)transtime;

            handleAmount(field,trans);

            trans->description=Matchup(field[3]);

            My402ListAppend(mylist,(void*) trans); //Appending to List Finally

        }
    }

    return TRUE;

}

char *preprocess(char *data)
{

    char *end;
    if(data == NULL)
        return NULL;
    while(isspace(*data))
        data++;
    if(*data == 0)
        return data;

    end = data + strlen(data) - 1;
    while(end > data && (isspace(*end) || (*end == '\n')))
        end--;

    *(end+1) = 0;

    return data;
}



void sorting(My402List* list)
{

    My402ListElem *elem1 = NULL, *elem2 = NULL, *elem3 = NULL;
    transaction *temp1 = NULL, *temp2 = NULL, *obj = NULL;
    int swapped = 1;
    for(elem1 = My402ListLast(list);  elem1 != NULL && swapped; elem1 = My402ListPrev(list,elem1))
    {
        swapped = 0;
        for(elem2 = My402ListFirst(list); elem2 != NULL; elem2 = My402ListNext(list,elem2))
        {
            elem3 = My402ListNext(list,elem2);

            if(elem3 != NULL)
            {

                temp1 =(transaction*)elem2->obj;
                temp2 = (transaction*)elem3->obj;

                if(temp1->time == temp2->time)
                {
                    fprintf(stderr,"Duplicate timestamp found,Program will exit!!!!!!!!\n");
                    exit(0);
                }
                if(temp1->time > temp2->time)
                {

                    swapped = 1;
                    obj = elem2->obj;
                    elem2->obj = elem3->obj;
                    elem3->obj = obj;
                }

            }
        }
    }

}


int main(int argc,char** argv)
{
    struct stat statbuf;
    int readStatus=0;
    My402List mylist;
    My402ListInit(&mylist);
    if(argc<2 || argc>3 )
    {
        fprintf(stderr,"Invalid number of command line arguments.\n");
        fprintf(stderr,"Usage: ./warmup1 sort [tfile name]\n");
        exit(0);
    }
    if(strcmp(argv[1], "sort"))
    {

        fprintf(stderr,"Invalid command line argument, expected sort as second argument.\n");
        fprintf(stderr,"Usage: ./warmup1 sort [tfile name]\n");
        exit(0);

    }
    else if(!strcmp(argv[1], "sort") && argc == 2 )
    {
        fp=stdin;
    }
    else if(argc==3)
    {
        fp=fopen(argv[2],"r");
        if( stat(argv[2],&statbuf)==0)
            if(S_ISDIR(statbuf.st_mode)==1) {
                fprintf(stderr,"\n Input %s is a directory.\n",argv[2]);
                fprintf(stderr,"Usage: ./warmup1 sort [tfile name]\n");
                exit(0);}
    }
    if(fp==NULL)
    {
        if(argc == 3)
            fprintf(stderr,"%s\n", strerror(errno));
            fprintf(stderr,"%s\n", strerror(errno));
        exit(0);
    }
    else
    {
        readStatus=parseData(&mylist);

        if(readStatus==FALSE)
        {
            fprintf(stderr,"Cannot read the file!!!!!!Error\n");
            exit(0);
        }

        sorting(&mylist);
        PrintList(&mylist);
    }
    return(0);
}
