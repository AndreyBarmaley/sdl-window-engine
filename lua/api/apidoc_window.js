SWE.Window
/**
    @api {window constructor} SWE.Window(posx,posy,width,height,parent) SWE.Window
    @apiGroup SWE.Window
    @apiName Constructor

    @apiParam {number}			posx		window position
    @apiParam {number}			posy		window position
    @apiParam {number}			width		window size
    @apiParam {number}			height		window size
    @apiParam {SWE.Window}		parent		parent object, (default display scene)
    @apiSuccess (Return) {SWE.Window}	result		window object

    @apiExample usage
    -- create window area(30,30,200,100) on display scene
    local win2 = SWE.Window(30,30,200,100)

    @apiExample ext table fields (read only)
    | posx      | number  | window position
    | posy      | number  | window position
    | width     | number  | window size
    | height    | number  | window size
    | visible   | boolean | window is visible (defalut false)
    | modality  | boolean | window is modality (defalut false)
    | keyhangle | boolean | window is global key handle (defalut false)
*/
SWE.Window.MarkDestroyed
/**
    @api {window mark will be destroyed} SWE.Window.MarkDestroyed(self) SWE.Window.MarkDestroyed
    @apiGroup SWE.Window
    @apiName MarkDestroyed
    
    @apiParam {SWE.Window}              self		window object

    @apiExample usage
    -- create window area(30,30,200,100) on display scene
    local win2 = SWE.Window(30,30,200,100)

    ....
    win2:MarkDestroyed()

    -- equivalent to:
    win2 = nil
    collectgarbage()
*/
SWE.Window.SetVisible
/**
    @api {window set visible mode} SWE.Window.SetVisible(self,visible) SWE.Window.SetVisible
    @apiGroup SWE.Window
    @apiName SetVisible
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {boolean}                 visible		flag visible
*/
SWE.Window.SetModality
/**
    @api {window set modality mode} SWE.Window.SetModality(self,modality) SWE.Window.SetModality
    @apiGroup SWE.Window
    @apiName SetModality
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {boolean}                 modality	flag modality
*/
SWE.Window.SetKeyHandle
/**
    @api {window set global key handle mode} SWE.Window.SetKeyHandle(self,handle) SWE.Window.SetKeyHandle
    @apiGroup SWE.Window
    @apiName SetKeyHandle
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {boolean}                 handle		flag key handle
*/
SWE.Window.SetResultCode
/**
    @api {window set result code} SWE.Window.SetResultCode(self,code) SWE.Window.SetResultCode
    @apiGroup SWE.Window
    @apiName SetResultCode
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {number}                  code		set result code, see SWE.MainLoop
*/
SWE.Window.SetPosition
/**
    @api {window set position} SWE.Window.SetPosition(self,posx,posy) SWE.Window.SetPosition
    @apiGroup SWE.Window
    @apiName SetPosition
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {number}			posx		window position
    @apiParam {number}			posy		window position
*/
SWE.Window.SetSize
/**
    @api {window set size} SWE.Window.SetSize(self,width,height) SWE.Window.SetSize
    @apiGroup SWE.Window
    @apiName SetSize
    
    @apiParam {SWE.Window}              self		window object
    @apiParam {number}			width		window size
    @apiParam {number}			height		window size
*/
SWE.Window.RenderClear
/**
    @api {window clear} SWE.Window.RenderClear(self,color) SWE.Window.RenderClear
    @apiGroup SWE.Window
    @apiName RenderClear
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          color		SWE.Color
*/
SWE.Window.RenderRect
/**
    @api {window render rect} SWE.Window.RenderRect(self,color,rtx,rty,rtw,rth,filled)(self,color,rect,filled) SWE.Window.RenderRect
    @apiGroup SWE.Window
    @apiName RenderRect
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				rtx		rect position
    @apiParam {number}				rty		rect position
    @apiParam {number}				rtw		rect size
    @apiParam {number}				rth		rect size
    @apiParam {SWE.Rect}			rect		rect object
    @apiParam {boolean}				filled		fill mode (default false)
*/
SWE.Window.RenderLine
/**
    @api {window render line} SWE.Window.RenderLine(self,color,px1,py1,px2,py2)(self,color,point1,point2) SWE.Window.RenderLine
    @apiGroup SWE.Window
    @apiName RenderLine
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				px1		point1 position
    @apiParam {number}				py1		point1 position
    @apiParam {number}				px2		point2 position
    @apiParam {number}				py2		point2 position
    @apiParam {SWE.Point}			point1		point object
    @apiParam {SWE.Point}			point2		point object
*/
SWE.Window.RenderCyrcle
/**
    @api {window render cyrcle} SWE.Window.RenderCyrcle(self,color,centerx,centery,radius,filled)(self,color,center,radius,filled) SWE.Window.RenderCyrcle
    @apiGroup SWE.Window
    @apiName RenderCyrcle
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				centerx		center position
    @apiParam {number}				centery		center position
    @apiParam {SWE.Point}			center		point object
    @apiParam {number}				radius		radius value
    @apiParam {boolean}				filled		fill mode (default false)
*/
SWE.Window.RenderPoint
/**
    @api {window render point} SWE.Window.RenderPoint(self,color,posx,posy)(self,color,point) SWE.Window.RenderPoint
    @apiGroup SWE.Window
    @apiName RenderPoint
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				posx		point position
    @apiParam {number}				posy		point position
    @apiParam {SWE.Point}			point		point object
*/
SWE.Window.RenderText
/**
    @api {window render text} SWE.Window.RenderText(self,frs,color,posx,posy,halign,valign,horizontal)(self,frs,color,point,halign,valign,horizontal,render,style,hinting) SWE.Window.RenderText
    @apiGroup SWE.Window
    @apiName RenderText
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {SWE.FontRender}                  frs		fontrender object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				posx		text position
    @apiParam {number}				posy		text position
    @apiParam {SWE.Point}                       point		point object
    @apiParam {number}				halign		align text (default SWE.Align.Left)
    @apiParam {number}				valign		align text (default SWE.Align.Top)
    @apiParam {boolean}				horizontal	text orientation vertical or horizontal (default)
    @apiParam {number}                          render          text render (default RenderDefault, used from FontRender), see SWE.Font
    @apiParam {number}                          style           text style (default RenderDefault, used from FontRender), see SWE.Font
    @apiParam {number}                          hinting         text hinting (default RenderDefault, used from FontRender) see SWE.Font
    @apiSuccess (Return) {SWE.Rect}		result		SWE.Rect text position
*/
SWE.Window.RenderTexture
/**
    @api {window render texture} SWE.Window.RenderTexture(self,texture,srcx,srcy,srcw,srch,dstx,dsty,flip)(self,texture,srcrt,dstpt,flip)(self,texture,dstx,dsty,flip)(self,texture,dstpt,flip) SWE.Window.RenderTexture
    @apiGroup SWE.Window
    @apiName RenderTexture
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {SWE.Texture}                     texture         texture source object
    @apiParam {number}				srcx		src position
    @apiParam {number}				srcy		src position
    @apiParam {number}				srcw		src size
    @apiParam {number}				srch		src size
    @apiParam {number}				dstx		dst position
    @apiParam {number}				dsty		dst position
    @apiParam {SWE.Rect}			srcrt		SWE.Rect object or table
    @apiParam {SWE.Point}			dstpt		SWE.Point object or table
    @apiParam {number}				flip		possible value: SWE.Texture.FlipVertical or SWE.Texture.FlipHorizontal or combined
*/

SWE.Window.MousePressEvent
/**
    @api {window event: mouse press} SWE.Window.MousePressEvent(posx,posy,buttons) SWE.Window.MousePressEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.MouseReleaseEvent
/**
    @api {window event: mouse release} SWE.Window.MouseReleaseEvent(posx,posy,buttons) SWE.Window.MouseReleaseEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.MouseClickEvent
/**
    @api {window event: mouse click} SWE.Window.MouseClickEvent(press_x,press_y,press_btns,release_x,release_y,release_btns) SWE.Window.MouseClickEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.MouseMotionEvent
/**
    @api {window event: mouse motion} SWE.Window.MouseMotionEvent(posx,posy,buttons) SWE.Window.MouseMotionEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.MouseFocusEvent
/**
    @api {window event: gain focus of leave} SWE.Window.MouseFocusEvent(focus) SWE.Window.MouseFocusEvent
    @apiGroup SWE.Window.Events
    @apiName MouseFocusEvent
    
    @apiParam {boolean}                         focus		window if focused/leaved
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.MouseFocusEvent = function(f)
	print("focus:",f)
	return true
    end
*/
SWE.Window.DisplayResizeEvent
/**
    @api {window event: win size changed} SWE.Window.DisplayResizeEvent(width,height) SWE.Window.DisplayResizeEvent
    @apiGroup SWE.Window.Events
    @apiName DisplayResizeEvent

    @apiParam {number}                          width		window size
    @apiParam {number}				height		window size
    
    @apiExample usage
    win.DisplayResizeEvent = function(w,h)
	print("new win size",w,h)
    end
*/
SWE.Window.TextureInvalidEvent
/**
    @api {window event: invalid texture} SWE.Window.TextureInvalidEvent() SWE.Window.TextureInvalidEvent
    @apiGroup SWE.Window.Events
    @apiName TextureInvalidEvent
    @apiDescription after resizing the display or rotate (for mobile os), all textures (if used) must be reinitialized, because the internal SDL render is changed

    @apiExample usage
    win.tx1 = SWE.Texture.Text(frs, text, SWE.Color.Red)
    ...
    win.TextureInvalidEvent = function()
	win.tx1 = SWE.Texture.Text(frs, text, SWE.Color.Red)
    end
*/
SWE.Window.WindowCreateEvent
/**
    @api {window event: single event constructor} SWE.Window.WindowCreateEvent() SWE.Window.WindowCreateEvent
    @apiGroup SWE.Window.Events
    @apiName WindowCreateEvent
    
    @apiExample usage
    win.WindowCreateEvent = function()
	print("win created")
    end
*/
SWE.Window.WindowCloseEvent
/**
    @api {window event: destroy object} SWE.Window.WindowCloseEvent() SWE.Window.WindowCloseEvent
    @apiGroup SWE.Window.Events
    @apiName WindowCloseEvent
    
    @apiExample usage
    win.WindowCloseEvent = function()
	print("win closed")
    end
*/
SWE.Window.KeyPressEvent
/**
    @api {window event: key event} SWE.Window.KeyPressEvent(keycode,keymod,scancode) SWE.Window.KeyPressEvent
    @apiGroup SWE.Window.Events
    @apiName KeyPressEvent
    
    @apiParam {number}                          keycode		SWE.Key constant
    @apiParam {number}                          keymod		key mod SDL_Keymod
    @apiParam {number}                          scancode	key scancode
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.KeyPressEvent = function(k,m,s)
	if k == SWE.Key.ESCAPE then
	    win:SetVisible(false)
	    print("hide window")
	end
	return true
    end
*/
SWE.Window.KeyReleaseEvent
/**
    @api {window event: key event} SWE.Window.KeyReleaseEvent(keycode) SWE.Window.KeyReleaseEvent
    @apiGroup SWE.Window.Events
    @apiName KeyReleaseEvent
    
    @apiParam {number}                          keycode		SWE.Key constant
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.KeyReleaseEvent = function(k)
	if k == SWE.Key.ESCAPE then
	    win:SetVisible(false)
	    print("hide window")
	end
	return true
    end
*/
SWE.Window.ScrollUpEvent
/**
    @api {window event: scroll event} SWE.Window.ScrollUpEvent(posx,posy) SWE.Window.ScrollUpEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.ScrollDownEvent
/**
    @api {window event: scroll event} SWE.Window.ScrollDownEvent(posx,posy) SWE.Window.ScrollDownEvent
    @apiGroup SWE.Window.Events
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
SWE.Window.SystemTickEvent
/**
    @api {window event: system tick} SWE.Window.SystemTickEvent(tick) SWE.Window.SystemTickEvent
    @apiGroup SWE.Window.Events
    @apiName SystemTickEvent
    
    @apiParam {number}                          tick		constantly increasing counter (SWE.MainLoop cycle), in milliseconds, from the starting

    @apiExample usage
    win.SystemTickEvent = function(tick)
	print("tick:",tick)
    end
*/
SWE.Window.SystemRenderEvent
/**
    @api {window event: render compete event} SWE.Window.SystemRenderEvent(ms) SWE.Window.SystemRenderEvent
    @apiGroup SWE.Window.Events
    @apiName SystemRenderEvent
    
    @apiParam {number}                          ms		render work milliseconds
*/
SWE.Window.SystemUserEvent
/**
    @api {window event: system user event} SWE.Window.SystemUserEvent(code,lightuserdata) SWE.Window.SystemUserEvent
    @apiGroup SWE.Window.Events
    @apiName SystemUserEvent
    
    @apiParam {number}                          code		user code (0x00FFFFFF max value), see SWE.pushEvent
    @apiParam {table}                           object		any Lua object
    @apiSuccess (Return) {boolean}		result		success if processing

    @apiExample usage
    win.SystemUserEvent = function(code,data)
	print("code:",code,type(data))
    end
*/
SWE.Window.RenderWindow
/**
    @api {render window code} SWE.Window.RenderWindow() SWE.Window.RenderWindow
    @apiGroup SWE.Window.Virtual
    @apiName RenderWindow
    
    @apiSuccess (Return) {boolean}		result		always true

    @apiExample usage
    win.RenderWindow = function()
	win:RenderClear(SWE.Color.Silver)
	win:RenderRect(SWE.Color.Blue,0,0,win.width,win.height)
	win:RenderText(frs,"Hello World!",SWE.Color.Red,win.width/2,win.height/2,SWE.Align.Center,SWE.Align.Center)
	return true
    end
*/
SWE.Window.ToJson
/**
    @api {convert window to json string} SWE.Window.ToJson(self) SWE.Window.ToJson
    @apiGroup SWE.Window
    @apiName ToJson
            
    @apiParam {SWE.Window}                      self            window object
    @apiSuccess (Return) {string}               result          json string
*/
SWE.Window.IsVisible
/**
    @api {window is visible} SWE.Window.IsVisible() SWE.Window.IsVisible
    @apiGroup SWE.Window
    @apiName IsVisible
            
    @apiParam {SWE.Window}                      self            window object
    @apiSuccess (Return) {boolean}              result          true if visible
*/
SWE.Window.PointInArea
/**
    @api {point in window area} SWE.Window.PointInArea(self,ptx,pty)(self,point) SWE.Window.PointInArea
    @apiGroup SWE.Window
    @apiName PointInArea
            
    @apiParam {SWE.Window}                      self            window object
    @apiParam {number}                          ptx             point coord
    @apiParam {number}                          pty             point coord
    @apiParam {SWE.Point}                       point		point object
    @apiSuccess (Return) {boolean}              result          true if success
*/
SWE.Window.SetToolTip
/**
    @api {set tooltip info} SWE.Window.SetToolTip(self,text,frs,colorFn,colorBg,colorRt) SWE.Window.SetToolTip
    @apiGroup SWE.Window
    @apiName SetToolTip
    
    @apiParam {SWE.Window}                      self            window object
    @apiParam {string}                          text            tooltip text
    @apiParam {SWE.FontRender}                  frs             fontrender object (default: system)
    @apiParam {number}                          colorFn         SWE.Color constant, (default: black)
    @apiParam {number}                          colorBg         SWE.Color constant, (default: wheat)
    @apiParam {number}                          colorRt         SWE.Color constant, (default: midnightblue)

    @apiExample usage
    win:SetToolTip("Hello World!")
*/
SWE.Window.GetChildrens
/**
    @api {get children windows} SWE.Window.GetChildrens(self) SWE.Window.GetChildrens
    @apiGroup SWE.Window
    @apiName GetChildrens
    
    @apiParam {SWE.Window}                      self            window object
    @apiSuccess (Return) {table}                result          children windows list

    @apiExample usage
    local childs = win:GeChildrens()
    for i = 1,#childs do
	SWE.Dump(childs[i])
    end
*/

SWE.MouseButton.EnumConstant
/**
    @api {MouseButton enums} SWE.MouseButton.<EnumConstant> SWE.MouseButton.<EnumConstant>
    @apiGroup SWE.MouseButton
    @apiName <EnumConstant>

    @apiParam {number}      Left		mouse button left
    @apiParam {number}      Right		mouse button right
    @apiParam {number}      Middle		mouse button middle
    @apiParam {number}      X1			mouse button x1
    @apiParam {number}      X2			mouse button x2
**/

SWE.Scene.MoveTop
/**
    @api {scene: move window to top layer} SWE.Scene.MoveTop(win) SWE.Scene.MoveTop
    @apiGroup SWE.Scene
    @apiName MoveTop
    
    @apiParam {SWE.Window}                      win             window object
    @apiSuccess (Return) {boolean}              result          success operation
*/
SWE.Scene.Remove
/**
    @api {scene: remove window} SWE.Scene.Remove(win) SWE.Scene.Remove
    @apiGroup SWE.Scene
    @apiName Remove
    
    @apiParam {SWE.Window}                      win             window object
    @apiSuccess (Return) {boolean}              result          success operation
*/
