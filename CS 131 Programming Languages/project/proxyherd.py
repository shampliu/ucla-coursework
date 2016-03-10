from twisted.internet import protocol, reactor
from twisted.protocols.basic import LineReceiver
from twisted.python import log
from twisted.web.client import getPage
from twisted.application import service, internet

import time
import datetime
import logging
import re
import sys
import json
import urllib2

API_KEY = "AIzaSyBAsb6hRrPOZkDTMhgd7BxhaJzNn_wVtck"
BASE_URL = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?"

SERVERS = {
    "Alford" : {
        "port" : 11640,
        "neighbors" : ["Welsh", "Parker"]
    },
    "Bolden" : {
        "port" : 11641,
        "neighbors" : ["Parker", "Welsh"]
    },
    "Hamilton" : {
        "port" : 11642,
        "neighbors" : ["Parker"]
    },
    "Parker" : { 
        "port" : 11643,
        "neighbors" : ["Alford", "Bolden", "Hamilton"]
    },
    "Welsh" : {
        "port" : 11644,
        "neighbors" : ["Alford", "Bolden"]
    }
}


class ProxyHerdServerProtocol(LineReceiver):
    def __init__(self,factory):
        self.factory = factory 

    def connectionMade(self):
        self.factory.connections += 1
        logging.info("Connection made! {0} active connections.".format(self.factory.connections))

    def lineReceived(self, line):
        logging.info("Received line: {0}".format(line))
        params = line.split(" ")
        if (params[0] == "IAMAT"):
            self.handle_IAMAT(line)
        elif (params[0] == "WHATSAT"):
            self.handle_WHATSAT(line)
        elif (params[0] == "AT"):
            self.handle_AT(line)
        elif (params[0] == "PRINTCLIENTS"):
            self.printClients()
        else:
            logging.error("? Invalid line: {0}".format(line))
            self.transport.write("{0}\n".format(line))
        return

    # IAMAT kiwi.cs.ucla.edu +34.068930-118.445127 1400794645.392014450
    def handle_IAMAT(self, line):
        params = line.split(" ")
        if len(params) != 4:
            logging.error("? Invalid IAMAT command: {0}".format(line))
            self.transport.write("{0}\n".format(line))
            return

        client_id = params[1]
        loc = params[2]
        client_time = params[3]

        delta = time.time() - float(client_time)

        if (delta >= 0):
            res = "AT {0} +{1} {2}".format(self.factory.server_id, delta, client_id + ' ' + loc + ' ' + client_time)
        else:
            res = "AT {0} {1} {2}".format(self.factory.server_id, delta, client_id + ' ' + loc + ' ' + client_time)


        self.factory.clients[client_id] = { 
            "message" : res, 
            "time" : client_time
        }

        self.transport.write("{0}\n".format(res))
        self.flood(res)
        return

    # WHATSAT kiwi.cs.ucla.edu 10 5
    def handle_WHATSAT(self, line):
        params = line.split(" ")
        if len(params) != 4:
            logging.error("? Invalid number of args in WHATSAT command: {0}".format(line))
            self.transport.write("? {0}\n".format(line))
            return

        client_id = params[1]
        radius = int(params[2])
        limit = int(params[3])

        if (radius > 50 or radius < 0):
            logging.error("? Invalid radius entered: {0}".format(radius))

        if (limit > 20 or limit < 0):
            logging.error("? Invalid limit entered: {0}".format(limit))


        # AT Alford +0.563873386 kiwi.cs.ucla.edu +34.068930-118.445127 1400794699.108893381
        message = self.factory.clients[client_id]["message"].split()

        if (len(message) != 6):
            logging.error("? No message found at client: {0}".format(message[3]))

        loc = message[4]
        loc = re.sub(r'[-]', ' -', loc)
        loc = re.sub(r'[ ]', ',', loc)

        req = BASE_URL + "location=" + loc + "&radius=" + str(radius) + "&key=" + API_KEY

 
        res = urllib2.urlopen(req)

        data = json.load(res)
        results = data["results"]
        data["results"] = results[0:int(limit)]
        message = self.factory.clients[client_id]["message"]
        full_message = "{0}\n{1}\n\n".format(message, json.dumps(data, indent=4))

        self.transport.write(full_message)

    # AT Alford +56496987.7067 kiwi.cs.ucla.edu +34.068930-118.445127 1400794645.392014450
    def handle_AT(self, line):
        params = line.split(" ")

        if len(params) != 6:
            logging.error("? Invalid number of args in AT command: {0}".format(line))
            self.transport.write("? {0}\n".format(line))
            return

        server_id = params[1]
        time_diff = params[2]
        client_id = params[3]
        loc = params[4]
        client_time = params[5]
        
        if (client_id in self.factory.clients) and (client_time <= self.factory.clients[client_id]["time"]):
            logging.info("Already updated from {0}".format(server_id))
            return

        if client_id in self.factory.clients:
            logging.info("(AT) Location update from existing client: {0}".format(client_id))
        else:
            logging.info("(AT) Location update from new client: {0}".format(client_id))

        self.factory.clients[client_id] = { 
            "message"   :   line, 
            "time"      :   client_time 
        }

        logging.info("Added {0} : {1}".format(client_id, self.factory.clients[client_id]["message"]))
        self.flood(self.factory.clients[client_id]["message"])

        return

    def printClients(self):
        for i in self.factory.clients:
            print i + " : " + self.factory.clients[i]["message"]

    def connectionLost(self, reason):
        self.factory.connections -= 1
        logging.info("Connection lost. Active connections: {0}".format(
            self.factory.connections))

    def flood(self, message):
        for server in SERVERS[self.factory.server_id]["neighbors"]:
            reactor.connectTCP('localhost', SERVERS[server]["port"], ProxyHerdClient(message))


class ProxyHerdServer(protocol.ServerFactory):
    def __init__(self, server_id): 
        self.server_id = server_id
        self.port = SERVERS[server_id]["port"]
        self.clients = {}
        self.connections = 0

    def buildProtocol(self, addr):
        return ProxyHerdServerProtocol(self)

    def stopFactory(self):
        logging.info("{0} has shut down".format(self.server_id))

class ProxyHerdClientProtocol(LineReceiver):
    def __init__(self, factory):
        self.factory = factory


    def connectionMade(self):
        self.sendLine(self.factory.message)
        self.transport.loseConnection()


class ProxyHerdClient(protocol.ClientFactory):
    def __init__(self, message):
        self.message = message

    def buildProtocol(self, addr):
        return ProxyHerdClientProtocol(self)

def main():

    for server in SERVERS:
        factory = ProxyHerdServer(server)
        reactor.listenTCP(SERVERS[server]["port"], factory)
    reactor.run()

if __name__ == '__main__':
    main()
