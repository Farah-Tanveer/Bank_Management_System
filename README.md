# 💳 Bank Management System (C++ | File Handling | Binary Storage)

This is a console-based Bank Management System implemented in C++ using  
structured programming, file handling, dynamic arrays, binary storage, and robust input validation.  
The system supports customer accounts, admin controls, secure login, money transfer, and complete transaction history.

---

## 🚀 Features

### 🔹 Customer Functionalities
- Create a new account (Sign-Up)
- Login with masked password
- View account details
- Deposit & Withdraw money
- Send money to another user
- Check received money
- Full transaction history (sent & received)

### 🔹 Admin Functionalities
- Create new accounts
- View all accounts
- Search account by account number
- Modify username, email, or password
- Close (disable) accounts
- Authentication using admin password (`admin123`)

### 🔹 System Features
- All data saved in binary file (`accounts.dat`)
- Full persistence (data remains even after closing the program)
- Dynamic resizing of accounts array
- Email & phone validation
- Password masking using `_getch()`
- Prevents duplicate emails
- Complete transaction logging

---

## 🛠️ Technologies & Concepts Used
- C++ (Procedural + Struct-Based Design)
- Binary File Handling (`fstream`)
- Dynamic Memory Allocation
- Password Masking
- Input Validation
- Transaction Recording
- Custom Validation Functions
- Array Resize Logic

---

## 📂 Project Structure
Bank Management System<br>
│<br>
├── accounts.dat      # Auto-generated binary database file<br>
├── main.cpp          # Main source code<br>
└── README.md         # Project documentation

---

## 🏦 How It Works

### ➤ Account Data Includes:
- Account Number  
- Username  
- Email  
- Phone  
- Password  
- Balance  
- Closed/Active Status  
- Received Money Counter  
- Transaction History (sent & received)

### ➤ Transactions Tracked As:
- Sender account  
- Receiver account  
- Amount transferred  

---

## ▶️ How to Run

1. Copy the code into a file named **main.cpp**
2. Compile using any C++ compiler:

g++ main.cpp -o bank

3. Run the program:

./bank

---

## 🔑 Default Admin Password
admin123

---

## 📌 Future Improvements (Optional)
- Replace struct-based model with OOP classes  
- Add login attempt lockout  
- Add date/time stamps to transactions  
- Integrate GUI (Qt / SFML / ImGui)  
- Export transaction history to CSV  

---
## 👨‍💻 Authors
- **Farah Tanveer**  
- **Rabia Mubeen**  
- **Emaan Fatima**  

## 📜 License
This project is open-source and free to modify and use for educational purposes.


