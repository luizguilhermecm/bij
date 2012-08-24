FILE = bijstruct
SERVER = server
CLIENT = client
FLAGS = -lnsl
CC = cc
RPCGEN = rpcgen

default:
	$(RPCGEN) -C $(FILE).x
	$(CC) $(SERVER).c $(FILE)_svc.c $(FILE)_xdr.c -o $(SERVER) 
	$(CC) $(CLIENT).c $(FILE)_clnt.c $(FILE)_xdr.c -o $(CLIENT)

clean:
	$(RM) $(SERVER) $(CLIENT) $(FILE)_* $(FILE).h

only:
	$(RM) $(FILE)_* $(FILE).h
