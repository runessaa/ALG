def binary_search(nums, target, find_first):
    #задаем границы поиска
    left, right = 0, len(nums) - 1
    #сохраняем результат
    result = -1
    
    #пока левая граница не больше правой
    while left <= right:
        #находим средний элемент
        mid = (left + right) // 2
        #если нашли целевое число
        if nums[mid] == target:
            #сохраняем индекс
            result = mid
            #если ищем первое вхождение, смотрим левее
            if find_first:
                right = mid - 1
            #если последнее, смотрим правее
            else:
                left = mid + 1
        #если средний меньше целевого, ищем справа
        elif nums[mid] < target:
            left = mid + 1
        #если больше, ищем слева
        else:
            right = mid - 1
    
    #возвращаем найденный индекс или -1
    return result

def find_first_and_last(nums, target):
    #ищем первое вхождение
    first = binary_search(nums, target, True)
    #ищем последнее вхождение
    last = binary_search(nums, target, False)
    
    #если не нашли, выводим сообщение
    if first == -1:
        return "Element not found in the array"
    #возвращаем результат
    return f"The first occurrence of element {target} is located at index {first}\nThe last occurrence of element {target} is located at index {last}"

#примеры
nums1 = [2, 5, 5, 5, 6, 6, 8, 9, 9, 9]
target1 = 5
print(find_first_and_last(nums1, target1))

nums2 = [2, 5, 5, 5, 6, 6, 8, 9, 9, 9]
target2 = 4
print(find_first_and_last(nums2, target2))