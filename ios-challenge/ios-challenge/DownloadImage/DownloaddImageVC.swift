//
//  DownloaddImageVC.swift
//  ios-challenge
//
//  Created by Charlie Barber on 6/1/19.
//  Copyright © 2019 Owlet Baby Care Inc. All rights reserved.
//

import UIKit

class DownloadImageVC: UIViewController {
    
    let defaultSession: URLSession = URLSession(configuration: .ephemeral)
    var dataTask: URLSessionDataTask?
    var imgURL: URL = URL(string: "https://i.vimeocdn.com/portrait/25429616_300x300")!
    @IBOutlet weak var imageView: UIImageView!
    
    
    override func viewDidLoad() {
//        imageView.image  = image
        imageView.layer.shadowColor = UIColor.black.cgColor
        imageView.layer.shadowOffset = CGSize(width: 0, height: 1)
        imageView.layer.shadowOpacity = 1
        imageView.layer.shadowRadius = 5.0
        imageView.clipsToBounds = false
        downloadImage()
    }

    func downloadImage () {
        dataTask?.cancel()
        dataTask = defaultSession.dataTask(with: imgURL, completionHandler: { (data, response, error) in
            if let error = error {
                let alert: UIAlertController = UIAlertController(title: "Error", message: "\(error.localizedDescription)", preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "Ok", style: .destructive, handler: { (action) in
                    self.dismiss(animated: true, completion: nil)
                }))
                self.present(alert, animated: true)
            } else if let data = data,
                let response = response as? HTTPURLResponse,
                response.statusCode == 200 {
                DispatchQueue.main.async {
                    self.imageView.image = UIImage(data: data)
                }
            }
        })
        dataTask?.resume()
    }
    
    @IBAction func dismissVC(_ sender: Any) {
        self.dismiss(animated: true)
    }
}
