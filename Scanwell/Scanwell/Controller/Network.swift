//
//  Network.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/9/21.
//

import Foundation

class Network {
    static let shared: Network = Network()
    
    private init() {}
     
    
    func getData() {
        let task = URLSession.shared.dataTask(with: URL(string: usersURL)!) { data, response, error in
            guard let httpResponse = response as? HTTPURLResponse, httpResponse.statusCode == OK else {
                // Put function to call here that displays the modal
                print("Could not get data")
                return
            }
            let decoder = JSONDecoder()
                
            if let jsonData = try? decoder.decode(Users.self, from: data!) {
            
                let users = jsonData.items
                users.forEach { user in
                    self.get(with: URL(string: user.profileImage)!, handler: { imageData in
                        DispatchQueue.main.async {
                            Database.shared.save(user, with: imageData)
                        }
                    })
                }

            } else {
                print("Could not load JSON data")
            }
           
        }
    
        task.resume()
    }
    
    func get(with url: URL, handler: @escaping (_ imageData: String) -> Void) -> Void {
       
        
        let task = URLSession.shared.dataTask(with: url) { data, response, error in
            guard let imageData = data?.base64EncodedString() else { return }
            handler(imageData)
        }
        
        task.resume()
    }
}
