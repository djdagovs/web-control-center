#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "60:FA:CD:41:0B:BC";

    // allocate a socket
    printf("<btconnect>allocate a socket\r\n");
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    printf("<btconnect>allocate a socket ok\r\n");
    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    printf("<btconnect>connect\r\n");
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("<btconnect>connect ok\r\n");
    // send a message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");
    //while(1)
    //	{printf("<btconnect>wait...\r\n");}
    close(s);
    return 0;
}
