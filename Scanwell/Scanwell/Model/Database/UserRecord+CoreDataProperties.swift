//
//  UserRecord+CoreDataProperties.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/9/21.
//
//

import Foundation
import CoreData


extension UserRecord {

    @nonobjc public class func fetchRequest() -> NSFetchRequest<UserRecord> {
        return NSFetchRequest<UserRecord>(entityName: "UserRecord")
    }

    @NSManaged public var avatar: String?
    @NSManaged public var bronze: Int64
    @NSManaged public var displayName: String?
    @NSManaged public var gold: Int64
    @NSManaged public var link: String?
    @NSManaged public var reputation: Int64
    @NSManaged public var silver: Int64

}

extension UserRecord : Identifiable {

}
