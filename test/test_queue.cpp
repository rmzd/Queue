#include "Queue.h"
#include <gtest.h>
#include <fstream>
#include <sstream>

using namespace std;

TEST(tqueue_test, default_constructor)
{
    TQueue<int> q;
    EXPECT_EQ(q.GetLen(), 0);
    EXPECT_EQ(q.GetStart(), 0);
    EXPECT_EQ(q.GetEnd(), 0);
    EXPECT_EQ(q.GetCount(), 0);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(tqueue_test, size_constructor)
{
    TQueue<int> q(6);
    EXPECT_EQ(q.GetLen(), 6);
    EXPECT_EQ(q.GetCount(), 0);
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
}

TEST(tqueue_test, size_constructor_negative)
{
    EXPECT_THROW(TQueue<int> q(-2), invalid_argument);
}

TEST(tqueue_test, copy_constructor)
{
    TQueue<int> q1(4);
    q1.Push(11);
    q1.Push(22);

    TQueue<int> q2(q1);
    EXPECT_EQ(q2.GetLen(), 4);
    EXPECT_EQ(q2.GetCount(), 2);
    EXPECT_EQ(q2.Pop(), 11);
    EXPECT_EQ(q2.Pop(), 22);
}

TEST(tqueue_test, move_constructor)
{
    TQueue<int> q1(5);
    q1.Push(13);
    q1.Push(26);

    TQueue<int> q2(move(q1));
    EXPECT_EQ(q2.GetLen(), 5);
    EXPECT_EQ(q2.GetCount(), 2);
    EXPECT_EQ(q1.GetLen(), 0);
}

TEST(tqueue_test, push_and_pop)
{
    TQueue<int> q(4);
    q.Push(101);
    EXPECT_EQ(q.GetCount(), 1);
    EXPECT_FALSE(q.IsEmpty());

    q.Push(202);
    q.Push(303);
    EXPECT_EQ(q.GetCount(), 3);
    EXPECT_TRUE(q.IsFull());

    EXPECT_EQ(q.Pop(), 101);
    EXPECT_EQ(q.Pop(), 202);
    EXPECT_EQ(q.Pop(), 303);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(tqueue_test, push_full_queue)
{
    TQueue<int> q(2);
    q.Push(7);
    EXPECT_THROW(q.Push(8), logic_error);
}

TEST(tqueue_test, pop_empty_queue)
{
    TQueue<int> q(3);
    EXPECT_THROW(q.Pop(), logic_error);
}

TEST(tqueue_test, circular_behavior)
{
    TQueue<int> q(4);

    q.Push(5);
    q.Push(10);
    q.Push(15);
    EXPECT_TRUE(q.IsFull());

    EXPECT_EQ(q.Pop(), 5);
    EXPECT_EQ(q.Pop(), 10);

    q.Push(20);
    q.Push(25);

    EXPECT_EQ(q.Pop(), 15);
    EXPECT_EQ(q.Pop(), 20);
    EXPECT_EQ(q.Pop(), 25);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(tqueue_test, copy_assignment)
{
    TQueue<int> q1(3);
    q1.Push(33);
    q1.Push(66);

    TQueue<int> q2;
    q2 = q1;

    EXPECT_EQ(q2.GetLen(), 3);
    EXPECT_EQ(q2.GetCount(), 2);
    EXPECT_EQ(q2.Pop(), 33);
    EXPECT_EQ(q2.Pop(), 66);
}

TEST(tqueue_test, move_assignment)
{
    TQueue<int> q1(3);
    q1.Push(44);
    q1.Push(88);

    TQueue<int> q2;
    q2 = move(q1);

    EXPECT_EQ(q2.GetLen(), 3);
    EXPECT_EQ(q2.GetCount(), 2);
    EXPECT_EQ(q1.GetLen(), 0);
}

TEST(tqueue_test, self_assignment)
{
    TQueue<int> q(3);
    q.Push(55);
    q.Push(99);

    q = q;

    EXPECT_EQ(q.GetCount(), 2);
    EXPECT_EQ(q.Pop(), 55);
}

// Операторы сравнения
TEST(tqueue_test, equality_operator)
{
    TQueue<int> q1(3);
    q1.Push(7);
    q1.Push(14);

    TQueue<int> q2(3);
    q2.Push(7);
    q2.Push(14);

    EXPECT_TRUE(q1 == q2);
}

TEST(tqueue_test, inequality_operator)
{
    TQueue<int> q1(3);
    q1.Push(8);
    q1.Push(16);

    TQueue<int> q2(3);
    q2.Push(8);
    q2.Push(18);

    EXPECT_TRUE(q1 != q2);
}

TEST(tqueue_test, equality_different_sizes)
{
    TQueue<int> q1(3);
    q1.Push(9);

    TQueue<int> q2(4);
    q2.Push(9);

    EXPECT_FALSE(q1 == q2);
}

// Методы доступа
TEST(tqueue_test, get_count)
{
    TQueue<int> q(5);
    EXPECT_EQ(q.GetCount(), 0);

    q.Push(12);
    EXPECT_EQ(q.GetCount(), 1);

    q.Push(24);
    EXPECT_EQ(q.GetCount(), 2);

    q.Pop();
    EXPECT_EQ(q.GetCount(), 1);
}

TEST(tqueue_test, is_empty_and_is_full)
{
    TQueue<int> q(2);

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());

    q.Push(1);
    EXPECT_FALSE(q.IsEmpty());
    EXPECT_TRUE(q.IsFull());

    q.Pop();
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
}

// FindMin
TEST(tqueue_test, find_min)
{
    TQueue<int> q(6);
    q.Push(23);
    q.Push(7);
    q.Push(31);
    q.Push(2);
    q.Push(15);

    EXPECT_EQ(q.FindMin(), 2);
}

TEST(tqueue_test, find_min_single_element)
{
    TQueue<int> q(3);
    q.Push(42);
    EXPECT_EQ(q.FindMin(), 42);
}

TEST(tqueue_test, find_min_empty_queue)
{
    TQueue<int> q(3);
    EXPECT_THROW(q.FindMin(), logic_error);
}

// Resize
TEST(tqueue_test, resize)
{
    TQueue<int> q(3);
    q.Push(13);
    q.Push(26);

    q.Resize(5);
    EXPECT_EQ(q.GetLen(), 5);
    EXPECT_EQ(q.GetCount(), 2);
    EXPECT_EQ(q.Pop(), 13);
    EXPECT_EQ(q.Pop(), 26);
}

TEST(tqueue_test, resize_too_small)
{
    TQueue<int> q(3);
    q.Push(17);
    q.Push(34);

    EXPECT_THROW(q.Resize(1), logic_error);
}

// Потоки
TEST(tqueue_test, output_operator)
{
    TQueue<int> q(3);
    q.Push(21);
    q.Push(42);

    stringstream ss;
    ss << q;
    string result = ss.str();

    EXPECT_TRUE(result.find("TQueue") != string::npos);
    EXPECT_TRUE(result.find("count=2") != string::npos);
}

TEST(tqueue_test, save_and_load_to_file)
{
    TQueue<int> q1(5);
    q1.Push(101);
    q1.Push(202);
    q1.Push(303);

    q1.SaveToFile("test_queue.txt");

    TQueue<int> q2;
    q2.LoadFromFile("test_queue.txt");

    EXPECT_EQ(q1.GetCount(), q2.GetCount());
    EXPECT_EQ(q1.Pop(), q2.Pop());
    EXPECT_EQ(q1.Pop(), q2.Pop());
    EXPECT_EQ(q1.Pop(), q2.Pop());
}

TEST(tqueue_test, load_from_nonexistent_file)
{
    TQueue<int> q;
    EXPECT_THROW(q.LoadFromFile("nonexistent_file.txt"), runtime_error);
}

TEST(tqueue_test, save_empty_queue)
{
    TQueue<int> q(3);
    EXPECT_NO_THROW(q.SaveToFile("empty_queue.txt"));
}

// Разные типы
TEST(tqueue_test, double_queue)
{
    TQueue<double> q(3);
    q.Push(1.11);
    q.Push(2.22);

    EXPECT_EQ(q.GetCount(), 2);
    EXPECT_DOUBLE_EQ(q.Pop(), 1.11);
    EXPECT_DOUBLE_EQ(q.FindMin(), 2.22);
}

TEST(tqueue_test, string_queue)
{
    TQueue<string> q(4);
    q.Push("apple");
    q.Push("banana");
    q.Push("cherry");

    EXPECT_EQ(q.GetCount(), 3);
    EXPECT_EQ(q.FindMin(), "apple");
}

// Особые случаи
TEST(tqueue_test, single_element_queue)
{
    TQueue<int> q(2);
    q.Push(99);

    EXPECT_EQ(q.GetCount(), 1);
    EXPECT_FALSE(q.IsEmpty());
    EXPECT_TRUE(q.IsFull());
    EXPECT_EQ(q.Pop(), 99);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(tqueue_test, wrap_around_multiple_times)
{
    TQueue<int> q(3);

    for (int i = 0; i < 10; i++) {
        q.Push(i * 100);
        q.Push(i * 100 + 1);
        EXPECT_EQ(q.Pop(), i * 100);
        EXPECT_EQ(q.Pop(), i * 100 + 1);
        EXPECT_TRUE(q.IsEmpty());
    }
}
