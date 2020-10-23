
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

    File main.c

    Brief TCP license generate for client

    Description:
        TCP connection to Server to generate license file for Client

    Date:
        2020/07/13

    Author:
        Neo

    History:
        2020/07/08 - Development one file transfer to Server side to get license file
        2020/07/12 - Development multiple file transfer to Server side to get multiple license file
        2020/07/14 - Development Windows base tool

-----------------------------------------------------------------------*/

#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>
#include <io.h>
#include <string.h>

/* Command Setting */
void Print_Help( void )
{
	/* Please edit your print help here */
	printf( "****************************************************\n" );
	printf( "LicenseGenerator.exe 'Server IP Address' 'User Name'\n" );
	printf( "****************************************************\n" );
}

/* IP Check*/
int IP_Check(char *ip)
{
	if(INADDR_NONE == inet_addr(ip))
	{
		return 2;
	}

	return 2;
}

int main(int argc, char *argv[])
{
    const char* definePath = "C:\\LicenseGenerate_Client\\0_Activate_Code\\*.ac";
    char buff[1024];
    int i=0;
    _chdir(definePath);
    unsigned long long hFile;
    struct _finddata_t fileName;
    hFile = _findfirst(definePath, &fileName);

    do
    {
        i++;
        /* Remove '.ac' name */
        sprintf(buff, "%s", fileName.name);

        int a;
        char *FName = strstr(buff,""); // Set for Activate code file name
        while(1)
        {
            if(FName)
            {
                for(a=0; a<strlen(FName); a++)
                {
                    if(FName[a]=='.')
                    {
                        FName[a] = '_';
                        break;
                    }
                }
                printf("File Name: %s\n", FName);
                break;
            }
        }
        ////////////////////////////////////////////////////////////////
        // TCP Start ---------------------------------------------------
        ////////////////////////////////////////////////////////////////
        /*value declare*/
        int  sfd =0, b;
        int  txtBytes, nBytesback, j;
        int n, nBytes;
        char codepath[50];

        /*Socket value declare*/
        SOCKET sockfd;
        struct sockaddr_in address;
        int result;
        char ch = 'F';

        /*Data send value declare*/
        char sendbuff0[300];     // For send Activate Code
        char sendbuff1[12];      // For send User name
        char sendbuff2[50];      // For send Activate code file name
        int  ActivateCode, name;
        char buff[300] = {0};
        char *AC = buff;
        char *username = argv[2];

        /*Data receive value declare*/
        char recvbuffer[1000000]; // Received license data
        char filename[128];       // Set for export license file
        char checkbuffer[15];      // Send value for issue checking

        /*Clean buffer*/
        memset(sendbuff0, '0', sizeof(sendbuff0));
        memset(sendbuff1, '0', sizeof(sendbuff1));
        memset(sendbuff2, '0', sizeof(sendbuff2));
        memset(recvbuffer, '0', sizeof(recvbuffer));
        memset(checkbuffer, '0', sizeof(checkbuffer));

        /* User Friendly */
        if(3 != argc)
        {
            Print_Help();
            return 2;
        }

        /* Server IP Check */
        if(-1 == IP_Check(argv[1]))
        {
            printf("Server IP is not valid\n");
            return 2;
        }
        else
        {
            printf("Server IP is valid\n");
        }

        /* Connect to Winsock library to start up socket*/
        WSADATA wsadata;
        if(0 != WSAStartup(0x101,(LPWSADATA)&wsadata))
        {
            printf("Winsock Error\n");
            return 2;
        }

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(argv[1]);
        address.sin_port = htons(23);
        // Port 23 for telnet, Port 21 for FTP and Port 443 for HTTPS

        result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
        if(-1 == result)
        {
            printf("Connect Error");
            return 2;
        }
        printf("connect server(IP:%s).\n\n",argv[1]);
        printf("Start Activate Code Generate-----------------------------------------------\n");

        /* CHANGE THE FILENAME */
        sprintf(codepath, "C:\\LicenseGenerate_Client\\0_Activate_Code\\%s", fileName.name);

        printf("Activate Code file location: %s \n\n", codepath);

        FILE *fpread = fopen(codepath ,"rb"); //File open
        if (NULL != fgets(buff, sizeof(buff), fpread))
        {
            printf("Activate Code: %s\n", buff);
        }
        else
        {
            printf("Cannot reach %s file -----\n", fileName.name);
            return 2;
        }
        fclose(fpread);

        ///////////////////////////////////////////////////////////////////
        // Data Send ------------------------------------------------------
        ///////////////////////////////////////////////////////////////////

        /* Send Activate Code information */
        strcpy(sendbuff0, AC);
        if (0 > (ActivateCode = send(sockfd, AC, 300, 0)))
        {
            puts("Activate Code Send Failed -----\n");
            return 2;
        }
        printf("Activate Code send to Server -----\n");

        /* Send User name */
        strcpy(sendbuff1, username);
        if (0 > (name = send(sockfd, username, 10, 0)))
        {
            puts("User Name Send Failed\n");
            return 2;
        }
        printf("User Name '%s' send to Server -----\n\n", username);

        /* Send File Name */

        strcpy(sendbuff2, FName);
        if (0 > (name = send(sockfd, FName, 50, 0)))
        {
            puts("File Name Send Failed\n");
            return 2;
        }
        printf("File Name '%s' send to Server -----\n\n", FName);


        //////////////////////////////////////////////////////////
        // Received Lisnece file from Server -----------------
        //////////////////////////////////////////////////////////

        /* UUID Result Check */
        // ********************************************************************************************
        char Result[5];
        recv(sockfd, checkbuffer, 15, 0);

        printf("%s \n", checkbuffer);

        if (strstr(checkbuffer, "Wrong"))
        //if (Result == 6420544)
        {
            printf("%d \n",Result);
            printf("Receive Check Result:%s\n", checkbuffer);
            printf("UUID Creation has problem #############################\n");
            return 1;
        }
        else
        {
            printf("Receive Check Result:%s\n", checkbuffer);
            printf("UUID Creation without no issue &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
        }
        // ********************************************************************************************


        /* Export the license file to the path */
        sprintf(filename, "C:\\LicenseGenerate_Client\\1_License_File\\License_tmp_%s", FName);
        printf("File Store into path folder: %s \n\n", filename);

        /* Check License content transfer status */
        /*
        if (recvbuffer == recvbuffer)
        {
            printf("Cannot received license information -----\n\n ");
            exit(2);
        }*/

        /* Write license information in to file */
        FILE *fpfile = fopen(filename, "wb"); //File open

        txtBytes = recv(sockfd, recvbuffer, 1000000,0);
        char *LicenseInfo = recvbuffer;
        if (0 > txtBytes)
        {
            printf("Cannot received license information -----\n\n");
            return 2;
        }
        //printf("Received Info: %s \n\n", LicenseInfo);
        txtBytes = fwrite(recvbuffer, sizeof(char), txtBytes, fpfile); // Write into license file
        fclose(fpfile);

        printf("Receiving license from Server -----\n");

        closesocket(sockfd);

    }
    while(hFile != -1 && _findnext(hFile, &fileName)==0); //Find next file

    printf("\n\n");
    printf("Total File Number: %d \n", i);
    printf("License Generate Finish ----------------------------\n");

    exit(0);
}
