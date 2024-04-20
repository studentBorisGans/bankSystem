-- SQLite
CREATE TABLE IF NOT EXISTS BANK_ACCOUNTS (
    account_name string PRIMARY KEY,
    account_id string UNIQUE NOT NULL,
    account_balance float NOT NULL

    

);
INSERT INTO BANK_ACCOUNTS (account_name, account_id, account_balance) VALUES ("boris", "1", 3093.2);
-- INSERT INTO BANK_ACCOUNTS (account_id) VALUES ("1");
-- INSERT INTO BANK_ACCOUNTS (account_balance) VALUES (392.1);
