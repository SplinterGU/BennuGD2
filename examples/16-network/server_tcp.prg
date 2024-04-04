import "mod_misc";
import "mod_net";

#define PORT 12345
#define BUFFER_SIZE 1024

process int main()
begin
    int *server = net_open(NET_MODE_SERVER, NET_PROTO_TCP, "", PORT);
    if (!server)
        say("Error opening the server");
        exit(-1);
    end

    say("TCP server waiting for connections on port " + PORT + "...");

    char buffer[BUFFER_SIZE];

    int *socks = list_create();
    int *events = list_create();

    list_insertItem(socks, server);

    while (1)
        int num_events = net_wait(socks, -1, events);

        if (num_events == NET_ERROR)
            say("Error in net_wait");
            break;
        end

        if (num_events == NET_TIMEOUT)
            say("Timeout expired");
            continue;
        end

        int *evt;
        int * ctx = NULL;
        while( ( evt = list_walk( events, &ctx ) ) )
            if ( net_is_new_connection( evt ) )
                say("accept new client from " + net_getremoteaddr( evt ));
                list_insertItem( socks, evt );
            else
                // If the client sends a message, we print it
                int recv_bytes = net_recv(evt, &buffer, BUFFER_SIZE);
                if (recv_bytes <= NET_DISCONNECTED)
                    if (recv_bytes == NET_DISCONNECTED)
                        say("The client closed the connection");
                    else
                        say("Error receiving data from client");
                    end
                    net_close( evt );
                    list_removeItem( socks, evt );
                    continue;
                end

                say("Message from client: " + buffer);

                // Respond to the client
                char response[5] = "Pong";
                int sent_bytes = net_send(evt, &response, strlen(response) + 1);
                if (sent_bytes == -1)
                    say("Error sending data to client");
                    break;
                end
            end
        end
        say("exit walk");
    end

    net_close(server);
    return 0;
end
