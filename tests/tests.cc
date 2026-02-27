#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "atm.hpp"
#include "catch.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helper Definitions //
/////////////////////////////////////////////////////////////////////////////////////////////

bool CompareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1);
  std::ifstream f2(p2);

  if (f1.fail() || f2.fail()) {
    return false;  // file problem
  }

  std::string f1_read;
  std::string f2_read;
  while (f1.good() || f2.good()) {
    f1 >> f1_read;
    f2 >> f2_read;
    if (f1_read != f2_read || (f1.good() && !f2.good()) ||
        (!f1.good() && f2.good()))
      return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Test Cases
/////////////////////////////////////////////////////////////////////////////////////////////

 TEST_CASE("Example: Create a new account", "[ex-1]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto accounts = atm.GetAccounts();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);

  Account sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.owner_name == "Sam Sepiol");
  REQUIRE(sam_account.balance == 300.30);

  auto transactions = atm.GetTransactions();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);
  std::vector<std::string> empty;
  REQUIRE(transactions[{12345678, 1234}] == empty);
  
  REQUIRE_THROWS_AS(atm.RegisterAccount(12345678, 1234, "dupe", 734.23), std::invalid_argument);

}

TEST_CASE("2", "[2]") {
  Atm atm;
  atm.RegisterAccount(445566, 8888, "Steph Curry", 330.30);
  auto accounts = atm.GetAccounts();
  REQUIRE(accounts.contains({445566, 8888}));
  REQUIRE(accounts.size() == 1);

  Account steph_account = accounts[{445566, 8888}];
  REQUIRE(steph_account.owner_name == "Steph Curry");
  REQUIRE(steph_account.balance == 330.30);

  auto transactions = atm.GetTransactions();
  REQUIRE(accounts.contains({445566, 8888}));
  REQUIRE(accounts.size() == 1);
  std::vector<std::string> empty;
  REQUIRE(transactions[{445566, 8888}] == empty);


  REQUIRE_THROWS_AS(atm.RegisterAccount(445566, 8888, "dupe2", 345.09), std::invalid_argument);
}

TEST_CASE("Example: Simple withdraw", "[ex-2]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  atm.WithdrawCash(12345678, 1234, 20);
  auto accounts = atm.GetAccounts();
  Account sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.balance == 280.30);
  
  auto transactions = atm.GetTransactions();
  std::vector<std::string> v = {"Withdrawal - Amount: $20.00, Updated Balance: $280.30"};
  REQUIRE(transactions[{12345678, 1234}] == v);

  atm.WithdrawCash(12345678, 1234, 10);
  accounts = atm.GetAccounts();
  sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.balance == 270.30);
  transactions = atm.GetTransactions();
  v = {"Withdrawal - Amount: $20, Updated Balance: $280.30", "Withdrawal - Amount: $10.00, Updated Balance: $270.30"};
  REQUIRE(transactions[{12345678, 1234}] == v);

  REQUIRE_THROWS_AS(atm.WithdrawCash(239043, 3932, 40), std::invalid_argument);
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345678, 1234, -10), std::invalid_argument);
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345678, 1234, 300), std::runtime_error);
}

TEST_CASE("Example: Simple deposit", "[ex-3]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  atm.DepositCash(12345678, 1234, 20);
  auto accounts = atm.GetAccounts();
  Account sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.balance == 320.30);

  auto transactions = atm.GetTransactions();
  std::vector<std::string> v = {"Deposit - Amount: $20.00, Updated Balance: $320.00"};
  REQUIRE(transactions[{12345678, 1234}] == v);


  REQUIRE_THROWS_AS(atm.DepositCash(239043, 3932, 40), std::invalid_argument);
  REQUIRE_THROWS_AS(atm.DepositCash(12345678, 1234, -10), std::invalid_argument);
}

TEST_CASE("Example: Print Prompt Ledger", "[ex-4]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto& transactions = atm.GetTransactions();
  transactions[{12345678, 1234}].push_back(
      "Withdrawal - Amount: $200.40, Updated Balance: $99.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $40000.00, Updated Balance: $40099.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $32000.00, Updated Balance: $72099.90");
  atm.PrintLedger("./prompt.txt", 12345678, 1234);
  REQUIRE(CompareFiles("./ex-1.txt", "./prompt.txt"));


  REQUIRE_THROWS_AS(atm.PrintLedger("./prompt.txt", 397432, 9640), std::invalid_argument);
}