#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct unit {
    char *name;
    char *info;
    int numOfPickups;
    int numOfFrets;
    int numOfString;
    float menzureLength;
    float neckRadius;
    int *stringsWidth;
} guitar;

enum values {
    nameLength = 35,
    inputLength = 31,
    readStep = 15,
    keyboardReadStep = 70,
    splitSymbolCompensation = 1,
    sep = ';'
};

void MainMenu(); //вывод главного меню  @@
void AddMenu(); //вывод меню добавления элемента    @@
void InfoMenu(); //вывод меню помощи    @@
void numberSortMenu(); //вывод меню сортировки числовых полей   @@
void mainSortMenu(); //вывод главного меню сортировки   @@
void printAll(struct unit **table, int N); //вывод всех строк структуры @@
void structPrint(struct unit *table); //вывод определенной строки структуры @@

guitar **fileInput(struct unit **table, int *N); //организовывает ввод данных из файла  @@
int csvCount(FILE *df); //подсчитывает количество строк в файле (кол-во элементов массива структур) @@
void unitFill(FILE *df, struct unit **table,int i); //Заполняет один элемент (строку) массива структур  @@
void strRead(FILE *df, char **dest); //считывает одну строку из файла   @@
void StringDelete(char *s,int count); //удаление из строки первых count символов    @@
void clearUnit(struct unit *table); //очистка строки структуры  @@

void nameInp(int count, char **source, struct unit *table); //вводы различных полей структуры из строки в массив структур   @@
void infoInp(int count, char **source, struct unit *table);
void pickupsInp(int count, char **source, struct unit *table);
void fretsInp(int count, char **source, struct unit *table);
void numStringsInp(int count, char **source, struct unit *table);
void menzureInp(int count, char **source, struct unit *table);
void radiusInp(int count, char **source, struct unit *table);
void stringsWidthInp(int count, char **source, struct unit *table);

void keyboardInput(struct unit ***table,int *N); //формирование строки входных данных с клавиатуры  @@
void keyboardUnitFill(struct unit *table, char *str); //заполнение полей структуры из строки, полученной с клавиатуры @@
void scanField(char **dest,char **source,int *mult,int mod); //соединение данных полей в строку (ввод с клавиатуры) @@
char *DataScan(); //основная функция для ввода данных с клавиатуры  @@

void nameSort(struct unit **table,int N,char *search); //сортировка по последнему слову поля name   @@
void numberSearch(struct unit **table, int N, void (*funcName)(guitar**, int)); //метафункция сортировки числовых полей @@
void pickupsSort(struct unit **table,int N); //сортировка по numOfPickups   @@
void fretsSort(struct unit **table, int N); //сортировка по numOfFrets
void numberOfStringsSort(struct unit **table, int N); //сортировка по numOfStrings
void menzureSort(struct unit **table, int N); //сортировка по menzureLength
void radiusSort(struct unit **table, int N); //сортировка по neckRadius
void stringWidthSort(struct unit **table, int N); //сортировка по stringsWidth

void clearUnit(struct unit *table)
{
    free(table->name);
    free(table->info);
    free(table->stringsWidth);
    table->name = NULL;
    table->info = NULL;
    table->stringsWidth = NULL;
    free(table);
    table = NULL;
}

void StringDelete(char *s,int count)
{
    int j,k;
    if (strlen(s) != 1) {
        for (j = 0; j <= count; j++) {
            for (k = 0; k < strlen(s) - 1; k++) {
                s[k] = s[k + 1];
            }
            s[k] = '\0';
        }
    } else s[0] = '\0';
}

void nameInp(int count, char **source, struct unit *table)
{
    char *temp = NULL;

    temp = (char*)malloc(sizeof(char) * (count + 1));
    if (temp != NULL) {
        strncpy(temp,*source,count);
        temp[count] = '\0';
        table->name = (char*)malloc(sizeof(char) * (count + 1));
        if (table->name != NULL) {
            strcpy(table->name, temp);
        } else {
            perror("Can't copy a string, error at memory allocation!");
            exit(3);
        }
        free(temp);
        temp = NULL;
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(5);
    }

}

void infoInp(int count, char **source, struct unit *table)
{
    char *temp = NULL;
    int prevCount;

    prevCount = strlen(table->name);
    temp = (char*)malloc(sizeof(char) * (count + 1));
    if (temp != NULL) {
        strncpy(temp, *source, count);
        temp[count] = '\0';
        StringDelete(temp, prevCount);
        table->info = (char *) malloc(sizeof(char) * (strlen(temp) + 1));
        if (table->info != NULL) {
            strcpy(table->info, temp);
        } else {
            perror("Can't copy a string, error at memory allocation!");
            exit(3);
        }
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(4);
    }
    free(temp);
    temp = NULL;
}

void pickupsInp(int count, char **source, struct unit *table)
{
    int prevCount,tempInt;
    char *tempStr = NULL;

    prevCount = strlen(table->name) + strlen(table->info) + splitSymbolCompensation;
    tempStr = (char*)malloc(sizeof(char) * (count + 1));
    if (tempStr != NULL) {
        strncpy(tempStr,*source,count);
        tempStr[count] = '\0';
        StringDelete(tempStr,prevCount);
        tempInt = atoi(tempStr);
        table->numOfPickups = tempInt;
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(6);
    }
    free(tempStr);
    tempStr = NULL;
}

void fretsInp(int count, char **source, struct unit *table)
{
    int prevCount,tempInt,pickupsLen = 0;
    char *tempStr = NULL;

    tempStr = (char*)malloc(sizeof(char) * (count + 1));
    if (tempStr != NULL) {
        strncpy(tempStr,*source,count);
        tempStr[count] = '\0';

        tempInt = table->numOfPickups;
        while (tempInt != 0) {
            pickupsLen++;
            tempInt /= 10;
        }

        prevCount = strlen(table->name) + strlen(table->info) + pickupsLen + 2*splitSymbolCompensation;
        StringDelete(tempStr,prevCount);
        tempInt = atoi(tempStr);
        table->numOfFrets = tempInt;
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(7);
    }
    free(tempStr);
    tempStr = NULL;
}

void numStringsInp(int count, char **source, struct unit *table)
{
    int prevCount,tempInt,intFieldsLength = 0;
    char *tempStr = NULL;

    tempStr = (char*)malloc(sizeof(char) * (count + 1));
    if (tempStr != NULL) {
        strncpy(tempStr,*source,count);
        tempStr[count] = '\0';

        tempInt = table->numOfPickups;
        while (tempInt != 0) {
            intFieldsLength++;
            tempInt /= 10;
        }
        tempInt = table->numOfFrets;
        while (tempInt != 0) {
            intFieldsLength++;
            tempInt /= 10;
        }

        prevCount = strlen(table->name) + strlen(table->info) + intFieldsLength + 3*splitSymbolCompensation;
        StringDelete(tempStr,prevCount);
        tempInt = atoi(tempStr);
        table->numOfString = tempInt;
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(8);
    }
    free(tempStr);
    tempStr = NULL;
}

void menzureInp(int count, char **source, struct unit *table)
{
    int prevCount,tempInt,intFieldsLength = 0;
    float tempFloat;
    char *tempStr = NULL;

    tempStr = (char*)malloc(sizeof(char) * (count + 1));
    if (tempStr != NULL) {
        strncpy(tempStr,*source,count);
        tempStr[count] = '\0';

        tempInt = table->numOfPickups;
        while (tempInt != 0) {
            intFieldsLength++;
            tempInt /= 10;
        }
        tempInt = table->numOfFrets;
        while (tempInt != 0) {
            intFieldsLength++;
            tempInt /= 10;
        }
        tempInt = table->numOfString;
        while (tempInt != 0){
            intFieldsLength++;
            tempInt /= 10;
        }

        prevCount = strlen(table->name) + strlen(table->info) + intFieldsLength + 4*splitSymbolCompensation;
        StringDelete(tempStr,prevCount);
        tempFloat = atof(tempStr);
        table->menzureLength = tempFloat;
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(9);
    }
    free(tempStr);
    tempStr = NULL;
}

void radiusInp(int count, char **source, struct unit *table)
{
    int prevCount,tempInt,i,numberFieldsLength = 0;
    float tempFloat;
    char *tempStr = NULL,*temp2 = NULL;

    tempStr = (char*)malloc(sizeof(char) * (count + 1));
    if (tempStr != NULL) {
        strncpy(tempStr,*source,count);
        tempStr[count] = '\0';

        tempInt = table->numOfPickups;
        while (tempInt != 0) {
            numberFieldsLength++;
            tempInt /= 10;
        }
        tempInt = table->numOfFrets;
        while (tempInt != 0) {
            numberFieldsLength++;
            tempInt /= 10;
        }
        tempInt = table->numOfString;
        while (tempInt != 0) {
            numberFieldsLength++;
            tempInt /= 10;
        }
        prevCount = strlen(table->name) + strlen(table->info) + numberFieldsLength + 4 * splitSymbolCompensation;
        temp2 = (char*)malloc(sizeof(char) * (count + 1));
        if (temp2 != NULL) {
            strncpy(temp2, *source, count);
            temp2[count] = '\0';
            StringDelete(temp2, prevCount);

            for (i = 0; temp2[i] != sep; i++,numberFieldsLength++);

            prevCount = strlen(table->name) + strlen(table->info) + numberFieldsLength + 5 * splitSymbolCompensation;
            StringDelete(tempStr, prevCount);
            tempFloat = atof(tempStr);
            table->neckRadius = tempFloat;
        } else {
            perror("Ooops, seems like something's wrong with your memory!");
            exit(10);
        }
    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(11);
    }
    free(tempStr);
    free(temp2);
    tempStr = NULL;
    temp2 = NULL;
}

void stringsWidthInp(int count, char **source, struct unit *table)
{
    char *tempStr = NULL;
    int i,iteration = 0,temp;

    tempStr = (char *) malloc(sizeof(char) * (strlen(*source) + 1));
    if (tempStr != NULL) {
        strcpy(tempStr, *source);
        StringDelete(tempStr, count);
        table->stringsWidth = (int*)malloc(sizeof(int) * (table->numOfString));

        if (table->stringsWidth != NULL) {

            for (i = 0; iteration < table->numOfString; i++) {
                if (tempStr[i] == sep || tempStr[i + 1] == '\0') {
                    table->stringsWidth[iteration] = atoi(tempStr);
                    temp = atoi(tempStr);
                    StringDelete(tempStr,i);
                    i = 0;
                    iteration++;
                }
            }

        } else {
            perror("Ooops, seems like something's wrong with your memory!");
            exit(13);
        }

    } else {
        perror("Ooops, seems like something's wrong with your memory!");
        exit(12);
    }
    free(tempStr);
    tempStr = NULL;
}

void strRead(FILE *df, char **dest)
{
    char symbol = '\0';
    int length = 0, multiplier = 1,errCount = 0;

    (*dest) = (char *)malloc(sizeof(char) * (readStep + 1));
    while (symbol != '\n') {
        symbol = fgetc(df);
        if (symbol >= 32) {
            if ((length % readStep == 0) && (length != 0)) {
                multiplier++;
                (*dest) = (char *) realloc((*dest), sizeof(char) * (readStep * multiplier + 1));
                if ((*dest) == NULL) {
                    perror("Ooops, looks like there's an error with memory reassignment...");
                    exit(1);
                }
            }
            (*dest)[length] = symbol;
            length++;
        } else errCount++;
    }
    (*dest)[length] = '\0';
}

int csvCount(FILE *df)
{
    char symbol = '\0';
    int count = 0;

    while (symbol != EOF) {
        symbol = fgetc(df);
        if (symbol == '\n') {
            count++;
        }
    }
    return count;
}

void unitFill(FILE *df, struct unit **table,int i)
{
    char *str = NULL;
    int j,endFlag = 0;
    table[i] = (guitar*)malloc(sizeof(guitar));

    strRead(df, &str);
    for (j = 0;endFlag < 7;j++) {
        if (str[j] == sep || str[j + 1] == '\0') {
            if (endFlag == 0) {
                nameInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 1) {
                infoInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 2) {
                pickupsInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 3) {
                fretsInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 4) {
                numStringsInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 5) {
                menzureInp(j,&str,table[i]);
                endFlag++;
            } else if (endFlag == 6) {
                radiusInp(j,&str,table[i]);
                stringsWidthInp(j,&str,table[i]);
                endFlag++;
            }
        }
    }
    free(str);
    str = NULL;
}

void keyboardUnitFill(struct unit *table,char *str)
{
    int j,endFlag = 0;

    for (j = 0;endFlag < 7;j++) {
        if (str[j] == sep || str[j + 1] == '\0') {
            if (endFlag == 0) {
                nameInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 1) {
                infoInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 2) {
                pickupsInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 3) {
                fretsInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 4) {
                numStringsInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 5) {
                menzureInp(j, &str, table);
                endFlag++;
            } else if (endFlag == 6) {
                radiusInp(j, &str, table);
                stringsWidthInp(j, &str, table);
                endFlag++;
            }
        }
    }
}

guitar **fileInput(struct unit **table, int *N) {
    FILE *df;
    char filename[nameLength];
    int length,i = 0;
    int size;

    do {

        puts("Enter the name of the file from which you want to read the data (without the .csv reference):\n"
             "WARNING!! MAX length of the file name is 30!");
        if (i == 0) {
            getchar();
            i++;
        }
        fgets(filename, inputLength, stdin);
        while (filename[0] == '\n') {
            puts("Empty string cannot be a filename!\nPlease try again:");
            fgets(filename, inputLength, stdin);
        }
        length = strlen(filename);
        if (filename[length - 1] == '\n') {
            filename[length - 1] = '\0';
        }
        strcat(filename, ".csv");
        df = fopen(filename, "r");

        if (!df) {
            puts("File does not exist, please enter another name!");
            fclose(df);
            getchar();
            system("cls");
        }

    } while (!df);

    size = csvCount(df);
    table = (guitar**)malloc(sizeof(guitar*) * size);
    if ((*table) != NULL) {
        fseek(df,0,SEEK_SET);
        for (i = 0;i < size;i++) {
            unitFill(df,table,i);
            (*N)++;
        }
    } else {
        perror("Ooops, something went wrong with the memory...");
        exit(2);
    }
    fclose(df);

    return table;
}

void scanField(char **dest,char **source,int *mult,int mod)
{
    char split[] = ";";
    int len;

        strRead(stdin, source);
        if (*dest == NULL) {
            *dest = (char*)malloc(sizeof(char) * (keyboardReadStep + 1));
            strcpy(*dest,*source);
            strcat(*dest, split);
        } else {
            len = strlen(*dest) + strlen(*source);
            if (len / (keyboardReadStep * (*mult)) >= 1) {
                (*mult)++;
                realloc(*dest,sizeof(char) * ((*mult) * keyboardReadStep + 1));
            }
            strcat(*dest, *source);
            if (mod==0) {
                strcat(*dest, split);
            }
        }
    free(*source);
    *source = NULL;
}

char *DataScan()
{
    char *inp = NULL,*temp = NULL,split[] = ";";
    int multiplier = 0,numOfStrings,mult = 1;

    puts("Enter unit name:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter description or special info:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter number of pickups:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter number of frets:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter number of strings:");
    strRead(stdin,&temp);
    if ((strlen(inp) + strlen(temp)) / (keyboardReadStep * mult) >= 1) {
        mult++;
        realloc(inp,sizeof(char) * (mult * keyboardReadStep + 1));
    }
    strcat(inp,temp);
    strcat(inp,split);
    numOfStrings = atoi(temp);

    puts("Enter menzure length:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter neck radius:");
    scanField(&inp,&temp,&mult,0);

    puts("Enter strings width");
    while (multiplier < numOfStrings) {
        if (multiplier == numOfStrings - 1) {
            scanField(&inp,&temp,&mult,1);
        } else {
            scanField(&inp,&temp,&mult,0);
        }
        multiplier++;
    }

    return inp;
}

void keyboardInput(struct unit ***table,int *N)
{
    char *data = NULL;

    data = DataScan();
    (*N)++;
    (*table) = (guitar**)realloc((*table),sizeof(guitar*) * (*N));
    (*table)[(*N)-1] = (guitar*)malloc(sizeof(guitar));

    if (table != NULL) {
        keyboardUnitFill((*table)[(*N)-1],data);
    } else {
        perror("Ooops, something went wrong with the memory...");
        exit(2);
    }

    free(data);
    data = NULL;
}

void nameSort(struct unit **table,int N,char *search)
{
    int i,j,len,startpoint = 0,printed = 0,scanFlag = 0;
    char *temp = NULL;

    for (i = 0;i < N;i++) {
        len = strlen(table[i]->name);
        for (j = 0;j < len;j++) {
            if (table[i]->name[j] == ' ') {
                startpoint = j;
            }
        }
        temp = (char*)malloc(sizeof(char) * (len + 1));
        strcpy(temp,table[i]->name);
        if (temp[0] != 32 && startpoint == 0) {} else {
            StringDelete(temp, startpoint);
        }
        startpoint = 0;

        for (j = 0;j < strlen(temp);j++) {
            temp[j] = toupper(temp[j]);
        }
        if (scanFlag == 0) {
            for (j = 0; j < strlen(search); j++) {
                search[j] = toupper(search[j]);
            }
            scanFlag++;
        }

        if (strcmp(temp,search) == 0) {
            structPrint(table[i]);
            printed++;
        }
    }
    if (printed == 0) {
        printf("No results!\n\n");
    }
    free(temp);
    temp = NULL;
}

void pickupsSort(struct unit **table,int N)
{
    int i,j;
    guitar *tempStruct = NULL;

    for (i = 1; i < N; i++) {
        tempStruct = table[i];
        for (j = i - 1;(j >= 0) && (table[j]->numOfPickups < tempStruct->numOfPickups);j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }

    }
    tempStruct = NULL;
}

void fretsSort(struct unit **table, int N)
{
    int i,j;
    guitar *tempStruct = NULL;

    for (i = 1;i < N;i++) {
        tempStruct = table[i];
        for (j = i - 1;(j >= 0) && (table[j]->numOfFrets < tempStruct->numOfFrets);j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }
    }
    tempStruct = NULL;
}

void numberOfStringsSort(struct unit **table, int N)
{
    int i,j;
    guitar *tempStruct = NULL;

    for (i = 1;i < N;i++) {
        tempStruct = table[i];
        for (j = i - 1;(j >= 0) && (table[j]->numOfString < tempStruct->numOfString);j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }
    }
    tempStruct = NULL;
}

void menzureSort(struct unit **table, int N)
{
    int i,j;
    guitar *tempStruct = NULL;

    for (i = 1;i < N;i++) {
        tempStruct = table[i];
        for (j = i - 1;(j >= 0) && (table[j]->menzureLength < tempStruct->menzureLength);j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }
    }
    tempStruct = NULL;
}

void radiusSort(struct unit **table, int N)
{
    int i, j;
    guitar *tempStruct = NULL;

    for (i = 1; i < N; i++) {
        tempStruct = table[i];
        for (j = i - 1; (j >= 0) && (table[j]->neckRadius < tempStruct->neckRadius); j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }
    }
    tempStruct = NULL;
}

void stringWidthSort(struct unit **table, int N)
{
    int i,j;
    guitar *tempStruct = NULL;

    for (i = 1;i < N;i++) {
        tempStruct = table[i];
        for (j = i - 1;(j >= 0) && (table[j]->stringsWidth[0] < tempStruct->stringsWidth[0]);j--) {
            table[j + 1] = table[j];
            table[j] = tempStruct;
        }
    }
    tempStruct = NULL;
}

void numberSearch(struct unit **table, int N, void (*funcName)(guitar**, int))
{
    funcName(table,N);
    printAll(table,N);
}

int main() {

    guitar **table = NULL;
    int flag,csvReadFlag = 0,i,trash,N = 0,sortCount = 0;
    char *search = NULL;

    do {
        system("cls");
        MainMenu();
        flag = getchar();
        i = 0;
        if (flag == '\n') while ((trash = getchar()) != '\n') i++;
        if (i) {
            flag = '`';
        }
        switch (flag) {
            case '1':
                if (csvReadFlag == 0) {
                    system("cls");
                    table = fileInput(table, &N);
                }
                do {
                    system("cls");
                    AddMenu();
                    if (csvReadFlag != 0 && flag != 51 && flag != 56 && flag != 50) {
                        getchar();
                    } else if (csvReadFlag == 0) csvReadFlag++;
                    flag = getchar();
                    i = 0;
                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                    if (i) {
                        flag = '`';
                    }
                    switch (flag) {
                        case '1':
                            do {
                                //system("cls");
                                keyboardInput(&table,&N);
                                puts("1 - Enter one more line");
                                puts("8 - Stop and proceed to sorting");
                                flag = getchar();
                                i = 0;
                                if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                if (i) {
                                    flag = '`';
                                }
                            } while (flag != '8');
                            break;
                        case '2':
                            do {
                                system("cls");
                                mainSortMenu();
                                flag = getchar();
                                i = 0;
                                if (flag == '\n') while ((trash = getchar()) != '\n') i++;
                                if (i) {
                                    flag = '`';
                                }
                                switch (flag) {
                                    case '1':
                                        system("cls");
                                        printf("Enter searching word (non case-sensitive): ");
                                        getchar();
                                        strRead(stdin,&search);
                                        printf("--------------------------------\n");
                                        nameSort(table, N, search);
                                        free(search);
                                        search = NULL;
                                        puts("Press ENTER to return");
                                        i = 0;
                                        if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                        if (i) {
                                            flag = '`';
                                        }
                                        break;
                                    case '2':
                                        do {
                                            system("cls");
                                            numberSortMenu();
                                            if (sortCount == 0) {
                                                getchar();
                                                sortCount++;
                                            }
                                            flag = getchar();
                                            i = 0;
                                            if (flag == '\n') while ((trash = getchar()) != '\n') i++;
                                            if (i) {
                                                flag = '`';
                                            }
                                            switch (flag) {
                                                case '1':
                                                    system("cls");
                                                    numberSearch(table,N,pickupsSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '2':
                                                    system("cls");
                                                    numberSearch(table,N,fretsSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '3':
                                                    system("cls");
                                                    numberSearch(table,N,numberOfStringsSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '4':
                                                    system("cls");
                                                    numberSearch(table,N,menzureSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '5':
                                                    system("cls");
                                                    numberSearch(table,N,radiusSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '6':
                                                    system("cls");
                                                    numberSearch(table,N,stringWidthSort);
                                                    puts("Press ENTER to return");
                                                    i = 0;
                                                    if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                                                    if (i) {
                                                        flag = '`';
                                                    }
                                                    getchar();
                                                    break;
                                                case '7':
                                                    sortCount = 0;
                                                    getchar();
                                                    break;
                                                default:
                                                    system("cls");
                                                    puts("There's no such paragraph!");
                                                    puts("Press ENTER when ready...");
                                                    do {
                                                        flag = getchar();
                                                    } while (flag != '\n');
                                            }
                                        } while (flag != '7');
                                        //flag = '`';
                                        break;
                                    case '8':
                                        break;
                                    default:
                                        system("cls");
                                        puts("There's no such paragraph!");
                                        puts("Press ENTER when ready...");
                                        do {
                                            flag = getchar();
                                        } while (flag != '\n');
                                }
                            } while (flag != '8');
                            flag = '`';
                            break;
                        case '3':
                            system("cls");
                            printAll(table, N);
                            puts("Press ENTER to return");
                            i = 0;
                            if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                            if (i) {
                                flag = '`';
                            }
                            break;
                        case '9':
                            break;
                        default:
                            system("cls");
                            puts("There's no such paragraph!");
                            puts("Press ENTER when ready...");
                            do {
                                flag = getchar();
                            } while (flag != '\n');
                    }
                } while (flag != '9');
                flag = '`';
                break;
            case '2':
                system("cls");
                InfoMenu();
                puts("Press ENTER to return");
                i = 0;
                if (flag != '\n') while ((trash = getchar()) != '\n') i++;
                if (i) {
                    flag = '`';
                }
                getchar();
                break;
            case '0':
                for (i = 0;i < N;i++) {
                    clearUnit(table[i]);
                }
                free(table);
                table = NULL;
                break;
            default:
                system("cls");
                puts("There's no such paragraph!");
                puts("Press ENTER when ready...");
                do {
                    flag = getchar();
                } while (flag != '\n');
        }
    } while (flag != '0');

    return 0;
}

void MainMenu()
{
    puts("1 - Start");
    puts("2 - Info");
    puts("0 - Exit");
}

void AddMenu()
{
    puts("1 - Add custom lines from keyboard");
    puts("2 - Proceed to sorting");
    puts("3 - Print current table");
    puts("9 - Back");
}

void InfoMenu()
{
    puts("NOTE!!! The 'pick lines with serain word' sorting prints all the structure lines with the chosen word\nequal the last word in line's name");
}

void numberSortMenu()
{
    puts("Pick the number field by which you want to sort:");
    puts("1 - Number of pickups\n2 - Number of frets\n3 - Number of strings\n4 - Menzure length\n5 - Neck radius");
    puts("6 - Strings width (sorted by the thinnest string)\n7 - Exit");
    //getchar();
}

void mainSortMenu()
{
    printf("1 - Pick lines with certain word\n");
    printf("2 - Number fields sorting\n");
    printf("8 - Back\n");
}

void printAll(struct unit **table, int N)
{
    int i,j;

    for (i = 0;i < N;i++) {
        printf("Name: %s;\n", table[i]->name);
        printf("Description/info: %s;\n",table[i]->info);
        printf("Number of pickups: %d;\n", table[i]->numOfPickups);
        printf("Number of frets: %d;\n",table[i]->numOfFrets);
        printf("Number of strings: %d;\n",table[i]->numOfString);
        printf("Menzure length: %.2f;\n", table[i]->menzureLength);
        printf("Neck radius: %.2f;\n", table[i]->neckRadius);
        printf("Strings width: ");
        for (j = 0;j < table[i]->numOfString;j++) {
            printf("%d ",table[i]->stringsWidth[j]);
        }
        printf("\n\n");
    }
}

void structPrint(struct unit *table)
{
    int j;

    printf("Name: %s;\n", table->name);
    printf("Description/info: %s;\n",table->info);
    printf("Number of pickups: %d;\n", table->numOfPickups);
    printf("Number of frets: %d;\n",table->numOfFrets);
    printf("Number of strings: %d;\n",table->numOfString);
    printf("Menzure length: %.2f;\n", table->menzureLength);
    printf("Neck radius: %.2f;\n", table->neckRadius);
    printf("Strings width: ");
    for (j = 0;j < table->numOfString;j++) {
        printf("%d ",table->stringsWidth[j]);
    }
    printf("\n\n");
}