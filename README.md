Networking socket programming project. 

------http_client.cc------ 
Basic networking client. Takes a the server path, port number, and file name then parses the information into an HTTP request. Sends request to server, then writes back the server response to standard out. 

------http_server1.cc------ 
Basic networking server. Sits on a single port and listens to connections from clients. Once a connection request is made, the server will parse the HTTP request and read the file which the user requested, then send the contents of the file (and the appropriate HTTP response header) to the client. If an invalid request is made, or invalid file name given, a failure response is sent back to the client. 

------http_server2.cc------ 
Select based server which behaves similarly to http_server 1.cc. Avoids waiting for a connection to be established, and waiting on the read after a connection has been established in order to able to serve multiple clients simultaneously.
