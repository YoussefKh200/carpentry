#include "usercrud.h"

#include <QCryptographicHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

#include "../db/connection.h"

namespace {
QString hashPassword(const QString &password)
{
    const QString salt = QUuid::createUuid().toString(QUuid::WithoutBraces).remove('-').left(16);
    const QByteArray digest = QCryptographicHash::hash((password + salt).toUtf8(), QCryptographicHash::Sha256).toHex();
    return QString::fromUtf8(digest) + "$" + salt;
}

bool verifyPassword(const QString &rawPassword, const QString &stored)
{
    if (stored.contains("$")) {
        const QStringList parts = stored.split('$');
        if (parts.size() != 2) return false;
        const QString expectedHash = parts[0];
        const QString salt = parts[1];
        const QByteArray digest = QCryptographicHash::hash((rawPassword + salt).toUtf8(), QCryptographicHash::Sha256).toHex();
        return QString::fromUtf8(digest) == expectedHash;
    }
    return rawPassword == stored;
}
}

UserCrud::UserCrud(QObject *parent) : QObject(parent)
{
}

QString UserCrud::validateUser(const UserData &user)
{
    if (user.nom.trimmed().isEmpty()) return "Le nom est obligatoire";
    if (user.prenom.trimmed().isEmpty()) return "Le prenom est obligatoire";
    if (user.tel.trimmed().size() != 8) return "Telephone: 8 chiffres obligatoires";
    if (user.mail.trimmed().isEmpty() || !user.mail.contains('@') || !user.mail.contains('.')) return "Email invalide";
    if (user.mdp.size() < 8) return "Mot de passe: minimum 8 caracteres";
    if (user.salaire.trimmed().isEmpty()) return "Salaire obligatoire";

    bool salaryOk = false;
    user.salaire.toDouble(&salaryOk);
    if (!salaryOk) return "Salaire invalide";

    if (user.role.trimmed().isEmpty()) return "Role obligatoire";
    return QString();
}

QString UserCrud::normalizeEtat(const QString &etat)
{
    const QString v = etat.trimmed().toLower();
    if (v == "inactif" || v == "i" || v == "0") {
        return "Inactif";
    }
    return "Actif";
}

QString UserCrud::normalizeRole(const QString &role)
{
    const QString r = role.trimmed().toLower();
    if (r == "responsable client") return "Responsable client";
    if (r == "responsable materiaux") return "Responsable materiaux";
    if (r == "responsable achats") return "Responsable achats";
    if (r == "responsable commandes") return "Responsable commandes";
    if (r == "ouvrier") return "Ouvrier";

    // Fallback mappings to satisfy USERS_ROLE_CHK
    if (r == "responsable") return "Responsable client";
    if (r.contains("mater")) return "Responsable materiaux";
    if (r.contains("achat")) return "Responsable achats";
    if (r.contains("command")) return "Responsable commandes";
    return "Ouvrier";
}

int UserCrud::getNextId(QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return -1;
    }
    QSqlQuery q(Connection::instance()->database());
    if (!q.exec("SELECT NVL(MAX(ID_USER),0)+1 FROM USERS") || !q.next()) {
        error = "Impossible de calculer le prochain ID";
        return -1;
    }
    return q.value(0).toInt();
}

UserData UserCrud::readUserFromQuery(QSqlQuery &query)
{
    UserData u;
    u.id = query.value(0).toInt();
    u.nom = query.value(1).toString();
    u.prenom = query.value(2).toString();
    u.tel = query.value(3).toString();
    u.mail = query.value(4).toString();
    u.mdp = query.value(5).toString();
    u.salaire = query.value(6).toString();
    u.role = query.value(7).toString();
    u.etat = query.value(8).toString();
    return u;
}

bool UserCrud::addUser(const UserData &user, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }

    error = validateUser(user);
    if (!error.isEmpty()) return false;

    const int id = getNextId(error);
    if (id <= 0) return false;

    QSqlQuery q(Connection::instance()->database());
    q.prepare(
        "INSERT INTO USERS (ID_USER, NOM, PRENOM, TEL, MAIL, MDP, SALAIRE, ROLE, ETAT) "
        "VALUES (:id, :nom, :prenom, :tel, :mail, :mdp, :salaire, :role, :etat)"
    );
    q.bindValue(":id", id);
    q.bindValue(":nom", user.nom.trimmed());
    q.bindValue(":prenom", user.prenom.trimmed());
    q.bindValue(":tel", user.tel.trimmed());
    q.bindValue(":mail", user.mail.trimmed());
    q.bindValue(":mdp", hashPassword(user.mdp));
    q.bindValue(":salaire", user.salaire.toDouble());
    q.bindValue(":role", normalizeRole(user.role));
    q.bindValue(":etat", normalizeEtat(user.etat));

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool UserCrud::updateUser(const UserData &user, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }

    if (user.id <= 0) {
        error = "ID invalide";
        return false;
    }

    UserData userForValidation = user;
    if (userForValidation.mdp.trimmed().isEmpty()) {
        userForValidation.mdp = "temporaryPass123";
    }

    error = validateUser(userForValidation);
    if (!error.isEmpty()) return false;

    const QString finalPassword = resolvePasswordForUpdate(user, error);
    if (!error.isEmpty()) return false;

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE USERS SET NOM=:nom, PRENOM=:prenom, TEL=:tel, MAIL=:mail, MDP=:mdp, SALAIRE=:salaire, ROLE=:role, ETAT=:etat WHERE ID_USER=:id");
    q.bindValue(":id", user.id);
    q.bindValue(":nom", user.nom.trimmed());
    q.bindValue(":prenom", user.prenom.trimmed());
    q.bindValue(":tel", user.tel.trimmed());
    q.bindValue(":mail", user.mail.trimmed());
    q.bindValue(":mdp", finalPassword);
    q.bindValue(":salaire", user.salaire.toDouble());
    q.bindValue(":role", normalizeRole(user.role));
    q.bindValue(":etat", normalizeEtat(user.etat));

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool UserCrud::deleteUser(int id, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }

    if (id <= 0) {
        error = "ID invalide";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare("DELETE FROM USERS WHERE ID_USER=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

QList<UserData> UserCrud::getAllUsers(const QString &orderBy)
{
    QList<UserData> out;
    if (!Connection::instance()->createConnect()) return out;
    QString sql = "SELECT ID_USER, NOM, PRENOM, TEL, MAIL, MDP, SALAIRE, ROLE, ETAT FROM USERS";
    if (orderBy == "nom") sql += " ORDER BY NOM";
    if (orderBy == "prenom") sql += " ORDER BY PRENOM";

    QSqlQuery q(Connection::instance()->database());
    if (!q.exec(sql)) return out;

    while (q.next()) out.append(readUserFromQuery(q));
    return out;
}

QList<UserData> UserCrud::searchUsers(const QString &searchText, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (searchText.trimmed().isEmpty()) {
        return getAllUsers(orderBy);
    }

    QList<UserData> out;
    QString sql =
        "SELECT ID_USER, NOM, PRENOM, TEL, MAIL, MDP, SALAIRE, ROLE, ETAT FROM USERS "
        "WHERE LOWER(NOM) LIKE :s OR LOWER(PRENOM) LIKE :s OR LOWER(TEL) LIKE :s OR LOWER(MAIL) LIKE :s";
    if (orderBy == "nom") sql += " ORDER BY NOM";
    if (orderBy == "prenom") sql += " ORDER BY PRENOM";

    QSqlQuery q(Connection::instance()->database());
    q.prepare(sql);
    q.bindValue(":s", "%" + searchText.toLower() + "%");

    if (!q.exec()) return out;

    while (q.next()) out.append(readUserFromQuery(q));
    return out;
}

QString UserCrud::resolvePasswordForUpdate(const UserData &user, QString &error)
{
    const QString raw = user.mdp.trimmed();
    if (!raw.isEmpty()) {
        // Already hashed in DB format hash$salt, keep as is.
        if (raw.contains("$")) {
            return raw;
        }
        return hashPassword(raw);
    }

    // Keep existing password if user leaves the field empty.
    QSqlQuery query(Connection::instance()->database());
    query.prepare("SELECT MDP FROM USERS WHERE ID_USER=:id");
    query.bindValue(":id", user.id);
    if (!query.exec() || !query.next()) {
        error = "Impossible de recuperer le mot de passe actuel";
        return QString();
    }
    return query.value(0).toString();
}

bool UserCrud::authenticateUser(const QString &identifier, const QString &password, UserData &userOut, QString &error)
{
    // Temporary hardcoded admin access (requested).
    if (identifier.trimmed().compare("admin", Qt::CaseInsensitive) == 0 && password == "admin") {
        userOut.id = 0;
        userOut.nom = "admin";
        userOut.prenom = "admin";
        userOut.role = "Administrateur";
        userOut.etat = "Actif";
        error.clear();
        return true;
    }

    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }

    const QString idf = identifier.trimmed();
    if (idf.isEmpty() || password.isEmpty()) {
        error = "Nom d'utilisateur et mot de passe obligatoires";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare(
        "SELECT ID_USER, NOM, PRENOM, TEL, MAIL, MDP, SALAIRE, ROLE, ETAT "
        "FROM USERS WHERE LOWER(NOM)=LOWER(:idf)"
    );
    q.bindValue(":idf", idf);
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        error = "Utilisateur introuvable";
        return false;
    }

    const QString storedPassword = q.value(5).toString();
    if (!verifyPassword(password, storedPassword)) {
        error = "Mot de passe incorrect";
        return false;
    }

    const QString etat = q.value(8).toString().trimmed().toLower();
    if (etat == "inactif" || etat == "i" || etat == "0") {
        error = "Compte inactif";
        return false;
    }

    userOut = readUserFromQuery(q);
    error.clear();
    return true;
}

bool UserCrud::authenticateFaceUser(const QString &recognizedName, UserData &userOut, QString &error)
{
    const QString name = recognizedName.trimmed();
    if (name.isEmpty()) {
        error = "Nom reconnu vide";
        return false;
    }

    // Temporary hardcoded admin compatibility.
    if (name.compare("admin", Qt::CaseInsensitive) == 0) {
        userOut.id = 0;
        userOut.nom = "admin";
        userOut.prenom = "admin";
        userOut.role = "Administrateur";
        userOut.etat = "Actif";
        error.clear();
        return true;
    }

    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare(
        "SELECT ID_USER, NOM, PRENOM, TEL, MAIL, MDP, SALAIRE, ROLE, ETAT "
        "FROM USERS WHERE LOWER(NOM)=LOWER(:n)"
    );
    q.bindValue(":n", name);
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        error = "Aucun utilisateur ne correspond au visage reconnu";
        return false;
    }

    const QString etat = q.value(8).toString().trimmed().toLower();
    if (etat == "inactif" || etat == "i" || etat == "0") {
        error = "Compte inactif";
        return false;
    }

    userOut = readUserFromQuery(q);
    error.clear();
    return true;
}

bool UserCrud::userExistsByEmail(const QString &email, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    const QString e = email.trimmed();
    if (e.isEmpty()) {
        error = "Email obligatoire";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare("SELECT COUNT(*) FROM USERS WHERE LOWER(MAIL)=LOWER(:mail)");
    q.bindValue(":mail", e);
    if (!q.exec() || !q.next()) {
        error = q.lastError().text();
        return false;
    }

    if (q.value(0).toInt() <= 0) {
        error = "Aucun compte associe a cet email";
        return false;
    }
    error.clear();
    return true;
}

bool UserCrud::updatePasswordByEmail(const QString &email, const QString &newPassword, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (newPassword.trimmed().size() < 8) {
        error = "Mot de passe: minimum 8 caracteres";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE USERS SET MDP=:mdp WHERE LOWER(MAIL)=LOWER(:mail)");
    q.bindValue(":mdp", hashPassword(newPassword.trimmed()));
    q.bindValue(":mail", email.trimmed());
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    if (q.numRowsAffected() <= 0) {
        error = "Aucun compte mis a jour";
        return false;
    }
    error.clear();
    return true;
}
