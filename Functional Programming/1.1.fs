let rec fac n =
    match n with
    | 0 | 1 -> 1
    | _ -> n * fac(n - 1)

let rec stupid (ans:float, cur:float, x:float, n:int) =
    if abs(ans - cur) < 1e-4 then
        [cur; float n]
    else
        stupid(ans, cur + (log(3.0) ** float n) / float (fac n) * (x ** float n), x, n + 1)

let rec fast (ans:float, cur:float, last:float, x:float, n:int) = 
    if abs(ans - cur) < 1e-4 then
        [cur; float n]
    else
        fast(ans, cur + last, last * log(3.0) / (float (n + 1)) * x, x, n + 1)

let rec solve (x:float) =
    if x >= 1.0 then
        ()
    else
        let ans = 3.0 ** x
        let s, f = stupid (ans, 0.0, x, 0), fast(ans, 0.0, 1.0, x, 0)
        printf "%f\t%f\t%d\t%f\t%d\t%f\n" x s.[0] (int s.[1]) f.[0] (int f.[1]) ans
        solve (x + 0.001)

solve(0.0)