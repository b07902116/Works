README:
Because the accept function and read function in handle_read may block, so I use select function to implement I/O multiplexing. Thus the server can deal with multiple clients.
The struct info is to keep every client's information, including current step, which account the client want to access, and the position in account_list.
To implement file protection on single write_server, I use an array record_lock to keep whether an account is being modified by other clients.
To implement file protection on multi-server, I use fcntl to add advisory locks to protect the specific data of the file.