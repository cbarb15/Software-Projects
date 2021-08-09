//
//  UserCell.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/8/21.
//

import UIKit

class UserCell: UITableViewCell {
    
    @IBOutlet var avatar: UIImageView!
    @IBOutlet var displayName: UILabel!
    @IBOutlet var reputation: UILabel!
    @IBOutlet var gold: UILabel!
    @IBOutlet var silver: UILabel!
    @IBOutlet var bronze: UILabel!
    
    var user: UserRecord! {
        didSet {
            getCellData()
        }
    }
   
    func getCellData() {
        let imageData = Data(base64Encoded: user.avatar!)
        self.avatar.image = UIImage(data: imageData!)
        self.displayName.text = user.displayName
        self.reputation.text = String(user.reputation)
        self.gold.text = String(user.gold)
        self.silver.text = String(user.silver)
        self.bronze.text = String(user.bronze)
    }
}
