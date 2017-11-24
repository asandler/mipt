density = function(p, mu, sigma) {
        k = length(p) / 2
        x = p - mu
        return (exp((x %*% solve(sigma, x)) / (-2)) / ((2 * pi)^k * sqrt(det(sigma))))
}

classify = function(objects, m0, s0, m1, s1) {
        ans = rep(0, nrow(objects))
        for (i in 1:nrow(objects)) {
                ans[i] = if (density(as.vector(t(objects[i,])), m0, s0) > density(as.vector(t(objects[i,])), m1, s1)) 0 else 1
        }
        return (ans)
}

train = read.csv('train.csv', header = FALSE)
n = ncol(train) - 1
#r = nrow(train) - 1
r = nrow(train)
classes = train[,ncol(train)]
#loo = c()
#for (tau in seq(from=0.1, to=2, by=0.01)) {
#        err = 0
#        for (i in 1:nrow(train)) {
#                classes[i] = classes[i] + 3
                mu_0 = colMeans(train[classes == 0, 1:n])
                mu_1 = colMeans(train[classes == 1, 1:n])
                sigma_0 = cov(train[classes == 0, 1:n]) * (r - 1) / r + diag(0.97, n)#diag(tau, n)
                sigma_1 = cov(train[classes == 1, 1:n]) * (r - 1) / r + diag(0.97, n)#diag(tau, n)
#                classes[i] = classes[i] - 3
#                err = err + abs(classify(train[i, 1:n], mu_0, sigma_0, mu_1, sigma_1) - classes[i])
#        }
#        loo = c(loo, err)
#}
#png('loo.png')
#plot(seq(from=0.1, to=2, by=0.01), loo, type='l')
#dev.off()
#print(loo)
ans = classify(read.csv('test.csv', header = FALSE), mu_0, sigma_0, mu_1, sigma_1)
print(cbind(1:length(ans), ans))
