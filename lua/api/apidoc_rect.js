SWE.Point
/**
    @api {point constructor} SWE.Point(posx,posy)SWE.Point(string) SWE.Point
    @apiGroup SWE.Point
    @apiName Constructor
            
    @apiParam {number}				posx		position
    @apiParam {number}				posy		position
    @apiParam {string}				string		parse string (delimiter: any non digital)
    @apiSuccess (Return) {SWE.Point}		result		point object

    @apiExample usage
    local pt = SWE.Point(100, 200)
    print(pt.posx,pt.posy)

    @apiExample ext table fields (read write)
    | posx    | number  |
    | posy    | number  |
*/
SWE.Point.ToJson
/**
    @api {convert point to json string} SWE.Point.ToJson(self) SWE.Point.ToJson
    @apiGroup SWE.Point
    @apiName ToJson

    @apiParam {SWE.Point}			self		point object
    @apiSuccess (Return) {string}		result		json string

    @apiExample usage
    local pt = SWE.Point(10, 20)
    print(pt:ToJson())
    .....
    '{"posx":10,"posy":20}'
*/
SWE.Point.Unpack
/**
    @api {get list point fields} SWE.Point.Unpack(self) SWE.Point.Unpack
    @apiGroup SWE.Point
    @apiName Unpack
            
    @apiParam {SWE.Point}			self		point object
    @apiSuccess (Return) {list}			values		coord values (posx, posy)

    @apiExample usage
    local pt = SWE.Point(10, 20)
    print(pt:Unpack())
    .....
    10    20
*/

SWE.Size
/**
    @api {size constructor} SWE.Size(width,height)SWE.Size(string) SWE.Size
    @apiGroup SWE.Size
    @apiName Constructor
            
    @apiParam {number}				width		size value
    @apiParam {number}				height		size value
    @apiParam {string}				string		parse string (delimiter: any non digital)
    @apiSuccess (Return) {SWE.Size}		result		size object

    @apiExample usage
    local sz = SWE.Size(100, 200)
    print(sz.width,sz.height)

    @apiExample ext table fields (read write)
    | width   | number  |
    | height  | number  |
*/
SWE.Size.ToJson
/**
    @api {convert size to json string} SWE.Size.ToJson(self) SWE.Size.ToJson
    @apiGroup SWE.Size
    @apiName ToJson

    @apiParam {SWE.Size}			self		size object
    @apiSuccess (Return) {string}		result		json string

    @apiExample usage
    local sz = SWE.Size(100, 200)
    print(sz:ToJson())
    .....
    '{"width":100,"height":200}'
*/
SWE.Size.Unpack
/**
    @api {get list size fields} SWE.Size.Unpack(self) SWE.Size.Unpack
    @apiGroup SWE.Size
    @apiName Unpack
            
    @apiParam {SWE.Size}			self		size object
    @apiSuccess (Return) {list}			values		size values (width, height)
*/

SWE.Rect
/**
    @api {rect constructor} SWE.Rect(posx,posy,width,height)SWE.Rect(string) SWE.Rect
    @apiGroup SWE.Rect
    @apiName Constructor
            
    @apiParam {number}				posx		position
    @apiParam {number}				posy		position
    @apiParam {number}				width		size
    @apiParam {number}				height		size
    @apiParam {string}				string		parse string (delimiter: any non digital)
    @apiSuccess (Return) {SWE.Rect}		result		rect object

    @apiExample usage
    local rt = SWE.Rect(10, 20, 100, 200)
    print(rt.posx,rt.posy,rt.width,rt.height)

    @apiExample ext table fields (read write)
    | posx    | number  |
    | posy    | number  |
    | width   | number  |
    | height  | number  |
*/
SWE.Rect.ToJson
/**
    @api {convert rect to json string} SWE.Rect.ToJson(self) SWE.Rect.ToJson
    @apiGroup SWE.Rect
    @apiName ToJson

    @apiParam {SWE.Rect}			self		rect object
    @apiSuccess (Return) {string}		result		json string

    @apiExample usage
    local rt = SWE.Rect(10, 20, 100, 200)
    print(rt:ToJson())
    .....
    '{"posx":10,"posy":20,"width":100,"height":200}'
*/
SWE.Rect.Unpack
/**
    @api {get list rect fields} SWE.Rect.Unpack(self) SWE.Rect.Unpack
    @apiGroup SWE.Rect
    @apiName Unpack

    @apiParam {SWE.Rect}			self		rect object
    @apiSuccess (Return) {list}			values		rect values (posx,posy,width, height)
*/
SWE.Rect.PointInRect
/**
    @api {check point in rect} SWE.Rect.PointInRect(self,point) SWE.Rect.PointInRect
    @apiGroup SWE.Rect
    @apiName PointInRect

    @apiParam {SWE.Rect}			self		rect object
    @apiParam {SWE.Point}			point		point object
    @apiSuccess (Return) {boolean}		result		true if point resides inside rectangle

    @apiExample usage
    local rt = SWE.Rect(10,20,100, 200)
    local pt = SWE.Point(150, 150)
    local res = rt:PointInRect(pt)
    print("point in rect",res)
*/
SWE.Rect.HasIntersection
/**
    @api {check has intersection rects} SWE.Rect.HasIntersection(rect1,rect2) SWE.Rect.HasIntersection
    @apiGroup SWE.Rect
    @apiName HasIntersection

    @apiParam {SWE.Rect}			rect1		rect object
    @apiParam {SWE.Rect}			rect2		rect object
    @apiSuccess (Return) {boolean}		result		true if there is an intersection

    @apiExample usage
    local rt1 = SWE.Rect(10,20,100, 200)
    local rt2 = SWE.Rect(50,80,100, 200)
    local res = rt1:HasIntersection(rt2)
    print("has intersection",res)
*/
SWE.Rect.GetIntersectRect
/**
    @api {get intersect rects} SWE.Rect.GetIntersectRect(rect1,rect2) SWE.Rect.GetIntersectRect
    @apiGroup SWE.Rect
    @apiName GetIntersectRect

    @apiParam {SWE.Rect}			rect1		rect object
    @apiParam {SWE.Rect}			rect2		rect object
    @apiSuccess (Return) {SWE.Rect}		result		rect intersect

    @apiExample usage
    local rt1 = SWE.Rect(10,20,100, 200)
    local rt2 = SWE.Rect(50,80,100, 200)
    local res = rt1:GetIntersectRect(rt2)
    print("intersect rect",res.ToJson())
*/
SWE.Rect.RectEquals
/**
    @api {check rect equals} SWE.Rect.RectEquals(rect1,rect2) SWE.Rect.RectEquals
    @apiGroup SWE.Rect
    @apiName RectEquals

    @apiParam {SWE.Rect}			rect1		rect object
    @apiParam {SWE.Rect}			rect2		rect object
    @apiSuccess (Return) {boolean}		result		true if the two rectangles are exactly the same
*/
