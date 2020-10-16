SWE.Line.EnumConstant
/**
    @api {line enums} SWE.Line.<EnumConstant> SWE.Line.<EnumConstant>
    @apiGroup SWE.Line
    @apiName <EnumConstant>

    @apiParam {number}      Ascii	line ascii code
    @apiParam {number}      Thin	line thin utf16
    @apiParam {number}      Bold	line bold utf16
    @apiParam {number}      Double	line double utf16

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
**/

SWE.Property.EnumConstant
/**
    @api {terminal char property enums} SWE.Property.<EnumConstant> SWE.Property.<EnumConstant>
    @apiGroup SWE.Property
    @apiName <EnumConstant>

    @apiParam {number}      RenderSolid		SDL ttf render constant
    @apiParam {number}      RenderBlended	SDL ttf render constant
    @apiParam {number}      RenderShaded	SDL ttf render constant
    @apiParam {number}      StyleNormal		SDL ttf style constant
    @apiParam {number}      StyleBold		SDL ttf style constant
    @apiParam {number}      StyleItalic		SDL ttf style constant
    @apiParam {number}      StyleUnderLine	SDL ttf style constant
    @apiParam {number}      StyleStrikeThrough	SDL ttf style constant
    @apiParam {number}      HintingNormal	SDL ttf hinting constant
    @apiParam {number}      HintingLight	SDL ttf hinting constant
    @apiParam {number}      HintingMono		SDL ttf hinting constant
    @apiParam {number}      HintingNone		SDL ttf hinting constant
**/

SWE.Char.LTee
/**
    @api {unicode box drawings: vertical and right} SWE.Char.LTee(line) SWE.Char.LTee
    @apiGroup SWE.Char
    @apiName LTee

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.RTee
/**
    @api {unicode box drawings: vertical and left} SWE.Char.RTee(line) SWE.Char.RTee
    @apiGroup SWE.Char
    @apiName RTee

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.TTee
/**
    @api {unicode box drawings: down and horizontal} SWE.Char.TTee(line) SWE.Char.TTee
    @apiGroup SWE.Char
    @apiName TTee

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.BTee
/**
    @api {unicode box drawings: up and horizontal} SWE.Char.BTee(line) SWE.Char.BTee
    @apiGroup SWE.Char
    @apiName BTee

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.HLine
/**
    @api {unicode box drawings: horizontal} SWE.Char.HLine(line) SWE.Char.HLine
    @apiGroup SWE.Char
    @apiName HLine

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.VLine
/**
    @api {unicode box drawings: vertical} SWE.Char.VLine(line) SWE.Char.VLine
    @apiGroup SWE.Char
    @apiName VLine

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.Plus
/**
    @api {unicode box drawings: vertical and horizontal} SWE.Char.Plus(line) SWE.Char.Plus
    @apiGroup SWE.Char
    @apiName Plus

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.ULCorner
/**
    @api {unicode box drawings: down and right} SWE.Char.ULCorner(line) SWE.Char.ULCorner
    @apiGroup SWE.Char
    @apiName ULCorner

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.URCorner
/**
    @api {unicode box drawings: down and left} SWE.Char.URCorner(line) SWE.Char.URCorner
    @apiGroup SWE.Char
    @apiName URCorner

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.LLCorner
/**
    @api {unicode box drawings: up and right} SWE.Char.LLCorner(line) SWE.Char.LLCorner
    @apiGroup SWE.Char
    @apiName LLCorner

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Char.LRCorner
/**
    @api {unicode box drawings: up and left} SWE.Char.LRCorner(line) SWE.Char.LRCorner
    @apiGroup SWE.Char
    @apiName LRCorner

    @apiParam {number}		line			line type, (SWE.Line enum)
    @apiSuccess (Return) {number} result                unicode code

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- draw box
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/

SWE.Terminal
/**
    @api {terminal constructor} SWE.Terminal(frs,cols,rows,parent) SWE.Terminal
    @apiGroup SWE.Terminal
    @apiName Constructor

    @apiParam {SWE.FontRender}            frs             fontrender object
    @apiParam {number}                    cols            terminal size
    @apiParam {number}                    rows            terminal size
    @apiParam {SWE.Terminal}              parent          parent object, (default display scene)
    @apiSuccess (Return) {SWE.Terminal}   result          terminal object

    @apiExample usage
    local term = SWE.Terminal(frs,20,10)

    @apiExample ext table fields (read only)
    | posx      | number  | window position
    | posy      | number  | window position
    | width     | number  | window size
    | height    | number  | window size
    | cols      | number  | terminal size
    | rows      | number  | terminal size
    | visible   | boolean | window is visible (defalut false)
    | modality  | boolean | window is modality (defalut false)
    | keyhangle | boolean | window is global key handle (defalut false)
*/

SWE.Terminal.SetVisible
/**
    @api {terminal set visible mode} SWE.Terminal.SetVisible(self,visible) SWE.Terminal.SetVisible
    @apiGroup SWE.Terminal.Window
    @apiName SetVisible

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {boolean}                 visible         flag visible
*/
SWE.Terminal.SetModality
/**
    @api {terminal set modality mode} SWE.Terminal.SetModality(self,modality) SWE.Terminal.SetModality
    @apiGroup SWE.Terminal.Window
    @apiName SetModality

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {boolean}                 modality        flag modality
*/
SWE.Terminal.SetKeyHandle
/**
    @api {terminal set global key handle mode} SWE.Terminal.SetKeyHandle(self,handle) SWE.Terminal.SetKeyHandle
    @apiGroup SWE.Terminal.Window
    @apiName SetKeyHandle

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {boolean}                 handle          flag key handle
*/
SWE.Terminal.SetResultCode
/**
    @api {terminal set result code} SWE.Terminal.SetResultCode(self,code) SWE.Terminal.SetResultCode
    @apiGroup SWE.Terminal.Window
    @apiName SetResultCode

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {number}                  code            set result code, see SWE.MainLoop
*/
SWE.Terminal.SetPosition
/**
    @api {terminal set position} SWE.Terminal.SetPosition(self,posx,posy) SWE.Terminal.SetPosition
    @apiGroup SWE.Terminal.Window
    @apiName SetPosition

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {number}                  posx            window position
    @apiParam {number}                  posy            window position
*/
SWE.Terminal.SetTermSize
/**
    @api {terminal set size} SWE.Terminal.SetTermSize(self,cols,rows) SWE.Terminal.SetSize
    @apiGroup SWE.Terminal
    @apiName SetSize

    @apiParam {SWE.Terminal}            self            terminal object
    @apiParam {number}                  cols            terminal size
    @apiParam {number}                  rows            terminal size
*/
SWE.Terminal.RenderTexture
/**
    @api {terminal render texture} SWE.Terminal.RenderTexture(self,texture,srcx,srcy,srcw,srch,dstx,dsty)(self,texture,src_rt,dst_pt)(self,texture,dstx,dsty)(self,texture,dst_pt) SWE.Terminal.RenderTexture
    @apiGroup SWE.Terminal.Window
    @apiName RenderTexture

    @apiParam {SWE.Terminal}                    self            terminal object
    @apiParam {SWE.Texture}                     texture         texture source object
    @apiParam {number}                          srcx            src position
    @apiParam {number}                          srcy            src position
    @apiParam {number}                          srcw            src size
    @apiParam {number}                          srch            src size
    @apiParam {number}                          dstx            dst position
    @apiParam {number}                          dsty            dst position
    @apiParam {SWE.Rect}                        src_rt          src rect object
    @apiParam {SWE.Point}                       dst_pt          dst point object
*/
SWE.Terminal.ToJson
/**
    @api {convert terminal to json string} SWE.Terminal.ToJson(self) SWE.Terminal.ToJson
    @apiGroup SWE.Terminal
    @apiName ToJson

    @apiParam {SWE.Terminal}                    self            terminal object
    @apiSuccess (Return) {string}               result          json string
*/
SWE.Terminal.PointInArea
/**
    @api {point in terminal area} SWE.Terminal.PointInArea(self,ptx,pty)(self,point) SWE.Terminal.PointInArea
    @apiGroup SWE.Terminal.Window
    @apiName PointInArea

    @apiParam {SWE.Terminal}                    self            terminal object
    @apiParam {number}                          ptx             point coord
    @apiParam {number}                          pty             point coord
    @apiParam {SWE.Point}                       point           point object
    @apiSuccess (Return) {boolean}              result          true if success
*/
SWE.Terminal.TextureInvalidEvent
/**
    @api {terminal event: invalid texture} SWE.Terminal.TextureInvalidEvent() SWE.Terminal.TextureInvalidEvent
    @apiGroup SWE.Terminal.Events
    @apiName TextureInvalidEvent
    @apiDescription after resizing the display or rotate (for mobile os), all textures (if used) must be reinitialized, because the internal SDL render is changed

    @apiExample usage
    win.tx1 = SWE.Texture.Text(frs, text, SWE.Color.Red)
    ...
    win.TextureInvalidEvent = function()
	win.tx1 = SWE.Texture.Text(frs, text, SWE.Color.Red)
    end
*/
SWE.Terminal.WindowCreateEvent
/**
    @api {terminal event: single event constructor} SWE.Terminal.WindowCreateEvent() SWE.Terminal.WindowCreateEvent
    @apiGroup SWE.Terminal.Events
    @apiName WindowCreateEvent
    
    @apiExample usage
    win.WindowCreateEvent = function()
	print("win created")
    end
*/
SWE.Terminal.WindowCloseEvent
/**
    @api {terminal event: destroy object} SWE.Terminal.WindowCloseEvent() SWE.Terminal.WindowCloseEvent
    @apiGroup SWE.Terminal.Events
    @apiName WindowCloseEvent
    
    @apiExample usage
    win.WindowCloseEvent = function()
	print("win closed")
    end
*/
SWE.Terminal.MousePressEvent
/**
    @api {terminal event: mouse press} SWE.Terminal.MousePressEvent(posx,posy,buttons) SWE.Terminal.MousePressEvent
    @apiGroup SWE.Terminal.Events
    @apiName MousePressEvent
    
    @apiParam {number}                          posx		mouse position
    @apiParam {number}				posy		mouse position
    @apiParam {number}				buttons		SWE.Button constant
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MousePressEvent = function(x,y,btn)
	print(x,y,btn)
	return true
    end
*/
SWE.Terminal.MouseReleaseEvent
/**
    @api {terminal event: mouse release} SWE.Terminal.MouseReleaseEvent(posx,posy,buttons) SWE.Terminal.MouseReleaseEvent
    @apiGroup SWE.Terminal.Events
    @apiName MouseReleaseEvent
    
    @apiParam {number}                          posx		mouse position
    @apiParam {number}				posy		mouse position
    @apiParam {number}				buttons		SWE.Button constant
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MouseReleaseEvent = function(x,y,btn)
	print(x,y,btn)
	return true
    end
*/
SWE.Terminal.MouseClickEvent
/**
    @api {terminal event: mouse click} SWE.Terminal.MouseClickEvent(press_x,press_y,press_btns,release_x,release_y,release_btns) SWE.Terminal.MouseClickEvent
    @apiGroup SWE.Terminal.Events
    @apiName MouseClickEvent
    
    @apiParam {number}                          press_x		mouse press position
    @apiParam {number}				press_y		mouse press position
    @apiParam {number}				press_btns	SWE.Button constant (press state)
    @apiParam {number}                          release_x	mouse release position
    @apiParam {number}				release_y	mouse release position
    @apiParam {number}				release_btns	SWE.Button constant (release state)
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MouseClickEvent = function(px,py,pbtn,rx,ry,rbtn)
	print(px,py,rx,ry)
	return true
    end
*/
SWE.Terminal.MouseMotionEvent
/**
    @api {terminal event: mouse motion} SWE.Terminal.MouseMotionEvent(posx,posy,buttons) SWE.Terminal.MouseMotionEvent
    @apiGroup SWE.Terminal.Events
    @apiName MouseMotionEvent
    
    @apiParam {number}                          posx		mouse position
    @apiParam {number}				posy		mouse position
    @apiParam {number}				buttons		SWE.Button constant
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MouseMotionEvent = function(x,y,btn)
	print(x,y,btn)
	return true
    end
*/
SWE.Terminal.MouseFocusEvent
/**
    @api {terminal event: gain focus of leave} SWE.Terminal.MouseFocusEvent(focus) SWE.Terminal.MouseFocusEvent
    @apiGroup SWE.Terminal.Events
    @apiName MouseFocusEvent
    
    @apiParam {boolean}                         focus		terminal if focused/leaved
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MouseFocusEvent = function(f)
	print("focus:",f)
	return true
    end
*/
SWE.Terminal.KeyPressEvent
/**
    @api {terminal event: key event} SWE.Terminal.KeyPressEvent(keycode,keymod,scancode) SWE.Terminal.KeyPressEvent
    @apiGroup SWE.Terminal.Events
    @apiName KeyPressEvent
    
    @apiParam {number}                          keycode         SWE.Key constant
    @apiParam {number}                          keymod          key mod SDL_Keymod
    @apiParam {number}                          scancode        key scancode
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.KeyPressEvent = function(k,m,s)
	if k == SWE.Key.ESCAPE then
	    win:SetVisible(false)
	    print("hide terminal")
	end
	return true
    end
*/
SWE.Terminal.KeyReleaseEvent
/**
    @api {terminal event: key event} SWE.Terminal.KeyReleaseEvent(keycode,keymod,scancode) SWE.Terminal.KeyReleaseEvent
    @apiGroup SWE.Terminal.Events
    @apiName KeyReleaseEvent
    
    @apiParam {number}                          keycode         SWE.Key constant
    @apiParam {number}                          keymod          key mod SDL_Keymod
    @apiParam {number}                          scancode        key scancode
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.KeyReleaseEvent = function(k,m,s)
	if k == SWE.Key.ESCAPE then
	    win:SetVisible(false)
	    print("hide terminal")
	end
	return true
    end
*/
SWE.Terminal.ScrollUpEvent
/**
    @api {terminal event: scroll event} SWE.Terminal.ScrollUpEvent(posx,posy) SWE.Terminal.ScrollUpEvent
    @apiGroup SWE.Terminal.Events
    @apiName ScrollUpEvent
    
    @apiParam {number}                          posx		mouse position
    @apiParam {number}				posy		mouse position
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.ScrollUpEvent = function(x,y)
	print(x,y)
	return true
    end
*/
SWE.Terminal.ScrollDownEvent
/**
    @api {terminal event: scroll event} SWE.Terminal.ScrollDownEvent(posx,posy) SWE.Terminal.ScrollDownEvent
    @apiGroup SWE.Terminal.Events
    @apiName ScrollDownEvent
    
    @apiParam {number}                          posx		mouse position
    @apiParam {number}				posy		mouse position
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.ScrollDownEvent = function(x,y)
	print(x,y)
	return true
    end
*/
SWE.Terminal.SystemTickEvent
/**
    @api {terminal event: system tick} SWE.Terminal.SystemTickEvent(tick) SWE.Terminal.SystemTickEvent
    @apiGroup SWE.Terminal.Events
    @apiName SystemTickEvent
    
    @apiParam {number}                          tick		constantly increasing counter (SWE.MainLoop cycle), in milliseconds, from the starting

    @apiExample usage
    win.SystemTickEvent = function(tick)
	print("tick:",tick)
    end
*/
SWE.Terminal.SystemUserEvent
/**
    @api {terminal event: system user event} SWE.Terminal.SystemUserEvent(code,lightuserdata) SWE.Terminal.SystemUserEvent
    @apiGroup SWE.Terminal.Events
    @apiName SystemUserEvent
    
    @apiParam {number}                          code		user code (0x00FFFFFF max value), see SWE.pushEvent
    @apiParam {table}                           object		any Lua object
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.SystemUserEvent = function(code,data)
	print("code:",code,type(data))
    end
*/
SWE.Terminal.RenderWindow
/**
    @api {render terminal code} SWE.Terminal.RenderWindow() SWE.Terminal.RenderWindow
    @apiGroup SWE.Terminal.Virtual
    @apiName RenderWindow
    
    @apiSuccess (Return) {boolean}		result		always true

    @apiExample usage
    win.RenderWindow = function()
	-- terminal render code
	return true
    end
*/
SWE.Terminal.TerminalResizeEvent
/**
    @api {terminal event: resize terminal window} SWE.Terminal.TerminalResizeEvent() SWE.Terminal.TerminalResizeEvent
    @apiGroup SWE.Terminal.Events
    @apiName TerminalResizeEvent
    
    @apiExample usage
    term.TerminalResizeEvent = function()
        print("term resized")
    end
*/
SWE.Terminal.FontResizeEvent
/**
    @api {terminal event: resize terminal font} SWE.Terminal.FontResizeEvent() SWE.Terminal.FontResizeEvent
    @apiGroup SWE.Terminal.Events
    @apiName FontResizeEvent
    
    @apiExample usage
    term.FontResizeEvent = function()
        print("font resized")
    end
*/
SWE.Terminal.TerminalDefaultColors
/**
    @api {terminal default: fg/bg colors} SWE.Terminal.TerminalDefaultColors() SWE.Terminal.TerminalDefaultColors
    @apiGroup SWE.Terminal.Virtual
    @apiName TerminalDefaultColors
    
    @apiSuccess (Return) {list}			result		default colors, (fbcolor, bgcolor)
*/
SWE.Terminal.TerminalDefaultProperty
/**
    @api {terminal default: property render/style/hinting} SWE.Terminal.TerminalDefaultProperty() SWE.Terminal.TerminalDefaultProperty
    @apiGroup SWE.Terminal.Virtual
    @apiName TerminalDefaultProperty
    
    @apiSuccess (Return) {list}			result		default property, (render, style, hinting), see SWE.Property
*/


SWE.Terminal.FillFGColor
/**
    @api {terminal func: fill fg color} SWE.Terminal.FillFGColor(self,color,cols,rows) SWE.Terminal.FillFGColor
    @apiGroup SWE.Terminal
    @apiName FillFGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}                          cols		terminal size area (default: 1)
    @apiParam {number}                          rows		terminal size area (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.FillBGColor
/**
    @api {terminal func: fill bg color} SWE.Terminal.FillBGColor(self,color,cols,rows) SWE.Terminal.FillBGColor
    @apiGroup SWE.Terminal
    @apiName FillBGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}                          cols		terminal size area (default: 1)
    @apiParam {number}                          rows		terminal size area (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.FillColors
/**
    @api {terminal func: fill fg/bg colors} SWE.Terminal.FillColors(self,fgcolor,bgcolor,cols,rows) SWE.Terminal.FillColors
    @apiGroup SWE.Terminal
    @apiName FillColors

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          fgcolor		SWE.Color
    @apiParam {number}                          bgcolor		SWE.Color
    @apiParam {number}                          cols		terminal size area (default: 1)
    @apiParam {number}                          rows		terminal size area (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.FillCharset
/**
    @api {terminal func: fill charset area} SWE.Terminal.FillCharset(self,symbol,cols,rows) SWE.Terminal.FillCharset
    @apiGroup SWE.Terminal
    @apiName FillCharset

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          symbol		symbol charset
    @apiParam {number}                          cols		terminal size area (default: 1)
    @apiParam {number}                          rows		terminal size area (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.FillProperty
/**
    @api {terminal func: fill charset property area} SWE.Terminal.FillProperty(self,blend,style,hinting,cols,rows) SWE.Terminal.FillProperty
    @apiGroup SWE.Terminal
    @apiName FillProperty

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          blend		SDL TTF render type: SWE.Property.[RenderSolid,RenderBlended]
    @apiParam {number}                          style		SDL TTF style type: SWE.Property.[StyleNormal,StyleBold,StyleItalic,StyleUnderLine,StyleStrikeThrough]
    @apiParam {number}                          hinting		SDL TTF hinting type: SWE.Property.[HintingNormal,HintingLight,HintingMono,HintingNone]
    @apiParam {number}                          cols		terminal size area (default: 1)
    @apiParam {number}                          rows		terminal size area (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object

    @apiExample usage
    local frs = SWE.FontRender("dejavusans.ttf", 14)
    local area = SWE.Terminal(frs, cols, rows)

    area.RenderWindow = function()
	area:CursorMoveTopLeft():FillFGColor(SWE.Color.Silver, area.cols, area.rows)
	area:CursorMoveTopLeft():area:FillProperty(SWE.Property.RenderBlended, SWE.Property.StyleNormal, SWE.Property.HintingNormal, area.cols, 1)
	area:CursorMoveFirst():DrawText("Simple Text Style: Normal"):CursorMoveDown()
	area:CursorMoveFirst():FillProperty(SWE.Property.RenderBlended, SWE.Property.StyleBold, SWE.Property.HintingNormal, area.cols, 1)
	area:CursorMoveFirst():DrawText("Simple Text Style: Bold"):CursorMoveDown()
	area:CursorMoveFirst():FillProperty(SWE.Property.RenderBlended, SWE.Property.StyleItalic, SWE.Property.HintingNormal, area.cols, 1)
	area:CursorMoveFirst():DrawText("Simple Text Style: Italic"):CursorMoveDown()
	return true
    end
*/
SWE.Terminal.CursorPosition
/**
    @api {terminal func: set/get cursor position} SWE.Terminal.CursorPosition(self,curx,cury)(self) SWE.Terminal.CursorPosition
    @apiGroup SWE.Terminal
    @apiName CursorPosition

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          curx		set cursor position
    @apiParam {number}                          cury		set cursor position
    @apiSuccess (Return) {list} 	        coord		current cursor position (posx, posy)
    @apiSuccess (Return) {SWE.Terminal}		result		this object

    @apiExample usage
    local area = SWE.Terminal(frs, cols, rows)
    local line = SWE.Line.Thin
    ...
    -- set cursor position, dump area (terminal object)
    SWE.Dump(area:CursorPosition(0, 0))
    -- get cursor position, dump cursor position
    SWE.Debug(area:CursorPosition())

    -- draw box and set cursor position
    area:CursorPosition(0, 0):DrawChar(SWE.Char.ULCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.URCorner(line))
    area:CursorPosition(0, 1):DrawChar(SWE.Char.VLine(line)):CursorMoveRight(1):DrawChar(SWE.Char.VLine(line))
    area:CursorPosition(0, 2):DrawChar(SWE.Char.LLCorner(line)):DrawChar(SWE.Char.HLine(line)):DrawChar(SWE.Char.LRCorner(line))
    area:SetFlush()
*/
SWE.Terminal.CursorTopLeft
/**
    @api {terminal func: cursor top left area position} SWE.Terminal.CursorTopLeft(self) SWE.Terminal.CursorTopLeft
    @apiGroup SWE.Terminal
    @apiName CursorTopLeft

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorTopRight
/**
    @api {terminal func: cursor top right area position} SWE.Terminal.CursorTopRight(self) SWE.Terminal.CursorTopRight
    @apiGroup SWE.Terminal
    @apiName CursorTopRight

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorBottomLeft
/**
    @api {terminal func: cursor bottom left area position} SWE.Terminal.CursorBottomLeft(self) SWE.Terminal.CursorBottomLeft
    @apiGroup SWE.Terminal
    @apiName CursorBottomLeft

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorBottomRight
/**
    @api {terminal func: cursor bottom right area position} SWE.Terminal.CursorBottomRight(self) SWE.Terminal.CursorBottomRight
    @apiGroup SWE.Terminal
    @apiName CursorBottomRight

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveUp
/**
    @api {terminal func: cursor move up position} SWE.Terminal.CursorMoveUp(self,count) SWE.Terminal.CursorMoveUp
    @apiGroup SWE.Terminal
    @apiName CursorMoveUp

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          count		step count (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveDown
/**
    @api {terminal func: cursor move down position} SWE.Terminal.CursorMoveDown(self,count) SWE.Terminal.CursorMoveDown
    @apiGroup SWE.Terminal
    @apiName CursorMoveDown

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          count		step count (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveLeft
/**
    @api {terminal func: cursor move left position} SWE.Terminal.CursorMoveLeft(self,count) SWE.Terminal.CursorMoveLeft
    @apiGroup SWE.Terminal
    @apiName CursorMoveLeft

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          count		step count (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveRight
/**
    @api {terminal func: cursor move right position} SWE.Terminal.CursorMoveRight(self,count) SWE.Terminal.CursorMoveRight
    @apiGroup SWE.Terminal
    @apiName CursorMoveRight

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          count		step count (default: 1)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveFirst
/**
    @api {terminal func: cursor move first line position} SWE.Terminal.CursorMoveFirst(self) SWE.Terminal.CursorMoveFirst
    @apiGroup SWE.Terminal
    @apiName CursorMoveFirst

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.CursorMoveLast
/**
    @api {terminal func: cursor move last line position} SWE.Terminal.CursorMoveLast(self) SWE.Terminal.CursorMoveLast
    @apiGroup SWE.Terminal
    @apiName CursorMoveLast

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetFGColor
/**
    @api {terminal func: set fg color default} SWE.Terminal.SetFGColor(self,color) SWE.Terminal.SetFGColor
    @apiGroup SWE.Terminal
    @apiName SetFGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          color		SWE.Color
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.SetBGColor
/**
    @api {terminal func: set bg color default} SWE.Terminal.SetBGColor(self,color) SWE.Terminal.SetBGColor
    @apiGroup SWE.Terminal
    @apiName SetBGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          color		SWE.Color
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetColors
/**
    @api {terminal func: set fg/bg colors default} SWE.Terminal.SetColors(self,fgcolor,bgcolor) SWE.Terminal.SetColors
    @apiGroup SWE.Terminal
    @apiName SetColors

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          fgcolor		SWE.Color
    @apiParam {number}                          bgcolor		SWE.Color
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetProperty
/**
    @api {terminal func: set property render/style/hinting default} SWE.Terminal.SetProperty(self,render,style,hinting) SWE.Terminal.SetProperty
    @apiGroup SWE.Terminal
    @apiName SetProperty

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          render		SWE.Property constants
    @apiParam {number}                          style		SWE.Property constants
    @apiParam {number}                          hinting		SWE.Property constants
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetWrap
/**
    @api {terminal func: set wrap mode} SWE.Terminal.SetWrap(self) SWE.Terminal.SetWrap
    @apiGroup SWE.Terminal
    @apiName SetWrap

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetBlink
/**
    @api {terminal func: set blink mode} SWE.Terminal.SetBlink(self) SWE.Terminal.SetBlink
    @apiGroup SWE.Terminal
    @apiName SetBlink

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetInvert
/**
    @api {terminal func: set invert mode} SWE.Terminal.SetInvert(self) SWE.Terminal.SetInvert
    @apiGroup SWE.Terminal
    @apiName SetInvert

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetFlipVertical
/**
    @api {terminal func: set flip vertical mode} SWE.Terminal.SetFlipVertical(self) SWE.Terminal.SetFlipVertical
    @apiGroup SWE.Terminal
    @apiName SetFlipVertical

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetFlipHorizontal
/**
    @api {terminal func: set flip horizontal mode} SWE.Terminal.SetFlipHorizontal(self) SWE.Terminal.SetFlipHorizontal
    @apiGroup SWE.Terminal
    @apiName SetFlipHorizontal

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetAlpha
/**
    @api {terminal func: set alpha mode} SWE.Terminal.SetAlpha(self,alpha) SWE.Terminal.SetAlpha
    @apiGroup SWE.Terminal
    @apiName SetAlpha

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          alpha		alpha value
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.SetPadding
/**
    @api {terminal func: set padding space} SWE.Terminal.SetPadding(self,left,right,top,bottom) SWE.Terminal.SetPadding
    @apiGroup SWE.Terminal
    @apiName SetPadding

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          left		left padding
    @apiParam {number}                          right		right padding
    @apiParam {number}                          top		top padding
    @apiParam {number}                          bottom		bottom padding
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.DrawHLine
/**
    @api {terminal func: draw hline chars} SWE.Terminal.DrawHLine(self,length,char,fgcol,bgcol) SWE.Terminal.DrawHLine
    @apiGroup SWE.Terminal
    @apiName DrawHLine

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          length		line length
    @apiParam {number}                          char		line symbol
    @apiParam {number}                          fgcol		SWE.Color (foreground color)
    @apiParam {number}                          bgcol		SWE.Color (background color)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.DrawVLine
/**
    @api {terminal func: draw vline chars} SWE.Terminal.DrawVLine(self,length,char,fgcol,bgcol) SWE.Terminal.DrawVLine
    @apiGroup SWE.Terminal
    @apiName DrawVLine

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          length		line length
    @apiParam {number}                          char		line symbol
    @apiParam {number}                          fgcol		SWE.Color (foreground color)
    @apiParam {number}                          bgcol		SWE.Color (background color)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.DrawRect
/**
    @api {terminal func: draw rectangle} SWE.Terminal.DrawRect(self,width,height,line) SWE.Terminal.DrawRect
    @apiGroup SWE.Terminal
    @apiName DrawRect

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {number}                          width		rect width
    @apiParam {number}                          height		rect height
    @apiParam {number}                          line		SWE.Line type
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.DrawText
/**
    @api {terminal func: draw text list} SWE.Terminal.DrawText(self,text,text,text) SWE.Terminal.DrawText
    @apiGroup SWE.Terminal
    @apiName DrawText

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {string}                          text		utf8 string
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.DrawChar
/**
    @api {terminal func: draw chars list} SWE.Terminal.DrawChar(self,char,char,char) SWE.Terminal.DrawChar
    @apiGroup SWE.Terminal
    @apiName DrawChar

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiParam {string}                          char		unicode char (utf16)
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.CharsetInfo
/**
    @api {terminal func: get charset info} SWE.Terminal.CharsetInfo(self) SWE.Terminal.CharsetInfo
    @apiGroup SWE.Terminal
    @apiName CharsetInfo

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {table}		result		table info

    @apiExample usage
    term:CursorPosition(2,3)
    local t = term:CharsetInfo()
    SWE.Dump(t)
    ...
    -- console output
    [`unicode'] = string(`0x002d')
    [`blended'] = integer(0)
    [`style'] = integer(2)
    [`bgcolor'] = string(`MidnightBlue')
    [`hinting'] = integer(0)
    [`fgcolor'] = string(`RosyBrown')
*/

SWE.Terminal.SetFlush
/**
    @api {terminal func: set flush} SWE.Terminal.SetFlush(self) SWE.Terminal.SetFlush
    @apiGroup SWE.Terminal
    @apiName SetFlush
    @apiDescription commit all changes to display

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetFGColor
/**
    @api {terminal func: reset default fg color to transparent} SWE.Terminal.ResetFGColor(self) SWE.Terminal.ResetFGColor
    @apiGroup SWE.Terminal
    @apiName ResetFGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetBGColor
/**
    @api {terminal func: reset default bg color to transparent} SWE.Terminal.ResetBGColor(self) SWE.Terminal.ResetBGColor
    @apiGroup SWE.Terminal
    @apiName ResetBGColor

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetColors
/**
    @api {terminal func: reset fg/bg colors} SWE.Terminal.ResetColors(self) SWE.Terminal.ResetColors
    @apiGroup SWE.Terminal
    @apiName ResetColors

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetProperty
/**
    @api {terminal func: reset property: render/style/hinting} SWE.Terminal.ResetProperty(self) SWE.Terminal.ResetProperty
    @apiGroup SWE.Terminal
    @apiName ResetProperty

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetPadding
/**
    @api {terminal func: reset padding} SWE.Terminal.ResetPadding(self) SWE.Terminal.ResetPadding
    @apiGroup SWE.Terminal
    @apiName ResetPadding

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetWrap
/**
    @api {terminal func: reset wrap mode} SWE.Terminal.ResetWrap(self) SWE.Terminal.ResetWrap
    @apiGroup SWE.Terminal
    @apiName ResetWrap

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/

SWE.Terminal.ResetBlink
/**
    @api {terminal func: reset blink mode} SWE.Terminal.ResetBlink(self) SWE.Terminal.ResetBlink
    @apiGroup SWE.Terminal
    @apiName ResetBlink

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.ResetInvert
/**
    @api {terminal func: reset invert mode} SWE.Terminal.ResetInvert(self) SWE.Terminal.ResetInvert
    @apiGroup SWE.Terminal
    @apiName ResetInvert

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.ResetFlip
/**
    @api {terminal func: reset flip mode} SWE.Terminal.ResetFlip(self) SWE.Terminal.ResetFlip
    @apiGroup SWE.Terminal
    @apiName ResetFlip

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
SWE.Terminal.ResetAlpha
/**
    @api {terminal func: reset alpha mode} SWE.Terminal.ResetAlpha(self) SWE.Terminal.ResetAlpha
    @apiGroup SWE.Terminal
    @apiName ResetAlpha

    @apiParam {SWE.Terminal}                    self		SWE.Terminal object
    @apiSuccess (Return) {SWE.Terminal}		result		this object
*/
