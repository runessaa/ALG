#sys для увеличения лимита рекурсии
import sys

#увеличение лимита рекурсии
sys.setrecursionlimit(10**6)

#описываем узел дерева
class Node:
    def __init__(self, key, left=None, right=None):
        self.key = key #ключ узла
        self.left = left #левый потомок (ключ меньше)
        self.right = right #правый потомок (ключ больше)
        self.size = 1 #размер поддерева (включая себя)

#получение размера (если узел None, вернет 0)
def get_size(node):
    return node.size if node else 0

#обновление размера узла после изменений
def fix_size(node):
    if node:
        #размер = 1 (сам узел) + размер левого + размер правого
        node.size = 1 + get_size(node.left) + get_size(node.right)

#основная функция для разбиения дерева
def split(node, k):
    #если дерева нет, делить нечего
    if not node:
        return None, None

    #сколько узлов в левом поддереве
    left_size = get_size(node.left)

    #если k больше, чем узлов слева
    if k > left_size:
        #то идем в правое поддерево, чтобы добрать недостающие
        #просим от правого поддерева k - left_size - 1 элементов
        small_part, large_part = split(node.right, k - left_size - 1)
        
        #маленькая часть от правого поддерева становится новым правым потомком
        node.right = small_part
        fix_size(node) #пересчитываем размер
        
        #возвращаем текущий узел + всё что слева, остаток справа
        return node, large_part
    
    #если все k элементов находятся в левом поддереве
    else:
        #делим левое поддерево на k элементов и остаток
        small_part, large_part = split(node.left, k)
        
        #остаток от левого поддерева становится новым левым потомком
        node.left = large_part
        fix_size(node) #пересчитываем размер

        #возвращаем нужные k элементов, текущий узел + всё что справа
        return small_part, node

#функции для сборки и вывода дерева

def build_tree(descriptions):
    if not descriptions:
        return None, []
    
    nodes = [Node(key) for key, _, _ in descriptions]
    
    for i, (key, left_idx, right_idx) in enumerate(descriptions):
        if left_idx != -1:
            nodes[i].left = nodes[left_idx - 1]
        if right_idx != -1:
            nodes[i].right = nodes[right_idx - 1]
            
    #считаем размеры, идя с конца списка
    for node in reversed(nodes):
        fix_size(node)
        
    return nodes[0] if nodes else None, nodes

def print_tree(root, all_nodes):
    if not root:
        print(0)
        return

    #обход дерева
    q = [root]
    output_nodes = [root]
    visited_nodes = {root}
    head = 0
    while head < len(q):
        curr = q[head]; head += 1
        if curr.left and curr.left not in visited_nodes:
            visited_nodes.add(curr.left); output_nodes.append(curr.left); q.append(curr.left)
        if curr.right and curr.right not in visited_nodes:
            visited_nodes.add(curr.right); output_nodes.append(curr.right); q.append(curr.right)
            
    #перенумеровка узлов для вывода
    new_node_to_idx = {node: i + 1 for i, node in enumerate(output_nodes)}

    print(len(output_nodes))
    for node in output_nodes:
        key = node.key
        left_idx = new_node_to_idx.get(node.left, -1)
        right_idx = new_node_to_idx.get(node.right, -1)
        print(key, left_idx, right_idx)

#для проверки
if __name__ == '__main__':
    #входные данные из примера
    descriptions = [
        (668, 2, 5), (298, 3, -1), (191, -1, 4), (266, -1, -1),
        (702, 6, 7), (701, -1, -1), (870, 8, 9), (822, -1, -1),
        (912, -1, 10), (932, -1, -1)
    ]
    k = 1 #отдеяем 1 элемент
    
    #сборка дерева из данных
    root, all_nodes_list = build_tree(descriptions)
    
    #вызов функции
    tree1_root, tree2_root = split(root, k)
    
    #результат
    print("Первое дерево (k самых маленьких)")
    print_tree(tree1_root, all_nodes_list)
    print("\nВторое дерево (остальные)")
    print_tree(tree2_root, all_nodes_list)