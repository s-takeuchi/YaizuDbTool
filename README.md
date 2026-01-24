# CmdFreak 1.3.0

## Summary of CmdFreak
Several information systems and application software suites use a RDBMS (Relational Database Management System) as a persistence mechanism for their own data. The range of RDBMS usage shows a tendency of expansion. Most RDBMSs are used as a background service for information systems and application software. However, such systems can also be accessed directly for data mining, quicker commercial operation and maintenance. An appropriate RDBMS can be chosen to match the characteristics of the target information system and application software. As a result, multiple RDBMSs are often used simultaneously in the same business operation and maintenance environment. However, multiple RDBMS usage is characterized by the following problems:

- Different data reference software applications need to be provided, which may increase the learning cost.
- Data reference software needs to be installed on each client-side PC. The cost of introduction is therefore higher with a greater number of client PCs.

CmdFreak is a relational database viewer (data reference software) that provides solutions for the above problems.

- CmdFreak supports the major free RDBMS MySQL, PostgreSQL and MariaDB systems, providing a common user interface for these RDBMSs and enabling data reference as a common operation. It does not need to learn different reference software applications for multiple RDBMSs, which helps to reduce related learning costs.
- CmdFreak works as web application software, and comes with an all-in-one installer incorporating web server software. This eliminates the need for web server pre-configuration and the use of script language. The introduction cost is practically zero.

CmdFreak provides the following functions for database reference:

- List records/paging
- Table selection
- Table information reference
- Column filtering and sorting
- DBMS selection

![scr5](https://user-images.githubusercontent.com/4883168/80087693-6ee03f00-8596-11ea-9c9c-d9fece120257.png)

## System requirements
CmdFreak works on systems with the following specifications:

- CPU: 2 GHz or more
- Memory:1 GB or more
- Free disk space: 40 MB or more
- Operating systems/editions supported:  
  Windows 11  
  Windows Server 2022  
  Windows Server 2025  
  Red Hat Enterprise Linux 9  
  Red Hat Enterprise Linux 10  
  AlmaLinux 9  
  AlmaLinux 10  
- Web browsers supported:  
  Microsoft Edge  
  Google Chrome  
  Mozilla Firefox  
- DBMSs supported:  
  MySQL 8.4  
  MariaDB 11.4  
  MariaDB 11.8  
  PostgreSQL 17  
  PostgreSQL 18  
  (CmdFreak connects to DBMSs via ODBC; an appropriate ODBC driver for the target DBMS is required.)

## Change history
### Version 1.2.0 ---> 1.3.0

- Added Linux support.
- Optimized for x64 CPU architecture.
- ODBC connection lifecycle stability improved.

### Version 1.1.0 ---> 1.2.0

- Responsive design has been adopted for smartphone user.
- User management function has been added.

### Version 1.0.0 ---> 1.1.0

- Support for displaying unlimited record.
- Performance improvement for multiple user access.

## Download
[1.3.0 Release: 2026/01/24](https://github.com/s-takeuchi/YaizuDbTool/releases/tag/1.3.0)  
[1.2.0 Release: 2020/04/22](https://github.com/s-takeuchi/YaizuDbTool/releases/tag/1.2.0)  
[1.1.0 Release: 2018/01/28](https://github.com/s-takeuchi/YaizuDbTool/releases/tag/1.1.0)
