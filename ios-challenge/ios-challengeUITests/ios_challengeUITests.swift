//
//  ios_challengeUITests.swift
//  ios-challengeUITests
//
//  Created by Lorraine Alexander on 4/15/19.
//  Copyright © 2019 Owlet Baby Care Inc. All rights reserved.
//

import XCTest

class ios_challengeUITests: XCTestCase {
    var app: XCUIApplication!
    
    override func setUp() {
        continueAfterFailure = false
        app = XCUIApplication()
        app.launch()
    }
    
    func testIncrementButton() {
        app.buttons["+"].tap()
        let countLabelValue = app.staticTexts["1"]
        XCTAssertTrue(countLabelValue.exists)
    }
    
    func testDecrementButton() {
        app.buttons["-"].tap()
        let countLabelValue = app.staticTexts["-1"]
        XCTAssertTrue(countLabelValue.exists)
    }
    
    func testIncrementButtonTapped5Times() {
        for _ in 1...5 {
            app.buttons["+"].tap()
        }
        let countLabelValue = app.staticTexts["5"]
        XCTAssertTrue(countLabelValue.exists)
    }
    
    func testDecrementButtonTapped5Times() {
        for _ in 1...5 {
            app.buttons["-"].tap()
        }
        let countLabelValue = app.staticTexts["-5"]
        XCTAssertTrue(countLabelValue.exists)
    }
    
    func testMixButtonTapped() {
        for _ in 1...5 {
            app.buttons["-"].tap()
        }
        for _ in 1...10 {
            app.buttons["+"].tap()
        }
        for _ in 1...3 {
            app.buttons["-"].tap()
        }
        let countLabelValue = app.staticTexts["2"]
        XCTAssertTrue(countLabelValue.exists)
    }
}

