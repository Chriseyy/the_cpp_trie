#include <sstream>
#include "testTrie.h"
#include "gtest/gtest.h"

TEST(BasicTest, ConstructDestructEmpty) {
    Trie<int> trie;
    EXPECT_TRUE(trie.empty());
    Trie<int>* t = new Trie<int>;
    EXPECT_TRUE(t->empty());
    delete t;
}

TEST(BasicTest, InsertSimple) {
    Trie<string> trie;
    Trie<string>::value_type t("wer", "who");
    trie.insert(t);
    EXPECT_FALSE(trie.empty());
}

TEST(BasicTest, InsertPrint) {
    Trie<string> trie;
    Trie<string>::value_type t("wer", "who");
    trie.insert(t);
    stringstream strstr;
    strstr << trie;
    string res("w\n"
               "  e\n"
               "    r\n"
               "      :who\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, InsertComplex) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    stringstream strstr;
    strstr << trie;
    string res("w\n"
               "  e\n"
               "    r\n"
               "      :who\n"
               "  i\n"
               "    r\n"
               "      :we\n"
               "      d\n"
               "        :will\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}


TEST(BasicTest, InsertDouble) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "woh");
    Trie<string>::value_type t2("wer", "who");
    trie.insert(t1);
    trie.insert(t2);
    stringstream strstr;
    strstr << trie;
    string res("w\n"
               "  e\n"
               "    r\n"
               "      :who\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, InsertNumber) {
    Trie<int> trie;
    Trie<int>::value_type t1("zehn", 10);
    Trie<int>::value_type t2("elf", 11);
    Trie<int>::value_type t3("fuenfzig", 50);
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    size_t count = 0;
    for ( Trie<int>::iterator it = trie.begin(); it != trie.end(); ++it ){
        switch( count ) {
            case 0:
                EXPECT_STREQ((*it).first.c_str(), "elf");
                EXPECT_EQ((*it).second, 11);
                break;
            case 1:
                EXPECT_STREQ((*it).first.c_str(), "fuenfzig");
                EXPECT_EQ((*it).second, 50);
                break;
            case 2:
                EXPECT_STREQ((*it).first.c_str(), "zehn");
                EXPECT_EQ((*it).second, 10);
                break;
            default:
                ASSERT_TRUE(true);
                break;
        }
        count++;
    }
    EXPECT_TRUE(count==3);
}

TEST(BasicTest, InsertNumberClearEraseFind) {
    Trie<int> trie;
    Trie<int>::value_type t1("zehn", 10);
    Trie<int>::value_type t2("elf", 11);
    Trie<int>::value_type t3("fuenfzig", 50);
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    trie.erase("fuenfzig");
    EXPECT_FALSE(trie.empty());
    Trie<int>::iterator it = trie.find("elf");
    ASSERT_NE(it, trie.end());
    EXPECT_EQ((*it).second, 11);
    trie.clear();
    EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, InsertSameElement) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    trie.insert(t1);
    trie.erase("was");
    EXPECT_FALSE(trie.empty());
    stringstream strstr;
    strstr << trie;
    string res("w\n"
               "  e\n"
               "    r\n"
               "      :who\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());
    trie.erase("wer");
    EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, EraseSimple) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    trie.erase("wer");
    EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, EraseFail) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    trie.erase("was");
    EXPECT_FALSE(trie.empty());
}

TEST(BasicTest, EraseComplex) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    trie.erase("wir");
    stringstream strstr;
    strstr << trie;
    string res("w\n"
               "  e\n"
               "    r\n"
               "      :who\n"
               "  i\n"
               "    r\n"
               "      d\n"
               "        :will\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, Clear) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    trie.clear();
    EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, ClearEmptyTrie) {
    Trie<string> trie;
    EXPECT_TRUE(trie.empty());
    trie.clear();
    EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, EmptyTrieBeginEqualEnd) {
    Trie<string> trie;
    EXPECT_TRUE(trie.empty());
    EXPECT_TRUE(trie.begin() == trie.end());
}

TEST(IteratorTest, Begin) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    EXPECT_TRUE(trie.begin() != trie.end());
}

TEST(IteratorTest, Star) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    Trie<string>::iterator it = trie.begin();
    EXPECT_STREQ((*it).first.c_str(), "wer");
    EXPECT_STREQ((*it).second.c_str(), "who");
}

TEST(IteratorTest, PlusPlus) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    trie.insert(t1);
    Trie<string>::iterator it = trie.begin();
    Trie<string>::iterator it2 = it;
    EXPECT_TRUE(it == it2);
    Trie<string>::iterator it3 = it++;
    EXPECT_TRUE(it3 == trie.begin());
    EXPECT_TRUE(it == trie.end());
    Trie<string>::iterator it4 = ++it3;
    EXPECT_TRUE(it4 == trie.end());
}

TEST(IteratorTest, ForLoop) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    size_t count = 0;
    for ( Trie<string>::iterator it = trie.begin(); it != trie.end(); ++it ){
        switch( count ) {
            case 0:
                EXPECT_STREQ((*it).first.c_str(), "wer");
                EXPECT_STREQ((*it).second.c_str(), "who");
                break;
            case 1:
                EXPECT_STREQ((*it).first.c_str(), "wir");
                EXPECT_STREQ((*it).second.c_str(), "we");
                break;
            case 2:
                EXPECT_STREQ((*it).first.c_str(), "wird");
                EXPECT_STREQ((*it).second.c_str(), "will");
                break;
            default:
                ASSERT_TRUE(true);
                break;
        }
        count++;
    }
    EXPECT_TRUE(count==3);
}


TEST(FindTest, Found) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);

    Trie<string>::iterator it = trie.find("wird");
    ASSERT_NE(it, trie.end());
    EXPECT_STREQ((*it).second.c_str(), "will");
}

TEST(FindTest, NotFound) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);

    Trie<string>::iterator it = trie.find("unknown_key");
    EXPECT_EQ(it, trie.end());
}

TEST(BasicTest, BigTest) {
    Trie<string> trie;
    Trie<string>::value_type t1("wer", "who");
    Trie<string>::value_type t2("wir", "we");
    Trie<string>::value_type t3("wird", "will");
    Trie<string>::value_type t4("wo", "where");
    Trie<string>::value_type t5("sind", "are");
    Trie<string>::value_type t6("weiss", "know");

    trie.insert(t1);
    trie.insert(t2);
    trie.insert(t3);
    trie.insert(t4);
    trie.insert(t5);
    trie.insert(t6);

    stringstream strstr;
    strstr << trie;
    string res("s\n"
               "  i\n"
               "    n\n"
               "      d\n"
               "        :are\n"
               "w\n"
               "  e\n"
               "    i\n"
               "      s\n"
               "        s\n"
               "          :know\n"
               "    r\n"
               "      :who\n"
               "  i\n"
               "    r\n"
               "      :we\n"
               "      d\n"
               "        :will\n"
               "  o\n"
               "    :where\n");
    EXPECT_STREQ(res.c_str(), strstr.str().c_str());

    size_t count = 0;
    for ( Trie<string>::iterator it = trie.begin(); it != trie.end(); ++it ){
        switch( count ) {
            case 0:
                EXPECT_STREQ((*it).first.c_str(), "sind");
                EXPECT_STREQ((*it).second.c_str(), "are");
                break;
            case 1:
                EXPECT_STREQ((*it).first.c_str(), "weiss");
                EXPECT_STREQ((*it).second.c_str(), "know");
                break;
            case 2:
                EXPECT_STREQ((*it).first.c_str(), "wer");
                EXPECT_STREQ((*it).second.c_str(), "who");
                break;
            case 3:
                EXPECT_STREQ((*it).first.c_str(), "wir");
                EXPECT_STREQ((*it).second.c_str(), "we");
                break;
            case 4:
                EXPECT_STREQ((*it).first.c_str(), "wird");
                EXPECT_STREQ((*it).second.c_str(), "will");
                break;
            case 5:
                EXPECT_STREQ((*it).first.c_str(), "wo");
                EXPECT_STREQ((*it).second.c_str(), "where");
                break;
            default:
                ASSERT_TRUE(true);
                break;
        }
        count++;
    }
    EXPECT_TRUE(count==6);

    Trie<string>::iterator it = trie.find("wer");
    ASSERT_NE(it, trie.end());
    EXPECT_STREQ((*it).second.c_str(), "who");

    Trie<string>::iterator itu = trie.find("unknown_key");
    EXPECT_EQ(itu, trie.end());

    Trie<string>::iterator itb = trie.begin();
    EXPECT_STREQ((*itb).first.c_str(), "sind");
    EXPECT_STREQ((*itb).second.c_str(), "are");

    trie.erase("was");
    
    trie.erase("weiss");
    trie.erase("wo");
    EXPECT_FALSE(trie.empty());

    trie.clear();
    EXPECT_TRUE(trie.empty());

    EXPECT_TRUE(trie.begin() == trie.end());
}