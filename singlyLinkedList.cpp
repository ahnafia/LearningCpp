#include <memory>
#include <cstddef>

class SinglyLinkedList {
private:
    struct Node {
        int data;
        std::unique_ptr<Node> next;

        Node(int v) : data(v), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
    std::size_t size;

public:
    SinglyLinkedList() : head(nullptr), size(0) {}

    std::size_t length() const {
        return size;
    }

    void add(int value) { // push_back
        auto newNode = std::make_unique<Node>(value);

        if (!head) {
            head = std::move(newNode);
            ++size;
            return;
        }

        Node* curr = head.get();
        while (curr->next) {
            curr = curr->next.get();
        }

        curr->next = std::move(newNode);
        ++size;
    }

    bool remove_at(std::size_t index) { // 0-based
        if (index >= size) return false;

        if (index == 0) {
            head = std::move(head->next);
            --size;
            return true;
        }

        Node* curr = head.get();
        for (std::size_t i = 0; i < index - 1; ++i) {
            curr = curr->next.get();
        }

        curr->next = std::move(curr->next->next);
        --size;
        return true;
    }

    int find(int value) const {
        std::size_t index = 0;
        Node* curr = head.get();

        while (curr) {
            if (curr->data == value) return (int)index;
            curr = curr->next.get();
            ++index;
        }
        return -1;
    }
};
