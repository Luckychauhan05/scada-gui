#include "src/usermanager.h"

UserManager::UserManager(QObject *parent)
    : QObject(parent),
      m_authenticated(false),
      m_userName("Guest"),
      m_role(Role::Guest)
{
}

bool UserManager::login(const QString &username, const QString &password)
{
    struct Account {
        const char *user;
        const char *pass;
        Role role;
    };

    const Account accounts[] = {
        {"operator", "op123", Role::Operator},
        {"supervisor", "sv123", Role::Supervisor},
        {"engineer", "en123", Role::Engineer},
        {"admin", "admin123", Role::Administrator}
    };

    for (const auto &account : accounts) {
        if (username.compare(account.user, Qt::CaseInsensitive) == 0 && password == account.pass) {
            m_authenticated = true;
            m_userName = username;
            m_role = account.role;
            emit authenticationChanged();
            return true;
        }
    }

    return false;
}

void UserManager::logout()
{
    m_authenticated = false;
    m_userName = "Guest";
    m_role = Role::Guest;
    emit authenticationChanged();
}

bool UserManager::isAuthenticated() const
{
    return m_authenticated;
}

QString UserManager::userName() const
{
    return m_userName;
}

UserManager::Role UserManager::role() const
{
    return m_role;
}

QString UserManager::roleToString(Role role)
{
    switch (role) {
    case Role::Guest:
        return "Guest";
    case Role::Operator:
        return "Operator";
    case Role::Supervisor:
        return "Supervisor";
    case Role::Engineer:
        return "Engineer";
    case Role::Administrator:
        return "Administrator";
    }

    return "Unknown";
}
