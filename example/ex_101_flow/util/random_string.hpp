//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <QRandomGenerator>
#include <QString>

//产生随机字符串
inline QString generateRandomString(int min_length, int max_length)
{
    //确定字符串长度
    int length = QRandomGenerator::global()->bounded(min_length, max_length + 1);
    //预设的字符集，可根据需要修改
    const QString char_pool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString random_string;
    random_string.reserve(length);
    for (int i = 0; i < length; ++i)
    {
        //从字符集中随机选择一个字符
        int index = QRandomGenerator::global()->bounded(char_pool.length());
        QChar next_char = char_pool.at(index);
        random_string.append(next_char);
    }
    return random_string;
}
