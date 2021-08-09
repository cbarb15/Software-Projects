//
//  UserDetails.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/8/21.
//

import UIKit

class UserDetails: UIViewController {
    
    @IBOutlet var avatar: UIImageView!
    @IBOutlet var displayName: UILabel!
    @IBOutlet var reputation: UILabel!
    @IBOutlet var gold: UILabel!
    @IBOutlet var silver: UILabel!
    @IBOutlet var bronze: UILabel!
    
    var user: UserRecord!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let imageData = Data(base64Encoded: user.avatar!)
        avatar.image = UIImage(data: imageData!)
        displayName.text = user.displayName
        reputation.text = String(user.reputation)
        gold.text = String(user.gold)
        silver.text = String(user.silver)
        bronze.text = String(user.bronze)
    }
    
    @IBAction func imageTapped(_ sender: UITapGestureRecognizer) {
        guard let link = user.link else {
            return
        }
        if let url = URL(string: link) {
            UIApplication.shared.open(url)
        }
    }
}
