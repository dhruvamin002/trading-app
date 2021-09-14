#include<bits/stdc++.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAX 200
using namespace std;
struct request{
int price,quantity,ccode;
    request(int x,int y,int z){
        price = x; 
        quantity = y;
        ccode = z;
    }
};
struct compareprice {
    bool operator()(request const& r1, request const& r2)
    {
        
        return r1.price < r2.price;
    }
};
struct cprice {
    bool operator()(request const& r1, request const& r2)
    {
        
        return r1.price > r2.price;
    }
};

struct itemlist
{
    int icode,price,quantity;
    itemlist(int x,int y,int z)
    {
        icode=x;
        price=y;
        quantity=z;
    }
};
#define pq priority_queue<request,vector<request>,compareprice>
#define pq1 priority_queue<request,vector<request>,cprice>
map<int,pq1> sellm;
map<int,pq > buym;
map<int,vector<itemlist> > initiallist;
map<string, string> t1;
int c_id;

struct trade{              
    int buyerid;          
    int sellerid;         
    int itemno;       
    int tradeprice;            
    int tradeqty;              
};
int tradeno = 0;
struct trade tradedetail[1000];
int flag_trade=-1;

void init()
{
    t1.insert(pair<string, string>("1\n", "1234\n"));
    t1.insert(pair<string, string>("2\n", "2345\n"));
    t1.insert(pair<string, string>("3\n", "3456\n"));
    t1.insert(pair<string, string>("4\n", "4567\n"));
    t1.insert(pair<string, string>("5\n", "5678\n"));
    t1.insert(pair<string, string>("6\n", "6789\n"));
}

bool check(string s1, string s2) {  
    if(t1[s1] == s2)
        return true;
    return false;
}

void sell(map<int,pq1 > &sellm,int icode,int price,int quantity,int ccode) {
    request s(price,quantity,ccode);
    sellm[icode].push(s);
}

void buy(map<int,pq > &buym,int icode,int price,int quantity,int ccode) {
    request b(price,quantity,ccode);
    buym[icode].push(b);
}

int findmatching(map<int,pq1 > &sellm,map<int,pq > &buym) {
    int flag=0;
    for(int i=1;i<=10;i++) {
        if(sellm[i].size()!=0&& buym[i].size()!=0&&buym.find(i)!=buym.end() && sellm.find(i)!=sellm.end()) {
            if(sellm[i].top().price<=buym[i].top().price) {
            		struct trade temp;
			temp.itemno=i;
			temp.tradeqty=sellm[i].top().quantity;
			if(flag_trade==0){
			temp.tradeprice=buym[i].top().price;
			}
			else if(flag_trade==1){
			temp.tradeprice=sellm[i].top().price;
			}
			temp.buyerid=buym[i].top().ccode;
			temp.sellerid=sellm[i].top().ccode;
			tradedetail[tradeno++] = temp;
                if(sellm[i].top().quantity==buym[i].top().quantity) {
                    
                    //remove both
                     if(initiallist.find(buym[i].top().ccode)!=initiallist.end())
                                        {
                                            for(int j=0;j<initiallist[buym[i].top().ccode].size();j++)
                                            {
                                                int k=buym[i].top().ccode;
                                                if((initiallist[k].begin()+j)->icode==i)
                                                {
                                                (initiallist[k].begin()+j)->quantity=(initiallist[k].begin()+j)->quantity+buym[i].top().quantity;
                                                (initiallist[k].begin()+j)->price=buym[i].top().price;
                                                break;
                                                }
                                            }
                                        }
                                        else
                                        {
                                        itemlist r(i,buym[i].top().ccode,buym[i].top().quantity);
                                        initiallist[buym[i].top().ccode].push_back(r);
                                        }
                    cout << "[+] both remove" <<endl;
                    sellm[i].pop();
                    buym[i].pop();
                    flag=1;
                } else {
                    if(sellm[i].top().quantity<buym[i].top().quantity) {
                    	 if(initiallist.find(buym[i].top().ccode)!=initiallist.end())
                                        {
                                            for(int j=0;j<initiallist[buym[i].top().ccode].size();j++)
                                            {
                                                int k=buym[i].top().ccode;
                                                if((initiallist[k].begin()+j)->icode==i)
                                                {
                                                (initiallist[k].begin()+j)->quantity=(initiallist[k].begin()+j)->quantity+buym[i].top().quantity;
                                                (initiallist[k].begin()+j)->price=buym[i].top().price;
                                                break;
                                                }
                                            }
                                        }
                                        else
                                        {
                                        itemlist r(i,buym[i].top().ccode,buym[i].top().quantity);
                                        initiallist[buym[i].top().ccode].push_back(r);
                                        }
                        request k(buym[i].top().price,buym[i].top().quantity-sellm[i].top().quantity,buym[i].top().ccode);
                        sellm[i].pop();
                        buym[i].pop();
                        buym[i].push(k);
                        cout << "[+] removed from sell" << endl;
                        flag=1;
                    } else {
                    	 if(initiallist.find(buym[i].top().ccode)!=initiallist.end())
                                        {
                                            for(int j=0;j<initiallist[buym[i].top().ccode].size();j++)
                                            {
                                                int k=buym[i].top().ccode;
                                                if((initiallist[k].begin()+j)->icode==i)
                                                {
                                                (initiallist[k].begin()+j)->quantity=(initiallist[k].begin()+j)->quantity+buym[i].top().quantity;
                                                (initiallist[k].begin()+j)->price=buym[i].top().price;
                                                break;
                                                }
                                            }
                                        }
                                        else
                                        {
                                        itemlist r(i,buym[i].top().ccode,buym[i].top().quantity);
                                        initiallist[buym[i].top().ccode].push_back(r);
                                        }
                        request k(sellm[i].top().price,sellm[i].top().quantity-buym[i].top().quantity,sellm[i].top().ccode);
                        buym[i].pop();
                        sellm[i].pop();
                        sellm[i].push(k);
                        cout << "[+] removed from buy" << endl;
                        flag=1;
                    }
                }
            }
        }
    }  
    return flag;
}

void buy_handler(int connfd){
    flag_trade=0;
    cout << "[+] inside buy handler\n";
    char buffer[1025];
    int price,icode,quantity,i=0;
    read(connfd,buffer,sizeof(buffer));
    icode=buffer[i]-'0';
    i++;
    while(buffer[i]!='N') {
        icode=icode*10+(buffer[i]-'0');
        i++;
    }
    i++;
    price=buffer[i]-'0';
    i++;
    while(buffer[i]!='N') {
        price=price*10+(buffer[i]-'0');
        i++;
    }
    i++;
    quantity=buffer[i]-'0';
    i++;
    while(buffer[i]!='N') {
        quantity=quantity*10+(buffer[i]-'0');  
        i++;      
    }
    cout<<"[+] icode "<<icode<<"\n"<<"[+] price "<<price<<"\n[+] quantity "<<quantity<<"\n";
    buy(buym,icode,price,quantity,c_id);
    while(findmatching(sellm,buym));
} 

void sell_handler(int connfd){
    flag_trade=1;
    cout << "[+] inside sell handler\n";
    char buffer[1025];
    int price,icode,quantity,i=0;
    read(connfd,buffer,sizeof(buffer));
    cout << "[+] " << buffer << "\n";
    icode = buffer[i] - '0';
    i++;
    while(buffer[i]!='N') {
        icode = icode*10+(buffer[i]-'0');
        i++;
    }
    i++;
    price=buffer[i]-'0';
    i++;
    while(buffer[i]!='N') {
        price=price*10+(buffer[i]-'0');
        i++;
    }
    i++;
    quantity = buffer[i]-'0';
    i++;
    while(buffer[i]!='N') {
        quantity = quantity*10 + (buffer[i]-'0');  
        i++;      
    }
    cout << "[+] " << i << "\n";
    cout << "[+] icode " << icode << "\n" << "[+] price " << price << "\n[+] quantity " << quantity <<"\n";
    sell(sellm,icode,price,quantity,c_id);
    while(findmatching(sellm,buym));
}

void order_status_handler(int connfd){
    cout << "[+] inside order status handler\n";
    int tmp1 = 0, tmp2 = 0;
    string buy_data = "", sell_data = "";
    char buy_data_response[1024], sell_data_response[1024];
    for(int i=1;i<=10;i++) {
        if(buym[i].size() != 0 && buym.find(i) != buym.end()) {
            buy_data += to_string(i) + ',' + to_string(buym[i].top().price) + ',' + to_string(buym[i].top().quantity) + '&';
        }
        if(sellm[i].size() != 0 && sellm.find(i) != sellm.end()) {
            sell_data += to_string(i) + ',' + to_string(sellm[i].top().price) + ',' + to_string(sellm[i].top().quantity) + '&';
        }
    }
    strcpy(buy_data_response, buy_data.c_str());
    strcpy(sell_data_response, sell_data.c_str());
    write(connfd, buy_data_response, sizeof(buy_data_response));
    write(connfd, sell_data_response, sizeof(sell_data_response));
}

void traded_set_handler(int connfd){
    cout << "[+] inside traded set handler\n";
    char temp[1000];
    sprintf(temp,"ITEM_NO   BUYER_ID    SELLER_ID   QUANTITY    PRICE\n\n");
    int n=write(connfd,temp,strlen(temp));
    for (int i=0;i<tradeno;i++){
        if(tradedetail[i].buyerid==c_id) {
            sprintf(temp,"%d        \t%d      \t%d       \t%d    \t%d\n",tradedetail[i].itemno,tradedetail[i].buyerid,tradedetail[i].sellerid,tradedetail[i].tradeqty,tradedetail[i].tradeprice);
            write(connfd, temp, sizeof(temp));
        } else if(tradedetail[i].sellerid==c_id) {
            sprintf(temp,"%d        \t%d      \t%d       \t%d            \t%d\n",tradedetail[i].itemno,tradedetail[i].buyerid,tradedetail[i].sellerid,tradedetail[i].tradeqty,tradedetail[i].tradeprice);
            write(connfd, temp, sizeof(temp));
        }
    }
    return;
}

void login_handler(int connfd){
    cout << "[+] inside login handler\n";
    init();
    char buff[MAX] = "Hello from server, please login to use further:";
    write(connfd, buff, sizeof(buff)); 
    bzero(buff, MAX); 
    //Read id;
    bzero(buff, MAX); 
    read(connfd, buff, sizeof(buff)); 
    char trader_id[MAX];
    char pwd[MAX];
    strcpy(trader_id, buff);
    c_id=atoi(trader_id);
    bzero(buff, MAX); 
    read(connfd, buff, sizeof(buff)); 
    strcpy(pwd, buff);
    /*printf("%s\n", trader_id);
    printf("%s", pwd);*/
    bool c = check(trader_id, pwd);
    if(c)
    {
        char  s[MAX] = "Login Success welcome trader ";
        write(connfd, s, sizeof(s)); 
        char s1[5] = "1";
        write(connfd, s1 , sizeof(s1)); 
    } else {
        char  s[MAX] = "Invalid ID or Password Combination";
        write(connfd, s,  sizeof(s)); 
        char s1[5] = "0";
        write(connfd, s1 ,  sizeof(s1)); 
   }
}

void *handle_connection(void * p_client_socket){
    int newSocket = *((int*)p_client_socket);
    free(p_client_socket);
    char buffer[1024];
    while(1){
        bzero(buffer, sizeof(buffer));
        recv(newSocket, buffer, 1024, 0);
        cout << "[+] " << buffer << "\n";
        if (strcmp(buffer, "log") == 0){
            login_handler(newSocket);
        } else if (strcmp(buffer, "buy") == 0){
            buy_handler(newSocket);
        } else if (strcmp(buffer, "sell") == 0){
            sell_handler(newSocket);
        } else if (strcmp(buffer, "ord") == 0){
            order_status_handler(newSocket);
        } else if(strcmp(buffer, "trd") == 0){
            traded_set_handler(newSocket);
        } else if(strcmp(buffer, "exit") == 0){
            printf("Disconnected");
            break;
        }
    }
    return NULL;
}

int main(int argc , char *argv[]){
	//initializing

	itemlist i(4,1000,10),j(5,2000,20),k(2,1000,50),i1(1,5000,50),i2(2,4000,40);
	initiallist[1].push_back(i);
	initiallist[1].push_back(k);
	initiallist[1].push_back(j);
	initiallist[2].push_back(i);
	initiallist[2].push_back(k);
	initiallist[2].push_back(j);
	initiallist[3].push_back(i);
	initiallist[3].push_back(i2);
	initiallist[3].push_back(j);
    initiallist[4].push_back(j);
    initiallist[5].push_back(i);
    initiallist[7].push_back(i2);
    initiallist[2].push_back(j);
    initiallist[6].push_back(i1);
    initiallist[6].push_back(i2);
    initiallist[4].push_back(i1);


    int sockfd, ret;
    struct sockaddr_in serverAddr;
    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("[-] Error in connection.\n");
        exit(1);
    }
    printf("[+] Server Socket is created.\n");
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1]));
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-] Error in binding.\n");
        exit(1);
    }
    printf("[+] Bind to port %d\n", atoi(argv[1]));

    if(listen(sockfd, 5) == 0){
        printf("[+] Listening....\n");
    }else{
        printf("[-] Error in binding.\n");
    }
    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        pthread_t t;
        int *pclient = (int *)malloc(sizeof(int));
        *pclient = newSocket;
        pthread_create(&t, NULL, handle_connection, pclient);
    }
    close(newSocket);
    
    return 0;
}
