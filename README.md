# Будущие результаты future

В качестве шаблонного параметра `future` принимает тип возвращаемого значения функции сущности, ссылку на которую мы передали в `async`. В данном примере возвращаемое значение функции `changeValue` - `int`.
```cpp
int changeValue(int value) 
{
	…
	++value;
	return value;
}
…
future<int> simple_futer = async(changeValue, 10);
…
/*
 оба метода останавливают выполнение основного потока 
и дожидаются выполнения потока async
при этом get позволяет получить возвращаемое значение 
*/
value = simple_futer.get(); или simple_futer.wait();
```

Тут мы можем наблюдать как функционирует `future` - при инициализации объекта класса создается отдельный поток. Далее, параллельно продолает выполнение основной поток и блокируется только когда мы вызываем один их методов `get` или `wait`. При этом, этот метод можно вызвать лишь один раз. Кроме того, мы можем получить статус `future`. Есть метод ожидания до определенного времени и тп.
```cpp
…
auto status = simple_futer.wait_for();

// задача еще не запущена
status == std::future_status::deferred

// не дождались выполнения
status == std::future_status::timeout

// задача готова
status == std::future_status::ready
```
## Вызов метода get

```cpp 
// во вторичном потоке
int worker(int value) 
{
	…
	for (int index = 0; index < 100; index++) 
{
++value;
 cout << "=";
            this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return value;
}
``` 
```cpp
// в главном потоке
// возвращаемое значение worker - int
future<int> simple_futer = async(worker, value);

for (int32_t index = 0; index < 50; ++index) 
{
            cout << "+";
            this_thread::sleep_for(std::chrono::milliseconds(10));
}

value = simple_futer.get();
cout << "\nValue = " << value << endl;
 ```
При вызове метода `get()` основной поток был остановлен и ожидал результата выполнения функции `worker`.
```
+=+==++=+=+==++==++=+==++=+==++=+==++=+==++=+=+==+=++=+==++==++==+=++=+==++==++=+==++==++=+==++==+=+==================================================
Value = 100
```