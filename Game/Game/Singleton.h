#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton
{
private:
    static std::shared_ptr<T> instance_;
    static std::once_flag flag_;
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    /**
     * \brief Singleton ��ü�� ��ȯ�մϴ�.
     * \return instance
     */
    static std::shared_ptr<T> GetInstance()
    {
        std::call_once(flag_, []
            {
                instance_.reset(new T);
            });

        return instance_;
    }

    /**
     * \brief Singleton ��ü�� �޸� �����մϴ�.
     */
    void Release()
    {
        instance_.reset();
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_;
template <typename T>
std::once_flag Singleton<T>::flag_;