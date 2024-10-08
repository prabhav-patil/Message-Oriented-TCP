# Message_Oriented_TCP

Documentation


--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Data Structures:

1.	Struct Message: This structure stores a message of max size 5000 bytes in the field ‘void *msg[5000]’ and the size of the particular message in ‘msg_size’.

2.	Struct Table: This structure stores 10 messages in form of an array in the field ‘Message table[10]’. It has 2 integers ‘in’ and ‘out’ that points to the index of the table where data will be written and read respectively. It also has a field ‘counter’ that keeps track of total space occupied in the table.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Functions:

1.	void* func_R(void *param): The receive thread runs this function. First it receives the size of message from the peer and then receives the body of the message. Then it enters the received message into the Received_Message table (it waits if the table is full).

2.	void* func_S(void *param): The send thread runs this functions. It waits till it gets some entry into the Send_Message table. Once it gets message to send, it copies the message and its size to a local variable. Then it sends the size of the message as the header and the sends the entire message.


3.	 int my_socket(int domain, int type, int protocol): This function opens a standard TCP socket with the socket call. It also creates two threads R and S, allocates and initializes space for two tables Send_Message and Received_Message and the 10 messages in each table. The parameters to these are the same as the normal socket( ) call, except that it will take only SOCK_MyTCP as the socket type. 

4.	int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen): It  binds the socket with some address-port using the bind call.

5.	int my_listen(int sockfd, int backlog): It makes a listen call.

6.	int my_accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen): It accepts a connection on the MyTCP socket by making the accept call on the TCP socket. It also sets newsockfd to the socket on which accept call is made

7.	int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen): It opens a connection through the MyTCP socket by making the connect call on the TCP socket. It also sets newsockfd to the socket on which connect call is made

8.	ssize_t my_send(int sockfd, const void *buf, size_t len, int flags): It first checks if the length of message is valid. Then it waits till the number of entries in the Send_Message table is less than 10. As soon as there is space to write in the table, it writes the message and the message size in the table and return the length.

9.	ssize_t my_recv(int sockfd, void *buf, size_t len, int flags): It firsts waits till there is a message in the Received_Message table. Then it removes the first message from the table, puts it into the buffer and returns the length of the message.

10.	int my_close(int fd): It first sleeps for 5 seconds and then cancels the 2 threads. Next it frees all the allocated memory and closes the socket.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Control Flow:

a.	When my_send() call is made, we first ensure that the length of message to send is valid.  Then we wait till the number of entries in the Send_Message table is less than 10. Proper locks are used to avoid race conditions. Next, we put the message and it’s size in the ‘in’ index of the table and increment the counter and ‘in’ index. The function then return the length of the message. As soon as the send thread comes to know that there is an entry in the Send_Message table, it locks the Send_Message table, copies the message and its size in local variables, updates the ‘out’ and ‘counter’ variable of the table and unlocks the lock. Then the thread sends the size of the message as 4 bytes of header followed by the message (max 1000 bytes at time).

b.	When my_recv() call is made, we initially wait till there is atleast 1 entry in Received_Message table. Lock is used when accessing the counter variable. If there is message in the table then we check if the buffer size is sufficient and store the message in buffer by locking the table, if the buffer size is insufficient then we discard the remaining bytes. Then we decrement the counter variable and returns the number of bytes received. In receive thread, it waits until newsockfd is updated by accept/connect call. Then it gets the size of message by receiving 4 bytes and then receives the entire message. It waits if the Received_Message table is full and after that acquires lock and puts the message in table at position in and updates the counter variable.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






