"""
Checking the (possible) ambiguity of code in ['a'..'z'] alphabet

input.txt: input code words separated by whitespaces
output.txt: minimal word decoded ambiguously (if any); otherwise empty

Andrew Sandler, MIPT, 097
andrei.sandler@phystech.edu
"""

c_star = {}

def c_star_mem(code, code_len_max, beta_cur = ''):
    c_star[beta_cur] = True
    for s in code:
        if len(beta_cur + s) <= code_len_max: c_star_mem(code, code_len_max, beta_cur + s)


def edge_check(v1, v2, code, edge_names):
    """
    Check if there is an edge between two vertices in graph
    """
    if v1 == v2:
        return None

    for s in code:
        p1, p2 = s.find(v1), s.rfind(v2)
        if p1 != 0 or p2 == -1 or p2 + len(v2) != len(s) or len(v1) + len(v2) > len(s): continue
        try:
            if c_star[s[p1 + len(v1) : p2]]:
                edge_names[(v1, v2)] = s[p1 + len(v1) : p2]
                return len(s) - len(v1)
        except KeyError:
            continue

    return None


def solve(vertices, graph, edge_names):
    """
    Solve function applies Bellman-Ford algorithm to the graph obtained in __main__,
    and then calculates the shortest ambiguosly decoded word, if there are any.
    """
    d = {}
    for v in vertices:
        d[v] = 10e9
    d[''] = 0
    ancestors = {}

    # Bellman-Ford algorithm
    for i in range(0, len(vertices) - 1):
        for key, value in graph.items():
            if d[key[1]] > d[key[0]] + value:
                d[key[1]] = d[key[0]] + value
                ancestors[key[1]] = key[0]

    # Restoration of the minimal word decoded in two ways,
    # bypassing the minimal loop extending through the empty word ('')
    min_cycle_len = 10e9
    answer = ''
    for s in vertices:
        try:
            if min_cycle_len > d[s] + graph[(s, '')]:
                min_cycle_len = d[s] + graph[(s, '')]
                go = s
                answer = edge_names[(s, '')]
                while go != '':
                    answer = edge_names[(ancestors[go], go)] + go + answer
                    go = ancestors[go]
        except KeyError:
            continue

    return answer


if __name__ == "__main__":
    with open("input.txt", "r") as input_file:
        code = sorted(set(input_file.readline().split()))
    edge_names, vertices, graph = {}, [''], {}

    # Vertices are all the prefixes and all the suffixes of code words,
    # which are not code words themselves.
    for s in code:
        for i in range(0, len(s) - 1):
            vertices.append(s[0:i])
        for i in range(1, len(s)):
            vertices.append(s[i:])

    vertices = sorted(set(vertices) - set(code))
    code_len_max = max([len(s) for s in code])
    c_star_mem(code, code_len_max)

    # Graph constructing
    for v1 in vertices:
        for v2 in vertices:
            tmp = edge_check(v1, v2, code, edge_names)
            if tmp != None: graph[(v1, v2)] = tmp

    answer = solve(vertices, graph, edge_names)
    if answer != '': answer += '\n'
    with open("output.txt", "w") as output_file:
        output_file.write(answer)
