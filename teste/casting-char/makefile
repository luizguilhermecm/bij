default:
	rpcgen -C bijcast.x
	cc server.c bijcast_svc.c bijcast_xdr.c -o server
	cc client.c bijcast_clnt.c bijcast_xdr.c -o client
