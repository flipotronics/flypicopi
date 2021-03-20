//
//  Controller.swift
//  PatchBuilder
//
//  Created by Mathias Dietrich on 10.03.21.
//

import Cocoa

class Controller: NSViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
        var view = self.view as! UI
        view.start()
    }
}


