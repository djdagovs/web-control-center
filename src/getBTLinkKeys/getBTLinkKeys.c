#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


int main(int argc, char **argv)
{
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
	//printf("<device_list></device_list>");
        exit(1);
    }

    struct hci_dev_info di;
    hci_devinfo(dev_id, &di);
    //char addr[18];
    ba2str(&di.bdaddr, addr);

    char strLinkData[1024] = "";
    char strFilePath[255] = "";
    sprintf(strFilePath,"/var/lib/bluetooth/%s/linkkeys",addr);
    //printf("%s",strFilePath);

    FILE *fPtr;
     
    fPtr = fopen(strFilePath, "rb");
    if (!fPtr) {
        printf("open %s fail",strFilePath);
        exit(1);
    }
     
    while (!feof(fPtr)) {
        fread(strLinkData, sizeof(char), 1024, fPtr);
        printf("%s\n", strLinkData);
    }
     
    fclose(fPtr);

    
    close( sock );
    return 0;
}
