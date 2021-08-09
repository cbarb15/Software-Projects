//
//  ViewController.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/7/21.
//

import UIKit

class ViewController: UITableViewController {

    var users: [UserRecord] = [UserRecord]()
    var path: IndexPath = IndexPath()

    override func viewDidLoad() {
        super.viewDidLoad()
        
        navigationItem.title = tableTitle
        Network.shared.getData()
    }
        
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        users = Database.shared.getData()
        tableView.reloadData()
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return users.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "UserCell", for: indexPath) as! UserCell
        let user = users[indexPath.row]
        cell.user = user
        
        return cell
    }
    
    override func tableView(_ tableView: UITableView, willSelectRowAt indexPath: IndexPath) -> IndexPath? {
        path = indexPath
        return indexPath
    }
    
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if let selectedIndexPath = tableView.indexPathForSelectedRow {
            tableView.deselectRow(at: selectedIndexPath, animated: true)
            performSegue(withIdentifier: "showDetails", sender: self)
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "showDetails" {
            let details = segue.destination as! UserDetails
            details.user = users[path.row]
        }
    }
}

