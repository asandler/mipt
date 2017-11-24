#coding utf-8
require 'active_support/core_ext'
require 'svm'

class Array
    def sum
        answer = 0
        self.each{ |i| answer += i.to_i }
        return answer
    end
end

class SuffixFactor
    def learn input
        @f = {}
        input.each do |fam, cl|
            (2..7).each do |l|
                if fam.length >= l
                    @f[fam[-l..-1]] ||= []
                    @f[fam[-l..-1]] << cl.to_i
                end
            end
        end
    end
    
    def calc key, length
        k = key[-length..-1]
        @f[k] ? @f[k].sum.to_f / @f[k].size : 0
    end
end

class PrefixFactor
    def learn input
        @f = {}
        input.each do |fam, cl|
            (2..7).each do |l|
                if fam.length >= l
                    @f[fam[0..l-1]] ||= []
                    @f[fam[0..l-1]] << cl.to_i
                end
            end
        end
    end

    def calc key, length
        k = key[0..length-1]
        @f[k] ? @f[k].sum.to_f / @f[k].size : 0
    end
end

class NGramFactor
    def learn input
        @f = {}
        @sum = Array.new(2, Array.new(10, 0))
        input.each do |fam, cl|
            (3..8).each do |l|
                (0..fam.length - l).each do |p|
                    @f[fam[p..p+l-1]] ||= [0, 0]
                    @f[fam[p..p+l-1]][cl.to_i] += 1
                    @sum[cl.to_i][l] += 1
                end
            end
        end
    end
    
    def calc key, length
        answer = 0
        (0..key.length - length).each do |p|
            if m = @f[key[p..p+length-1]]
                t = m[1].to_f / @sum[1][length] - m[0].to_f / @sum[0][length]
                answer = t if t.abs > answer.abs
            end
        end
        return answer * 1000
    end

    def calcSum key, length
        answer = 0
        (0..key.length - length).each do |p|
            if m = @f[key[p..p+length-1]]
                answer += m[1].to_f / @sum[1][length] - m[0].to_f / @sum[0][length]
            end
        end
        return answer * 1000
    end
end

$SUF = SuffixFactor.new
$PREF = PrefixFactor.new
$NGRAM = NGramFactor.new

class String
    def factors h
        answer = {}
        answer['suf2'] = $SUF.calc self, 2
        answer['suf3'] = $SUF.calc self, 3
        answer['suf4'] = $SUF.calc self, 4
        answer['pref2'] = $PREF.calc self, 2
        answer['pref3'] = $PREF.calc self, 3
        answer['pref4'] = $PREF.calc self, 4
        answer['ngram4'] = $NGRAM.calc self, 4
        answer['ngram5'] = $NGRAM.calc self, 5
        answer['ngram6'] = $NGRAM.calc self, 6
        answer['ngram_sum4'] = $NGRAM.calcSum self, 4
        answer['ngram_sum5'] = $NGRAM.calcSum self, 5
        answer['ngram_sum6'] = $NGRAM.calcSum self, 6
        answer['upcase_first'] = ('А'..'Я').include?(self[0]) ? 1 : 0
        return h['hash'] ? answer : answer.map{|a| a[1]}
    end

    def normalize
        if ('А'..'Я').include? self[0]
            return self[0] + self[1..-1].mb_chars.downcase.to_s
        else
            return self.mb_chars.downcase.to_s
        end
    end
end

a = File.open('train.txt', 'r').readlines.map{ |s| s.chomp.split(/, /).map{ |s| s.normalize } }.uniq
cls, train = [], []
STDERR.puts "Read"
$SUF.learn a
$PREF.learn a
$NGRAM.learn a
a.each do |s, cl|
    train << s.factors(:hash => false)
    cls << cl.to_i
end
STDERR.puts "Factors learned"
prob = Problem.new(cls, train)
#param = Parameter.new(:kernel_type => LINEAR, :C => 10) # 3884 errors
#param = Parameter.new(:kernel_type => RBF, :C => 10) # 3802 errors
#param = Parameter.new(:kernel_type => SIGMOID, :C => 10) # 5190 errors
param = Parameter.new(:svm_type => C_SVC, :kernel_type => POLY, :probability => 1) # 3715 errors
model = Model.new(prob, param)
STDERR.puts "Model OK"
File.open('test.txt', 'r').readlines.map{ |s| puts model.predict_probability(s.chomp.normalize.factors(:hash => false))[1][1] }
