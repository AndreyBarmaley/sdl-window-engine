SWE.UnicodeRegex
/**
    @api {unicoderegex constructor} SWE.UnicodeRegex(pattern) SWE.UnicodeRegex
    @apiGroup SWE.UnicodeRegex
    @apiName Constructor
            
    @apiParam {string}				pattern	        regex format
    @apiSuccess (Return) {SWE.UnicodeRegex}	result		unicoderegex data

    @apiExample usage Search
    local ureg = SWE.UnicodeRegex("([a-z]+)\\.([a-z]+)")
    local fnames = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"}

    for i = 1, #fnames do
        print(ureg2:Match(fnames[i]))
    end

    -- output result:
    -- 0       7       foo     txt
    -- 0       7       bar     txt
    -- 0       7       baz     dat
    -- nil

    @apiExample usage FindAll
    local ustr = SWE.UnicodeString("Quick brown fox.")
    local ureg = SWE.UnicodeRegex("[^\\s]+")

    local t = ureg:FindAll(ustr)

    -- table content:
    -- | 0 | 5 | 6 | 5 | 12 | 4 |

    for i = 1,#t,2 do
        print(ustr:SubString(t[i], t[i+1]):ToUtf8String())
    end

    -- output result:
    -- Quick
    -- brown
    -- fox.
*/
SWE.UnicodeRegex.Assign
/**
    @api {regex assign} SWE.UnicodeRegex.Assign(self,str) SWE.UnicodeRegex.Assign
    @apiGroup SWE.UnicodeRegex
    @apiName Assign

    @apiParam {SWE.UnicodeRegex}     		self   	        SWE.UnicodeRegex obj
    @apiParam {string}             		str   	        pattern
*/
SWE.UnicodeRegex.Search
/**
    @api {regex search} SWE.UnicodeRegex.Search(self,ustr,init) SWE.UnicodeRegex.Search
    @apiGroup SWE.UnicodeRegex
    @apiName Search

    @apiParam {SWE.UnicodeRegex}     		self   	        SWE.UnicodeRegex obj
    @apiParam {SWE.UnicodeString}     		ustr   	        SWE.UnicodeString content
    @apiParam {number}     		        init   	        offset (first: 0, last: -1)
    @apiSuccess (Return) {list}    		result   	search result, same as std::regex_search (offset, length, match, match, ...match)
*/
SWE.UnicodeRegex.Match
/**
    @api {regex match} SWE.UnicodeRegex.Match(self,ustr,init) SWE.UnicodeRegex.Match
    @apiGroup SWE.UnicodeRegex
    @apiName Match

    @apiParam {SWE.UnicodeRegex}     		self   	        SWE.UnicodeRegex obj
    @apiParam {SWE.UnicodeString}     		ustr   	        SWE.UnicodeString content
    @apiParam {number}     		        init   	        offset (first: 0, last: -1)
    @apiSuccess (Return) {list}    		result   	search result, same as std::regex_match (offset, length, match, match, ...match)
*/
SWE.UnicodeRegex.FindAll
/**
    @api {regex find all} SWE.UnicodeRegex.FindAll(self,ustr) SWE.UnicodeRegex.FindAll
    @apiGroup SWE.UnicodeRegex
    @apiName FindAll

    @apiParam {SWE.UnicodeRegex}     		self   	        SWE.UnicodeRegex obj
    @apiParam {SWE.UnicodeString}     		ustr   	        SWE.UnicodeString content
    @apiSuccess (Return) {table}    		result   	search result pairs, (offset, length, offset, length, ...offset, length)
*/
