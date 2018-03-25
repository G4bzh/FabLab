import network
import os
try:
    import usocket as socket
except:
    import socket


CONTENT = b"""\
HTTP/1.0 200 OK

"""

NOTFOUND = b"""\
HTTP/1.0 404 Not Found

"""

NOTIMPLEMENTED = b"""\
HTTP/1.0 500 Not Implemented

"""

def handleFile(stream, path):
    
    # Handle favicon
    if ( path.upper() == b"/FAVICON.ICO"):
        return

    try:
        f=open(path[1:])
        stream.write(CONTENT)
        while True:
            data = f.read(1)
            if not data:
                break
            stream.write(data)
        f.close()
    except:
        stream.write(NOTFOUND)



def main(micropython_optimize=False):

    # Create and go to 'www' dir
    os.chdir("/")
    try:
        os.mkdir("www")
    except:
        pass
    os.chdir("www")

    s = socket.socket()

    # Binding to all interfaces - server will be accessible to other hosts!
    ai = socket.getaddrinfo("0.0.0.0", 8080)
    print("Bind address info:", ai)
    addr = ai[0][-1]

    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(5)

    wlan = network.WLAN(network.STA_IF)
    print("Listening, connect your browser to http://%s:8080/" % wlan.ifconfig()[0] )

    while True:
        res = s.accept()
        client_sock = res[0]
        client_addr = res[1]
        # print("Client address:", client_addr)
        # print("Client socket:", client_sock)

        if not micropython_optimize:
            # To read line-oriented protocol (like HTTP) from a socket (and
            # avoid short read problem), it must be wrapped in a stream (aka
            # file-like) object. That's how you do it in CPython:
            client_stream = client_sock.makefile("rwb")
        else:
            # .. but MicroPython socket objects support stream interface
            # directly, so calling .makefile() method is not required. If
            # you develop application which will run only on MicroPython,
            # especially on a resource-constrained embedded device, you
            # may take this shortcut to save resources.
            client_stream = client_sock

        # print("Request:")
        # Get Method and Path
        req = client_stream.readline()
        method, path, _ = req.split()

        # Flush headers
        while True:
            h = client_stream.readline()
            if h == b"" or h == b"\r\n":
                break

        if method.upper() != b"GET" :
            client_stream.write(NOTIMPLEMENTED)
        else:
            handleFile(client_stream,path)

        client_stream.close()
        if not micropython_optimize:
            client_sock.close()

        print()


main()