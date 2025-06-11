#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <stdexcept>

class PriorityQueue {
private:
    std::vector<std::pair<int, std::string>> heap; // вектор пар <приоритет, строка>

    void heapifyUp(int index) { // поднимает элемент вверх , если его приоритет выше , чем у родителя
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].first >= heap[index].first) break;
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) { // опускает элемент вниз , если его приоритет ниже , чем у дочерних элементов
        while (true) {
            int left = 2 * index + 1; 
            int right = 2 * index + 2;
            int largest = index;

            if (left < heap.size() && heap[left].first > heap[largest].first)
                largest = left;
            if (right < heap.size() && heap[right].first > heap[largest].first)
                largest = right;
            if (largest == index) break;

            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    void Enqueue(int priority, const std::string& data) { // добавление элемента с заданным приоритетом и данными   
        heap.emplace_back(priority, data);
        heapifyUp(heap.size() - 1);
    }

    std::pair<int, std::string> Dequeue() { // возвращает элемент с наивысшим приоритетом и удаляет его
        if (heap.empty()) throw std::runtime_error("Queue is empty");
        auto max = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return max;
    }

    std::pair<int, std::string> Peek() const { // возвращает элемент с наивысшим приоритетом и не удаляет его
        if (heap.empty()) throw std::runtime_error("Queue is empty");
        return heap.front();
    }

    bool IsEmpty() const { return heap.empty(); } // пуста ли очередь
};

void TestPriorityQueue() {
    PriorityQueue pq;
    
    std::cout << "=== тестирование очереди ===" << std::endl;
    pq.Enqueue(3, "помыть посуду");
    pq.Enqueue(1, "купить хлеб");
    pq.Enqueue(4, "сделать ДЗ");
    pq.Enqueue(2, "позвонить маме");

    std::cout << "следующая задача: " << pq.Peek().second 
              << " (приоритет: " << pq.Peek().first << ")" << std::endl;

    std::cout << "\n обработка задач по приоритету:" << std::endl;
    while (!pq.IsEmpty()) {
        auto task = pq.Dequeue();
        std::cout << "выполнено: " << task.second 
                  << " (приоритет: " << task.first << ")" << std::endl;
    }
}

void MeasurePerformance() {
    const int start_size = 1000;
    const int end_size = 10000;
    const int step = 1000;
    const int measurements = 5;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000); // генерация случайных приоритетов от 1 ло 1000

    std::cout << "\n=== измерение производительности ===" << std::endl;
    std::cout << "размер | enqueue (мс) | dequeue (мс)" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    for (int size = start_size; size <= end_size; size += step) { // проходимся по различным размерам очереди , увеличивая шаг на step
        double total_enqueue = 0;
        double total_dequeue = 0;

        for (int m = 0; m < measurements; ++m) { // столько раз выполняем измерения 
            PriorityQueue pq; // и создаем каждый раз новый экземпляр очереди для каждого измерения 
            
            // измеряем время добавления
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < size; ++i) {
                pq.Enqueue(dist(gen), "Task" + std::to_string(i));
            }
            auto end = std::chrono::high_resolution_clock::now();
            total_enqueue += std::chrono::duration<double, std::milli>(end - start).count(); // сохраняем время выполнения в total_enqueue

            // измеряем время , необходимое для извлечения всех задач из очереди
            start = std::chrono::high_resolution_clock::now();
            while (!pq.IsEmpty()) {
                pq.Dequeue();
            }
            end = std::chrono::high_resolution_clock::now();
            total_dequeue += std::chrono::duration<double, std::milli>(end - start).count();
        }

        std::cout << size << " | " 
                  << total_enqueue / measurements << " | " 
                  << total_dequeue / measurements << std::endl;
    }
}

int main() {
    TestPriorityQueue();
    MeasurePerformance();
    return 0;
}