#!/usr/bin/env ruby

class Hash
        def set k
                self[k] = self.size if !self[k]
                return self[k]
        end
end

class Array
        def pu
                self.each{|i| print i.to_s + ' '}
                puts
        end
end

$vert = {}
$ends = []

def make_array_of_rules s
        if s.size == 1
                $ends.push(s[0])
                return [s]
        elsif s.size == 2
                if ('a'..'z').include?(s[1])
                        $ends.push($vert.set($vert.size))
                        return [[s[0], $vert.size - 1, s[1]]]
                else
                        return [s]
                end
        else
                if ('a'..'z').include?(s[1]) && ('a'..'z').include?(s[2])
                        k = $vert.set($vert.size)
                        return [[s[0], k, s[1]]] + make_array_of_rules([k] + s[2..s.size])
                else
                        return [[s[0], s[2], s[1]]]
                end
        end
end

ans = []
while (s = gets) do
        s.chomp!
        t = [$vert.set(s[0].chr)]
        (2..s.length - 1).each{|c| t.push(('A'..'Z').include?(s[c].chr) ? $vert.set(s[c].chr) : s[c].chr)}
        ans += make_array_of_rules(t)
end

puts $vert.size
$ends.pu
ans.each{|a| a.pu if a.size > 1}
