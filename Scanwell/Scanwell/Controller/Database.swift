//
//  Database.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/9/21.
//

import CoreData
import UIKit

import Foundation

class Database {
    static let shared: Database = Database()
    private let context: NSManagedObjectContext
    
    private init() {
        context = (UIApplication.shared.delegate as! AppDelegate).persistentContainer.viewContext
    }
    
    func userRecordExists(with user: User) -> Bool {
        var recordExists: Bool = false
        let fetchRequest = NSFetchRequest<NSFetchRequestResult>(entityName: "UserRecord")
        fetchRequest.predicate = NSPredicate(format: "displayName = %@", user.displayName)
        do {
            let result = try context.fetch(fetchRequest)
            recordExists = (result.count == 1)
        } catch {
            fatalError("Error searching for record in core data")
        }
        return recordExists
    }
    
    func save(_ user: User, with imageData: String) {
        
        if (!userRecordExists(with: user)) {
            let newUserRecord = UserRecord(context: self.context)
            newUserRecord.avatar = imageData
            newUserRecord.displayName = user.displayName
            newUserRecord.reputation = Int64(user.reputation)
            newUserRecord.link = user.link
            newUserRecord.gold = Int64(user.badgeCounts.gold)
            newUserRecord.silver = Int64(user.badgeCounts.silver)
            newUserRecord.bronze = Int64(user.badgeCounts.bronze)
            
            do {
                try context.save()
            } catch {
                fatalError("Unable to save user")
            }
        }
    }
    
    func getData() -> [UserRecord] {
        do {
            let users = try context.fetch(UserRecord.fetchRequest()) as [UserRecord]
            return users
        } catch {
            fatalError("Could not retrieve records")
        }
    }
}
