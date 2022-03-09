#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<list>
using namespace std;

typedef struct{
    string number;
    string name;
    int math;
    int cpp;
    int english;
    float gpa;
    int year;
}StuInfo;

typedef struct{
    string username;
    string password;
    int type;
}RegInfo;

class sys{
public:
    sys();
    ~sys();
    void Start();
protected:
    void ModifyPassword();      // 修改用户密码
    void Menu();                // 用户菜单
    void WriteStuLists();       // 写入学生信息
    void WriteRegLists();       // 写入用户信息
    void AddStu();              // 添加学生
    void FindStu();             // 寻找学生
    void ModifyStuInfo();       // 修改学生信息
    void DelStu();              // 删除学生
    void SortStu();             // 将学生信息排序
    void Find();                // 查找该生信息
private:
    list<RegInfo> regLists;     // 保存注册用户信息的容器
    list<StuInfo> stuLists;     // 保存学生信息的容器
    RegInfo user;               // 记录当前登陆账户
};

sys::sys() {
    fstream fileR;

    fileR.open("../reginfo.txt", ios::in);
    fileR.get();
    while(!fileR.eof()){
        RegInfo temp;
        fileR >> temp.username >> temp.password >> temp.type;
        regLists.push_back(temp);
    }
    fileR.close();
    // // 测试代码
    // for(list<RegInfo>::const_iterator it = regLists.begin(); it != regLists.end(); ++it){
    //     cout << it->username << "\t" << it->password << "\t" << it->type << endl;
    // }
    fileR.open("../stuinfo.txt", ios::in);
    fileR.get();
    while(!fileR.eof()){
        StuInfo temp;
        fileR >> temp.number >> temp.name >> temp.math >> temp.cpp 
            >> temp.english >> temp.gpa >> temp.year;
        stuLists.push_back(temp);   // 将所有学生信息数据读入stuLists列表中
    }
    fileR.close();
}

void sys::Start() {    
    bool flag = false;
    list<RegInfo>::const_iterator it;
    char select = '0';
    system("clear");
    cout << "|-------------------------|" << endl;
    cout << "|欢迎来到学生管理系统|" << endl;
    cout << "|-------------------------|" << endl;
    cout << "|1-登陆到已有的账户|" << endl;
    cout << "|2-注册新账户|" << endl;
    cout << "|q-退出|" << endl;
    while(select != 'q'){
        cout << "|-------------------------|" << endl;
        cout << "|请选择:";  cin >> select;
        switch(select){
            case '1':
                flag = false;
                while(!flag){                       // 判断用户是否存在
                    cout << "|用户名:";  cin >> user.username;
                    cout << "|密码:";    cin >> user.password;
                    for(it = regLists.begin(); it != regLists.end(); ++it){
                        if(user.username == it->username && user.password == it->password){
                            user.type = it->type;
                            flag = true;
                            break;
                        }
                    }
                    if(!flag){
                        cout << "-用户名或密码错误!请重试。" << endl;
                    }
                }
                system("clear");
                Menu();
                select = 'q';       // 防止二次进入
                break;
            case '2': 
                while(true){
                    cout << "|用户名:";  cin >> user.username;
                    for(it = regLists.begin(); it != regLists.end(); ++it){
                        if(user.username == it->username)
                            break;
                    }
                    if(it != regLists.end()){
                        cout << "-该用户名已存在，请重新输入." << endl;
                    }else{
                        break;
                    }
                }
                cout << "|密码:";    cin >> user.password;
                user.type = 1;
                regLists.push_back(user);
                break;
            case 'q':
                WriteRegLists();
                break;
            default:
                cout << "-输入有误!" << endl;
                break;
        }
    }
}

void sys::ModifyPassword() {
    cout << "|请输入新的密码:"; cin >> user.password;
    for(list<RegInfo>::iterator it = regLists.begin(); it != regLists.end(); ++it){
        if(it->username == user.username){
            regLists.erase(it);
            break;
        }
    }
    regLists.push_back(user);
    cout << "-修改密码成功。" << endl;
}

void sys::WriteStuLists() {
    fstream fileW("../stuinfo.txt", ios::out);

    stuLists.sort([](const StuInfo si1, const StuInfo si2){
        if(si1.number < si2.number) return true;
        else return false;
    });     // 按学号升序排序
    for(list<StuInfo>::const_iterator it = stuLists.begin(); it != stuLists.end(); ++it){
        fileW << endl << it->number << "\t" << it->name << "\t" << it->math << "\t" << it->cpp << "\t" 
            << it->english << "\t" << fixed << setprecision(2) << it->gpa << "\t" << setprecision(0) << it->year;
    }
    fileW.close();
}

void sys::WriteRegLists() {
    fstream fileW("../reginfo.txt", ios::out);

    for(list<RegInfo>::const_iterator it = regLists.begin(); it != regLists.end(); ++it){
        fileW << endl << it->username << "\t" << it->password << "\t" << it->type;
    }
    fileW.close();
}

void sys::AddStu() {
    StuInfo temp;

    cout << "|学号:";   cin >> temp.number;
    cout << "|姓名:";   cin >> temp.name;
    cout << "|高数成绩:";   cin >> temp.math;
    cout << "|CPP成绩:";    cin >> temp.cpp;
    cout << "|英语成绩:";   cin >> temp.english;
    cout << "|学年:";   cin >> temp.year;
    temp.gpa = (temp.math + temp.cpp + temp.english) * 1.0 / 3;
    stuLists.push_back(temp);

    list<RegInfo>::const_iterator it;
    for(it = regLists.begin(); it != regLists.end(); ++it){
        if(it->username == temp.number)
            break;
    }
    if(it == regLists.end()){                   // 该生第一次注册
        RegInfo ri;
        ri.username = ri.password = temp.number;
        ri.type = 2;
        regLists.push_back(ri);
    }
}

void sys::FindStu() {
    string str;
    int count = 0;
    cout << "|请输入要查询的学生(学号或姓名):"; cin >> str;
    for(list<StuInfo>::const_iterator it = stuLists.begin(); it != stuLists.end(); ++it){
        if(it->number == str || it->name == str){
            if(!count){
                cout << "-学号:" << it->number << endl;
                cout << "-姓名:" << it->name << endl;
            }
            cout << "-高数:" << it->math << " CPP:" << it->cpp << " 英语:" << it->english
                << " GPA:" << fixed << setprecision(2) << it->gpa << " 学年:" << it->year << endl;
            ++count;
        }
    }
    if(!count){
        cout << "-未查询到相关学生信息。" << endl;
    }
}

void sys::ModifyStuInfo() {
    list<StuInfo> perList;      // 可以改为只遍历stuLists就是实现该功能
    StuInfo si;
    string str;
    int year;
    char select = '0';

    cout << "|请输入要查询的学生(学号或姓名):"; cin >> str;
    for(list<StuInfo>::iterator it = stuLists.begin(); it != stuLists.end();){
        if(str == it->number || str == it->name){
            list<StuInfo>::iterator del = it++;
            perList.push_back(*del);
            stuLists.erase(del);
        }else{
            ++it;
        }
    }
    if(!perList.empty()){
        while(select != 'q'){
            cout << "|请输入要修改的学年:"; cin >> year;
            list<StuInfo>::iterator it;
            for(it = perList.begin(); it != perList.end(); ++it){
                if(year == it->year){
                    cout << "|--------------------------|" << endl;
                    cout << "|1-高数成绩|" << endl;
                    cout << "|2-CPP成绩|" << endl;
                    cout << "|3-英语成绩|" << endl;
                    cout << "|q-退出|" << endl;
                    while(select != 'q'){
                        cout << "|--------------------------|" << endl;
                        cout << "|请选择:"; cin >> select;
                        switch(select){
                            case '1':
                                cout << "|高数:";   cin >> it->math;    break;
                            case '2':
                                cout << "|CPP:";    cin >> it->cpp;     break;
                            case '3':
                                cout << "|英语:";   cin >> it->english; break;
                            case 'q':
                                break;
                            default:
                                cout << "-输入有误，请重新输入。" << endl;
                        }
                    } //while
                    it->gpa = (it->math + it->cpp + it->english) * 1.0 / 3;
                    break;
                } //if
            } //for
            if(it == perList.end()){
                cout << "-输入学年有误，请重新输入。" << endl;
            }
        }
    }else{
        cout << "-未查询到该生的信息。" << endl;
    }

    stuLists.splice(stuLists.begin(), perList); // 将修改后的数据合并到源容器中
}

void sys::DelStu() {
    bool flag = false;

    string str;
    cout << "|请输入要删除的学生学号:"; cin >> str;
    for(list<StuInfo>::iterator it = stuLists.begin(); it != stuLists.end();){
        if(it->number == str){
            list<StuInfo>::iterator del = it++;
            stuLists.erase(del);            // 删除学生信息
            flag = true;
        }else{
            ++it;   // 不能放在for(里面)
        }
    }
    if(flag){
        for(list<RegInfo>::iterator it = regLists.begin(); it != regLists.end(); ++it){
            if(it->username == str){
                regLists.erase(it);         // 删除学生登陆信息
                break;
            }
        }
        cout << "-学号:" << str << "的学生已删除成功。" << endl;
    }else{
        cout << "-未查询到该学生信息，无法删除！" << endl;
    }
}

void sys::SortStu() {
    char select = '0';
    string str;
    string path;

    cout << "|--------------------------|" << endl;
    cout << "|1-按高数成绩降序排序|" << endl;
    cout << "|2-按CPP成绩降序排序|" << endl;
    cout << "|3-按英语成绩降序排序|" << endl;
    cout << "|4-按GPA降序排序|" << endl;
    cout << "|q-退出|" << endl;
    while(select != 'q'){
        cout << "|--------------------------|" << endl;
        cout << "|请选择:"; cin >> select;
        switch(select){
            case '1':
                stuLists.sort([](const StuInfo si1, const StuInfo si2){
                    if(si1.math > si2.math) return true;
                    else    return false;
                });
                break;
            case '2':
                stuLists.sort([](const StuInfo si1, const StuInfo si2){
                    if(si1.cpp > si2.cpp)   return true;
                    else    return false;
                });
                break;
            case '3':
                stuLists.sort([](const StuInfo si1, const StuInfo si2){
                    if(si1.english > si2.english)   return true;
                    else    return false;
                });
                break;
            case '4':
                stuLists.sort([](const StuInfo si1, const StuInfo si2){
                    if(si1.gpa > si1.gpa)   return true;
                    else    return false;
                });
                break;
            case 'q':   break;
            default:
                cout << "-输入有误，请重新输入。" << endl;
                break;
        }

        // // 测试代码
        // for(list<StuInfo>::const_iterator it = stuLists.begin(); it != stuLists.end(); ++it){
        //     cout << it->number << "\t" << it->name << "\t" << it->math << "\t" << it->cpp << "\t"
        //         << it->english << "\t" << it->gpa << endl;
        // }

        if(select != 'q'){
            cout << "|请输入保存文件名:";   cin >> str;
            path = "../.txt";
            path.insert(3, str);            // path = "../str.txt"
            fstream fileW(path, ios::out);
            for(list<StuInfo>::const_iterator it = stuLists.begin(); it != stuLists.end(); ++it){
                fileW << endl << it->number << "\t" << it->name << "\t" << it->math << "\t"
                    << it->cpp << "\t" << it->english << "\t" << fixed << setprecision(2) << it->gpa << "\t"
                    << setprecision(0) << it->year;
            }
            fileW.close();
        }
    }
}

void sys::Find() {
    int count = 0;
    for(list<StuInfo>::const_iterator it = stuLists.begin(); it != stuLists.end(); ++it){
        if(it->number == user.username){
            if(!count){
                cout << "-学号:" << it->number << endl;
                cout << "-姓名:" << it->name << endl;
            }
            cout << "-高数:" << it->math << " CPP:" << it->cpp << " 英语:" << it->english
                << " GPA:" << it->gpa << " 学年:" << it->year << endl;
            ++count;
        }
    }
}

void sys::Menu() {
    char select = '0';

    if(user.type == 1){             // 管理员菜单
        cout << "|--------------------------|" << endl;
        cout << "|1-添加学生信息|" << endl;
        cout << "|2-查询学生信息|" << endl;
        cout << "|3-删除学生信息|" << endl;
        cout << "|4-修改学生信息|" << endl;
        cout << "|5-将学生排序|" << endl;
        cout << "|6-修改密码|" << endl;
        cout << "|q-退出|" << endl;
        while(select != 'q'){
            cout << "|--------------------------|" << endl;
            cout << "|请选择:"; cin >> select;
            switch(select){
                case '1':   AddStu();   break;
                case '2':   FindStu();  break;
                case '3':   DelStu();   break;
                case '4':   ModifyStuInfo();    break;
                case '5':   SortStu();  break;
                case '6':   ModifyPassword();   break;
                case 'q':   Start();    break;
                default:
                    cout << "-输入有误，请重新输入。" << endl;
                    break;
            }
        }
    }else{                          // 学生菜单
        cout << "|--------------------------|" << endl;
        cout << "|1-查询成绩信息|" << endl;
        cout << "|2-修改密码|" << endl;
        cout << "|q-退出|" << endl;
        while(select != 'q'){
            cout << "|--------------------------|" << endl;
            cout << "|请选择:"; cin >> select;
            switch(select){
                case '1':   Find();     break;
                case '2':   ModifyPassword();   break;
                case 'q':   Start();    break;
                default:
                    cout << "-输入有误，请重新输入。" << endl;
                    break;
            }
        }
    }
}

sys::~sys(){
    WriteStuLists();    // 将系统激活后对学生信息做的修改写入磁盘
    WriteRegLists();    // 将系统激活后对用户信息做的修改写入磁盘
    cout << "|-------------------------|" << endl;
    cout << "|再见|" << endl;
}

int main()
{
    sys s;

    s.Start();

    return 0;
}