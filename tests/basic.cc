#include <gtest/gtest.h>
#include <regexlite/regexlite.h>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

static QLatin1String drv_sqlite ("REGEXLITE");
static int cn_name = 1;


class TestUsage : public ::testing::Test {

public:

    TestUsage ()
    {}

    virtual ~TestUsage ()
    {}

    void SetUp()
    {
        RegexLite::installDriver ();
        db_ = QSqlDatabase::addDatabase (
                    drv_sqlite, QString("Connection_%1").arg (cn_name++));
        db_.setDatabaseName(":memory:");
        if (!db_.open ()) {
            qDebug() << "Cannot open the database";
        }
    }

    void TearDown()
    {
        db_.close ();
    }

    void initializeContent ()
    {
        QSqlQuery query (db_);
        if (!query.exec(
                    "CREATE TABLE Person "
                    "  (id INTEGER PRIMARY KEY ASC,"
                    "   name TEXT,"
                    "   age INTEGER);"
                    )) {
            qDebug() << "addPerson error:  "
                     << query.lastError();
        } else {
            addPerson ("Pers1A Pers1B", 80);
            addPerson ("Pers2A Pers2B", 70);
            addPerson ("Pers3A Pers3B", 60);
            addPerson ("Pers4A Pers4B", 50);
            addPerson ("Pers5A Pers5B", 40);
            addPerson ("Pers6A Pers6B", 30);
            addPerson ("Ent1A Ent1B", 80);
            addPerson ("Ent2A Ent2B", 70);
            addPerson ("Ent3A Ent3B", 60);
            addPerson ("Ent4A Ent4B", 50);
            addPerson ("Ent5A Ent5B", 40);
            addPerson ("Ent6A Ent6B", 30);

        }
    }

    int addPerson (const QString &name, int age)
    {
        bool success = false;
        int id = -1;
        QSqlQuery query (db_);
        query.prepare("INSERT INTO Person"
                      " (name, age) "
                      "VALUES (:name, :age)");
        query.bindValue (":name", name);
        query.bindValue (":age", age);
        if(query.exec ()) {
            id = query.lastInsertId().toInt (&success);
        } else {
             qDebug() << "addPerson error:  "
                      << query.lastError();
        }
        return id;
    }

    QSqlDatabase db_;
};

TEST_F(TestUsage, count_all) {
    initializeContent ();
    int count = -1;

    QSqlQuery q1 (QString (
                     "SELECT COUNT(*) FROM Person;"), db_);
    if (q1.exec () && q1.next ()) {
        count = q1.value (0).toInt ();
    } else {
        qDebug() << "SELECT error:  "
                 << q1.lastError();
    }
    EXPECT_EQ(count, 12);
}


TEST_F(TestUsage, count_persons) {
    initializeContent ();
    int count = -1;

    QSqlQuery q2 (QString (
                     "SELECT COUNT(*) FROM Person WHERE name REGEXP 'P.+';"), db_);
    if (q2.exec () && q2.next ()) {
        count = q2.value (0).toInt ();
    } else {
        qDebug() << "SELECT error:  "
                 << q2.lastError();
    }
    EXPECT_EQ(count, 6);
}


TEST_F(TestUsage, count_1A) {
    initializeContent ();
    int count = -1;

    QSqlQuery q3 (QString (
                     "SELECT COUNT(*) FROM Person WHERE name REGEXP '.+1A.+';"), db_);
    if (q3.exec () && q3.next ()) {
        count = q3.value (0).toInt ();
    } else {
        qDebug() << "SELECT error:  "
                 << q3.lastError();
    }
    EXPECT_EQ(count, 2);
}

