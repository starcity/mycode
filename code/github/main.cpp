#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"




class User {
public:
    User() {};
    ~User() {};
public:
    // 登录
    virtual bool Login(const std::string& username, const std::string& password) = 0;
    // 支付
    virtual bool Pay(int money) = 0;
    // 是否登录
    virtual bool Online() = 0;

};



class Biz {
public:
    void SetUser(User* user) {
        _user = user;

    }

    std::string pay(const std::string& username, const std::string& password, int money) {
        std::string ret;
        if (!_user) {
            ret = "pointer is null.";
            return ret;

        }

        if (!_user->Online()) {
            ret = "logout status.";
            // 尚未登录，要求登录
            if (!_user->Login(username, password)) {
                // 登录失败
                ret += "login error.";
                return ret;
            } else {
                // 登录成功
                ret += "login success.";
            }
        } else {
            // 已登录
            ret = "login.status";
        }

        if (!_user->Pay(money)) {
            ret += "pay error.";
        } else {
            ret += "pay success.";
        }

        return ret;
    }

private:
    User* _user;
};



class TestUser : public User {
public:
    MOCK_METHOD2(Login, bool(const std::string&, const std::string&));
    MOCK_METHOD1(Pay, bool(int));
    MOCK_METHOD0(Online, bool());

};



TEST(Test, USER)
{
    using ::testing::_;

    TestUser test_user;
    EXPECT_CALL(test_user, Online()).WillOnce(testing::Return(true));
    EXPECT_CALL(test_user, Login(_,_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(test_user, Pay(_)).WillRepeatedly(testing::Return(true));

    Biz biz;
    biz.SetUser(&test_user);
    std::string admin_ret = biz.pay("user", "", 1);
    admin_ret = biz.pay("user", "", 1);
}



int main(int argc,char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


