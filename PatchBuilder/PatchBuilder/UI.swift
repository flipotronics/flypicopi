//
//  UI.swift
//  PatchBuilder
//
//  Created by Mathias Dietrich on 09.03.21.
//
// https://www.raywenderlich.com/830-macos-nstableview-tutorial

import Cocoa

protocol MidiHandler{
    func update(p0: UInt8, p1: UInt8, p2: UInt8 )
}

class UI: NSView, NSTableViewDelegate, NSTableViewDataSource, MidiHandler {

    let  midi =  Midi()
    let osc =  OSC()
    
    // fields
    @IBOutlet var tbxOscPrefix: NSTextField!
    @IBOutlet var tbxOscTargetHost: NSTextField!
    @IBOutlet var tbxOscTargetPort: NSTextField!
    @IBOutlet var tbxPatchName: NSTextField!
    @IBOutlet var tbxAuthor: NSTextField!
    @IBOutlet var tbxTags: NSTextField!
    @IBOutlet var tbxPatchFolder: NSTextField!
    @IBOutlet var tbxFileName: NSTextField!
    @IBOutlet var tbxURL: NSTextField!
    @IBOutlet var tbxCcControl: NSTextField!
    @IBOutlet var tbxCcValue: NSTextField!
    @IBOutlet var tbxComment: NSTextField!
    
    @IBOutlet var slider: NSSlider!
    @IBOutlet var sliderMod: NSSlider!
    @IBOutlet var sliderPitch: NSSlider!
    
    @IBOutlet var cbxSendMidi: NSButton!
    @IBOutlet var cbxSendFader: NSButton!
    @IBOutlet var cbxSendOsc: NSButton!
    @IBOutlet var cbxListenOsc: NSButton!
    @IBOutlet var cbxListenMidi: NSButton!
    
    @IBOutlet var table: NSTableView!
    
    struct controlline {
        var cc: Int?
        var value: Int?
        var comment: String?
        var url: String?
    }

    var data :[controlline] = []
    
    required init?(coder: NSCoder) {
        super.init(coder:coder)
    }
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)
    }
    
    func start(){
        midi.handler = self
        midi.setup()
        
        table.delegate = self
        table.dataSource = self
        
        let h = tbxOscTargetHost.stringValue
        let p = Int(self.tbxOscTargetPort.intValue)
        self.osc.setup(host:h, port:p)
    }
    
    func sort(){
        data.sort {
            $0.cc! < $1.cc!
        }
    }
    
    func update(p0: UInt8, p1: UInt8, p2: UInt8) {
        if(p0 == 176){
            let cc = p1
            let val = p2
            print("CC" + String(cc) + " Val: " + String(val) + "")
            
            var count = 0
            for line in data{
                if(line.cc! == cc){
                    data[count].value = Int(val) ;
                    break
                }
                count += 1
            }
            DispatchQueue.main.async {
                self.table.reloadData()
                if(cc == self.tbxCcControl.intValue){
                    self.slider.intValue = Int32(val)
                }
            }
        }
    }
    
    func send(){
        osc.send(prefix:tbxOscPrefix.stringValue, cc: Int(tbxCcControl.intValue), value: Int(tbxCcValue.intValue))
        midi.send(cc: Int(tbxCcControl.intValue), value: Int(tbxCcValue.intValue))
    }
    
    func send(cc:Int, value:Int){
        osc.send(prefix:tbxOscPrefix.stringValue, cc: cc, value: value)
        midi.send(cc: cc, value: value)
    }

    // Table ===============================================================
    func numberOfRows(in tableView: NSTableView) -> Int {
        return data.count
    }
    
    func tableViewSelectionDidChange(_ notification: Notification){
        let  rowsel = table.selectedRow;
        tbxCcControl.intValue = Int32(data[rowsel].cc!)
        tbxCcValue.intValue = Int32(data[rowsel].value!)
        tbxComment.stringValue = data[rowsel].comment!
        slider.intValue = Int32(data[rowsel].value!)
    }

    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        let line = data[row]

        print("loading view " + String(row))
        if tableColumn?.identifier == NSUserInterfaceItemIdentifier(rawValue: "cc") {
            let cellIdentifier = NSUserInterfaceItemIdentifier(rawValue: "cc")
                guard let cellView = tableView.makeView(withIdentifier: cellIdentifier, owner: self) as? NSTableCellView else { return nil }
                cellView.textField?.integerValue = line.cc ?? 0
                return cellView
        } else if tableColumn?.identifier == NSUserInterfaceItemIdentifier(rawValue: "value") {
            let cellIdentifier = NSUserInterfaceItemIdentifier(rawValue: "value")
                guard let cellView = tableView.makeView(withIdentifier: cellIdentifier, owner: self) as? NSTableCellView else { return nil }
                cellView.textField?.integerValue = line.value ?? 0
                return cellView
        } else {
            let cellIdentifier = NSUserInterfaceItemIdentifier(rawValue: "comment")
                guard let cellView = tableView.makeView(withIdentifier: cellIdentifier, owner: self) as? NSTableCellView else { return nil }
                cellView.textField?.stringValue = line.comment ?? ""
                return cellView
        }
    }
    
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        return data.count
    }
    
    // Slider Handler ================================================================================================
    @IBAction func slider(sender: AnyObject) {
        tbxCcValue.intValue = slider.intValue
        var count = 0
        for line in data{
            if(line.cc! == tbxCcControl.intValue){
                data[count].value = Int(tbxCcValue.intValue)
                break
            }
            count += 1
        }
       
        if (cbxSendFader.intValue == 1){
            send()
        }
        table.reloadData()
    }
    
    @IBAction func sliderMod(sender: AnyObject) {
        let val = sliderMod.intValue
    }
    
    @IBAction func sliderPitch(sender: AnyObject) {
        let event = NSApplication.shared.currentEvent
        let val = sliderPitch.intValue;
        if event?.type == NSEvent.EventType.leftMouseUp {
            let seconds = 0.1
            DispatchQueue.main.asyncAfter(deadline: .now() + seconds) {
                self.sliderPitch.intValue = 8192
            }
            midi.send(cmd:Int(0xE0), cc: Int(val/256), value: Int(val % 256))
        }
    }
    
    // Button Handler ================================================================================================
    @IBAction func btnLoad(sender: NSButton) {
        print("btnLoad")
        let dialog = NSOpenPanel();
        dialog.directoryURL = URL(string: tbxPatchFolder.stringValue);
        dialog.title                   = "Choose Patch file";
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.allowsMultipleSelection = false;
        dialog.canChooseDirectories = false;
        dialog.allowedFileTypes        = ["txt", "fly"];
        if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
            let result = dialog.url // Pathname of the file
            if (result != nil) {
                let path: String = result!.path
                do {
                    let content = try String(contentsOfFile:path, encoding: String.Encoding.utf8)
                    let lines = content.split(separator:"\n")
                    tbxFileName.stringValue = path
                    tbxPatchName.stringValue = String(lines[0])
                    tbxAuthor.stringValue = String(lines[1])
                    tbxTags.stringValue = String(lines[2])
                    tbxURL.stringValue = String(lines[3])
                    var i = 4;
                    data.removeAll()
                    while(i < lines.count){
                        let l = String(lines[i])
                        let p = l.split(separator:":")
                        var line :controlline = controlline()
                        line.cc = Int(p[0]) as Int?
                        line.value = Int(p[1]) as Int?
                        line.comment = String(p[2]) as String?
                        data.append(line)
                        i += 1
                    }
                    sort()
                    table.reloadData()
                   }
                catch _ as NSError {
                }
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func btnSave(sender: NSButton) {
        var msg = "";
        msg += tbxPatchName.stringValue + " \n"
        msg += tbxAuthor.stringValue + " \n"
        msg += tbxTags.stringValue + " \n"
        msg += tbxURL.stringValue + " \n"
        for line in data {
            let m = String(line.cc!) + ":" + String(line.value!) + ":" + line.comment!
            msg += m + " \n"
        }
        let filename = NSURL.fileURL(withPath: tbxFileName.stringValue)
        do {
            try msg.write(to: filename, atomically: true, encoding: String.Encoding.utf8)
        }catch let error as NSError {
            print("An error took place: \(error)")
        }
    }
    
    @IBAction func btnAdd(sender: NSButton) {
        var count = 0
        for line in data{
            if(line.cc! == tbxCcControl.intValue){
                data[count].value = Int(tbxCcValue.intValue)
                data[count].comment = tbxComment.stringValue
                table.reloadData()
                return;
            }
            count = count + 1
        }
        var line  = controlline()
        line.cc = Int(tbxCcControl.intValue)
        line.value = Int(tbxCcValue.intValue)
        line.comment = tbxComment.stringValue
        line.url = tbxURL.stringValue
        data.append(line)
        sort()
        table.reloadData()
    }
    
    @IBAction func btnSend(sender: NSButton) {
        send()
    }
    
    @IBAction func btnSendAll(sender: NSButton) {
        for line in data {
            send(cc:line.cc!, value:line.value!)
        }
    }
    
    @IBAction func btnDelete(sender: NSButton) {
        print("btnDelete")
        let ccId = tbxCcControl.intValue
        var count = 0
        for line in data{
            if(line.cc! == ccId){
                data.remove(at: count)
                table.reloadData()
                return
            }
            count += 1
        }
    }
    
    @IBAction func btnSetup(sender: NSButton) {
        let h = tbxOscTargetHost.stringValue
        let p = Int(self.tbxOscTargetPort.intValue)
        DispatchQueue.global(qos: .background).async {
            print("This is run on the background queue")
            self.osc.setup(host:h, port:p)
        }
        print("btnSetup")
    }
}
