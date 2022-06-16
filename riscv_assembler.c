#include <string.h>

#include <stdio.h>

#include<ctype.h>

#include<stdlib.h>


/***
  Name : Ali Shair Khan 018

Submitted as  project for AKEANA training
  ***/

int isOpcode(char * lptr);
int opcode_type;
int size = 0;
int flag = 0;
int sec_pass = 0;
#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255
#define MAX_LINE_LENGTH 255

typedef struct symboltable {
    int address;
    char label[MAX_LABEL_LEN + 1];
    struct symboltable * next;
}
symboltable;

symboltable * first, * last;

enum {
    DONE,
    OK,
    EMPTY_LINE
};

int bin_instruction[32] = {
    0.0
};
symboltable mytable[MAX_SYMBOLS];
FILE * infile = NULL;
FILE * outfile = NULL;

int search(char lab[]);
void insert(char label[], int curr_pc);
char * decToBin(int num, int length);
char * hexToBin(char * num, int length);
char * regToBin(char * reg);
int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4);
int toNum(char * pStr);
char bin2hex(int highbit);
void write_instruction();
void sType(char * opcode, char * arg1, char * arg2, char * arg3);
void bType(char * opcode, char * arg1, char * arg2, char * label, int curr_pc);
void iType(char * opcode, char * arg1, char * arg2, char * arg3, int curr_pc);
void RType(char * opcode, char * arg1, char * arg2, char * arg3);
void uType(char * opcode, char * arg1, char * arg2);
void jType(char * opcode, char * arg1, char * label, int curr_pc);

int main(int argc, char * argv[]) {

    infile = fopen(argv[1], "r");
    outfile = fopen(argv[2], "w");
    char c;
    char lLine[MAX_LINE_LENGTH + 1], * lLabel, * lOpcode, * lArg1,
        * lArg2, * lArg3, * lArg4;

    int lRet1 = 1;
    int immediate_op;
    if (!infile) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        exit(4);
    }
    if (!outfile) {
        printf("Error: Cannot open output file %s\n", argv[2]);
        exit(4);
    }

    FILE * lInfile;
    FILE * lInfile2;

    lInfile = infile;
    lInfile2 = infile;
    int current_pc = 0;
    int start_address = 0;
    char * templabel;
    int index_table = 0;
    for (start_address = 0; lRet1 != DONE; start_address += 4) {
        lRet1 = readAndParse(lInfile, lLine, & lLabel, & lOpcode, & lArg1, & lArg2, & lArg3, & lArg4); //first pass

        if (lRet1 != DONE) {
            insert(lLabel, start_address);

        }

    }

    lRet1 = 20;
    current_pc = 0;
    opcode_type = 0;
    fseek((lInfile + 0), 0, SEEK_SET);
    printf("Check machine code file!");
    for (current_pc = 0; lRet1 != DONE; current_pc += 4) {
        sec_pass = 1;
        lRet1 = readAndParse(lInfile, lLine, & lLabel, & lOpcode, & lArg1, & lArg2, & lArg3, & lArg4); //second pass

        if (lRet1 != DONE) {

            if (opcode_type == 1) {
                RType(lOpcode, lArg1, lArg2, lArg3);
                write_instruction();

            } else if (opcode_type == 2) {
                uType(lOpcode, lArg1, lArg2);
                write_instruction();

            } else if (opcode_type == 3 || opcode_type == 4 || opcode_type == 5 || opcode_type == 6) {

                iType(lOpcode, lArg1, lArg2, lArg3, current_pc);
                write_instruction();

            } else if (opcode_type == 7) //stype
            {

                sType(lOpcode, lArg1, lArg2, lArg3);
                write_instruction();

            } else if (opcode_type == 8) //jtype
            {
                jType(lOpcode, lArg1, lArg2, current_pc);
                write_instruction();

            } else if (opcode_type == 9) //u type
            {
                bType(lOpcode, lArg1, lArg2, lArg3, current_pc);
                write_instruction();

            } else if (opcode_type == 10) {
                if (search(lLabel) != 0) {
                    exit(2);
                }
            }
        }
    }
   
    while (lRet1 != DONE);

    fclose(infile);
    fclose(outfile);
}

int search(char lab[]) {
    int i, address = 0;
    flag = 0;
    symboltable * p;
    p = first;
    for (i = 0; i < size; i++) {
        if (strcmp(p -> label, lab) == 0) {
            address = p -> address;
            flag = 1;
            return address;
        }
        p = p -> next;

    }

    if (i == size && sec_pass == 1) {

        exit(1);
    } else
        return 0;

}

void insert(char label[], int curr_pc) {

    search(label);
    if (flag == 1) {
        return;
    } else {
        symboltable * p;
        p = malloc(sizeof(symboltable));
        strcpy(p -> label, label);
        p -> address = curr_pc;
        p -> next = NULL;

        if (size == 0) {
            first = p;
            last = p;
        } else {
            last -> next = p;
            last = p;
        }
        size++;
    }
    return;
}

char * decToBin(int num, int length) {
    int i = length - 1;
    int * binary = (int * ) malloc(sizeof(int) * length);
    int temp;
    int rem;
    int neg_flag = 0;
    int carry = 1;
    int one[length], two[length];
    char * char_dec;
    char_dec = (char * ) malloc(sizeof(char) * (length + 1));
    if (num < 0) {
        neg_flag = 1;
        num = num * -1;
    }
    for (i; i >= 0; i--) {
        rem = num % 2;
        num = num / 2;
        binary[i] = rem;

    }
    if (neg_flag == 1) {
        for (i = 0; i < length; i++) {
            if (binary[i] == 0) {
                one[i] = 1;
            } else if (binary[i] == 1) {
                one[i] = 0;
            }
        }

        for (i = 0; i < length; i++) printf("%d", one[i]);
        printf(" \n");

        for (i = length - 1; i >= 0; i--) {
            if (one[i] == 1 && carry == 1) {
                two[i] = 0;
            } else if (one[i] == 0 && carry == 1) {
                two[i] = 1;
                carry = 0;
            } else {
                two[i] = one[i];
            }
        }

        for (i = 0; i < length; ++i) {
            char_dec[i] = two[i] + '0';
        }

        char_dec[length] = '\0';

        return char_dec;
    }

    for (i = 0; i < length; ++i) {
        char_dec[i] = binary[i] + '0';
    }

    char_dec[length] = '\0';

    return char_dec;

}

char * hexToBin(char * num, int length) {
    int i = 0, lNeg = 0, arr_length = 0;
    int decimal = 0;
    int base = 1;
    num = num + 2;
    if ( * num == '-') {
        lNeg = 1;
        num++;
    }

    arr_length = strlen(num);

    for (i = --arr_length; i >= 0; i--) {
        if (num[i] >= '0' && num[i] <= '9') {
            decimal += (num[i] - 48) * base;
            base *= 16;
        } else if (num[i] >= 'A' && num[i] <= 'F') {
            decimal += (num[i] - 55) * base;
            base *= 16;
        } else if (num[i] >= 'a' && num[i] <= 'f') {
            decimal += (num[i] - 87) * base;
            base *= 16;
        }
    }

    if (lNeg == 1) {
        decimal = -decimal;
    }
    char * binary;
    binary = decToBin(decimal, length);
    return binary;

}

char * regToBin(char * reg) {

    char * t_ptr;
    t_ptr = reg;
    if ( * (t_ptr + 0) != 'x' && * (t_ptr + 0) != '\0') {
        exit(4);
    } else if ( * (t_ptr + 0) == '\0') {

        exit(4);
    }

    int length = strlen(reg);
    reg = ++reg;
    int dec;
    dec = atoi(reg);
    if (dec > 31 || dec < 0) {
        exit(4);
    }

    char * binary = decToBin(dec, 5);
    return binary;

}

int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char **
    pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4) {

    char * lRet, * lPtr;
    int i;
    if (!fgets(pLine, MAX_LINE_LENGTH, pInfile))
        return (DONE);

    for (i = 0; i < strlen(pLine); i++)
        pLine[i] = tolower(pLine[i]);

    * pLabel = * pOpcode = * pArg1 = * pArg2 = * pArg3 = * pArg4 = pLine + strlen(pLine);

    lPtr = pLine;

    while ( * lPtr != '#' && * lPtr != '\0' && * lPtr != '\n') {
        lPtr++;
    }

    * lPtr = '\0';
    if (!(lPtr = strtok(pLine, "\t\n ,():")))
        return (EMPTY_LINE);

    if (isOpcode(lPtr) == 0 && lPtr[0] != '.') {

        * pLabel = lPtr;

        if (!(lPtr = strtok(NULL, "\t\n ,():"))) return (OK);
    }

    isOpcode(lPtr);

    * pOpcode = lPtr;

    if (!(lPtr = strtok(NULL, "\t\n ,():"))) return (OK);

    * pArg1 = lPtr;

    if (!(lPtr = strtok(NULL, "\t\n ,():"))) return (OK);

    * pArg2 = lPtr;
    if (!(lPtr = strtok(NULL, "\t\n ,():"))) return (OK);

    * pArg3 = lPtr;

    if (!(lPtr = strtok(NULL, "\t\n ,():"))) return (OK);

    * pArg4 = lPtr;

    return (OK);

}

int isOpcode(char * lptr) {

    if (strcmp(lptr, "add") == 0 || strcmp(lptr, "and") == 0 || strcmp(lptr, "sub") == 0 || strcmp(lptr, "xor") == 0 || strcmp(lptr, "or") == 0 ||
        strcmp(lptr, "sll") == 0 || strcmp(lptr, "srl") == 0 || strcmp(lptr, "sra") == 0 || strcmp(lptr, "slt") == 0 || strcmp(lptr, "sltu") == 0) {
        opcode_type = 1;
        return (1);

    } else if (strcmp(lptr, "lui") == 0 || strcmp(lptr, "auipc") == 0) {
        opcode_type = 2;
        return (2);
    } else if (strcmp(lptr, "addi") == 0 || strcmp(lptr, "andi") == 0 || strcmp(lptr, "xori") == 0 || strcmp(lptr, "ori") == 0 ||
        strcmp(lptr, "slli") == 0 || strcmp(lptr, "srli") == 0 || strcmp(lptr, "srai") == 0 || strcmp(lptr, "slti") == 0 || strcmp(lptr, "sltiu") == 0) {
        opcode_type = 3;
        return (3);

    } else if (strcmp(lptr, "lb") == 0 || strcmp(lptr, "lh") == 0 || strcmp(lptr, "lw") == 0 || strcmp(lptr, "lbu") == 0 || strcmp(lptr, "lhu") == 0) {
        opcode_type = 4;
        return (4);

    } else if (strcmp(lptr, "ecall") == 0 || strcmp(lptr, "ebreak") == 0) {
        opcode_type = 5;
        return (5);

    } else if (strcmp(lptr, "jalr") == 0) {
        opcode_type = 6;
        return (6);
    } else if (strcmp(lptr, "sb") == 0 || strcmp(lptr, "sh") == 0 || strcmp(lptr, "sw") == 0) {
        opcode_type = 7;
        return (7);
    } else if (strcmp(lptr, "jal") == 0) {
        opcode_type = 8;
        return (8);
    } else if (strcmp(lptr, "beq") == 0 || strcmp(lptr, "bne") == 0 || strcmp(lptr, "blt") == 0 || strcmp(lptr, "bge") == 0 || strcmp(lptr, "bltu") == 0 || strcmp(lptr, "bgeu") == 0) {
        opcode_type = 9;
        return (9);
    } else {
        opcode_type = 10;
        return 0;
    }
}

int toNum(char * pStr) {
    char lBuf[10];
    char * t_ptr;
    char * orig_pStr;
    int t_length, k;
    int lNum, lNeg = 0;
    orig_pStr = pStr;
    if ( * (pStr + 0) == '\0') {
        exit(4);
    } else if ( * pStr == '0' && * (pStr + 1) == 'x') {
        pStr = pStr + 2;
        if ( * pStr == '-') {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for (k = 0; k < t_length; k++) {
            if (!isxdigit( * t_ptr)) {
                exit(4);
            }

            t_ptr++;
        }
        sprintf(lBuf, "0x%s", pStr);
        sscanf(lBuf, "%i", & lNum);
        if (lNeg)
            lNum = -lNum;
        return lNum;
    } else {
        if ( * pStr == '-') {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for (k = 0; k < t_length; k++) {
            if (!isdigit( * t_ptr)) {
                exit(4);
            }
            t_ptr++;
        }
        lNum = atoi(pStr);
        if (lNeg)
            lNum = -lNum;

        return lNum;

    }

}

char bin2hex(int highbit) {
    char retval;
    int i;
    unsigned char dec;
    dec = 0;

    for (i = 0; i < 4; i++) {
        dec = 2 * dec;
        dec = dec + bin_instruction[highbit + i];
    }

    if (dec > 9) {
        dec = dec - 10;
        retval = dec + 'A';
    } else {
        retval = dec + '0';
    }

    return retval;
}

void write_instruction() {
    char a;

    fputc('0', outfile);
    fputc('x', outfile);
    for (int i = 0; i <= 31; i = i + 4) {
        a = bin2hex(i);
        fputc(a, outfile);

    }

    fputc('\n', outfile);
    return;
}

void sType(char * opcode, char * arg1, char * arg2, char * arg3) {
    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {

        exit(4);
    }
    char opcode1[] = "0100011";
    char func3[] = "000";
    char * bin_arg1 = regToBin(arg1);
    printf("2 s type is called %s\n", arg3);
    char * bin_arg3 = regToBin(arg3);
    printf("3 s type is called\n");
    int binary_array[12] = {
        0.0
    };
    char arg2_p1[] = "0000000";
    char arg2_p2[] = "00000";
    for (int i = 0; i < 32; i++)
        bin_instruction[i] = 0;
    int i;
    int dec_num;

    if ((strcmp(opcode, "sh") == 0)) {
        strcpy(func3, "001");
    } else if ((strcmp(opcode, "sw") == 0)) {
        strcpy(func3, "010");
    }

    if (arg2[0] == '0' && arg2[1] == 'x') {
        int hex_max = toNum(arg2);
        if (hex_max > 2047 || hex_max < -2048) {
            exit(3);
        }
        arg2 = hexToBin(arg2, 12);
    } else if (arg2[0] == '0' && arg2[1] == 'b') {
        arg2 = arg2 + 2;

        int len = strlen(arg2) - 1;

        for (i = 11; len >= 0; --i) {
            binary_array[i] = * (arg2 + len) - 48;
            len = --len;
        }
        for (i = 0; i <= 6; ++i) {
            bin_instruction[i] = binary_array[i];
        }

        for (i = 11; i >= 7; --i) {
            bin_instruction[i] = * (bin_arg1 + i - 7) - 48;
        }
        for (i = 16; i >= 12; --i) {
            bin_instruction[i] = * (bin_arg3 + i - 12) - 48;
        }

        for (i = 19; i >= 17; --i) {
            bin_instruction[i] = * (func3 + i - 17) - 48;
        }
        for (i = 24; i >= 20; --i) {
            bin_instruction[i] = binary_array[i - 13];
        }
        for (i = 31; i >= 25; --i) {

            bin_instruction[i] = * (opcode1 + i - 25) - 48;
        }
        for (i = 0; i <= 31; ++i)
            printf("%d", bin_instruction[i]);

        return;
    } else {
        dec_num = toNum(arg2);
        if (dec_num > 2047 || dec_num < -2048) {
            exit(3);
        }
        arg2 = decToBin(dec_num, 12);

    }

    for (i = 0; i <= 6; ++i)
        arg2_p1[i] = * (arg2 + i);

    for (i = 7; i <= 11; i++)
        arg2_p2[i - 7] = * (arg2 + i);
    char arr[32];

    strcpy(arr, arg2_p1);

    strcat(arr, bin_arg1);

    strcat(arr, bin_arg3);

    strcat(arr, func3);

    strcat(arr, arg2_p2);

    strcat(arr, opcode1);

    for (i = 0; i < 32; i++) {
        bin_instruction[i] = * (arr + i) - 48;
        printf("%d", bin_instruction[i]);
    }

    return;
}

void bType(char * opcode, char * arg1, char * arg2, char * label, int curr_pc) {

    if (label[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0') {
        exit(4);
    }
    char opcode1[] = "1100011";
    char func3[] = "000";
    char * bin_arg1 = regToBin(arg1);
    char * bin_arg2 = regToBin(arg2);
    int offset;
    int i;

    if (isalpha(label[0]) == 0) {

        int num = toNum(label);
        offset = num;

    } else {
        int label_address = 0;
        label_address = search(label);

        offset = label_address - curr_pc;

    }

    if (offset > 2047 || offset < -2048) {
        exit(5);
    }

    char * bin_offset;
    bin_offset = decToBin(offset, 13);

    for (int i = 0; i < 32; i++)
        bin_instruction[i] = 0;

    if ((strcmp(opcode, "bne") == 0)) {
        strcpy(func3, "001");
    } else if ((strcmp(opcode, "blt") == 0)) {
        strcpy(func3, "100");
    } else if ((strcmp(opcode, "bge") == 0)) {
        strcpy(func3, "101");
    } else if ((strcmp(opcode, "bltu") == 0)) {
        strcpy(func3, "110");
    } else if ((strcmp(opcode, "bgeu") == 0)) {
        strcpy(func3, "111");
    }
    for (i = 31; i >= 25; i--)
        bin_instruction[i] = * (opcode1 + i - 25) - 48;
    for (i = 19; i >= 17; i--)
        bin_instruction[i] = * (func3 + i - 17) - 48;
    for (i = 16; i >= 12; i--)
        bin_instruction[i] = * (bin_arg1 + i - 12) - 48;
    for (i = 11; i >= 7; i--)
        bin_instruction[i] = * (bin_arg2 + i - 7) - 48;
    bin_instruction[24] = * (bin_offset + 1) - 48;
    for (i = 23; i >= 20; i--)
        bin_instruction[i] = * (bin_offset + i - 12) - 48;
    for (i = 6; i >= 1; i--)
        bin_instruction[i] = * (bin_offset + i + 1) - 48;
    bin_instruction[0] = * (bin_offset + 0) - 48;
    for (i = 0; i < 32; i++) {
        printf("%d", bin_instruction[i]);
    }

    return;
}

void iType(char * opcode, char * arg1, char * arg2, char * arg3, int curr_pc) {
    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        exit(4);
    }
    int dec_num;
    int binary_array[12] = {
        0.0
    };
    char opcode1[] = "0010011";
    char * bin_arg1 = regToBin(arg1);
    char func7[] = "0000000";
    char func3[] = "101";
    int i;
    if ((strcmp(opcode, "srai") == 0)) {
        func7[1] = '1';
    }
    if ((strcmp(opcode, "addi") == 0)) {
        strcpy(func3, "000");
    } else if ((strcmp(opcode, "xori") == 0)) {
        strcpy(func3, "100");
    } else if ((strcmp(opcode, "ori") == 0)) {
        strcpy(func3, "110");
    } else if ((strcmp(opcode, "andi") == 0)) {
        strcpy(func3, "111");
    } else if ((strcmp(opcode, "slli") == 0)) {
        strcpy(func3, "001");
    } else if ((strcmp(opcode, "slti") == 0)) {
        strcpy(func3, "010");
    } else if ((strcmp(opcode, "sltiu") == 0)) {
        strcpy(func3, "011");
    }

    if (strcmp(opcode, "srli") != 0 && strcmp(opcode, "srai") != 0 && strcmp(opcode, "slli") != 0 && strcmp(opcode, "lb") != 0 && strcmp(opcode, "lh") != 0 && strcmp(opcode, "lw") != 0 && strcmp(opcode, "lbu") != 0 && strcmp(opcode, "lhu") != 0 && strcmp(opcode, "ecall") != 0 && strcmp(opcode, "ebreak") != 0) {
        if (strcmp(opcode, "jalr") == 0) {
            strcpy(opcode1, "1100111");
            strcpy(func3, "000");
        }
        char * bin_arg2 = regToBin(arg2);
        if (arg3[0] == '0' && arg3[1] == 'x') {
            int hex_max = toNum(arg3);
            if (hex_max > 2047 || hex_max < -2048) {
                exit(3);
            }
            arg3 = hexToBin(arg3, 12);
        } else if (arg3[0] == '0' && arg3[1] == 'b') {
            arg3 = arg3 + 2;
            int len = strlen(arg3) - 1;
            for (i = 11; len >= 0; --i) {
                binary_array[i] = * (arg3 + len) - 48;
                len = --len;
            }
            for (i = 0; i <= 11; ++i) {
                bin_instruction[i] = binary_array[i];
            }
            for (i = 0; i <= 31; i++)
                for (i = 16; i >= 12; --i) {
                    bin_instruction[i] = * (bin_arg2 + i - 12) - 48;
                }
            for (i = 19; i >= 17; --i) {
                bin_instruction[i] = * (func3 + i - 17) - 48;
            }
            for (i = 24; i >= 20; --i) {
                bin_instruction[i] = * (bin_arg1 + i - 20) - 48;
            }
            for (i = 31; i >= 25; --i) {
                bin_instruction[i] = * (opcode1 + i - 25) - 48;
            }
            for (i = 0; i <= 31; ++i)
                return;

        } else {
            if (isalpha(arg3[0]) != 0) {

                int address = search(arg3);
                if (address > 2047 || address < -2048) {
                    exit(3);
                }
                dec_num = address;
            } else {
                dec_num = toNum(arg3);
                if (dec_num > 2047 || dec_num < -2048) {
                    exit(3);
                }
            }
            arg3 = decToBin(dec_num, 12);
        }
        char arr[32];
        strcpy(arr, arg3);
        strcat(arr, bin_arg2);
        strcat(arr, func3);
        strcat(arr, bin_arg1);
        strcat(arr, opcode1);
        for (i = 0; i < 32; i++) {
            bin_instruction[i] = * (arr + i) - 48;
        }

    } else if (strcmp(opcode, "srli") == 0 || strcmp(opcode, "srai") == 0 || strcmp(opcode, "slli") == 0) {
        char * bin_arg2 = regToBin(arg2);

        if (arg3[0] == '0' && arg3[1] == 'x') {
            int hex_max_s = toNum(arg3);
            if (hex_max_s > 31 || hex_max_s < 0) {
                exit(3);
            }

            arg3 = hexToBin(arg3, 5);
        } else if (arg3[0] == '0' && arg3[1] == 'b') {
            for (i = 6; i >= 0; i--) {
                binary_array[i] = * (func7 + i) - 48;
            }

            arg3 = arg3 + 2;
            int len = strlen(arg3) - 1;
            for (i = 7; len >= 0; --i) {
                binary_array[i] = * (arg3 + len) - 48;
                len = --len;
            }

            for (i = 16; i >= 12; --i) {
                bin_instruction[i] = * (bin_arg2 + i - 12) - 48;
            }

            for (i = 19; i >= 17; --i) {
                bin_instruction[i] = * (func3 + i - 17) - 48;
            }
            for (i = 24; i >= 20; --i) {

                bin_instruction[i] = * (bin_arg1 + i - 20) - 48;
            }
            for (i = 31; i >= 25; --i) {
                bin_instruction[i] = * (opcode1 + i - 25) - 48;
            }
            for (i = 0; i <= 31; ++i)
                printf("%d", bin_instruction[i]);

            return;

        } else {
            dec_num = toNum(arg3);
            if (dec_num > 31 || dec_num < -0) {
                exit(3);
            }

            arg3 = decToBin(dec_num, 5);

        }
        char arr[32];
        strcpy(arr, func7);
        strcat(arr, arg3);
        strcat(arr, bin_arg2);
        strcat(arr, func3);
        strcat(arr, bin_arg1);
        strcat(arr, opcode1);
        for (i = 0; i < 32; i++) {
            bin_instruction[i] = * (arr + i) - 48;
            printf("%d", bin_instruction[i]);
        }
        printf("    I Type\n");
    } else if (strcmp(opcode, "lb") == 0 || strcmp(opcode, "lh") == 0 || strcmp(opcode, "lw") == 0 || strcmp(opcode, "lbu") == 0 || strcmp(opcode, "lhu") == 0) {
        strcpy(opcode1, "0000011");

        char * bin_arg3 = regToBin(arg3);

        if ((strcmp(opcode, "lb") == 0)) {
            strcpy(func3, "000");
        } else if ((strcmp(opcode, "lh") == 0)) {
            strcpy(func3, "001");
        } else if ((strcmp(opcode, "lw") == 0)) {
            strcpy(func3, "010");
        } else if ((strcmp(opcode, "lbu") == 0)) {
            strcpy(func3, "100");
        } else if ((strcmp(opcode, "lhu") == 0)) {
            strcpy(func3, "101");
        }

        if (arg2[0] == '0' && arg2[1] == 'x') {
            int hex_max_l = toNum(arg2);
            if (hex_max_l > 2047 || hex_max_l < -2048) {
                exit(3);
            }
            arg2 = hexToBin(arg2, 12);
        } else if (arg2[0] == '0' && arg2[1] == 'b') {
            arg2 = arg2 + 2;
            int len = strlen(arg2) - 1;
            for (i = 11; len >= 0; --i) {
                binary_array[i] = * (arg2 + len) - 48;
                len = --len;
            }

            for (i = 0; i <= 11; ++i) {
                bin_instruction[i] = binary_array[i];
            }
            for (i = 0; i <= 31; i++)
                printf("%d", bin_instruction[i]);

            for (i = 16; i >= 12; --i) {
                bin_instruction[i] = * (bin_arg3 + i - 12) - 48;
            }

            for (i = 19; i >= 17; --i) {

                bin_instruction[i] = * (func3 + i - 17) - 48;
            }
            for (i = 24; i >= 20; --i) {

                bin_instruction[i] = * (bin_arg1 + i - 20) - 48;
            }
            for (i = 31; i >= 25; --i) {

                bin_instruction[i] = * (opcode1 + i - 25) - 48;
            }
            for (i = 0; i <= 31; ++i)
                printf("%d", bin_instruction[i]);

            return;

        } else {
            dec_num = toNum(arg2);

            if (dec_num > 2047 || dec_num < -2048) {
                exit(3);
            }
            arg2 = decToBin(dec_num, 12);

        }
        char arr[32];
        strcpy(arr, arg2);
        strcat(arr, bin_arg3);
        strcat(arr, func3);
        strcat(arr, bin_arg1);
        strcat(arr, opcode1);
        for (i = 0; i < 32; i++) {
            bin_instruction[i] = * (arr + i) - 48;
            printf("%d", bin_instruction[i]);
        }
        printf("    I Type\n");

    } else if (strcmp(opcode, "ecall") == 0 || strcmp(opcode, "ebreak") == 0) {
        strcpy(opcode1, "1110011");

        for (i = 0; i < 32; i++)
            bin_instruction[i] = 0;

        if (strcmp(opcode, "ebreak") == 0) {
            bin_instruction[11] = 1;
        }
        for (i = 31; i >= 25; i--) {
            bin_instruction[i] = * (opcode1 + i - 25) - 48;
        }
        for (i = 0; i < 32; i++)
            printf("%d", bin_instruction[i]);

    }

    return;

}

void RType(char * opcode, char * arg1, char * arg2, char * arg3) {
    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        exit(4);
    }
    char * opcode1 = "0110011";
    char * bin_arg1 = regToBin(arg1);
    char * bin_arg2 = regToBin(arg2);
    char * bin_arg3 = regToBin(arg3);
    char func7[] = "0000000";
    char func3[] = "000";
    int i;
    if ((strcmp(opcode, "sub") == 0 || strcmp(opcode, "sra") == 0)) {
        func7[1] = '1';
    }

    if (((strcmp(opcode, "srl") == 0) || (strcmp(opcode, "sra") == 0))) {
        strcpy(func3, "101");
    } else if ((strcmp(opcode, "xor") == 0)) {
        strcpy(func3, "100");
    } else if ((strcmp(opcode, "or") == 0)) {
        strcpy(func3, "110");
    } else if ((strcmp(opcode, "and") == 0)) {
        strcpy(func3, "111");
    } else if ((strcmp(opcode, "sll") == 0)) {
        strcpy(func3, "001");
    } else if ((strcmp(opcode, "slt") == 0)) {
        strcpy(func3, "010");
    } else if ((strcmp(opcode, "sltu") == 0)) {
        strcpy(func3, "011");
    }
    char arr[32];
    strcpy(arr, func7);
    strcat(arr, bin_arg3);
    strcat(arr, bin_arg2);
    strcat(arr, func3);
    strcat(arr, bin_arg1);
    strcat(arr, opcode1);
    for (i = 0; i < 32; i++) {
        bin_instruction[i] = * (arr + i) - 48;
    }
    return;
}

void uType(char * opcode, char * arg1, char * arg2) {
    if (arg1[0] == '\0' || arg2[0] == '\0') {
        exit(4);
    }

    char opcode1[8];
    int dec_num;
    int binary_array[20] = {
        0.0
    };
    int i;
    char * bin_arg1 = regToBin(arg1);
    if (strcmp(opcode, "lui") == 0) {
        strcpy(opcode1, "0110111");
    } else if (strcmp(opcode, "auipc") == 0) {
        strcpy(opcode1, "0010111");
    }

    if (arg2[0] == '0' && arg2[1] == 'x') {

        int hex_max = toNum(arg2);
        if (hex_max > 1048575 || hex_max < 0) {
            exit(3);
        }
        arg2 = hexToBin(arg2, 20);
    } else if (arg2[0] == '0' && arg2[1] == 'b') {
        arg2 = arg2 + 2;
        int len = strlen(arg2) - 1;
        for (i = 19; len >= 0; --i) {
            binary_array[i] = * (arg2 + len) - 48;
            len = --len;
        }
        for (i = 0; i <= 19; ++i) {
            bin_instruction[i] = binary_array[i];
        }
        for (i = 24; i >= 20; --i) {
            bin_instruction[i] = * (bin_arg1 + i - 20) - 48;
        }
        for (i = 31; i >= 25; --i) {
            bin_instruction[i] = * (opcode1 + i - 25) - 48;
        }
        for (i = 0; i <= 31; ++i)
            printf("%d", bin_instruction[i]);
        return;
    } else {
        dec_num = toNum(arg2);
        if (dec_num > 1048575 || dec_num < 0) {
            exit(3);
        }
        arg2 = decToBin(dec_num, 20);

    }
    char arr[32];
    strcpy(arr, arg2);
    strcat(arr, bin_arg1);
    strcat(arr, opcode1);
    for (i = 0; i < 32; i++) {
        bin_instruction[i] = * (arr + i) - 48;

    }
}

void jType(char * opcode, char * arg1, char * label, int curr_pc) {
    if (arg1[0] == '\0' || label[0] == '\0') {
        exit(4);
    }

    int i, offset = 0;
    int label_address = 0;
    char opcode1[] = "1101111";

    for (int i = 0; i < 32; i++)
        bin_instruction[i] = 0;

    char * bin_arg1 = regToBin(arg1);
    if (isalpha(label[0]) == 0) {
        int num = toNum(label);
        offset = num;
    } else {
        label_address = search(label);
        offset = label_address - curr_pc;
    }

    if (offset > 52427 || offset < -52428) {
        exit(5);
    }

    char * bin_offset;
    bin_offset = decToBin(offset, 21);
    for (i = 31; i >= 25; i--)
        bin_instruction[i] = * (opcode1 + i - 25) - 48;
    for (i = 24; i >= 20; i--)
        bin_instruction[i] = * (bin_arg1 + i - 20) - 48;
    for (i = 19; i >= 12; i--)
        bin_instruction[i] = * (bin_offset + i - 11) - 48;
    bin_instruction[11] = * (bin_offset + 9) - 48;
    for (i = 10; i >= 1; i--)
        bin_instruction[i] = * (bin_offset + i + 9) - 48;
    bin_instruction[0] = * (bin_offset + 0) - 48;
    return;
}
