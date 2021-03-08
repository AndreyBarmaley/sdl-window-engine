function ToInt(x)
    local integral, fractal = math.modf(x)
    return integral
end

function StringSplit(str, sep)
   local result = {}
   local regex = ("([^%s]+)"):format(sep)
   for each in str:gmatch(regex) do
      table.insert(result, each)
   end
   return result
end

function ToolsKeyExit(key)
    if key == SWE.Key.ESCAPE then
        return true
    end

    local mobos = SWE.SystemMobileOs()

    if mobos ~= nil then
        if key == SWE.Key.ANDROID_BACK and mobos == "android" then
            return true
        end

        if key == SWE.Key.WINCE_EXIT and mobos == "wince" then
            return true
        end
    end
end
