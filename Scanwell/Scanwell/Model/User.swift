//
//  User.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/7/21.
//

import Foundation

struct User: Codable {
    var profileImage: String
    var displayName: String
    var reputation: Int
    var link: String
    var badgeCounts: BadgeCounts
    
    enum CodingKeys: String, CodingKey {
        case profileImage = "profile_image"
        case displayName = "display_name"
        case badgeCounts = "badge_counts"

        case reputation
        case link
    }
}
