import os
import uhashlib
import ubinascii

def mysha1(path):
	try:
		sh = uhashlib.sha1()
		f=open(path)
		while True:
		    data = f.read(1)
		    if not data:
		        break
		    sh.update(data)
		f.close()
		return ubinascii.hexlify(sh.digest()).upper()
	except:
		print("SHA1 Error")
		return 0
