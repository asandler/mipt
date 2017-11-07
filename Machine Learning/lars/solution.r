train = read.csv('train.txt', header=FALSE)
test = read.csv('test.txt', header=FALSE)

library(lars)

#lasso = lars(as.matrix(train[1:4000, 1:444]), train[1:4000, 445], type='lasso')
#err = rep(0, 100)
#err_min = 100000
#lambda = 0
#for (i in 1:100 / 100) {
#    Yp = predict(lasso, train[4001:5000,1:444], s=i, type='fit', mode='lambda')
#    err = sum((Yp$fit - train[4001:5000, 445])^2) / 1000
#    if (err < err_min) {
#        err_min = err
#        lambda = i
#    }
#}
#
#Yp = predict(lasso, test, s=lambda, type='fit', mode='lambda')
#write(Yp$fit, file="result.txt", ncolumns=1)

top50 = read.csv('50', header=FALSE)

lasso = lars(as.matrix(train[1:4000, top50[,1]]), train[1:4000, 445], type='lasso')
err = rep(0, 100)
err_min = 100000
lambda = 0
for (i in 1:100 / 100) {
    Yp = predict(lasso, train[4001:5000, top50[,1]], s=i, type='fit', mode='lambda')
    err = sum((Yp$fit - train[4001:5000, 445])^2) / 1000
    if (err < err_min) {
        err_min = err
        lambda = i
    }
}

Yp = predict(lasso, test[,top50[,1]], s=lambda, type='fit', mode='lambda')
write(Yp$fit, file="result_50selected.txt", ncolumns=1)
