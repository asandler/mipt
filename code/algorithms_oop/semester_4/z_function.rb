#!/usr/bin/env ruby

def zf s
    z = [1]
    l, r = 0, 0
    (1..s.length - 1).each do |i|
        if (l..r).include? i
            j = i - l
            if i + z[j] <= r
                z << z[j]
            else
                t = r
                while t < s.length and s[t] == s[t - i]
                    t += 1
                end
                z << t - l
                l, r = i, t - 1
            end
        else
            t = 0
            while t < s.length and s[t] == s[i + t]
                t += 1
            end
            z << t
        end
    end
    z
end

p zf("aaaaaaaaaaaaa")
