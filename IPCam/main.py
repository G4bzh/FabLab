import os
import network


if __name__ == '__main__':
	wlan = network.WLAN(network.STA_IF) # create station interface
	wlan.active(True)       # activate the interface
	print(wlan.scan())
