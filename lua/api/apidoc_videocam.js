SWE.VideoCam
/**
    @api {videocam constructor} SWE.VideoCam(params) SWE.VideoCam
    @apiGroup SWE.VideoCam
    @apiName Constructor
            
    @apiParam {table}			params		ffmpeg init params
    @apiSuccess (Return) {SWE.VideoCam}	result		videocam object

    @apiExample usage
    local win = SWE.DisplayInit("VideoCam Example", 640, 480, fullscreen)

    -- init videocam object
    local params = { format = "v4l2", device = "/dev/video0", debug = true, video_standard = "pal", video_size = "640x480" }
    local cam = SWE.VideoCam(params)

    function win.RenderWindow()
	local frame = cam:GetFrame()
	if frame ~= nil then
    	    win:RenderTexture(frame, 0, 0, frame.width, frame.height, 0, 0, win.width, win.height)
	else
	    -- no signal
    	    win:RenderClear(SWE.Color.Blue)
	end
	return true
    end

    function win.SystemTickEvent()
	-- repeat redraw scene
	SWE.DisplayDirty()
	return true
    end

    function win.KeyPressEvent(key,mod,scancode)
	if SWE.Key.ESCAPE == key then
    	    -- close scene, see SWE.MainLoop
    	    win:SetVisible(false)
    	    return true
	end
	return false
    end

    SWE.MainLoop(win)
*/
SWE.VideoCam.GetFrame
/**
    @api {capture frame} SWE.VideoCam.GetFrame(self) SWE.VideoCam.GetFrame
    @apiGroup SWE.VideoCam
    @apiName GetFrame

    @apiParam {SWE.VideoCam}			self		videocam object
    @apiSuccess (Return) {SWE.Texture}		result		capture texture frame
*/
SWE.VideoCam.ToJson
/**
    @api {convert videocam to json string} SWE.VideoCam.ToJson(self) SWE.VideoCam.ToJson
    @apiGroup SWE.VideoCam
    @apiName ToJson

    @apiParam {SWE.VideoCam}                    self            videocam object
    @apiSuccess (Return) {string}               result          json string
*/
