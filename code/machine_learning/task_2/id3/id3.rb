require 'tree'
require 'set'

class DecisionTree
    def initialize
        @root = Tree::TreeNode.new("root", nil)
    end

    def learn features
        lrn(features, @root)
    end

    def classify object
        @go = @root
        while @go.has_children? do
            @go = @go.first_child
            while not @go.is_last_sibling? and @go.name != object[@go.parent.content]
                @go = @go.next_sibling
            end
            return "hz" if @go.name != object[@go.parent.content]
        end
        @go.content
    end

    def print
        @root.print_tree
    end

private
    def inf_gain features, index
        freqs, class_freqs = Hash.new(0), Hash.new(0)
        answer = 0.0
        features.each do |a|
            return 100 if not a[index]
            freqs[[a[index], a[-1]]] += 1
            class_freqs[a[index]] += 1
        end
        freqs.to_a.each do |ar, val|
            answer -= val.to_f / features.size * Math::log2(val.to_f / class_freqs[ar[0]]) if val.to_f > 0
        end
        answer
    end

    def lrn features, node
        if features[0].compact.size == 1
            node.content = features.group_by{|e| e}.values.max_by(&:size).first.last
            return
        end
        classes = []
        features.each{ |a| classes << a[-1] }
        if classes.uniq.size == 1
            node.content = classes[0]
            return
        end
        enthropy = []
        (0..features[0].size - 2).each do |feat_num|
            enthropy << inf_gain(features, feat_num)
        end
        min_ind = enthropy.index(enthropy.min)
        node.content = min_ind
        features.each.map{ |a| a[min_ind] }.compact.uniq.each do |cls|
            feature_subset = []
            features.each do |a|
                feature_subset << a.each_with_index.map{|e, ind| ind == min_ind ? nil : e} if a[min_ind] == cls
            end
            child = Tree::TreeNode.new(cls, nil)
            node << child
            lrn(feature_subset, child)
        end
    end
end

features = readlines.each.map{|s| s.chomp.split(",")}
dtree = DecisionTree.new
dtree.learn(features[0..features.size / 2 - 1])
t, f = 0, 0
features[features.size / 2..features.size - 1].each{ |a| dtree.classify(a) == a[-1] ? t += 1 : f += 1 }
p t.to_f / (t + f)
