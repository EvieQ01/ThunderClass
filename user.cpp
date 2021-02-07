/*************************************************************************
【文件名】User.cpp
【功能模块和目的】用户类操作定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【版权信息】
【更改记录】
    2020-06-10 由邱亦文添加User的几个只读函数
    2020-06-21 由邱亦文增添注释
*************************************************************************/
#include "user.h"
#include "admin.h"
#include "teacher.h"
#include "student.h"

unsigned int User::m_uUserCount = 0;
const unsigned int& User::UserCount = User::m_uUserCount;
vector<User*> User::m_UserList;
User* User::m_LoginedUser = nullptr;




/*************************************************************************
【函数名称】User::User
【函数功能】构造函数，生成一个拥有用户名，密码，账户类型的账户
【参数】均为入口参数，用户名和密码和账户类型
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
User::User(const string& Name, const string& Password, const string& Type):Type(m_sType) {
    m_sName = Name;
    m_sPassword = Password;
    m_sType = Type;
    m_uUserCount++;
}


/*************************************************************************
【函数名称】User::User
【函数功能】构造函数，从文件中读取一个拥有用户名，密码，账户类型的账户
【参数】入口参数，文件名
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
User::User(ifstream& inFile):Type(m_sType) {
    getline(inFile, m_sName);
    getline(inFile, m_sPassword);
    getline(inFile, m_sType);
    m_uUserCount++;
}

//析构函数
User::~User() {
    m_uUserCount--;
}

//拷贝构造函数
User::User(const User& anUser):Type(m_sType) {
    m_sName = anUser.m_sName;
    m_sPassword = anUser.m_sPassword;
    m_sType = anUser.m_sType;
}

//赋值运算符
User& User::operator=(const User& anUser) {
    if (this != &anUser) {
        m_sName = anUser.m_sName;
        m_sPassword = anUser.m_sPassword;
        m_sType = anUser.m_sType;
    }
    return *this;
}


/*************************************************************************
【函数名称】 User::ToMessage()
【函数功能】把用户名，密码打包成一个Message，用于上课时向教师端发送
【参数】无
【返回值】Message
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
Message User::ToMessage() const {
    unsigned int NameLen = m_sName.length();
    unsigned int PasswordLen = m_sPassword.length();
    unsigned char* Buffer = new unsigned char[NameLen + PasswordLen + 2 * sizeof (NameLen)];
    memcpy(Buffer, (char*)&NameLen, sizeof(NameLen));
    memcpy(Buffer + sizeof(NameLen), m_sName.c_str(), NameLen);
    memcpy(Buffer + sizeof(NameLen) + NameLen, (char*)&PasswordLen, sizeof(PasswordLen));
    memcpy(Buffer +  2 * sizeof(NameLen) + NameLen, m_sPassword.c_str(), PasswordLen);
    Message ret(MSG_ID_PSW, Buffer, NameLen + PasswordLen + 2 * sizeof (NameLen));
    delete [] Buffer;
    return ret;
}

/*************************************************************************
【函数名称】 User::SaveToFileStream()
【函数功能】把用户名，密码，账户类型存入文件
【参数】出口参数，文件名
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
void User::SaveToFileStream(ofstream& OutFile) const {
    OutFile << m_sName << endl;
    OutFile << m_sPassword << endl;
    OutFile << m_sType << endl;
}


/*************************************************************************
【函数名称】 User::GetName()
【函数功能】获得用户名只读
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-10
【更改记录】
*************************************************************************/
string User::GetName() const{
    return m_sName;
}

/*************************************************************************
【函数名称】 User::GetPassword()
【函数功能】获得密码只读
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-10
【更改记录】
*************************************************************************/
string User::GetPassword() const{
    return m_sPassword;
}

/*************************************************************************
【函数名称】 User::GetUserList()
【函数功能】获得用户列表只读
【参数】无
【返回值】vector<User*>
【开发者及日期】邱亦文 2020-06-10
【更改记录】
*************************************************************************/
vector<User*> User::GetUserList() {
    return m_UserList;
}



/*************************************************************************
【函数名称】 User::TestPassword()
【函数功能】比对密码
【参数】入口参数，需要比对的密码
【返回值】密码一致则为true
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
bool User::TestPassword(const string& Password) const {
    if (Password == m_sPassword) {
        return true;
    }
    else {
        return false;
    }
}

/*************************************************************************
【函数名称】 User::AddUser()
【函数功能】增加一个用户（仅有Admin类可以实现）
【参数】入口参数，用户名、密码、账户类型
【返回值】成功添加则为true
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
bool User::AddUser(const string& Name, const string& Password, const string& Type) {
    Admin* p = nullptr;
    p = dynamic_cast<Admin*>(this);
    if (p != nullptr) {
        if (Type == "Admin") {
            for (unsigned int i  = 0; i < m_UserList.size(); i++) {
                if (m_UserList[i]->Type == "Admin") {
                    return false;
                }
            }
            m_UserList.push_back(new Admin());
            return true;
        }
        else if (Type == "Teacher") {
            m_UserList.push_back(new Teacher(Name, Password));
            return true;
        }
        else {
            m_UserList.push_back((User*) new Student(Name, Password));
            return true;
        }
    }
    else {
        return false;
    }
}


/*************************************************************************
【函数名称】 User::DeleteUser()
【函数功能】删除一个用户（仅有Admin类可以实现）
【参数】入口参数，用户名、密码、账户类型
【返回值】成功删除则为true
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
bool User::DeleteUser(const string& Name, const string& Password, const string& Type) {
    Admin* p = nullptr;
    p = dynamic_cast<Admin*>(this);
    if (p != nullptr) {
        if (Type == "Admin") {
            return  false;
        }
        else {
            vector<User* >::iterator it;//声明一个迭代器，来访问vector容器，作用：遍历或者指向vector容器的元素
            unsigned int i = 0;
            for(it = m_UserList.begin(); it != m_UserList.end();it++, i++)
            {
                if (m_UserList[i]->GetPassword() == Password && m_UserList[i]->GetName() == Name) {
                    m_UserList.erase(it);
                    m_uUserCount--;
                    return true;
                }
            }
            return true;
        }
    }
    return false;
}



//2020-05-19 范静涛增加了未找到文件则自动创建只包含Admin用户的功能
void User::LoadFromFile(const string& FileName) {
    ifstream inFile(FileName.c_str());
    if (inFile.fail()) {
        ofstream OutFile(FileName.c_str(), ios::out | ios::app);
        OutFile << 1 << endl << "Admin" << endl << "Admin" << endl << "Admin" << endl;
        OutFile.close();
    }
    inFile.close();
    inFile.open(FileName.c_str());

    m_UserList.clear();
    unsigned int UserCount;
    inFile >> UserCount;
    inFile.get();
    for (unsigned int i  = 0; i < UserCount; i++) {
        string Name;
        string Password;
        string Type;
        getline(inFile, Name);
        getline(inFile, Password);
        getline(inFile, Type);
        if (Type == "Admin") {
            m_UserList.push_back(new Admin());
        }
        else if (Type == "Teacher") {
            m_UserList.push_back(new Teacher(Name, Password));
        }
        else {
            m_UserList.push_back((User*)new Student(Name, Password));
        }
    }
    inFile.close();
}



/*************************************************************************
【函数名称】 User::SaveToFile()
【函数功能】保存全部用户到文件
【参数】入口参数，文件名
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
void User::SaveToFile(const string& FileName) {
    ofstream OutFile(FileName.c_str());
    OutFile << m_UserList.size() << endl;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        m_UserList[i]->SaveToFileStream(OutFile);
    }
    OutFile.close();
}


/*************************************************************************
【函数名称】 User::GetUser()
【函数功能】获得特定用户指针
【参数】入口参数，用户名密码
【返回值】特定用户指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
const User* User::GetUser(const string& Name, const string& Password) {
    const User* Current = nullptr;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->GetName() == Name && m_UserList[i]->TestPassword(Password)) {
            Current = m_UserList[i];
            m_LoginedUser = m_UserList[i];
            break;
        }
    }
    return Current;

}

/*************************************************************************
【函数名称】 User::GetUser()
【函数功能】获得特定用户指针
【参数】入口参数，用户名（没有密码）
【返回值】特定用户指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
const User* User::GetUser(const string& Name) {
    const User* Current = nullptr;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->GetName() == Name) {
            Current = m_UserList[i];
            break;
        }
    }
    return Current;

}


/*************************************************************************
【函数名称】 User::GetLoginedUser()
【函数功能】静态成员，获得当前登录用户指针
【参数】无
【返回值】当前登录用户指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
User* User::GetLoginedUser() {
    return m_LoginedUser;
}

/*************************************************************************
【函数名称】 User::OfflineAllStudents()
【函数功能】静态成员，使全部学生下线
【参数】无
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
*************************************************************************/
void User::OfflineAllStudents() {
    Student* pStudent;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->Type == "Student") {
            pStudent = dynamic_cast<Student*>(m_UserList[i]);
            pStudent->Offline();
        }
    }
}
