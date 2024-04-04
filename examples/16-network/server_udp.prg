import "mod_misc";
import "mod_net";

#define BUFFER_SIZE 1024

process int main()
begin
    int *server = net_open(NET_MODE_SERVER, NET_PROTO_UDP, "", 12345);
    if (!server)
        say("Error opening server socket");
        return 1;
    end

    char buffer[BUFFER_SIZE];

    int *socks = list_create(-1);

    list_insertItem(socks, server);

    while (1)
        int num_events = net_wait(socks, -1, NULL);

        if ( num_events <= 0 )
            say("invalid event");
            continue;
        end

        int bytes_received = net_recv(server, &buffer, sizeof(buffer));
        if (bytes_received > 0)
            say("Message from client: " + buffer);
            // Respond to the client
            char response[5] = "Pong";
            net_send(server, &response, strlen(response) + 1);
        end
    end

    net_close(server);
    return 0;
end
