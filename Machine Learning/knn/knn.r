KNN = function(t, objects, k) {
        classes = c()
        for (i in 1:length(objects[, 1])) {
                p = cbind((t[, 1] - objects[i, 1])**2 + (t[, 2] - objects[i, 2])**2, t[, 3])
                p = p[order(p[, 1]),]
                classes = c(classes, if (mean(p[1:k, 2]) > 0.5) 1 else 0)
        }
        return (cbind(objects[,1], objects[,2], classes))
}

error = function(train, test, k) {
        classes = KNN(train, cbind(test[, 1], test[, 2]), k)
        return (sum(abs(test[, 3] - classes[, 3])))
}

deviation = function(train, test, eq) {
        return (sapply(1:100 + eq, function(k) { return (error(train, test, k)) }))
}

crossvalidation = function(data, train_percent, k, n) {
        answers = c()
        l = nrow(data)
        train_length = floor(l * train_percent)
        for (i in 1:n) {
                train_indexes = sample(1:l, train_length)
                train = data[train_indexes,]
                test = data[-train_indexes,]
                answers[i] = error(train, test, k)
        }
        return (mean(answers))
}

color_names = c('green', 'blue', 'red', 'black')
t = read.csv('train.csv')
test = read.csv('test.csv')
print(crossvalidation(rbind(t, test), train_percent = 0.33, k = 11, n = 20))
#x = c()
#y = c()
#train_classes = c()
#for (i in seq(from = -5, to = 8, by = 0.1)) {
#        for (j in seq(from = -6, to = 10, by = 0.1)) {
#                x = c(x, i)
#                y = c(y, j)
#        }
#}
#for (i in t[,3])
#        train_classes = c(train_classes, color_names[i + 3])
#
#for (k in c(1, 5, 11, 101, 401)) {
#        png(paste('split_surface', as.character(k), '.png'))
#        classes = KNN(t, cbind(x, y), k)
#        cl = c()
#        for (i in classes[,3])
#                cl = c(cl, color_names[i + 1])
#        plot(classes[,1], classes[,2], col = cl, cex = 0.3)
#        points(t[,1], t[,2], col = train_classes, cex = 0.4)
#        dev.off()
#}

#png('deviation_test.png')
#plot(deviation(t, test, eq = 0), type = 'l')
#dev.off()
#png('deviation_train.png')
#plot(deviation(t, t, eq = 1), type = 'l')
#dev.off()
