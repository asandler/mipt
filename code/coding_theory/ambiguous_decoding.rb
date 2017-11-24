$code = gets.chomp.split.uniq
v = ['']
$code.each do |s|
    (0..s.size - 2).each{ |i| v << s[0..i] }
    (1..s.size - 1).each{ |i| v << s[i..-1] }
end
v = (v - $code).uniq
$code_len_max = $code.map{|s| s.size}.max
$edges = Hash.new('')
$cstar = {}

def cstar_mem beta_cur = ''
    $cstar[beta_cur] = true
    $code.each do |s|
        cstar_mem(beta_cur + s) if (beta_cur + s).size <= $code_len_max
    end
end

def edge_check v1, v2
    return nil if v1 == v2
    $code.each do |s|
        p1, p2 = s.index(v1), s.rindex(v2)
        next if not p1 or not p2 or p1 > 0 or p2 + v2.size != s.size or v1.size + v2.size > s.size
        if $cstar[s[p1 + v1.size..p2 - 1]]
            $edges[[v1, v2]] = s[p1 + v1.size..p2-1]
            return s.size - v1.size
        end
    end
    return nil
end

def solve v, h
    d = {'' => 0}
    d.default = 10e9
    ancestors = {}
    (v.size - 1).times do |i|
        h.each do |key, val|
            if d[key[1]] > d[key[0]] + val
                d[key[1]] = d[key[0]] + val
                ancestors[key[1]] = key[0]
            end
        end
    end
    min = 10e9
    answer = ''
    v.each do |s|
        if min > d[s] + h[[s, '']]
            min = d[s] + h[[s, '']]
            go = s
            answer = $edges[[s, '']]
            while go != ''
                answer = $edges[[ancestors[go], go]] + go + answer
                go = ancestors[go]
            end
        end
    end
    answer
end

h = Hash.new(10e9)
cstar_mem
v.each do |v1|
    v.each do |v2|
        t = edge_check(v1, v2)
        h[[v1, v2]] = t if t
    end
end
puts "Graph built"
puts solve v, h
