// 程序入口：
// - 负责创建 Qt 应用上下文与主窗口。
// - 不承载业务逻辑，所有交互均在 MainWindow 中完成。
#include <QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    // Qt 应用初始化（解析命令行参数、加载平台插件等）。
    QApplication a(argc, argv);
    // 创建并展示主窗口。窗口生命周期由 Qt 事件循环托管。
    MainWindow w;
    w.show();
    // 进入事件循环，直到窗口关闭。
    return a.exec();
}
