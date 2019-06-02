//
//  ViewController.swift
//  ios-challenge
//
//  Created by Lorraine Alexander on 4/15/19.
//  Copyright © 2019 Owlet Baby Care Inc. All rights reserved.
//

import UIKit

class ViewController: UIViewController, StoreSubscriber {
    
    @IBOutlet weak var countLabel: UILabel!
    @IBOutlet weak var downloadButton: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        store.subscribe(self)
        downloadButton.backgroundColor = UIColor(red: 0, green: 122/255, blue: 255/255, alpha: 1)
        downloadButton.layer.cornerRadius = 15
        downloadButton.layer.borderWidth = 1
        downloadButton.layer.shadowColor = UIColor.black.cgColor
        downloadButton.layer.shadowOffset = CGSize(width: 0, height: 1)
        downloadButton.layer.shadowOpacity = 1
        downloadButton.layer.shadowRadius = 2
        downloadButton.clipsToBounds = false
    }
    
    func newState(state: State) {
        let count = (store.state as? AppState)?.counter ?? 0
        countLabel.text = String(count)
    }
    
    @IBAction func didTapIncreaseButton(_ sender: Any) {
        store.dispatch(action: IncreaseAction(increaseBy: 1))
    }
    
    @IBAction func didTapDecreaseButton(_ sender: Any) {
        store.dispatch(action: DecreaseAction(decreaseBy: 1))
    }
}

