//
//  ios_challengeTests.swift
//  ios-challengeTests
//
//  Created by Lorraine Alexander on 4/15/19.
//  Copyright © 2019 Owlet Baby Care Inc. All rights reserved.
//

import XCTest
@testable import ios_challenge

class ios_challengeTests: XCTestCase {
    var httpRequestTest: DownloadImageVC!

    override func setUp() {
        super.setUp()
        httpRequestTest = DownloadImageVC()
    }

    override func tearDown() {
        httpRequestTest = nil
        super.tearDown()
    }

    func testIncrementValue() {
        let state: State = appReducer(IncreaseAction(increaseBy: 1), nil)
        
        XCTAssertEqual((state as! AppState).counter, 1)
        
    }
    
    func testDecrementValue() {
        let state: State = appReducer(DecreaseAction(decreaseBy: 1), nil)
        
        XCTAssertEqual((state as! AppState).counter, -1)
        
    }
    
    func testLargerDecrementValue() {
        let state: State = appReducer(DecreaseAction(decreaseBy: 5), nil)
        
        XCTAssertEqual((state as! AppState).counter, -5)
        
    }
    
    func testLargerIncrementValue() {
        let state: State = appReducer(IncreaseAction(increaseBy: 5), nil)
        
        XCTAssertEqual((state as! AppState).counter, 5)
        
    }

    func testValidCallToImageURLGetsHTTPStatusCode200() {
        
        let url = URL(string: "https://i.vimeocdn.com/portrait/25429616_300x300")
        
        let promise = expectation(description: "Status code: 200")
        
        let dataTask = httpRequestTest.defaultSession.dataTask(with: url!) { data, response, error in
            if let error = error {
                XCTFail("Error: \(error.localizedDescription)")
                return
            } else if let statusCode = (response as? HTTPURLResponse)?.statusCode {
                if statusCode == 200 {
                    promise.fulfill()
                } else {
                    XCTFail("Status code: \(statusCode)")
                }
            }
        }
        dataTask.resume()
        
        wait(for: [promise], timeout: 5)
    }
    
    //This test would require putting iOS device in airplane mode to simulate no internet connection
    func testFailedCallToImageURLGetsHTTP() {
        
        let url = URL(string: "https://i.vimeocdn.com/portrait/25429616_300x300")
        
        let promise = expectation(description: "The Internet connection appears to be offline")
        
        let dataTask = httpRequestTest.defaultSession.dataTask(with: url!) { data, response, error in
            if let error = error {
                promise.fulfill()
                return
            } else if let statusCode = (response as? HTTPURLResponse)?.statusCode {
                if statusCode == 200 {
                    XCTFail("Status code: \(statusCode)")
                }
            }
        }
        dataTask.resume()
        
        wait(for: [promise], timeout: 5)
    }

    func testAsyncOrderedList() {
        var listOfNumbers: [Int] = []

        DispatchQueue.main.asyncAfter(deadline: .now() + 5.0) {
            for number in 1...100 {
                listOfNumbers.append(number)
            }
            
            for index in 0...99 {
                XCTAssertEqual(listOfNumbers[index], index + 1)
            }
        }
    }
}
