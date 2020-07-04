#pragma once

namespace tagc
{
    template <typename T>
    class LinkedList;
    template <typename T>
    class LinkedListNode;

    template <typename T>
    void tagc_free_node(LinkedListNode<T> *head);
    template <typename T>
    LinkedListNode<T> *tagc_copy_list(const LinkedListNode<T> *other);
    template <typename T>
    unsigned long tagc_list_len(const LinkedListNode<T> *head);
    template <typename T>
    LinkedListNode<T> *tagc_append_node(LinkedListNode<T> *head, LinkedListNode<T> *node);

    namespace linked_list
    {
        template <typename T>
        class iterator
        {
        public:
            iterator() : iterator(nullptr) {}
            explicit iterator(LinkedListNode<T> *p) : _p(p) {}

            T &operator*() { return _p->value; }
            iterator<T> &operator++()
            {
                if (_p)
                {
                    _p = _p->next;
                }
                return *this;
            }
            iterator<T> operator++(int)
            {
                iterator<T> ret = *this;
                ++(*this);

                return ret;
            }

            bool operator==(const iterator<T> &other) const { return _p == other._p; }
            bool operator!=(const iterator<T> &other) const { return _p != other._p; }

        private:
            LinkedListNode<T> *_p;
        };

        template <typename T>
        class const_iterator
        {
        public:
            const_iterator() : const_iterator(nullptr) {}
            explicit const_iterator(const LinkedListNode<T> *p) : _p(p) {}

            const T &operator*() const { return _p->value; }
            const_iterator<T> &operator++()
            {
                if (_p)
                {
                    _p = _p->next;
                }
                return *this;
            }
            const_iterator<T> operator++(int)
            {
                const_iterator<T> ret = *this;
                ++(*this);

                return ret;
            }

            bool operator==(const const_iterator<T> &other) const { return _p == other._p; }
            bool operator!=(const const_iterator<T> &other) const { return _p != other._p; }

        private:
            const LinkedListNode<T> *_p;
        };
    } // namespace linked_list

    template <typename T>
    LinkedListNode<T> *tagc_copy_list(const LinkedListNode<T> *other)
    {
        if (!other)
            return nullptr;

        auto ret = new LinkedListNode<T>{other->value, nullptr};
        ret->next = tagc_copy_list(other->next);

        return ret;
    }

    template <typename T>
    const T &tagc_list_last(const LinkedListNode<T> *head)
    {
        assert(head != nullptr);
        if (head->next == nullptr)
        {
            return head->value;
        }
        else
        {
            return tagc_list_last(head->next);
        }
    }

    template <typename T>
    class LinkedList
    {
    public:
        LinkedList();
        ~LinkedList();
        void push(const T &val);
        void append(const T &val);

        LinkedList(const LinkedList<T> &other)
        {
            _head = tagc_copy_list(other._head);
        }

        LinkedList<T> &operator=(const LinkedList<T> &other)
        {
            if (this == &other)
            {
                return *this;
            }
            else
            {
                tagc_free_node(this->_head);
                this->_head = tagc_copy_list(other._head);
                return *this;
            }
        }

        LinkedList(LinkedList<T> &&other) noexcept : _head(nullptr)
        {
            _head = other._head;

            other._head = nullptr;
        }

        LinkedList<T> &operator=(LinkedList<T> &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            else
            {
                tagc_free_node(_head);
                _head = other._head;

                other._head = nullptr;
                return *this;
            }
        }

        linked_list::iterator<T> begin() { return linked_list::iterator<T>{_head}; }
        linked_list::iterator<T> end() const { return linked_list::iterator<T>{nullptr}; }

        linked_list::const_iterator<T> cbegin() const { return linked_list::const_iterator<T>{_head}; }
        linked_list::const_iterator<T> cend() const { return linked_list::const_iterator<T>{nullptr}; }

        unsigned long length() const { return tagc_list_len(_head); }
        const T &last() const { return tagc_list_last(_head); }

    private:
        LinkedListNode<T> *_head;
    };

    template <typename T>
    struct LinkedListNode
    {
        T value;
        LinkedListNode<T> *next;
    };

    template <typename T>
    LinkedList<T>::LinkedList()
    {
        _head = nullptr;
    }

    template <typename T>
    void tagc_free_node(LinkedListNode<T> *head)
    {
        if (head == nullptr)
            return;
        auto next = head->next;
        delete head;
        tagc_free_node(next);
    }

    template <typename T>
    LinkedList<T>::~LinkedList()
    {
        tagc_free_node(this->_head);
    }

    template <typename T>
    void LinkedList<T>::push(const T &val)
    {
        auto *node = new LinkedListNode<T>{val, this->_head};
        this->_head = node;
    }

    template <typename T>
    void LinkedList<T>::append(const T &val)
    {
        auto *node = new LinkedListNode<T>{val, nullptr};
        this->_head = tagc_append_node(_head, node);
    }

    template <typename T>
    unsigned long tagc_list_len(const LinkedListNode<T> *head)
    {
        if (!head)
            return 0;
        return 1 + tagc_list_len(head->next);
    }

    template <typename T>
    LinkedListNode<T> *tagc_append_node(LinkedListNode<T> *head, LinkedListNode<T> *node)
    {
        if (!head) {
            node->next = nullptr;
            return node;
        }
        head->next = tagc_append_node(head->next, node);

        return head;
    }
} // namespace tagc
