
import Foundation
import Network
import SystemConfiguration

protocol UDPListener {
  func handleResponse(_ client: UDPClient, data: Data)
}

class UDPClient{
  var listener: NWListener?
  var port: NWEndpoint.Port
  var delegate: UDPListener?
   
  init(port: UInt16){
    self.port = NWEndpoint.Port(integerLiteral: port)
  }
   
  func startListening(){
    do{
      let params = NWParameters.init(dtls: nil)
      params.allowLocalEndpointReuse = true
      params.allowFastOpen = true
      
        // get all interfaces
        let interfaces = SCNetworkInterfaceCopyAll() as? Array<SCNetworkInterface> ?? []
        // convert to NWInterface
        let nwInterfaces = interfaces.compactMap { interface -> NWInterface? in
            guard let bsdName = SCNetworkInterfaceGetBSDName(interface) else { return nil }
            return IPv4Address("192.168.0.73%\(bsdName)")?.interface
        }

      print(nwInterfaces)
      params.requiredInterface = nwInterfaces[0];

      self.listener = try NWListener(using: params, on: port)
      listener?.stateUpdateHandler = { newState in
        switch (newState) {
          case .ready:
            print("State: Ready")
            return
          case .waiting(let error):
            print(error)
          case .setup:
            print("State: Setup")
          case .cancelled:
            print("State: Cancelled")
          case .failed(let error):
            print(error)
          default:
            print("ERROR! State not defined!\n")
        }
      }
      listener?.newConnectionHandler = { newConnection in
        newConnection.stateUpdateHandler = { newState in
          switch (newState) {
            case .ready:
              print("State: Ready")
              self.receive(on: newConnection)
              return
            case .setup:
              print("State: Setup")
            case .cancelled:
              print("State: Cancelled")
            case .preparing:
              print("State: Preparing")
            default:
              print("ERROR! State not defined!\n")
          }
        }
      }
    }catch{
      print("failed to listen")
      abort()
    }
    self.listener?.start(queue: .main)
  }
   
  func receive(on connection: NWConnection) {
    connection.receiveMessage { data, context, isComplete, error in
      guard let data = data else {
        print("Error: Received nil Data")
        return
      }
      guard self.delegate != nil else {
        print("Error: UDPClient response handler is nil")
        return
      }
      self.delegate?.handleResponse(self, data: data)
    }
  }
}
