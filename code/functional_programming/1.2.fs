let f1 (x:float) = log(x) - x + 1.8 // iteration [2, 3], x0 ~ 2.8459
let f2 (x:float) = x * tan(x) - (1.0 / 3.0) // Newton [0.2, 1], x0 ~ 0.5472
let f3 (x:float) = tan(x / 2.0) - (tan(x / 2.0) ** -1.0) + x // dichotomy [1, 2], x0 ~ 1.0769

let f1_ (x:float) = 1.0 / x - 1.0
let f2_ (x:float) = tan(x) + x / (cos(x) ** 2.0)
let f3_ (x:float) = 0.5 / (cos(x) ** 2.0) + 0.5 / (sin(x) ** 2.0) + 1.0

let rec dichotomy (f:float->float, l:float, r:float) =
    if abs(f((l + r) / 2.0)) < 1e-5 then
        (l + r) / 2.0
    elif f((l + r) / 2.0) * f(r) > 0.0 then
        dichotomy(f, l, (l + r) / 2.0)
    else
        dichotomy(f, (l + r) / 2.0, r)

let rec iterations (f:float->float, x:float) =
    if abs(f(x)) < 1e-5 then
        x
    else
        iterations(f, x - f(x))

let rec newton (f:float->float, deriv:float->float, x:float) =
    if abs(f(x)) < 1e-5 then
        x
    else
        newton (f, deriv, x - f(x)/deriv(x))

printf "%f\n" (dichotomy(f1, 2.0, 3.0))
printf "%f\n\n" (newton(f1, f1_, 2.2))

printf "%f\n" (dichotomy(f2, 0.2, 1.0))
printf "%f\n" (iterations(f2, 1.0))
printf "%f\n\n" (newton(f2, f2_, 1.0))

printf "%f\n" (dichotomy(f3, 1.0, 2.0))
printf "%f\n" (iterations(f3, 1.5))
printf "%f\n" (newton(f3, f3_, 1.5))