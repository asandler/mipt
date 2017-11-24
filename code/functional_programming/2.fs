open System

type student = {name:string; surname:string; sex:char; day:int; month:int; year:int; group:int; iq:int}
let students = [{name="Максим"; surname="Чуркин"; sex='m'; day=13; month=12; year=1991; group=331; iq=100};
                 {name="Вера"; surname="Аминова"; sex='f'; day=1; month=6; year=1991; group=331; iq=115};
                 {name="Елизавета"; surname="Анохина"; sex='f'; day=27; month=2; year=1992; group=332; iq=105};
                 {name="Дарья"; surname="Грекова"; sex='f'; day=18; month=3; year=1992; group=333; iq=106};
                 {name="Иван"; surname="Демкович"; sex='m'; day=3; month=1; year=1993; group=331; iq=107};
                 {name="Денис"; surname="Дубленых"; sex='m'; day=6; month=4; year=1991; group=334; iq=106};
                 {name="Алексей"; surname="Королев"; sex='m'; day=24; month=5; year=1993; group=333; iq=114};
                 {name="Вячеслав"; surname="Шапрынский"; sex='m'; day=28; month=11; year=1992; group=334; iq=103};
                 {name="Владислав"; surname="Исенбаев"; sex='m'; day=11; month=10; year=1991; group=331; iq=103};
                 {name="Дмитрий"; surname="Зонин"; sex='m'; day=10; month=4; year=1991; group=332; iq=108};
                 {name="Дмитрий"; surname="Карпов"; sex='m'; day=9; month=8; year=1991; group=331; iq=108};
                 {name="Александр"; surname="Голубев"; sex='m'; day=19; month=9; year=1992; group=332; iq=106};
                 {name="Вадим"; surname="Шакуро"; sex='m'; day=5; month=1; year=1992; group=333; iq=108};
                 {name="Сергей"; surname="Ларюшкин"; sex='m'; day=9; month=3; year=1993; group=333; iq=107};
                 {name="Александр"; surname="Сухман"; sex='m'; day=20; month=2; year=1993; group=331; iq=115};
                 {name="Кирилл"; surname="Корнюхин"; sex='m'; day=30; month=12; year=1992; group=332; iq=106};
                 {name="Марина"; surname="Масленникова"; sex='f'; day=29; month=3; year=1990; group=332; iq=114};
                 {name="Антон"; surname="Путрик"; sex='m'; day=2; month=11; year=1991; group=331; iq=107};
                 {name="Максим"; surname="Путрик"; sex='m'; day=16; month=10; year=1991; group=332; iq=107};
                 {name="Александр"; surname="Чегодаев"; sex='m'; day=12; month=9; year=1993; group=332; iq=105};
                 {name="Виктор"; surname="Самунь"; sex='m'; day=19; month=7; year=1992; group=331; iq=108};
                 {name="Дмитрий"; surname="Королев"; sex='m'; day=22; month=6; year=1991; group=333; iq=106};
                 {name="Павел"; surname="Скрипниченко"; sex='m'; day=11; month=5; year=1991; group=331; iq=115};
                 {name="Александр"; surname="Топоров"; sex='m'; day=4; month=11; year=1991; group=334; iq=102};
                 {name="Елена"; surname="Шалашугина"; sex='f'; day=5; month=12; year=1992; group=331; iq=105};
                 {name="Денис"; surname="Ядаринкин"; sex='m'; day=17; month=8; year=1992; group=331; iq=103};
                 {name="Даниил"; surname="Айзенштейн"; sex='m'; day=15; month=5; year=1993; group=331; iq=105};
                 {name="Вера"; surname="Аминова"; sex='f'; day=26; month=4; year=1993; group=332; iq=106};
                 {name="Ксения"; surname="Афоненко"; sex='f'; day=18; month=3; year=1993; group=332; iq=103};
                 {name="Илья"; surname="Дегтярев"; sex='m'; day=28; month=2; year=1992; group=333; iq=106};
                 {name="Дмитрий"; surname="Зонин"; sex='m'; day=22; month=1; year=1992; group=331; iq=106};
                 {name="Мария"; surname="Колмогорова"; sex='f'; day=21; month=9; year=1991; group=333; iq=103};
                 {name="Виктор"; surname="Самунь"; sex='m'; day=7; month=7; year=1992; group=331; iq=109};
                 {name="Олег"; surname="Михайлов"; sex='m'; day=6; month=11; year=1992; group=331; iq=107};
                 {name="Григорий"; surname="Семенов"; sex='m'; day=14; month=10; year=1989; group=334; iq=102};
                 {name="Иван"; surname="Кадочников"; sex='m'; day=22; month=7; year=1993; group=334; iq=103};
                 {name="Алексей"; surname="Королев"; sex='m'; day=15; month=6; year=1991; group=331; iq=102};
                 {name="Алексей"; surname="Кутырев"; sex='m'; day=1; month=5; year=1991; group=331; iq=111};
                 {name="Елена"; surname="Лысенко"; sex='f'; day=28; month=1; year=1990; group=333; iq=107};
                 {name="Антон"; surname="Путрик"; sex='m'; day=23; month=11; year=1992; group=333; iq=107};
                 {name="Илья"; surname="Отрадников"; sex='m'; day=15; month=3; year=1992; group=331; iq=115};
                 {name="Анна"; surname="Уставщикова"; sex='f'; day=10; month=2; year=1991; group=334; iq=100};
                 {name="Борис"; surname="Сафронов"; sex='m'; day=20; month=8; year=1993; group=334; iq=105};
                 {name="Надежда"; surname="Скачкова"; sex='f'; day=1; month=12; year=1991; group=331; iq=111};
                 {name="Владимир"; surname="Теличкин"; sex='m'; day=8; month=7; year=1991; group=331; iq=114};
                 {name="Иван"; surname="Турмышев"; sex='m'; day=7; month=5; year=1992; group=332; iq=107};
                 {name="Евгений"; surname="Кузнецов"; sex='m'; day=4; month=11; year=1992; group=331; iq=108};
                 {name="Сергей"; surname="Зайков"; sex='m'; day=3; month=8; year=1991; group=331; iq=108};
                 {name="Юлия"; surname="Буркова"; sex='f'; day=13; month=3; year=1991; group=331; iq=109};
                 {name="Юрий"; surname="Старовойтов"; sex='m'; day=12; month=4; year=1993; group=334; iq=102};
                 {name="Яков"; surname="Мозгоев"; sex='m'; day=10; month=9; year=1991; group=332; iq=107};
                 {name="Елена"; surname="Михайлова"; sex='f'; day=27; month=12; year=1993; group=334; iq=111};
                 {name="Егор"; surname="Игнатьев"; sex='m'; day=26; month=9; year=1992; group=333; iq=107};
                 {name="Сергей"; surname="Торопкин"; sex='m'; day=21; month=4; year=1991; group=331; iq=101};
                 {name="Татьяна"; surname="Елисеева"; sex='f'; day=25; month=6; year=1991; group=333; iq=100};
                 {name="Адель"; surname="Аухатова"; sex='f'; day=12; month=3; year=1991; group=332; iq=105};
                 {name="Алексей"; surname="Ефремов"; sex='m'; day=19; month=2; year=1993; group=334; iq=105};
                 {name="Диана"; surname="Таратухина"; sex='f'; day=17; month=1; year=1992; group=334; iq=102};
                 {name="Ростислав"; surname="Девятов"; sex='m'; day=5; month=8; year=1992; group=334; iq=108};
                 {name="Антон"; surname="Васильев"; sex='m'; day=2; month=5; year=1991; group=332; iq=112};
                 {name="Антон"; surname="Махлин"; sex='m'; day=8; month=4; year=1990; group=332; iq=109};
                 {name="Елена"; surname="Третьяк"; sex='f'; day=16; month=3; year=1993; group=333; iq=105};
                 {name="Игорь"; surname="Гуламов"; sex='m'; day=25; month=6; year=1991; group=333; iq=105};
                 {name="Илья"; surname="Корнаков"; sex='m'; day=22; month=12; year=1992; group=333; iq=101};
                 {name="Владимир"; surname="Батаев"; sex='m'; day=10; month=10; year=1991; group=334; iq=102};
                 {name="Арсений"; surname="Климовский"; sex='m'; day=4; month=5; year=1991; group=331; iq=102};
                 {name="Федор"; surname="Пахомов"; sex='m'; day=17; month=3; year=1993; group=334; iq=107};
                 {name="Виктор"; surname="Алюшин"; sex='m'; day=10; month=12; year=1993; group=334; iq=101};
                 {name="Игорь"; surname="Махлин"; sex='m'; day=28; month=1; year=1992; group=332; iq=108};
                 {name="Иван"; surname="Козлов"; sex='m'; day=21; month=8; year=1991; group=332; iq=111};
                 {name="Вадим"; surname="Авдеев"; sex='m'; day=11; month=3; year=1992; group=333; iq=111};
                 {name="Антон"; surname="Фонарев"; sex='m'; day=3; month=9; year=1991; group=332; iq=107};
                 {name="Андрей"; surname="Колосов"; sex='m'; day=1; month=7; year=1991; group=332; iq=107};
                 {name="Марина"; surname="Степанова"; sex='f'; day=9; month=11; year=1992; group=334; iq=104};
                 {name="Матвей"; surname="Корнилов"; sex='m'; day=7; month=10; year=1993; group=334; iq=105};
                 {name="Яков"; surname="Длугач"; sex='m'; day=5; month=5; year=1991; group=333; iq=109};
                 {name="Игорь"; surname="Туркин"; sex='m'; day=4; month=5; year=1991; group=333; iq=109};
                 {name="Олег"; surname="Самойленко"; sex='m'; day=1; month=8; year=1992; group=332; iq=107};
                 {name="Екатерина"; surname="Самбукова"; sex='f'; day=15; month=3; year=1993; group=334; iq=104};
                 {name="Дмитрий"; surname="Алексеев"; sex='m'; day=25; month=2; year=1992; group=334; iq=104};
                 {name="Павел"; surname="Ющенко"; sex='m'; day=21; month=10; year=1990; group=334; iq=108};
                 {name="Иван"; surname="Дудинов"; sex='m'; day=22; month=12; year=1991; group=331; iq=108};
                 {name="Виктор"; surname="Чернышов"; sex='m'; day=16; month=4; year=1991; group=331; iq=99};
                 {name="Антон"; surname="Лупанов"; sex='m'; day=11; month=12; year=1993; group=333; iq=108};
                 {name="Иван"; surname="Пузыревский"; sex='m'; day=6; month=1; year=1991; group=333; iq=99};
                 {name="Александр"; surname="Петров"; sex='m'; day=20; month=1; year=1992; group=332; iq=107};
                 {name="Леонид"; surname="Андриевский"; sex='m'; day=17; month=7; year=1990; group=334; iq=107};
                 {name="Александр"; surname="Рощупкин"; sex='m'; day=1; month=3; year=1991; group=333; iq=98};
                 {name="Александр"; surname="Корчагин"; sex='m'; day=15; month=7; year=1991; group=334; iq=112};
                 {name="Кирилл"; surname="Жалнин"; sex='m'; day=10; month=8; year=1992; group=332; iq=104};
                 {name="Дмитрий"; surname="Горбик"; sex='m'; day=13; month=6; year=1991; group=332; iq=104};
                 {name="Антон"; surname="Бобков"; sex='m'; day=27; month=2; year=1991; group=333; iq=104};
                 {name="Константин"; surname="Новиков"; sex='m'; day=22; month=10; year=1993; group=332; iq=101};
                 {name="Владимир"; surname="Ткачев"; sex='m'; day=21; month=1; year=1992; group=334; iq=94};
                 {name="Иван"; surname="Кондрашов"; sex='m'; day=6; month=5; year=1994; group=331; iq=104};
                 {name="Данила"; surname="Потапов"; sex='m'; day=23; month=6; year=1991; group=332; iq=104};
                 {name="Борис"; surname="Агафонцев"; sex='m'; day=13; month=4; year=1993; group=334; iq=135};
                 {name="Алексей"; surname="Давыдов"; sex='m'; day=7; month=7; year=1993; group=333; iq=103};
                 {name="Денис"; surname="Лисов"; sex='m'; day=4; month=12; year=1990; group=333; iq=108};
                 {name="Иван"; surname="Богатый"; sex='m'; day=14; month=4; year=1991; group=333; iq=107};
                 {name="Мария"; surname="Фелицина"; sex='f'; day=18; month=2; year=1990; group=333; iq=104};
                 {name="Андрей"; surname="Викулов"; sex='m'; day=2; month=2; year=1993; group=332; iq=107};
                 {name="Алексей"; surname="Крицин"; sex='m'; day=22; month=12; year=1991; group=332; iq=107};
                 {name="Артем"; surname="Кухаренко"; sex='m'; day=26; month=7; year=1990; group=332; iq=103};
                 {name="Сергей"; surname="Аноховский"; sex='m'; day=30; month=4; year=1993; group=331; iq=103};
                 {name="Александр"; surname="Захаров"; sex='m'; day=29; month=12; year=1991; group=332; iq=103};
                 {name="Андрей"; surname="Котов"; sex='m'; day=28; month=10; year=1991; group=333; iq=111};
                 {name="Кирилл"; surname="Знаменский"; sex='m'; day=30; month=9; year=1993; group=334; iq=103};
                 {name="Владимир"; surname="Лесниченко"; sex='m'; day=31; month=8; year=1993; group=334; iq=103};
                 {name="Илья"; surname="Смирнов"; sex='m'; day=31; month=7; year=1991; group=331; iq=108};
                 {name="Александр"; surname="Колесников"; sex='m'; day=31; month=3; year=1991; group=334; iq=101};
                 {name="Николай"; surname="Печенкин"; sex='m'; day=13; month=11; year=1991; group=333; iq=115};
                 {name="Игнат"; surname="Колесниченко"; sex='m'; day=29; month=7; year=1990; group=332; iq=101};
                 {name="Алена"; surname="Шевцова"; sex='f'; day=30; month=2; year=1993; group=334; iq=106};
                 {name="Федор"; surname="Волков"; sex='m'; day=30; month=3; year=1993; group=333; iq=107};
                 {name="Константин"; surname="Яблочкин"; sex='m'; day=9; month=1; year=1990; group=334; iq=109};
                 {name="Алексей"; surname="Окунев"; sex='m'; day=31; month=12; year=1991; group=334; iq=107};
                 {name="Татьяна"; surname="Поиленкова"; sex='f'; day=31; month=7; year=1991; group=334; iq=101};
                 {name="Ольга"; surname="Пакуляк"; sex='f'; day=31; month=3; year=1991; group=333; iq=108};
                 {name="Элина"; surname="Имашева"; sex='f'; day=31; month=5; year=1993; group=333; iq=108};
                 {name="Алексей"; surname="Гронский"; sex='m'; day=30; month=7; year=1989; group=332; iq=109};
                 {name="Мария"; surname="Гребенкина"; sex='f'; day=31; month=12; year=1991; group=332; iq=106}
                 ]


let rec removeN(lst:List<int>, num:int) =
    if num = 0 then
        List.tail lst
    else if List.isEmpty lst then
        []
    else
        List.append [List.head lst] (removeN(List.tail(lst), num - 1))

let rec removeNtailRec(lst:List<int>, num:int) =
    let rec tailRecRemoving(lst:List<int>, num:int, acc:List<int>) =
        if num = 0 then
            List.append (acc) (List.tail(lst))
        else if List.isEmpty lst then
            acc
        else
            tailRecRemoving(List.tail(lst), num - 1, List.append (acc) [List.head lst])

    tailRecRemoving(lst, num, [])

let removeNmapi(lst:List<int>, num:int) =
    List.mapi(fun i x -> (i, x)) lst |> List.filter(fun x -> fst x <> num) |> List.map(fun x -> snd x)


let maxIndex(lst:List<int>) =
    let rec subMaxIndex(lst:List<int>, num:int) =
        if List.length lst = 1 then
            (List.head lst, num)
        else
            let tailAns = subMaxIndex(List.tail lst, num + 1)
            if fst tailAns > List.head lst then
                tailAns
            else
                (List.head lst, num)
    
    snd(subMaxIndex(lst, 0))

let maxIndexTailRec(lst:List<int>) =
    let rec subMaxIndex(lst:List<int>, acc, num:int) =
        if List.length lst = 0 then
            acc
        else
            if List.head lst > snd acc then
                subMaxIndex(List.tail lst, (num, List.head lst), num + 1)
            else
                subMaxIndex(List.tail lst, acc, num + 1)

    fst(subMaxIndex(lst, (0, -100000000), 0))

let maxIndexMapi(lst:List<int>) =
    fst(List.mapi(fun i x -> (i, x)) lst |> List.reduce(fun acc elem -> if (snd acc > snd elem) then acc else elem))

let Years(s:student) =
    let now = DateTime.Now.ToString("dd MM yyyy").Split(' ')
    let nowDays = int now.[0] + (int now.[1] - 1) * 30 + (int now.[2] - 1) * 365
    let born = s.day + (s.month - 1) * 30 + (s.year - 1) * 365
    (nowDays - born) / 365

[<EntryPoint>]
let main argv = 
    let l = [4; 2; 5637; 24; 13457; 23; 3; -5]
    printfn "%A" l
    printfn "%A" (removeN(l, 4))
    printfn "%A" (removeNtailRec(l, 4))
    printfn "%A" (removeNmapi(l, 4))
    printfn "%A" (removeN(l, 100))
    printfn "%A" (removeNtailRec(l, 100))
    printfn "%A" (removeNmapi(l, 100))

    printfn "%d" (maxIndex(l))
    printfn "%d" (maxIndexTailRec(l))
    printfn "%d" (maxIndexMapi(l))

    printfn "%A" (maxIndexMapi(removeNmapi(l, 4)))

    let studentsCount = maxIndexMapi(List.mapi (fun i elem -> i) students)
    printfn "%d" ((List.map(fun elem -> Years(elem)) students |> List.reduce(fun acc elem -> acc + elem)) / studentsCount)
    0