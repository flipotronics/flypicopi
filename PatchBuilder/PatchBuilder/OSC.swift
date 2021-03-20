//
//  OSC.swift
//  PatchBuilder
//
//  Created by Mathias Dietrich on 09.03.21.
//

import Foundation
import OSCKit
import Network
import NIO
import Socket

@available(macOS 10.14, *)
class ClientConnection {
    let  nwConnection: NWConnection
    let queue = DispatchQueue(label: "Client connection Q")

    init(nwConnection: NWConnection) {
        self.nwConnection = nwConnection
    }

    var didStopCallback: ((Error?) -> Void)? = nil

    func start() {
        print("connection will start")
        nwConnection.stateUpdateHandler = stateDidChange(to:)
        setupReceive()
        nwConnection.start(queue: queue)
    }

    private func stateDidChange(to state: NWConnection.State) {
        switch state {
        case .waiting(let error):
            connectionDidFail(error: error)
        case .ready:
            print("Client connection ready")
        case .failed(let error):
            connectionDidFail(error: error)
        default:
            break
        }
    }

    private func setupReceive() {
        nwConnection.receive(minimumIncompleteLength: 1, maximumLength: 65536) { (data, _, isComplete, error) in
            if let data = data, !data.isEmpty {
                let message = String(data: data, encoding: .ascii)
                print("connection did receive, data: \(data as NSData) string: \(message ?? "-" )")
            }
            if isComplete {
                self.connectionDidEnd()
                print("connection did end")
            } else if let error = error {
                self.connectionDidFail(error: error)
                print("connection error " + error.debugDescription)
            } else {
                self.setupReceive()
            }
        }
    }

    func send(data: Data) {
        nwConnection.send(content: data, completion: .contentProcessed( { error in
            if let error = error {
                self.connectionDidFail(error: error)
                return
            }
                print("connection did send, data: \(data as NSData)")
        }))
    }

    func stop() {
        print("connection will stop")
        stop(error: nil)
    }

    private func connectionDidFail(error: Error) {
        print("connection did fail, error: \(error)")
        self.stop(error: error)
    }

    private func connectionDidEnd() {
        print("connection did end")
        self.stop(error: nil)
    }

    private func stop(error: Error?) {
        self.nwConnection.stateUpdateHandler = nil
        self.nwConnection.cancel()
        if let didStopCallback = self.didStopCallback {
            self.didStopCallback = nil
            didStopCallback(error)
        }
    }
}

class OSC: OSCClientDelegate & OSCPacketDestination {
    var connection: ClientConnection!
    var host: NWEndpoint.Host!
    var port: NWEndpoint.Port!
    var client : OSCClient!

   // var clientUdp :UDPClient!
    
    var socket : Socket!
    
    func setup(host:String, port :Int){
 
        do{
       socket =  try Socket.create(family: .inet, type: .datagram, proto: .udp)
            socket.readBufferSize = 32768
            
            try socket.listen(on: 9000)
            try socket.acceptConnection()
            var readData = Data(capacity: 32768)
     
            let bytesRead = try socket.read(into: &readData)
            print(bytesRead)
            print(bytesRead)
            
        }catch let myError {
            print("UDP Error: \(myError)")
        }
       // clientUdp = UDPClient(port:9000)
       // clientUdp.startListening()
        
        
        
        client = OSCClient()
        client.interface = "en0"
       // client.interface = "llw0";
        client.host = host
        client.port = UInt16(port+1)
        client.useTCP = false
        client.delegate = self
        do {
        try client.connect();
           }
        catch let error {
               print("Cannot open socket to \(host):\(port): \(error)")
        }

    }
    
    func take(message: OSCMessage) {
        print("take")
    }
    
    func take(bundle: OSCBundle) {
        print("bundle")
    }
    
    func clientDidConnect(client: OSCClient) {
        print("Client did connect")
    }

    func clientDidDisconnect(client: OSCClient) {
        print("Client did disconnect")
    }
    

    func stop() {
         connection.stop()
     }

     func send(data: Data) {
         connection.send(data: data)
     }

     func didStopCallback(error: Error?) {
         if error == nil {
             exit(EXIT_SUCCESS)
         } else {
             exit(EXIT_FAILURE)
         }
     }
    
    func send(prefix: String, cc: Int,  value : Int){
        let message = OSCMessage(with: prefix + "/" + String(cc), arguments: [Float(value)])
        client.send(packet: message)
        let s = message.packetData().debugDescription
        print ("osc send " + s )
    }
}

