SWE.Signal.EnumConstant
/**
    @api {signal enums} SWE.Signal.<EnumConstant> SWE.Signal.<EnumConstant>
    @apiGroup SWE.Signal
    @apiName <EnumConstant>

    @apiParam {number}		GestureFingerUp		gesture signal: finger up
    @apiParam {number}		GestureFingerDown	gesture signal: finger down
    @apiParam {number}		GestureFingerLeft	gesture signal: finger left
    @apiParam {number}		GestureFingerRight	gesture signal: finger right
    @apiParam {number}		FingerMoveUp		finger signal: move up
    @apiParam {number}		FingerMoveDown		finger signal: move down
    @apiParam {number}		FingerMoveLeft		finger signal: move left
    @apiParam {number}		FingerMoveRight		finger signal: move right

    @apiExample usage
    local landscape = false
    local win = SWE.DisplayInit("GestureTest", landscape)

    win.SystemUserEvent = function(a,b)
        if a == SWE.Signal.GestureFingerUp then
            SWE.Debug("GestureFingerUp")
	    return true
        elseif a == SWE.Signal.GestureFingerDown then
            SWE.Debug("GestureFingerDown")
	    return true
        elseif a == SWE.Signal.GestureFingerLeft then
            SWE.Debug("GestureFingerLeft")
	    return true
        elseif a == SWE.Signal.GestureFingerRight then
            SWE.Debug("GestureFingerRight")
	    return true
        elseif a == SWE.Signal.FingerMoveUp then
            SWE.Debug("FingerMoveUp")
	    return true
        elseif a == SWE.Signal.FingerMoveDown then
            SWE.Debug("FingerMoveDown")
	    return true
        elseif a == SWE.Signal.FingerMoveLeft then
            SWE.Debug("FingerMoveLeft")
	    return true
        elseif a == SWE.Signal.FingerMoveRight then
            SWE.Debug("FingerMoveRight")
	    return true
        end
	return false
    end

    SWE.MainLoop(win)
*/
SWE.Signal.ToString
/**
    @api {get signal string} SWE.Signal.ToString(signal) SWE.Signal.ToString
    @apiGroup SWE.Signal
    @apiName ToString

    @apiParam {number}              signal           signal enum
    @apiSuccess (Return) {string}   name             signal name
*/
