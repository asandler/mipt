$table = Hash.new(0)

def in_rand_subset a, b, ar, n, filename
    if rand >= 0.5
        rand_subset = [rand(a..b)]
    else
        begin
            rand_subset = (a..b).each.map{ |i| i if rand <= 0.5 }.compact
        end while rand_subset.empty?
    end
    f = File.open(filename, 'a')
    f.puts "#{n}: rand_subset, " + rand_subset.to_s
    f.close
    ar.each.map{ |i| rand_subset.include?(i) }
end

def bound_more a, b, ar, n, filename
    bound = rand(b - a) + a
    f = File.open(filename, 'a')
    f.puts "#{n}: bound_more, #{bound}"
    f.close
    ar.each.map{ |i| i > bound }
end

def bound_less a, b, ar, n, filename
    bound = rand(b - a + 1) + a
    f = File.open(filename, 'a')
    f.puts "#{n}: bound_less, #{bound}"
    f.close
    ar.each.map{ |i| i < bound }
end

def interval a, b, ar, n, filename
    a = rand(b - a + 1) + a, rand(b - a + 1) + a
    l, r = a.min, a.max
    f = File.open(filename, 'a')
    f.puts "#{n}: interval, #{l}, #{r}"
    f.close
    ar.each.map{ |i| (l..r).include? i }
end

def numeric_feature a, b, ar, n, filename
    if rand <= 0.333
        return bound_less(a, b, ar, n, filename)
    elsif rand <= 0.5
        return bound_more(a, b, ar, n, filename)
    else
        return interval(a, b, ar, n, filename)
    end
end

def random_feature data, filename
    f = []
    f << in_rand_subset(1, 4, data.each.map{|a| a[0][2..-1].to_i}, 0, filename) if rand <= 0.2
    f << numeric_feature(4, 72, data.each.map{|a| a[1].to_i}, 1, filename) if rand <= 0.2
    f << in_rand_subset(0, 4, data.each.map{|a| a[2][2..-1].to_i}, 2, filename) if rand <= 0.2
    f << in_rand_subset(0, 10, data.each.map{|a| a[3][2..-1].to_i}, 3, filename) if rand <= 0.2
    f << numeric_feature(250, 18424, data.each.map{|a| a[4].to_i}, 4, filename) if rand <= 0.2
    f << in_rand_subset(1, 5, data.each.map{|a| a[5][2..-1].to_i}, 5, filename) if rand <= 0.2
    f << in_rand_subset(1, 5, data.each.map{|a| a[6][2..-1].to_i}, 6, filename) if rand <= 0.2
    f << numeric_feature(1, 4, data.each.map{|a| a[7].to_i}, 7, filename) if rand <= 0.2
    f << in_rand_subset(1, 5, data.each.map{|a| a[8][2..-1].to_i}, 8, filename) if rand <= 0.2
    f << in_rand_subset(1, 3, data.each.map{|a| a[9][2..-1].to_i}, 9, filename) if rand <= 0.2
    f << numeric_feature(1, 4, data.each.map{|a| a[10].to_i}, 10, filename) if rand <= 0.2
    f << in_rand_subset(1, 4, data.each.map{|a| a[11][3..-1].to_i}, 11, filename) if rand <= 0.2
    f << numeric_feature(19, 75, data.each.map{|a| a[12].to_i}, 12, filename) if rand <= 0.2
    f << in_rand_subset(1, 3, data.each.map{|a| a[13][3..-1].to_i}, 13, filename) if rand <= 0.2
    f << in_rand_subset(1, 3, data.each.map{|a| a[14][3..-1].to_i}, 14, filename) if rand <= 0.2
    f << numeric_feature(1, 4, data.each.map{|a| a[15].to_i}, 15, filename) if rand <= 0.2
    f << in_rand_subset(1, 4, data.each.map{|a| a[16][3..-1].to_i}, 16, filename) if rand <= 0.2
    f << numeric_feature(1, 2, data.each.map{|a| a[17].to_i}, 17, filename) if rand <= 0.2
    f << in_rand_subset(1, 2, data.each.map{|a| a[18][3..-1].to_i}, 18, filename) if rand <= 0.2
    f << in_rand_subset(1, 2, data.each.map{|a| a[19][3..-1].to_i}, 19, filename) if rand <= 0.2
    answer = []
    data.size.times do |t|
        cur_f = true
        if f.any?
            f.each{ |a| cur_f &= a[t] }
            answer << (cur_f ? 1 : 2)
        end
    end
    return ((answer.any?) ? answer : nil)
end

def calc_nCr
    $table[[0, 0]] = 1
    $table[[1, 0]] = 1
    $table[[1, 1]] = 1
    (2..1000).each do |n|
        (0..n).each do |k|
            $table[[n, k]] = $table[[n - 1, k]] + $table[[n - 1, k - 1]]
        end
    end
end

def nCr n, k
    return $table[[n, k]]
end

def stat_inf features, index
    p, n, pos, neg = 0, 0, 0, 0
    return 1000 if not features[index]
    features[index].each_with_index do |val, ind|
        p += 1 if val * features[-1][ind] == 1
        n += 1 if val * features[-1][ind] == 4
        pos += 1 if features[-1][ind] == 1
        neg += 1 if features[-1][ind] == 2
    end
    x = -Math::log2((nCr(pos, p) * nCr(neg, n)).to_f / nCr(pos + neg, p + n))
    puts "#{x}, p = #{p}, n = #{n}, #{index}"
    return x
end

def inf_gain features, index
    freqs, class_freqs = Hash.new(0), Hash.new(0)
    answer = 0.0
    return 1000 if not features[index]
    features[index].each_with_index do |cls, ind|
        return 1000 if not cls
        freqs[[cls, features[-1][ind]]] += 1
        class_freqs[cls] += 1
    end
    freqs.to_a.each do |ar, val|
        answer -= val.to_f / features[index].size * Math::log2(val.to_f / class_freqs[ar[0]]) if val.to_f > 0
    end
    puts "#{answer}, p = #{freqs[[1, 1]]} / #{freqs[[1, 1]] + freqs[[2, 1]]}, n = #{freqs[[2, 2]]} / #{freqs[[1, 2]] + freqs[[2, 2]]}, #{index}"
    answer
end

system("rm -r conjunct_data")
system("mkdir -p conjunct_data")
data = readlines.each.map{|s| s.chomp.split(" ")}
rand_features = []
30000.times{ |t| rand_features << random_feature(data, "conjunct_data/#{t.to_s}") }
rand_features << data.each.map{ |a| a[-1].to_i }
30000.times{ |t| inf_gain(rand_features, t) }
#calc_nCr
#10000.times{ |t| stat_inf(rand_features, t) }
