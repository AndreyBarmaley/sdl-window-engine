SWE.DisplayInit
/**
    @api {create display window object} SWE.DisplayInit(title,width,height,fullscreen)(title,landscape) SWE.DisplayInit
    @apiGroup SWE
    @apiName DisplayInit

    @apiParam {string}			title		display title
    @apiParam {number}			width		display size
    @apiParam {number}			height		display size
    @apiParam {boolean}			fullscreen	fullscreen mode, (default false)
    @apiParam {boolean}			landscape	init landscape of portrait mode (mobile os)
    @apiSuccess (Return) {SWE.Window}	result		window object

    @apiExample usage
    local win = SWE.DisplayInit("SWE Window",640,480)
    -- see also SWE.DisplayWindow

    local result = SWE.MainLoop(win)

    @apiExample usage
    local landscape = false
    local win = SWE.DisplayInit("SWE init: Portrait modes",landscape)
    print(win.width,win.height)

*/
SWE.TerminalInit
/**
    @api {create display terminal object} SWE.TerminalInit(title,frs,cols,rows) SWE.TerminalInit
    @apiGroup SWE
    @apiName TerminalInit

    @apiParam {string}                  title           display title
    @apiParam {table}                   frs             font render, (SWE.FontRender object)
    @apiParam {number}                  cols            terminal size
    @apiParam {number}                  rows            terminal size
    @apiSuccess (Return) {SWE.Terminal} result          terminal object

    @apiExample usage
    local frs = SWE.FontRender.System()
    local term = SWE.TerminalInit("SWE Terminal",frs,80,25)

    -- render terminal
    function term.RenderWindow()
        term:CursorPosition(0, 0):FillColors(SWE.Color.Silver, SWE.Color.Black, term.cols, term.rows)
        term:CursorPosition(0, 0):FillProperty(frs.blended, frs.style, frs.hinting, term.cols, term.rows)
        term:CursorPosition(0, 0):FillCharset(0x33, term.cols, term.rows)
        term:CursorPosition(0, 0):DrawRect(term.cols, term.rows, SWE.Line.Thin)
        term:SetFlush()
        return true
    end

    -- dump SWE.Terminal
    SWE.Dump(term)

    local result = SWE.MainLoop(term)
*/
SWE.DisplayWindow
/**
    @api {get display window object} SWE.DisplayWindow() SWE.DisplayWindow
    @apiGroup SWE
    @apiName DisplayWindow
    
    @apiSuccess (Return) {SWE.Window}	result		window object

    @apiExample usage
    local win1 = SWE.DisplayInit("SWE Text",640,480)

    ....

    -- win1 and win2 the same object
    win2 = SWE.DisplayWindow()

    local result = SWE.MainLoop(win2)
*/
SWE.DisplayDirty
/**
    @api {set display scene dirty mode} SWE.DisplayDirty() SWE.DisplayDirty
    @apiGroup SWE
    @apiName DisplayDirty
    @apiDescription sometimes it’s necessary to tell the display scene update
*/
SWE.DisplaySize
/**
    @api {get display size} SWE.DisplaySize() SWE.DisplaySize
    @apiGroup SWE
    @apiName DisplaySize

    @apiSuccess (Return) {list}		result		display size

    @apiExample usage
    local w,h,fullscreen = SWE.DisplaySize();
    print("display size:", w, h, fullscreen)
*/
SWE.DisplayVideoModes
/**
    @api {get display mode list} SWE.DisplayVideoModes(landscape) SWE.DisplayVideoModes
    @apiGroup SWE
    @apiName DisplayVideoModes

    @apiParam {boolean}			landscape	landscape (default) or portrait modes
    @apiSuccess (Return) {list}		result		modes list

    @apiExample usage
    local modes = { SWE.DisplayVideoModes() }
    for i=1,#modes do
	print("videomodes:", modes[i].width, modes[i].height)
    end
*/
SWE.DisplayHandleEvents
/**
    @api {handle events intervals} SWE.DisplayVideoModes(interval) SWE.DisplayHandleEvents
    @apiGroup SWE
    @apiName DisplayHandleEvents

    @apiParam {numberl}			interval	handle events loop for interval (ms)
*/
SWE.CursorHide
/**
    @api {hide hardware cursor} SWE.CursorHide() SWE.CursorHide
    @apiGroup SWE
    @apiName CursorHide
*/
SWE.CursorShow
/**
    @api {hide hardware cursor} SWE.CursorShow() SWE.CursorShow
    @apiGroup SWE
    @apiName CursorShow
*/
SWE.CursorLoad
/**
    @api {set custom cursor} SWE.CursorLoad(texture,offsetx,offsety) SWE.CursorLoad
    @apiGroup SWE
    @apiName CursorLoad

    @apiParam {SWE.Texture}		texture		image, see SWE.Texture
    @apiParam {number}			offsetx		offset position
    @apiParam {number}			offsety		offset position

    @apiExample usage
    local win = SWE.DisplayInit("Lua SWE", 640, 480)

    SWE.CursorHide()
    local txcur = SWE.Texture.Image("arrow.png")
    SWE.CursorLoad(txcur)

    ....

    local result = SWE.MainLoop(win)
*/
SWE.JsonParse
/**
    @api {parse json string} SWE.JsonParse(json) SWE.JsonParse
    @apiGroup SWE
    @apiName JsonParse

    @apiParam {string}			json		json value string
    @apiSuccess (Return) {table}	result		json object

    @apiExample usage
    local str = "{\"hello\": 123,\"world\":[1,2,3,4,5]}"
    local json = SWE.JsonParse(str)

    .... dump json
    | "hello" | number:123 |
    | "world" | table      |
*/
SWE.MainLoop
/**
    @api {main loop events processing} SWE.MainLoop(window) SWE.MainLoop
    @apiGroup SWE
    @apiName MainLoop

    @apiParam {SWE.Window}		window		only this window will be checked for a visible
    @apiSuccess (Return) {number}	result		window result

    @apiExample usage
    local win = SWE.DisplayInit("Lua SWE", 640, 480)
    win.SetResult(123)

    ......
    win.KeyPressEvent = function(k,m,s)
        if k == SWE.Key.ESCAPE then
            win:SetVisible(false)
        end
        return true
    end
    ......

    print(SWE.MainLoop(win))
*/
SWE.PushEvent
/**
    @api {push custom event to window object} SWE.PushEvent(code,object,window) SWE.PushEvent
    @apiGroup SWE
    @apiName PushEvent

    @apiParam {number}			code		code event (0x00FFFFFF max value)
    @apiParam {object}			object		any Lua object
    @apiParam {SWE.Window}		window		if nil to broadcast event, otherwise unicast to window

    @apiExample usage
    local win = SWE.DisplayInit("Lua SWE", 640, 480)
    local win2 = SWE.Window(50, 50, 200, 100)

    ......
    win.KeyPressEvent = function(k,m,s)
        if k == SWE.Key.ESCAPE then
            win:SetVisible(false)
	    return true
	elseif k == SWE.Key.RETURN then
	    SWE.PushEvent(3333, nil, win2)
	    return true
        end
        return false
    end

    win1.SystemUserEvent = function(c,d)
	if c == 3333 then
	    print("SUCCESS")
	end
    end
    ......

    SWE.MainLoop(win)
*/
SWE.RenderScreenshot
/**
    @api {display scene render to screenshot} SWE.RenderScreenshot(filename) SWE.RenderScreenshot
    @apiGroup SWE
    @apiName RenderScreenshot

    @apiParam {string}			filename	save to filename (png or jpg extension)
    @apiSuccess (Return) {boolean}	result		true if success

*/
SWE.SystemShareDirectories
/**
    @api {get current directory} SWE.SystemShareDirectories() SWE.SystemShareDirectories
    @apiGroup SWE
    @apiName SystemShareDirectories

    @apiSuccess (Return) {list}		directories	local writable directories list
*/
SWE.SystemCurrentDirectory
/**
    @api {get current directory} SWE.SystemCurrentDirectory() SWE.SystemCurrentDirectory
    @apiGroup SWE
    @apiName SystemCurrentDirectory

    @apiSuccess (Return) {string}	directory	directory, where does the program run
*/
SWE.SystemMakeDirectory
/**
    @api {make directory} SWE.SystemMakeDirectory(directory) SWE.SystemMakeDirectory
    @apiGroup SWE
    @apiName SystemMakeDirectory

    @apiParam {string}			directory	path to directory
    @apiSuccess (Return) {boolean}	result		true if success or present
*/
SWE.SystemReadDirectory
/**
    @api {read directory} SWE.SystemReadDirectory(directory) SWE.SystemReadDirectory
    @apiGroup SWE
    @apiName SystemReadDirectory

    @apiParam {string}			directory	path to directory
    @apiSuccess (Return) {table}	result		table result, full path name, type

    @apiExample usage
    local names = SWE.SystemReadDirectory("/var/tmp")

    .... dump names
    | "/var/tmp/world.log" | "file"      |
    | "/var/tmp/hello.log" | "file"      |
    | "/var/tmp/mc-root"   | "directory" |
*/
SWE.SystemDirnameBasename
/**
    @api {parse to dirname/basename} SWE.SystemDirnameBasename(path) SWE.SystemDirnameBasename
    @apiGroup SWE
    @apiName SystemDirnameBasename

    @apiParam {string}			path		path to object (file or directory)
    @apiSuccess (Return) {string}	dirname		dirname object
    @apiSuccess (Return) {string}	basename	basename object

    @apiExample usage
    local dirname,basename = SWE.SystemDirnameBasename("/var/tmp/screenshot.png")
    print(dirname,basename)

    ... console
    "/var/tmp"    "screenshot.png"
*/
SWE.SystemFileStat
/**
    @api {stat info for directory/file} SWE.SystemFileStat(path) SWE.SystemFileStat
    @apiGroup SWE
    @apiName SystemFileStat

    @apiParam {string}			path		path to object (file or directory)
    @apiSuccess (Return) {table}	stat		table stat info

    @apiExample usage
    local stat = SWE.SystemFileStat("/var/tmp/screenshot.png")

    .... dump stat
    | mode  | 33204                   | 
    | gid   | 1000                    | 
    | uid   | 1000                    | 
    | nlink | 1                       | 
    | size  | 404930                  | 
    | atime | 1565677681              | 
    | mtime | 1565677681              | 
    | ctime | 1565677681              | 
    | isdir | false                   |
    | type  | file                    |
    | path  | /var/tmp/screenshot.png |

*/
SWE.SystemSleep
/**
    @api {sleep ms} SWE.SystemSleep(delay) SWE.SystemSleep
    @apiGroup SWE
    @apiName SystemSleep

    @apiParam {number}			delay		sleep delay ms
*/
SWE.SystemMemoryUsage
/**
    @api {get memory usage} SWE.SystemMemoryUsage() SWE.SystemMemoryUsage
    @apiGroup SWE
    @apiName SystemMemoryUsage

    @apiSuccess (Return) {number}	value		memory usage
*/
SWE.SystemMobileOs
/**
    @api {get mobile os name} SWE.SystemMobileOs() SWE.SystemMobileOs
    @apiGroup SWE
    @apiName SystemMobileOs

    @apiSuccess (Return) {string}	name		mobile os name: android, wince, symbian
*/
SWE.SetDebug
/**
    @api {set debug mode} SWE.SetDebug(debug) SWE.SetDebug
    @apiGroup SWE
    @apiName SetDebug

    @apiParam {boolean}			debug		debug mode
*/
SWE.TableToJson
/**
    @api {convert table object to json string} SWE.TableToJson(table) SWE.TableToJson
    @apiGroup SWE
    @apiName TableToJson

    @apiParam {table}			table		table object
    @apiSuccess (Return) {string}	json		result: json string
*/
SWE.SystemConcatePath
/**
    @api {concate filesystem names} SWE.SystemConcatePath(item1,item2,itemN) SWE.SystemConcatePath
    @apiGroup SWE
    @apiName SystemConcatePath

    @apiParam {string}			item1		name
    @apiParam {string}			item2		name
    @apiParam {string}			itemN		name
    @apiSuccess (Return) {string}	path		result: filesystem path
*/
SWE.SystemRunCommand
/**
    @api {run command, get result outputs} SWE.SystemRunCommand(cmd,param1,param2,paramN) SWE.SystemRunCommand
    @apiGroup SWE
    @apiName SystemRunCommand

    @apiParam {string}			cmd		command binary
    @apiParam {string}			param1		command arguments
    @apiParam {string}			param2		command arguments
    @apiParam {string}			paramN		command arguments
    @apiSuccess (Return) {table}	result		result: output strings

    @apiExample usage
    local out = SWE.SystemRunCommand("getent", "passwd")

    .... dump output table
    | root:x:0:0:root:/root:/bin/bash         |
    | bin:x:1:1:bin:/bin:/sbin/nologin        |
    | daemon:x:2:2:daemon:/sbin:/sbin/nologin |
    | adm:x:3:4:adm:/var/adm:/sbin/nologin    |
*/
SWE.ResourcesRegisterDirectory
/**
    @api {register directory for ext resources} SWE.ResourcesRegisterDirectory(directory) SWE.ResourcesRegisterDirectory
    @apiGroup SWE
    @apiName ResourcesRegisterDirectory

    @apiParam {string}			path		directory
    @apiSuccess (Return) {boolean}	result          result: success

    @apiExample usage
    -- data/images/image1.png

    -- get texture variant 1
    local tx1 = SWE.Texture.Image("data/images/image1.png")

    -- get texture variant 2
    SWE.ResourcesRegisterDirectory("data")
    local tx2 = SWE.Texture.Image("images/image1.png")

    -- get texture variant 3
    SWE.ResourcesRegisterDirectory(SWE.SystemConcatePath("data", "images"))
    local tx3 = SWE.Texture.Image("image1.png")
*/
SWE.LuaRegisterDirectory
/**
    @api {register directory for lua modules} SWE.LuaRegisterDirectory(directory) SWE.LuaRegisterDirectory
    @apiGroup SWE
    @apiName LuaRegisterDirectory

    @apiParam {string}			path		directory
    @apiSuccess (Return) {boolean}	result          result: success
*/
SWE.GetVersion
/**
    @api {get api version} SWE.GetVersion() SWE.GetVersion
    @apiGroup SWE
    @apiName GetVersion

    @apiSuccess (Return) {integer}	result          version
*/
