#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>

using namespace std;

// инициализируем объект класса future вызовом async
// блокирование основного потока и ожидание выполнения вторичного
namespace futureWaitExample
{

    int value = 0;

    void worker()
    {

        for (int index = 0; index < 100; index++)
        {

            ++value;

            cout << "=";

            this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void showExample()
    {

        // передаем в async ссылку на функциональную сущность
        // инициализируем объект класса future
        future<void> simple_futer = async(worker);

        for (int index = 0; index < 50; ++index)
        {

            cout << "+";

            this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        simple_futer.wait();

        cout << "\nValue = " << value << endl;
    }
}

// ожидание резултатов, передача агрументов
namespace futureGetValue
{

    int worker(int value)
    {

        for (int index = 0; index < 100; index++)
        {

            ++value;

            cout << "=";

            this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return value;
    }

    void showExample()
    {

        int value = 0;

        // передаем в async ссылку на функциональную сущность
        // инициализируем объект класса future
        future<int> simple_futer = async(worker, value);

        // можем дождаться тут а после вызвать get
        // simple_futer.wait();

        for (int index = 0; index < 50; ++index)
        {

            cout << "+";

            this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // simple_futer.wait();
        value = simple_futer.get();
        // вызвать метот get можно только один раз
        // value = simple_futer.get(); // - terminate

        cout << "\nValue = " << value << endl;
    }
}

// разделяемый future
namespace sharedFutureExample
{

    int worker(int value)
    {

        std::thread::id id = this_thread::get_id();

        cout << "<wor START>"
             << " (thread: " << id << ")" << endl;

        for (int index = 0; index < 100; index++)
        {

            ++value;

            cout << index << " (thread: " << id << ")" << endl;

            this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        cout << "<wor END>"
             << " (thread: " << id << ")" << endl;

        // throw exception("Any exeption"); // искуственно вызываем исключение
        //  на самом деле в таком случае достаточно
        //  обрботать исключение обернув get во вторичных потоках

        // значение уже получено, но далее работает како-нибудь финализирующий код
        this_thread::sleep_for(std::chrono::seconds(2));

        return value;
    }

    void firstWorker(shared_future<int> &simple_future)
    {
        // void firstWorker(shared_future<int32_t> simple_future) {

        std::thread::id id = this_thread::get_id();

        cout << endl
             << "<fir START>"
             << " (thread: " << id << ")";

        // future в случае исключения пробросит его в качестве возвращаемого
        // поэтому можем его обработать во вторичном потоке
        try
        {
            int value = simple_future.get();
            cout << endl
                 << "fir processed value: " << value;
        }
        catch (const exception &exc)
        {
            cout << exc.what() << endl;
        }

        cout << endl
             << "<fir END>"
             << " (thread: " << id << ")";
    }

    void secondWorker(shared_future<int> &simple_future)
    {
        // void secondWorker(shared_future<int32_t> simple_future) {

        std::thread::id id = this_thread::get_id();

        cout << endl
             << "<sec START>"
             << " (thread: " << id << ")";

        try
        {
            int value = simple_future.get();
            cout << endl
                 << "sec processed value: " << value;
        }
        catch (const exception &exc)
        {
            cout << exc.what() << endl;
        }

        cout << endl
             << "<sec END>"
             << " (thread: " << id << ")";
    }

    void showExample()
    {

        int value = 0;

        //
        shared_future<int> simple_futer = async(worker, value);

        thread firThr(firstWorker, ref(simple_futer));
        thread secThr(secondWorker, ref(simple_futer));

        /* возможно для shared_future
        thread firThr(firstWorker, simple_futer);
        thread secThr(secondWorker, simple_futer);
        */

        firThr.join();
        secThr.join();
    }
}

int main()
{
    futureWaitExample::showExample();
    // sharedFutureExample::showExample();
    return 0;
}