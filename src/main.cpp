#include "mbed.h"
#include "WIZnetInterface.h"
#include "math.h"

#define UDP_SOCKET 12345
char IP_Addr[30] = "172.16.10.";
const char *IP_Subnet = "255.255.0.0";
const char *IP_Gateway = "172.16.0.1";

uint8_t mac[] = {0x00, 0x08, 0xDC, 0x1C, 0xAA, 0xCA};

SPI spi(PA_7, PA_6, PA_5);
WIZnetInterface ethernet(&spi, PA_1, PA_0);

BusIn addr(PB_10, PB_2, PB_1, PB_0, PC_5, PC_4);
Serial pc(USBTX, USBRX);
BusOut coil(PB_12, PB_13, PB_14, PB_15, PC_6, PC_7, PC_8, PC_9, PA_8, PA_9, PA_10, PA_11, PA_12, PA_15, PC_10, PC_11);

//OneWire DS18B20 = PC_3
//Cureent ACS711 = PC_12
UDPSocket client;
Endpoint recv_addr;

void f_ethernet_init();
void set_ip();
uint8_t action(char *payload);
void ack_relay(char *payload, uint8_t info_action);

int ret = -1;
int ret_set = -1;
int r_result = 0;
uint8_t ret_action;

char buff[256];

int main()
{

    pc.baud(115200);
    addr.mode(PullNone);
    coil = (0);
    set_ip();

    f_ethernet_init();
    while (ret == -1)
    {
        ret = client.bind(UDP_SOCKET);
        wait(3);
    }
    while (ret_set == -1)
    {
        ret_set = recv_addr.set_address(IP_Gateway, UDP_SOCKET);
        wait(3);
    }

    while (1)
    {
        r_result = client.receiveFrom(recv_addr, buff, 256);
        if (r_result > 0)
        {
            pc.printf("[%d]recv[%s]:[%d] : %s \n", r_result, recv_addr.get_address(), recv_addr.get_port(), buff);
            ret_action = action(buff);
            ack_relay(buff, ret_action);
        }
    }
    wait(1);
}

void f_ethernet_init()
{

    // mbed_mac_address((char *)mac);
    pc.printf("\t Initialize Ethernet Service...\n\r");
    ethernet.reset();

    wait(3);
    int ret = ethernet.init(mac, IP_Addr, IP_Subnet, IP_Gateway);
    if (!ret)
    {
        pc.printf("Inicialize, MAC= %s\n\r", ethernet.getMACAddress());
    }
    else
    {
        pc.printf("Failed Communication... Reset it...\n\r");
        wait(3);
        __NVIC_SystemReset();
    }
    pc.printf("Connecting.");
    wait(2);
    pc.printf(".");
    wait(2);
    pc.printf(".\n\r");
    wait(2);
    ret = ethernet.connect();
    if (ret == 0)
    {
        pc.printf("Conection Establish!\n\n\r");
        wait(1);
        pc.printf("IP=%s\n\rMASK=%s\n\rGW=%s\n\r", ethernet.getIPAddress(), ethernet.getNetworkMask(), ethernet.getGateway());
    }
    else
    {
        pc.printf("Failed Communication... Reset it 2...\n\r");
        wait(1);
        __NVIC_SystemReset();
    }
}

void set_ip()
{
    int value = addr.read();
    if (value == 0 || value == 1)
        value = 2;
    sprintf(IP_Addr, "172.16.10.%d", value);
    mac[5] = value;
}

uint8_t action(char *payload)
{
    //pc.printf("Payload is : %s \n", payload);
    // scanf("%s",payload);
    int l = strlen(payload) - 1;
    //pc.printf("Length == %d \n", l);
    int relay = (payload[2] - 48) * 10 + (payload[3] - 48);
    int etat = payload[5] - 48;
    //pc.printf("Numero Relay === %d \n", relay);
    //pc.printf("Etat Relay = %d", etat);
    //pc.printf("_____________________________ \r\n");
    if (relay != 0 && relay != 220 && relay != 190)
    {
        if (etat == 0)
        {
            coil = (0);
        }
        else if (etat == 1)
        {
            //pc.printf("Coil Value = %d",(pow(2,(relay -1))) );
            coil = (pow(2, (relay - 1)));
        }
        return 0;
    }
    else if (relay == 0 || relay == 220 || relay == 190)
    {
        if (payload[2] == 48) //Status
        {
            return 1;
        }
        else if (payload[2] == 70) //Temp
        {
            return 2;
        }
        else if (payload[2] == 67) //Curent
        {
            return 3;
        }
    }
}

// 0 -> relay ON/OFF ; 1 -> Status ; 2 -> Temp ; 3 -> Current
void ack_relay(char *payload, uint8_t info_action)
{
    if (info_action == 0)
    {
        char ack[50] = "1;";
        strcat(ack, payload);
        int l = strlen(ack);
        client.sendTo(recv_addr, ack, l);
    }
    else if (info_action == 1)
    {
        int statusRelay = coil.read();
        char ack[50];
        sprintf(ack, "%d", statusRelay);
        int l = strlen(ack);
        client.sendTo(recv_addr, ack, l);
    }
    else if (info_action == 2)
    {
        char ack[50] = "8.92";
        int l = strlen(ack);
        client.sendTo(recv_addr, ack, l);
    }
    else if (info_action == 3)
    {
        char ack[50] = "0.32";
        int l = strlen(ack);
        client.sendTo(recv_addr, ack, l);
    }
}