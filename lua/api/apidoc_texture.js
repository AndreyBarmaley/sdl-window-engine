SWE.Texture
/**
    @api {texture constructor} SWE.Texture(),(width,height) SWE.Texture
    @apiGroup SWE.Texture
    @apiName Constructor
            
    @apiParam {number}			width		texture size
    @apiParam {number}			height		texture size
    @apiSuccess (Return) {SWE.Texture}	result		texture object

    @apiExample usage
    local tx = SWE.Texture(200, 100)
    tx:RenderClear(SWE.Color.Red);
    print(tx.width,tx.height)

    @apiExample ext table fields (read only)
    | width    | number  | texture size
    | height   | number  | texture size
*/
SWE.Texture.SaveToFile
/**
    @api {texture save to file} SWE.Texture.SaveToFile(filename) SWE.Texture.SaveToFile
    @apiGroup SWE.Texture
    @apiName SaveToFile
    
    @apiParam {string}			filename	save to filename
    @apiParam {boolean}			result		if success
*/
SWE.Texture.RenderClear
/**
    @api {texture clear} SWE.Texture.RenderClear(self,color) SWE.Texture.RenderClear
    @apiGroup SWE.Texture
    @apiName RenderClear
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {number}                          color		SWE.Color
*/
SWE.Texture.RenderRect
/**
    @api {texture render rect} SWE.Texture.RenderRect(self,color,rtx,rty,rtw,rth,filled)(self,color,rect,filled) SWE.Texture.RenderRect
    @apiGroup SWE.Texture
    @apiName RenderRect
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				rtx		rect position
    @apiParam {number}				rty		rect position
    @apiParam {number}				rtw		rect size
    @apiParam {number}				rth		rect size
    @apiParam {SWE.Rect}			rect		rect object
    @apiParam {boolean}				filled		fill mode (default false)
*/
SWE.Texture.RenderLine
/**
    @api {texture render line} SWE.Texture.RenderLine(self,color,px1,py1,px2,py2)(self,color,point1,point2) SWE.Texture.RenderLine
    @apiGroup SWE.Texture
    @apiName RenderLine
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				px1		point1 position
    @apiParam {number}				py1		point1 position
    @apiParam {number}				px2		point2 position
    @apiParam {number}				py2		point2 position
    @apiParam {SWE.Point}			point1		point object
    @apiParam {SWE.Point}			point2		point object
*/
SWE.Texture.RenderCyrcle
/**
    @api {texture render cyrcle} SWE.Texture.RenderCyrcle(self,color,centerx,centery,radius,filled)(self,color,point_center,radius,filled) SWE.Texture.RenderCyrcle
    @apiGroup SWE.Texture
    @apiName RenderCyrcle
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				centerx		center position
    @apiParam {number}				centery		center position
    @apiParam {SWE.Point}			point_center	point object
    @apiParam {number}				radius		radius value
    @apiParam {boolean}				filled		fill mode (default false)
*/
SWE.Texture.RenderPoint
/**
    @api {texture render point} SWE.Texture.RenderPoint(self,color,ptx,pty)(self,color,point) SWE.Texture.RenderPoint
    @apiGroup SWE.Texture
    @apiName RenderPoint
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				ptx		point position
    @apiParam {number}				pty		point position
    @apiParam {SWE.Point}			point		point object
*/
SWE.Texture.RenderText
/**
    @api {texture render text} SWE.Texture.RenderText(self,frs,color,posx,posy,halign,valign,horizontal)(self,frs,color,point,halign,valign,horizontal,render,style,hinting) SWE.Texture.RenderText
    @apiGroup SWE.Texture
    @apiName RenderText
    
    @apiParam {SWE.Texture}                     self            texture object
    @apiParam {SWE.FontRender}                  frs		fontrender object
    @apiParam {number}                          color		SWE.Color
    @apiParam {number}				posx		text position
    @apiParam {number}				posy		text position
    @apiParam {SWE.Point}			point		point object
    @apiParam {number}				halign		align text (default SWE.Align.Left)
    @apiParam {number}				valign		align text (default SWE.Align.Top)
    @apiParam {boolean}				horizontal	text orientation, default true
    @apiParam {number}				render		text render (default RenderDefault, used from FontRender), see SWE.Font
    @apiParam {number}				style		text style (default StyleDefault, used from FontRender), see SWE.Font
    @apiParam {number}				hinting		text hinting (default HintingDefault, used from FontRender) see SWE.Font

    @apiExample usage
    local frs = SWE.FontRender("dejavusans.ttf", 24)
    local image1 = SWE.Texture.Image("logo.png")
    image1:RenderText(frs,SWE.Color.Red,"APPROVED",image1.width/2,image1.height/2,SWE.Align.Center,SWE.Align.Center)
*/
SWE.Texture.RenderTexture
/**
    @api {texture scaled render to texture} SWE.Texture.RenderTexture(self,texture),(self,texture,srcx,srcy,srcw,srch,dstx,dsty,dstw,dsth)(self,texture,rect_src,rect_dst) SWE.Texture.RenderTexture
    @apiGroup SWE.Texture
    @apiName RenderTexture
    
    @apiParam {SWE.Texture}                     self            dst texture object
    @apiParam {SWE.Texture}                     texture         src texture object
    @apiParam {number}				srcx		src position
    @apiParam {number}				srcy		src position
    @apiParam {number}				srcw		src size
    @apiParam {number}				srch		src size
    @apiParam {SWE.Rect}			rect_src	src rect object
    @apiParam {number}				dstx		dst position
    @apiParam {number}				dsty		dst position
    @apiParam {number}				dstw		dst size (default srcw)
    @apiParam {number}				dsth		dst size (default srch)
    @apiParam {SWE.Rect}			rect_dst	dst rect object

    @apiExample usage
    local image1 = SWE.Texture.Image("logo.png")
    local image2 = SWE.Texture.Texture(200,100)
    -- copy texture area(50, 50, 50, 50) from image1 to texture image2 dst pos(100, 50)
    image2:RenderTexture(image1,50,50,50,50,100,50)
    print(image2.width,image2.height)
*/
SWE.Texture.Rect
/**
    @api {texture constructor} SWE.TextureRect(colrt,colbg,width,height,thickness) SWE.Texture.Rect
    @apiGroup SWE.Texture.Rect
    @apiName Constructor
            
    @apiParam {number}			colrt		rect color, SWE.Color
    @apiParam {number}			colbg		fill color, SWE.Color
    @apiParam {number}			width		texture size
    @apiParam {number}			height		texture size
    @apiParam {number}			thickness	line thickness (default 1)
    @apiSuccess (Return) {SWE.Texture}	result		texture object

    @apiExample usage
    local tx = SWE.Texture.Rect(SWE.Color.Red,SWE.Color.Blue,200,100)
    print(tx.width,tx.height)

    @apiExample ext table fields (read only)
    | width    | number  | texture size
    | height   | number  | texture size
*/
SWE.Texture.Text
/**
    @api {texture constructor} SWE.Texture.Text(frs,text,coltx,colbg) SWE.Texture.Text
    @apiGroup SWE.Texture.Text
    @apiName Constructor
            
    @apiParam {SWE.FontRender}		frs		fontrender object
    @apiParam {string}			text		text string
    @apiParam {number}			coltx		text color, SWE.Color
    @apiParam {number}			colbg		fill backgroung color, SWE.Color (default Transparent)
    @apiSuccess (Return) {SWE.Texture}	result		texture object

    @apiExample usage
    local frs = SWE.FontRender("dejavusans.ttf", 14)
    local tx = SWE.Texture.Text(frs,"Hello World!"SWE.Color.Red,SWE.Color.Blue)
    print(tx.width,tx.height)

    @apiExample ext table fields (read only)
    | width    | number  | texture size
    | height   | number  | texture size
*/
SWE.Texture.Image
/**
    @api {texture constructor} SWE.Texture.Image(filename,cropx,cropy,cropw,croph,colorkey) SWE.Texture.Image
    @apiGroup SWE.Texture.Image
    @apiName Constructor
            
    @apiParam {string}			filename	filename image format
    @apiParam {number}			cropx		crop size
    @apiParam {number}			cropy		crop size
    @apiParam {number}			cropw		crop size
    @apiParam {number}			croph		crop size
    @apiParam {number}			colorkey	sdl color key value (if defined)
    @apiSuccess (Return) {SWE.Texture}	result		texture object

    @apiExample usage
    local image1 = SWE.Texture.Image("image.png")
    local image2 = SWE.Texture.Image("image.png",10,10,40,20)
    print(image2.width,image2.height)

    @apiExample ext table fields (read only)
    | width    | number  | texture size
    | height   | number  | texture size
*/
SWE.Texture.ToJson
/**
    @api {convert texture to json string} SWE.Texture.ToJson(self) SWE.Texture.ToJson
    @apiGroup SWE.Texture
    @apiName ToJson
            
    @apiParam {SWE.Texture}                     self            texture object
    @apiSuccess (Return) {string}               result          json string
*/
