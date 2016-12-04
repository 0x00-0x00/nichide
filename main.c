#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_BUF 1024
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/* Static Variables */
char* author = "shemhazai";
double version = 0.11;


char* help_section = YEL "HELP SECTION FOR NICHIDE\n" RESET
        "Notes from author:\n"
        "This program was written by shemhazai in effort to mantain hardware addressing anonymous\n"
        "inside an insecure network and/or to avoid sysadmin detection.\n"
        "To use my program, you must passe the following parameters to it at command-line: \n"
        "\n-i <INTERFACE_NAME>\n    Interface to change mac.\n"
        BLU "\nAvailable modes: \n" RESET
        "    -r\n        Random MAC mode.\n"
        "    -p\n        Original MAC mode.\n"
        "    -m MAC_ADDRESS\n        Manual MAC mode.\n";


typedef struct
{
    char interface[MAX_BUF];
    char new_mac[MAX_BUF];
    int op_mode;
} args_t;

int power_interface(int status, char* interface)
{
    int return_code;

    char cmd[MAX_BUF];
    memset(cmd, '\0', sizeof(cmd));

    char ist[MAX_BUF];
    memset(ist, '\0', sizeof(ist));
    char* pointer = ist;

    if(status == 0) //powers down
    {
        pointer = "down";
    } else {
        pointer = "up";
    }
    sprintf(cmd, "ifconfig %s %s", interface, pointer);
    return_code = system(cmd);
    return return_code;
}


int change_mac(char* new_mac, char* interface)
{
    int return_code;
    char cmd[MAX_BUF];
    sprintf(cmd, "ifconfig %s hw ether %s", interface, new_mac);
    return_code = system(cmd);
    return return_code;
}

int change_to_permanent_mac(char* interface)
{
    char cmd[MAX_BUF];
    sprintf(cmd, "ifconfig %s hw ether $(ethtool -P %s | awk {'print $3'})", interface, interface);
    if(system(cmd) != 0) return 1;
    return 0;
}


unsigned char octet_gen(void)
{
    int v1 = 0;
    v1 = rand() % 255;
    return (unsigned char)v1;
}


char* mac_gen(void)
{
    int k;
    char* new_mac = malloc(sizeof(char) * 10);
    char* start= new_mac;
    unsigned char octet;

    for(k = 0; k < 6; k++)
    {
        octet = octet_gen();
        if(k == 0)
        {
            /* This to ensure valid unicast MAC ADDR */
            while(octet % 2 != 0)
            {
                octet = octet_gen();
            }
        }
        char* buffer = malloc(sizeof(octet));
        if(k != 5)
        {
            sprintf(buffer, "%.2x:", octet);
        } else {
            sprintf(buffer, "%.2x", octet);
        }
        memcpy(new_mac, buffer, 0x8);
        new_mac += 0x3;
    }

    return start;
}

void program_header(void)
{
    char buf[MAX_BUF];
    sprintf(buf, ""
            YEL "|| NicHide v%.2f written by %s ||\n" RESET
            "Revision: 2016/12/04\n", version, author);
    fprintf(stderr, buf);

}

int main(int argc, char* argv[])
{
    program_header();
    srand((unsigned int)time(NULL)); // gera uma seed para numeros aleatorios
    args_t* args = malloc(sizeof(args_t));
    int i;
    uid_t user_priv = geteuid();
    char *k, *j;
    k = args->interface;
    j = args->new_mac;


   if(user_priv != 0x0)
   {
       fprintf(stderr, RED "ERROR: You dont have enough privileges to work on interfaces configuration.\n" RESET);
       fprintf(stderr, RED "This application was meant to be executed by root user only.\n" RESET);
       return -1;
   }



    if(argc < 2)
    {
        fprintf(stderr, RED "ERROR: Not enough arguments.\nInvoke --help argument to print the help section.\n" RESET);
        return -1;
    }


    if(!strcmp(argv[1], "--help"))
    {
        fprintf(stderr, help_section);
        return 0;
    }

    for(i = 0; i < argc; i ++)
    {
        if(!strcmp(argv[i], "-i")) //search for interface option
        {
            fprintf(stdout, "Interface set ....: " MAG "%s\n" RESET, argv[i+1]);
            k = argv[i+1];
            i++;
        }

        if(!strcmp(argv[i], "-r"))
        {
            fprintf(stdout, "Mode set .........: " MAG "RANDOM\n" RESET);
            args->op_mode = 0x1;
        }

        if(!strcmp(argv[i], "-p"))
        {
            fprintf(stdout, "Mode set .........: " MAG "PERMANENT\n" RESET);
            args->op_mode = 0x2;
        }

        if(!strcmp(argv[i], "-m"))
        {
            if(argc < i+2)
            {
                fprintf(stderr, RED "You need to specify MAC ADDRESS after '-m' parameter to use MANUAL MAC mode!\n" RESET);
                return -1;
            }
            fprintf(stdout, "Mode set .........: " MAG "MANUAL MAC\n" RESET);
            args->op_mode = 0x3;
            j = argv[i+1];
        }

    }


    if(args->op_mode < 0)
    {
        fprintf(stderr, RED "ERROR: No op_mode was set.\n" RESET);
        return -1;
    }



    int p = 0;
    while(!p)
    {

        if (args->op_mode == 0x1) //random mac
        {
            j = mac_gen();

            fprintf(stdout, "\nMAC address generation ......: ");
            if(strlen(j) != 0x0) {
                fprintf(stdout, GRN "OK.\n" RESET);
            }   else {
                fprintf(stdout, RED "FAILED.\n" RESET);
            }

            int r, s, t;
            r = power_interface(0x0, k);
            sleep(2);
            s = change_mac(j, k);
            sleep(2);
            t = power_interface(0x1, k);
            fprintf(stdout, "NIC address alteration ......: ");
            if(r == 0x0 && s == 0x0 && t == 0x0)
            {
                fprintf(stdout, GRN "OK.\n" RESET);
                fprintf(stdout, "Your new MAC address is " GRN "%s\n" RESET, j);
                break;
            } else {
                fprintf(stdout, RED "FAILED.\n" RESET);
                fprintf(stderr, "Invalid address: " YEL "%s\n" RESET, j);
                sleep(1);
            }
        }
        else if (args->op_mode == 0x2) //permanent mac
        {
            int r, s, t;
            r = power_interface(0x0, k);
            sleep(2);
            s = change_to_permanent_mac(k);
            sleep(2);
            t = power_interface(0x1, k);
            fprintf(stdout, "NIC address alteration ......: ");
            if(r == 0x0 && s == 0x0 && t == 0x0)
            {
                fprintf(stdout, GRN "OK.\n" RESET);
                fprintf(stdout, "Your permanent mac has been restored.\n");
                break;
            } else {
                fprintf(stdout, RED "FAILED.\n" RESET);
                fprintf(stderr, "Error changing permanent mac.\n");
                sleep(1);
            }

        }
        else if(args->op_mode == 0x3) //manual mode
        {
            int r, s, t;
            r = power_interface(0x0, k);
            sleep(2);
            s = change_mac(j, k);
            sleep(2);
            t = power_interface(0x1, k);
            fprintf(stdout, "NIC address alteration ......: ");
            if(r == 0x0 && s == 0x0 && t == 0x0)
            {
                fprintf(stdout, GRN "OK.\n" RESET);
                fprintf(stdout, "Your new mac is " GRN "%s\n" RESET, j);
                break;
            } else {
                fprintf(stdout, RED "FAILED.\n" RESET);
                fprintf(stderr, "Invalid address: " YEL "%s\n" RESET, j);
                break;
            }
        }


    }

    free(args);

    return 0;
}