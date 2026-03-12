#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>

class UserManager : public QObject
{
    Q_OBJECT
public:
    enum class Role {
        Guest,
        Operator,
        Supervisor,
        Engineer,
        Administrator
    };

    explicit UserManager(QObject *parent = nullptr);

    bool login(const QString &username, const QString &password);
    void logout();

    bool isAuthenticated() const;
    QString userName() const;
    Role role() const;

    static QString roleToString(Role role);

signals:
    void authenticationChanged();

private:
    bool m_authenticated;
    QString m_userName;
    Role m_role;
};

#endif // USERMANAGER_H
