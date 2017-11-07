type 'a tree = Empty | Tree of 'a tree * 'a * 'a tree

let rnd = new System.Random()

let rec randomTree() =
    if rnd.NextDouble() < 0.5 then
        Empty
    else
        Tree(randomTree(), rnd.Next(), randomTree())
 
let rec height tree =
    match tree with
    | Empty -> 0
    | Tree(left, _, right) -> 1 + max(height left)(height right)

let rec leavesCount tree =
    match tree with
    | Empty -> 0
    | Tree(Empty, _, Empty) -> 1
    | Tree(left, _, right) -> leavesCount left + leavesCount right

let main =
    let myTree = Tree(Tree(Empty, 3, Empty), 4, Tree(Empty, 2, Tree(Empty, 3, Empty)))

    printf "leavesCount of myTree: %d\n" (leavesCount(myTree))
    printf "height of myTree: %d\n\n" (height(myTree))

    for i in 1..5 do
        let rt = randomTree()
        printf "random tree No %d\n" i
        printf "leavesCount: %d\n" (leavesCount(rt))
        printf "height: %d\n\n" (height(rt))
    0