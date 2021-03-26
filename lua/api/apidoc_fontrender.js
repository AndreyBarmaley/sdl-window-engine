SWE.FontRender
/**
    @api {fontrender constructor} SWE.FontRender(filename,size,blended,style,hinting) SWE.FontRender
    @apiGroup SWE.FontRender
    @apiName Constructor
            
    @apiParam {string}				filename	ttf font
    @apiParam {number}				size		font size
    @apiParam {number}				blended		blended mode (default: RenderBlended), see SWE.Font
    @apiParam {number}				style		style enum, (default: StyleNormal), see SWE.Font
    @apiParam {number}				hinting		hinting enum, (default: HintingNormal), see SWE.Font
    @apiSuccess (Return) {SWE.FontRender}	result		fontrender object

    @apiExample usage
    local frs = SWE.FontRender("dejavusans.ttf", 14)
    print(frs.font,frs.size)

    @apiExample ext table fields (read only)
    | font       | string |
    | size       | number |
    | blended    | number |
    | style      | number |
    | hinting    | number |
    | fixedWidth | number | for fixed font return SymbolAdvance(0x20)
    | lineHeight | number | skip line height
*/
SWE.FontRender.SymbolAdvance
/**
    @api {get symbol advance value} SWE.FontRender.SymbolAdvance(self,symbol) SWE.FontRender.SymbolAdvance
    @apiGroup SWE.FontRender
    @apiName SymbolAdvance
            
    @apiParam {SWE.FontRender}			self		fontrender object
    @apiParam {number}				symbol		symbol chars
    @apiSuccess (Return) {number}		result		advance value
*/
SWE.FontRender.StringSize
/**
    @api {get render size} SWE.FontRender.StringSize(self,content,horizontal) SWE.FontRender.StringSize
    @apiGroup SWE.FontRender
    @apiName StringSize
            
    @apiParam {SWE.FontRender}			self		fontrender object
    @apiParam {string}				content		string content
    @apiParam {boolean}				horizontal	render is horizontal
    @apiSuccess (Return) {SWE.Size}		result		render size
*/
SWE.FontRender.UnicodeStringSize
/**
    @api {get render size} SWE.FontRender.UnicodeStringSize(self,content,horizontal) SWE.FontRender.UnicodeStringSize
    @apiGroup SWE.FontRender
    @apiName UnicodeStringSize
            
    @apiParam {SWE.FontRender}			self		fontrender object
    @apiParam {SWE.UnicodeString}		content		unicode content
    @apiParam {boolean}				horizontal	render is horizontal
    @apiSuccess (Return) {SWE.Size}		result		render size
*/
SWE.FontRender.SplitStringWidth
/**
    @api {split string width} SWE.FontRender.SplitStringWidth(self,content,width) SWE.FontRender.SplitStringWidth
    @apiGroup SWE.FontRender
    @apiName SplitStringWidth
            
    @apiParam {SWE.FontRender}			self		fontrender object
    @apiParam {string}				content		string content
    @apiParam {number}				width		width
    @apiSuccess (Return) {table}		result		string list result
*/
SWE.FontRender.ToJson
/**
    @api {convert fontrender to json string} SWE.FontRender.ToJson(self) SWE.FontRender.ToJson
    @apiGroup SWE.FontRender
    @apiName ToJson
            
    @apiParam {SWE.FontRender}			self		fontrender object
    @apiSuccess (Return) {string}		result		json string
*/
SWE.Font.EnumConstant
/**
    @api {color enums} SWE.Font.<EnumConstant> SWE.Font.<EnumConstant>
    @apiGroup SWE.Font
    @apiName <EnumConstant>

    @apiParam {number}     RenderSolid		font render: solid
    @apiParam {number}     RenderBlended	font render: blended
    @apiParam {number}     RenderShaded		font render: shaded
    @apiParam {number}     StyleNormal		font style: normal
    @apiParam {number}     StyleBold		font style: bold
    @apiParam {number}     StyleItalic		font style: italic
    @apiParam {number}     StyleUnderLine	font style: under line
    @apiParam {number}     StyleStrikeThrough	font style: strike through

    @apiParam {number}     HintingNormal	font hinting: normal
    @apiParam {number}     HintingLight		font hinting: light
    @apiParam {number}     HintingMono		font hinting: mono
    @apiParam {number}     HintingNone		font hinting: none
*/
SWE.FontRender.System
/**
    @api {fontrender system constructor} SWE.FontRender.System() SWE.FontRender.System
    @apiGroup SWE.FontRender.System
    @apiName Constructor
            
    @apiSuccess (Return) {SWE.FontRender}	result		fontrender object internal generated from "alt-8x16.psf"

    @apiExample usage
    local frs = SWE.FontRender.System()
    print(frs.font,frs.size)

    @apiExample ext table fields (read only)
    | font    | string  | system |

    | fixedWidth | number | for fixed font return SymbolAdvance(0x20)
    | lineHeight | number | skip line height
*/
