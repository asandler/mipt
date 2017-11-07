#!/usr/bin/env ruby

e = Complex(Math::E, 0)
(3..4).step(0.01).each do |i|
    (-10..10).step(0.01).each do |j|
        a = ['1+i'.to_c]
        10000.times{ a << (e ** a[-1] - Complex(i, j)) }
        uniq = [a[-1]]
        a[-32..-1].each do |c|
            flag = true
            uniq.each{ |u| flag = false if (c-u).abs < 10e-3 }
            uniq << c if flag
        end
        puts i.to_s + ' ' + j.to_s + ' ' + uniq.size.to_s
    end
    STDERR.puts i
    puts
end
