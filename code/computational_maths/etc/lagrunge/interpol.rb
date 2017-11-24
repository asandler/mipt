#!/usr/bin/env ruby

def lagr x, y, x_calc
    ans = 0.0
    (0..x.size - 1).each do |i|
        temp = 1.0
        (0..x.size - 1).each do |j|
            next if i == j
            temp *= (x_calc - x[j]) / (x[i] - x[j])
        end
        ans += y[i] * temp
    end
    return [x_calc, ans]
end

def solve_inv data
    x = data.map{|a| a[0]}
    y = data.map{|a| a[1]}
    (1..x.size - 3).each do |i|
        (x[i]..x[i + 1]).step(0.001).each do |x_calc|
            int = lagr(x[i - 1..i + 2], y[i - 1..i + 2], x_calc)
            puts "#{int[1]} #{int[0]}"
        end
    end
end

data = File.open("data.tsv").readlines.map{|s| s.chomp.split }.map{|a| [a[0].to_f, a[1].to_f]}

solve_inv data
