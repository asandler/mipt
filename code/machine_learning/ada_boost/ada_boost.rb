#!/usr/bin/env ruby

def weakCl object
    ans = []
    obj = object * 2
    (-1..1).each do |i|
        (0..object.size - 1).each do |j|
            ans << (obj[j] > i ? 1 : -1)
            ans << (obj[j] < i ? 1 : -1)
            ans << (obj[j] == i ? 1 : -1)
            (1..5).each do |k|
                ans << (obj[j] + obj[j+k] > i ? 1 : -1)
                ans << (obj[j] + obj[j+k] < i ? 1 : -1)
                ans << (obj[j] + obj[j+k] == i ? 1 : -1)
                ans << (obj[j] - obj[j+k] > i ? 1 : -1)
                ans << (obj[j] - obj[j+k] < i ? 1 : -1)
                ans << (obj[j] - obj[j+k] == i ? 1 : -1)
            end
        end
        ans << (obj.max - obj.min > i ? 1 : -1)
        ans << (obj.max - obj.min < i ? 1 : -1)
        ans << (obj.max - obj.min == i ? 1 : -1)
    end
    ans
end

class AdaBoost
    def initialize
        @answer = []
    end

    def Train train
        w = Array.new(train.size, 1.0 / train.size)
        classNumber = (weakCl train[0][0..-2]).size

        classNumber.times do |t|
            errors = Array.new(classNumber, 0)
            train.each_with_index do |obj, tr_ind|
                (weakCl obj[0..-2]).each_with_index do |feature, f_ind|
                    errors[f_ind] += (feature == obj[-1] ? 0 : w[tr_ind])
                end
            end
            errors = errors.map{|i| i == 0 ? 100 : i}

            eps = errors.min
            ind_min = errors.index(eps)
            alpha = 0.5 * Math::log((1 - eps) / eps)
            @answer << [alpha, ind_min]
            puts t.to_s + " " + eps.to_s

            w0, w1 = 0, []
            w.each_with_index do |wi, ind|
                w1 << wi * (Math::exp(-alpha * train[ind][-1] * (weakCl train[ind][0..-2])[ind_min]))
                w0 += w1[-1]
            end

            w = w1.each.map{|i| i / w0}
        end
    end

    def Classify obj
        sum, features = 0, weakCl(obj)
        @answer.each{ |w, ind| sum += w * features[ind] }
        return (sum > 0 ? 1 : -1)
    end
end


tr = File.open("tic.train").readlines.each.map{|s| s.chomp.split(',')}
ts = File.open("tic.test").readlines.each.map{|s| s.chomp.split(',')}

train, test = [], []

tr.each do |a|
    b = []
    a[0..-2].each{ |c| b << ["o", "b", "x"].index(c) - 1 }
    b << (a[-1] == "positive" ? 1 : -1)
    train << b
end
ts.each do |a|
    b = []
    a[0..-2].each{ |c| b << ["o", "b", "x"].index(c) - 1 }
    b << (a[-1] == "positive" ? 1 : -1)
    test << b
end

ada = AdaBoost.new
ada.Train train
count = 0
test.each{ |obj| count += ((ada.Classify(obj[0..-2]) == obj[-1]) ? 1 : 0) }
p count.to_f / test.size.to_f
