/******************************************************************
 * http-client.cpp - A simple binary/text http client using libaeon
 * Copyright (c) 2006-2016 Elden Armbrust
 ******************************************************************/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <libaeon.h>

#define HTML_HEADER_BREAK "\r\n\r\n" //the separator between header and content as per RFC1945



int main (int argc, char** argv)
{
    std::cout << "Checking arguments..." << std::endl;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s URL\r\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::cout << "Checked arguments..." << std::endl;
    char cBuffer[256];
    //let's parse the command-line now
    std::string sHostArg, sDomain, sPage;
    sHostArg = argv[1];
    if (sHostArg.find("http://") != std::string::npos) sHostArg = sHostArg.substr(7);
    sPage = "/";
    if (sHostArg.find("/") != std::string::npos) {
        sPage = sHostArg.substr(sHostArg.find("/"));
        sDomain = sHostArg.substr(0, sHostArg.find("/"));
    } else {
        sDomain = sHostArg;
    }
    std::cout << "Trimmed hostname..." << std::endl;
    if (sPage.size() <= 0 || sDomain.size() <= 0) { //if for some reason the url was invalid, return without connecting
        return EXIT_FAILURE;
    }


    std::cout << "Creating socket..." << std::endl;
    net::CClientSocket *sockClient = new net::CClientSocket(&sDomain, 80);

    if (sockClient->Connect((char*)sDomain.c_str(), 80) != true) {
        printf("Connection error!\r\n");
        delete sockClient;
        return EXIT_FAILURE;
    }

    std::string sReqPage = "GET " + sPage + " HTTP/1.0\r\n"; //generate the GET request
    std::string sReqHost = "Host: " + sDomain + "\r\n"; //we append the host, to please certain servers
    std::string sRequest = sReqPage + sReqHost + "\r\n";//combine it all into a cohesive request
    int iBytesSent = 0;
    iBytesSent = sockClient->Write(sRequest.c_str()); //send the request, returning the number of bytes
    //to iBytesSent for checking
    if (iBytesSent != sRequest.size()) {
        printf("Size consistency error!\r\n");
    }

    int iBytesRead = 0;
    std::string sTemp; //a string to
    int iOutputStart = 0; //define where the data starts
    bool bOutput = false;
    do {
        iBytesRead = sockClient->ReadUntil(cBuffer, sizeof(cBuffer)-1);
        if (iBytesRead < 0) break;
        sTemp.clear();
        sTemp = cBuffer;
        if (sTemp.find("\r\n\r\n") != std::string::npos && bOutput == false) {
            iOutputStart = sTemp.find("\r\n\r\n")+4;
            bOutput = true;
        } else {
            iOutputStart = 0;
        }
        if (bOutput == true) {
            for (int i = iOutputStart; i < iBytesRead; i++) { //start reading at the offset, to remove headers.

                fprintf(stdout, "%c", cBuffer[i]);
            }
        }
    } while (iBytesRead > 0);


    sockClient->Close();  //close the socket and clean up
    return 0;
}
