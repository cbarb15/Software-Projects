//
//  CircleView.swift
//  Scanwell
//
//  Created by Charlie Barber on 8/8/21.
//

import UIKit


@IBDesignable class CircleView: UIView {
  
    override func layoutSubviews() {
        super.layoutSubviews()
        
        layer.cornerRadius = 0.5 * bounds.size.width
    }
}
