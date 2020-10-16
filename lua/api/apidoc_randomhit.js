SWE.RandomHit
/**
    @api {randomhit constructor} SWE.RandomHit(chance) SWE.RandomHit
    @apiGroup SWE.RandomHit
    @apiName Constructor
            
    @apiParam {number}				chance		hit chance (1 - 99)
    @apiSuccess (Return) {SWE.RandomHit}	result		randomhit object

    @apiExample usage
    local hit = SWE.RandomHit(33)
    print(hit:ToJson())

    for i = 1,100 do
	local check = hit:Check()
    end

    local check = hit:Check()
    print(hit:ToJson())

    for i = 2,100 do
	local check = hit:Check()
    end

    local check = hit:Check()
    print(hit:ToJson())

    for i = 2,100 do
	local check = hit:Check()
    end

    @apiExample ext table fields (read only)
    | chance  | number  |
*/
SWE.RandomHit.Check
/**
    @api {check iteration hits} SWE.RandomHit.Check(self) SWE.RandomHit.Check
    @apiGroup SWE.RandomHit
    @apiName Check

    @apiParam {SWE.RandomHit}			self		randomhit object
    @apiSuccess (Return) {boolean}		result		true if hit
*/
SWE.RandomHit.Last
/**
    @api {get last checked} SWE.RandomHit.Last(self) SWE.RandomHit.Last
    @apiGroup SWE.RandomHit
    @apiName Last
            
    @apiParam {SWE.RandomHit}			self		randomhit object
    @apiSuccess (Return) {list}			result		last check value
*/
SWE.RandomHit.ToJson
/**
    @api {convert randomhit to json string} SWE.RandomHit.ToJson(self) SWE.RandomHit.ToJson
    @apiGroup SWE.RandomHit
    @apiName ToJson

    @apiParam {SWE.RandomHit}                   self            randomhit object
    @apiSuccess (Return) {string}               result          json string
*/
