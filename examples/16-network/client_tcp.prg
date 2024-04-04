import "mod_misc";
import "mod_net";

#define BUFFER_SIZE 1024

process int main()
begin
    int *client = net_open(NET_MODE_CLIENT, NET_PROTO_TCP, "127.0.0.1", 12345);
    if (!client)
        say("Error opening client socket");
        return 1;
    end

    char msg[BUFFER_SIZE] = "ping";

    net_send(client, &msg, strlen(msg)+1);

    int * clientList = list_create();
    list_insertItem( clientList, client );

    if ( net_wait(clientList, -1, NULL) <= 0 )
        say("Error receiving response from the server");
        return 1;
    end

    char buffer[BUFFER_SIZE];
    int bytes_received = net_recv(client, &buffer, sizeof(buffer));
    if (bytes_received > 0)
        say("Server response: " + buffer);
    end

    net_close(client);
    return 0;
end
