/*
    Copyright 2020 Foxconn Electronics Inc.
    All Rights Reserved
    No portions of this material may be reproduced in any form
    without the written permission of:

                    Foxconn Electronics Inc.

    All information contained in this document is Foxconn Electronics
    Corporation company private, proprietary, and trade secret.
*/

/*---------------------------------------------------------------------

    File FileSend_server.c

    Brief TCP license generate for Server

    Description:
        Generate license file and via TCP connection to send the license file to Client

    Date:
        2020/07/13

    Author:
        Neo

    Version:
        2020/07/08 - LicenseGenerator_server_V1
        2020/07/12 - LicenseGenerator_server_V2
        2020/09/17 - LicenseGenerator_server_V3
        2020/09/23 - LicenseGenerator_server_V4

    History:
        2020/07/08 - Development one file transfer to Server side to get license file
        2020/07/12 - Development multiple file transfer to Server side to get multiple license file
        2020/09/17 - Modify ActivateCode.ac file copy from /home/gtadmin/test/ to /var/www/html/config/code/test/ mechanism
                   - Delete the old ActivateCode.ac under the folder /var/www/html/config/code/test/
                   - Then Copy ActivateCode.ac to folder /var/www/html/config/code/test/
        2020/09/23 - Removed "Delete old ActivateCode.ac file" command

-----------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <mysql.h>

static void output_error(MYSQL * mysql);

//Setup sock to be non-blocking mode
/*
void setSockNonBlock(int sock)
{
    int flags;
    flags = fcntl(sock, F_GETFL, 0);

    if (flags < 0)
    {

        perror("fcntl(F_GETFL) failed");
        exit(EXIT_FAILURE);
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl(F_SETFL) failed");
        exit(EXIT_FAILURE);
    }
}
*/

int main(int argc, char *argv[])
{
    /* value declare */
    int fd = 0, confd = 0, nBytes, UseTimes;
	int num, a, b, checkResultY, checkResultN;;
	// int settimes = atoi(argv[3]); // Set aside for License generate times

    struct sockaddr_in serverInfo, clientInfo, serv_addr;
    int addrlen = sizeof(clientInfo);

	char *licensetime = argv[1], *function = argv[2];
	char filename[128];  // Store license file
	char *errorCheckY = "Yes";
	char *errorCheckN = "Wrong";

	/*Data receive value declare*/
	char recvbuff0[300]; // For license uudi receive
    char recvbuff1[12];  // For license function setting value receive
    char recvbuff2[50];  // File Name receive

	/*Data send value declare*/
	char issueCheckY[10];       // Send value for issue checking
    char issueCheckN[15];       // Send value for issue checking
    char sendbuffer[1000000]; // For License file send

    /////////////////////////////////////////////////////////////////////////////
    //---- Start Activate Code transfer from Client side to Server --------------
    /////////////////////////////////////////////////////////////////////////////
    printf("Start license create information transfer from Client to Server -------------\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(23); // Port 23 for telnet, Port 21 for FTP and Port 443 for HTTPS

    bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(fd, 10);

    while(1)
    {
        /*----------------------- Get System Time---------------------------------------*/
        char dateTimeBuffer1[50];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(dateTimeBuffer1, "%04d-%02d-%02d %02d:%02d:%02d", \
        tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("%s\n", dateTimeBuffer1);

        /*----------------------- TCP start---------------------------------------*/
        TCPRestart:
        memset(&serv_addr, '0', sizeof(serv_addr));
        memset(recvbuff0, '0', sizeof(recvbuff0));
        memset(recvbuff1, '0', sizeof(recvbuff1));
        memset(recvbuff2, '0', sizeof(recvbuff2));
		memset(issueCheckY, '0', sizeof(issueCheckY));
	    memset(issueCheckN, '0', sizeof(issueCheckN));

        printf("Socket created\n");
        printf("License time setting: %s\n", licensetime);
        printf("License function setting: %s\n", function);

        /* TCP connection */
        confd = accept(fd, (struct sockaddr*) NULL, NULL);
        if (-1 == confd)
        {
            perror("Accept");
            continue;
        }

        //Set up sock to be non-blocking
        //setSockNonBlock(fd);


        /* Receive Activate Code */
        if (0 > (recv(confd, recvbuff0, 300, 0)))
        {
            puts("No Data\n");
            // return -1;
            goto TCPRestart;

        }
        else
        {
            printf("Receive 'Activate Code': %s \n", recvbuff0);
        }

        /* Receive User Name */
        if (0 > (recv(confd, recvbuff1, 12, 0)))
        {
            puts("No Data\n");
            // return -1;
            goto TCPRestart;
        }
        else
        {
            printf("Receive 'User Name': %s \n", recvbuff1);
        }

        /* Receive File Name */
        if (0 > (recv(confd, recvbuff2, 50, 0)))
        {
            puts("No Data\n");
            //return -1;
            goto TCPRestart;
        }
        else
        {
            printf("Receive 'File Name': %s \n", recvbuff2);
        }

        /* Store Activate Code to be ActivateCode.ac File */
        FILE *fpwrite = fopen( "/home/gtadmin/test/ActivateCode.ac", "wb");

        // FILE *fpwrite = fopen( "/var/www/html/config/code/test/ActivateCode.ac", "wb");
        if(fpwrite == NULL)
        {
            printf("Cannot find the Activate Code file \n\n");
            //return -1;
            goto TCPRestart;
        }
        nBytes = fwrite(recvbuff0, sizeof(char), 300, fpwrite);
        fclose(fpwrite);

        printf("Store ActivateCode.ac file into server side \n");
        system("chmod 777 /home/gtadmin/test/ActivateCode.ac");

        // Transfer ActivateCode.ac file into root path folder
        system("rm -f /var/www/html/config/code/test/ActivateCode.ac");
        system("cp -f /home/gtadmin/test/ActivateCode.ac /var/www/html/config/code/test/");
        // system("mv -f /home/gtadmin/test/ActivateCode.ac /var/www/html/config/code/test/");

        chdir("test");  // cd to test folder
        char*command = (char*)"python activateCode_tool.py ActivateCode.ac";
        char*scanuuid;
        char*scanmemid;
        char uuid[64];
        char memid[64];
        memset(uuid,0,sizeof(uuid));
        memset(memid,0,sizeof(memid));

        printf("Start to create license Number **********************************\n");
        /* Execute command line and store the result to file */
        char buf[1000] = {0};
        int i;
        FILE *fpRead;
        /* pop out activateCode.ac of uuid */
        fpRead = popen(command, "r");
        while(1)
        {
            if (fgets(buf, sizeof(buf), fpRead) != NULL)
            {
                /* Check the string without extra string, if yes, need use '\0' to replace in the end */
                scanuuid  = strstr(buf, "sysuuid=");
                scanmemid = strstr(buf, "memid=");
                if(scanuuid)
                {
                    for(i=0; i<strlen(scanuuid); i++)
                    {
                        if(scanuuid[i]==';')
                        {
                            scanuuid[i] = '\0';
                            break;
                        }
                    }
                    strcpy(uuid,scanuuid + 8);
                    printf("uuid: %s\n", uuid);

					strcpy(issueCheckY, errorCheckY);
				    send(confd, issueCheckY, 10, 0);
				    printf("UUID Status Check : %s \n", issueCheckY);
                    printf("Create UUID Success ************************************\n");
                    break;
                }

                else if(scanmemid)
                {
                    for(i=0; i<strlen(scanmemid); i++)
                    {
                        if(scanmemid[i]==';')
                        {
                            scanmemid[i] = '\0';
                            break;
                        }
                    }
                    strcpy(memid,scanmemid + 6);
                    printf("memid: %s\n", memid);

					strcpy(issueCheckY, errorCheckY);
				    send(confd, issueCheckY, 10, 0);
				    printf("memid Status Check : %s \n", issueCheckY);
                    printf("Create memid Success ************************************\n");
                    break;
                }

            }
            else
            {
	            /* Send UUID Checking Result */
		        strcpy(issueCheckN, errorCheckN);
		        if (0 > (checkResultN = send(confd, issueCheckN, 15, 0)))
		        {
                    printf("Check Result Send Failed ***************************\n");
		            goto TCPRestart;
		        }
		        else
		        {
	                printf("Create Status : %s \n", issueCheckN);
		            printf("Create UUID Failed *************************************\n");
		            goto TCPRestart;
                }
            }
        }


        pclose(fpRead);
        printf("End Process Activate Code Generate-------------------\n");

        /* License tmp file create */
        char *name = recvbuff1;

        char out_buf1[128];
        memset(out_buf1, 0, sizeof(out_buf1)); // Clean buffer
        char out_buf2[128];
        memset(out_buf2, 0, sizeof(out_buf2)); // Clean buffer

        printf("\n");
        system("chmod 777 key_generator");
        system("chmod 777 life_time_generator");

        /* Execute Key generator, input uuid, license time and function */
        snprintf(out_buf1, sizeof(out_buf1), "./key_generator %s %s %s", uuid, licensetime, function);
        system(out_buf1);
        // printf("Key Generator Command:%s\n", out_buf1);

        /* Execute License time generator */
        snprintf(out_buf2, sizeof(out_buf2), "./life_time_generator ./lifetime %s", licensetime);
        system(out_buf2);
        // printf("License Commane:%s\n", out_buf2);

        /* Export the license_tmp file to the folder */
        system("cat license_file lifetime > license_tmp");

        printf("Finish to Create License tmp File--------------------\n\n");

        //////////////////////////////////////////////////////////
        // Send license_tmp file to Client -----------------------
        /////////////////////////////////////////////////////////
        sprintf(filename, "/home/gtadmin/License_File_Backup/License_tmp_%s", recvbuff2);

        printf("Start File transfer from Server to Client----------------\n");
        // FILE *fpsned = fopen("/home/gtadmin/test/license_tmp", "rb");
        FILE *fpsned = fopen("/var/www/html/config/code/test/license_tmp", "rb");
        if(fpsned == NULL)
        {
            printf("Cannot find the license file in the path \n\n");
            // return -1;
            goto TCPRestart;
        }
        printf("Find license file \n\n");

        b = fread(sendbuffer, 1, sizeof(sendbuffer), fpsned);

        send(confd, sendbuffer, b, 0);
        // printf("license_tmp: %s \n\n", sendbuffer);
        printf("Send the license file to Client \n\n");
        fclose(fpsned);

        /* License file backup in Server */
        int nBytesback;
        FILE *fpfile = fopen(filename, "wb");
        if(fpfile == NULL)
        {
            printf("Cannot backup the license file in the path \n\n");
            // return -1;
            goto TCPRestart;
        }
        nBytesback = fwrite(sendbuffer, sizeof(char), b, fpfile); // Write into license_tmp file
        printf("Finished backup license file \n\n");
        fclose(fpfile);

        printf("Finished File transfer from Server to Client-------------\n\n");

        /*----------------------- Get System Time---------------------------------------*/
        char dateTimeBuffer2[50];
        sprintf(dateTimeBuffer2, "%04d-%02d-%02d %02d:%02d:%02d", \
        tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("%s\n", dateTimeBuffer2);

        /////////////////////////////////////////////////////////////////////////////////
        //----------------------- MySQL Connection Start --------------------------------
        /////////////////////////////////////////////////////////////////////////////////
        MYSQL mysql;
        MYSQL_RES * result;
        MYSQL_ROW row;
        MYSQL_FIELD * fields;

        const char* host = "localhost";
        const char* user = "root";
        const char* password = "admingt1234";
        const char* database = "config";
        const int 	port = 3306;
        const char* socket = NULL;
        const int flag = 0;
        const char* sql ;
        int num_fields;
        unsigned long * lengths;
        int j;

        /* initialize the database */
        if(!mysql_init(&mysql) )
        {
            output_error(&mysql);
        }
        printf("Mysql Initialized Successfully ! \n");

        /* connect to the database */
        if(!mysql_real_connect(&mysql, host, user, password, database, port, socket, flag))
        {
            output_error(&mysql);
        }
        printf("Mysql Connect Successfully! \n\n");
        printf("The content of the table data in the database\n\n");
        printf("-----------------------------------------------------------\n");

        /*----------------------- Insert result into MySQL-----------*/
        char *currentime1 = dateTimeBuffer1;
        char *currentime2 = dateTimeBuffer2;
        char insert[100];

        sprintf(insert, "INSERT INTO TCP_Server(user, TCP_Start, uuid, license_time, function, TCP_End) VALUES('%s', '%s', '%s', '%s', '%s', '%s');" \
                , name, currentime1, uuid, licensetime, function, currentime2);
        printf("insert = %s\n", insert);

        if(mysql_query(&mysql,insert))
        {
            printf("Insert data is failed---------\n");
            // return -1;
            goto TCPRestart;
        }
        else
        {
            printf("Insert successfully-----------\n");
        }
        mysql_close(& mysql);
        /*----------------------- Insert finish-----------------------*/
        /*----------------------- MySQL Connection End ---------------------------------*/
    }
    close(fd);

    return 0;
}

static void output_error(MYSQL * mysql)
{
	fprintf(stderr, "errorno: %d \n", mysql_errno(mysql) );
	fprintf(stderr, "error info: %s\n", mysql_error(mysql) );
	exit(1);
}
