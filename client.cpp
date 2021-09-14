#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 200
using namespace std;

void display(char *data) {
    int i = 0;
    printf("Code\tPrice\tQuantity\n");
    while(data[i] != '\000'){
        while(data[i] != ',' && data[i] != '&'){
            printf("%c", data[i]);
            i++;
        }
        if (data[i] == '&'){
            printf("\n");
        } else {
            printf("\t");
        }
        i++;
        if(data[i] == '&'){
            printf("\n");
        }
    }
}

void buy_handler(int sockfd){
    char buff[80];
    cout << "Enter icode price quantity like 4N5N6N\n";
    cin >> buff;
    write(sockfd, buff, sizeof(buff));
} 

void sell_handler(int sockfd){
    char buff[80];
    cout << "Enter icode price quantity like 4N5N6N\n";
    cin >> buff;
    write(sockfd, buff, sizeof(buff));
}

bool login_handler(int sockfd){
    char buff[MAX];
    read(sockfd, buff, sizeof(buff)); 
    printf("From Server : %s \n", buff);
    bzero(buff, sizeof(buff)); 
    printf("Enter the Trader ID : "); 
    int n = 0; 
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff)); 
    printf("Enter the pwd : ");
    bzero(buff, sizeof(buff)); 
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff)); 
    bzero(buff, sizeof(buff)); 
    read(sockfd, buff, sizeof(buff)); 
    cout << buff<< endl;
    bzero(buff, sizeof(buff)); 
    read(sockfd, buff, sizeof(buff)); 
    cout << buff<< endl;
    if(buff [0] == '1')
       return  true;
    return false;
}

void order_status_handler(int sockfd){
    char buy_data[1024], sell_data[1024];
    bzero(buy_data, sizeof(buy_data));
    bzero(sell_data, sizeof(sell_data));
    read(sockfd, buy_data, sizeof(buy_data));
    read(sockfd, sell_data, sizeof(sell_data));
    printf("------- Buy Status ----------\n");
    display(buy_data);
    printf("------- Sell Status ----------\n");
    display(sell_data);
}

void traded_set_handler(int sockfd){
    char buffer[512];
    // bzero(buffer,512);
    // int n=read(sockfd,buffer,512);
    // if(n<0)
    //     cout<<"read error";
    // cout << buffer << endl;
    bzero(buffer,512);
    int n=read(sockfd,buffer,512);
    if(n<0)
        cout<<"read error";
    cout<<buffer<<endl;
}

int main(int argc , char *argv[]){

    int clientSocket, ret, opt;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    cout << "--------------------------------------\n";
    cout << "    Welcome to Trading Application    \n";
    cout << "--------------------------------------\n";
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Connected to Server.\n");
    bzero(buffer, sizeof(buffer));
    buffer[0] = 'l';buffer[1] = 'o';buffer[2]='g';
    send(clientSocket, buffer, strlen(buffer), 0);
    if (login_handler(clientSocket)){
        while(true){
            cout << "Select the option to continue:\n";
            cout << "1. Buy\n";
            cout << "2. Sell\n";
            cout << "3. View Order Status\n";
            cout << "4. View Trade Status\n";
            cout << "5. Exit\n";
            cin >> opt;
            bzero(buffer, sizeof(buffer));
            if (opt == 1){
                // send buy request
                buffer[0]='b';buffer[1]='u';buffer[2]='y';
                send(clientSocket, buffer, strlen(buffer), 0);
                buy_handler(clientSocket);
            } else if (opt == 2){
                // send sell request
                buffer[0]='s';buffer[1]='e';buffer[2]='l';buffer[3]='l';
                send(clientSocket, buffer, strlen(buffer), 0);
                sell_handler(clientSocket);
            } else if(opt == 3){
                // send order status request
                buffer[0]='o';buffer[1]='r';buffer[2]='d';
                send(clientSocket, buffer, strlen(buffer), 0);
                order_status_handler(clientSocket);
            } else if (opt == 4){
                // send view trade status request
                buffer[0]='t';buffer[1]='r';buffer[2]='d';
                send(clientSocket, buffer, strlen(buffer), 0);
                traded_set_handler(clientSocket);
            } else if(opt == 5){
                cout << "Thank you for using trading application\n";
                break;
            } else {
                cout << "Invalid request\n";
                break;
            }
        }
    }
    bzero(buffer, sizeof(buffer));
    buffer[0]='e';buffer[1]='x';buffer[2]='i';buffer[3]='t';
    send(clientSocket, buffer, strlen(buffer), 0);
    close(clientSocket);
    return 0;
}
