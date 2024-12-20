# Алгоритмы
Это последовательности шагов или инструкций, которые позволяют решать определённую задачу. Они представляют собой логичный и структурированный подход к решению проблемы. Алгоритм определяет, как данные обрабатываются, какие операции выполняются и в каком порядке. Алгоритмы помогают решать задачи эффективно, используя минимальные ресурсы (время и память). Позволяют автоматизировать процесс обработки данных, избегая ручного вмешательства. Некоторые задачи невозможно решить без чётко заданного алгоритма (например, поиск, сортировка, обработка больших данных). Алгоритмы дают гарантии того, что результат будет правильным и получен в разумные сроки.
Ниже приведена информация о типах алгоритмов, например: [поиск](#search), [сортировка](#sort), [графы](#grapth). Также информация о [сложности алгоритмов](#complexity).

### <a name="search"></a>Алгоритмы поиска:
   - Линейный поиск: поочерёдный просмотр всех элементов.
   - Бинарный поиск: более быстрый поиск в отсортированном массиве, который делит данные пополам.

### <a name="sort"></a>Алгоритмы сортировки:
   - Пузырьковая сортировка: простая, но медленная сортировка, где элементы сравниваются попарно и обмениваются местами.
   - Сортировка слиянием: более эффективная сортировка, использующая принцип "разделяй и властвуй".
   - Сортировка быстрая (QuickSort): ещё более эффективная, базируется на выборе опорного элемента и разделении массива.

### <a name="graph"></a>Алгоритмы графов:
   - Алгоритм Дейкстры: находит кратчайший путь от одной вершины к другим в графе.
   - Алгоритм поиска в глубину (DFS): используется для обхода или поиска в графах.
   - Алгоритм поиска в ширину (BFS): другой способ обхода графов, используется для нахождения кратчайшего пути в невзвешенных графах.

### Динамическое программирование:
   - Алгоритм, который решает задачу, разбивая её на подзадачи и запоминая их результаты для избегания повторных вычислений. Пример: нахождение наибольшей общей подпоследовательности.

### Жадные алгоритмы:
   - Алгоритмы, которые на каждом шаге принимают локально оптимальное решение, надеясь, что это приведёт к глобально оптимальному решению. Пример: задача о рюкзаке.

### Алгоритмы с обратным ходом (Backtracking):
   - Эти алгоритмы пробуют различные возможные варианты и откатываются назад, если текущий вариант не подходит. Пример: решение задач о раскраске графов, поиске решения в головоломках.

В репозитории постараюсь охватить разные типы данных и примеры их использования. Рассмотрю популярные алгоритмы и их сложность. На языке программирования C. Ниже представлены примеры для [компиляции](#compile) и [дебага](#debug) программ. Также [особенности самого языка](#info).

### <a name="complexity"></a>Сложность алгоритмов
Характеристика его производительности, которая показывает, как количество необходимых ресурсов (например, времени или памяти) зависит от размера входных данных. Основные виды сложности — это [временная сложность](#timeComplexity) (время, которое алгоритм тратит на выполнение) и [пространственная сложность](#spaceComplexity) (память, которая требуется для хранения данных). Сложность оценивается в зависимости от того, как меняется количество операций или потребность в памяти при увеличении размера входных данных. Выбор подходящего алгоритма с оптимальной сложностью важен для решения задач эффективно, особенно когда входные данные могут быть очень большими. Оценка сложности помогает разработчикам предсказать, как алгоритм будет вести себя при увеличении объёма данных и позволяет принимать решения о том, какой подход использовать для конкретной задачи.
##### <a name="timeComplexity"></a>Временная сложность (Time Complexity)
Временная сложность алгоритма — это функция, которая показывает, сколько времени алгоритм будет работать в зависимости от размера входных данных `n`.
Обозначение сложности: часто используют нотацию О-большое (Big-O), которая обозначает верхнюю границу сложности (пессимистический сценарий).
* O(1) — Константная сложность
   - Алгоритм выполняет одинаковое количество операций, независимо от размера входных данных.
   - Пример: Возвращение первого элемента массива требует всего одной операции, независимо от размера массива, поэтому сложность — `O(1)`.
* O(log n) — Логарифмическая сложность
   - Алгоритм сокращает количество данных, с которыми работает, на каждом шаге. Это характерно для алгоритмов поиска, которые уменьшают проблему в два раза.
   - Пример: Бинарный поиск.
     В бинарном поиске каждый шаг делит массив пополам, что уменьшает количество возможных вариантов на каждом шаге.
* O(n) — Линейная сложность
   - Алгоритм проходит по всем данным и выполняет операцию для каждого элемента. Чем больше данных, тем больше времени потребуется для выполнения.
   - Пример: Линейный поиск. Алгоритм проходит через все элементы массива, и его сложность — `O(n)`, где `n` — это количество элементов в массиве.
* O(n log n) — Линейно-логарифмическая сложность
   - Это типичная сложность для эффективных алгоритмов сортировки, таких как сортировка слиянием или быстрая сортировка.
   - Пример: Сортировка слиянием.
     В процессе сортировки слиянием массив рекурсивно делится на два, затем элементы сливаются обратно, что даёт сложность `O(n log n)`.
* O(n²) — Квадратичная сложность
   - Алгоритм выполняет вложенные циклы, каждый из которых проходит по всем элементам. Это типично для алгоритмов, которые требуют сравнения всех пар данных.
   - Пример: Пузырьковая сортировка. В пузырьковой сортировке каждый элемент массива сравнивается с каждым другим элементом, что даёт сложность `O(n^2)`.
* O(2^n) — Экспоненциальная сложность
   - Алгоритм выполняет количество операций, которое удваивается с увеличением размера входных данных. Это часто встречается в решениях с полной проверкой всех возможных вариантов.
   - Пример: Рекурсивное вычисление чисел Фибоначчи. Количество вызовов функции растёт экспоненциально, и сложность алгоритма — `O(2^n)`.
* O(n!) — Факториальная сложность
   - Это самая высокая сложность, которая возникает, например, при решении задач, связанных с перестановками.
   - Пример: Задача о коммивояжёре (поиск оптимального пути для посещения всех городов). Алгоритм с факториальной сложностью перебирает все возможные варианты, что может быть очень медленно при увеличении `n`.

##### <a name="spaceComplexity"></a>Пространственная сложность (Space Complexity)
Пространственная сложность описывает, сколько памяти требует алгоритм для хранения данных в процессе работы. Например, алгоритм может использовать дополнительную память для хранения промежуточных данных или копий входных данных.
* O(1) — Константная пространственная сложность. Например, если алгоритм использует только несколько переменных, независимо от размера входных данных.
* O(n) — Линейная пространственная сложность. Например, если алгоритм создаёт новый массив или структуру данных размером с входные данные.
* O(n²) — Квадратичная пространственная сложность. Например, если алгоритм использует двумерный массив для хранения данных.