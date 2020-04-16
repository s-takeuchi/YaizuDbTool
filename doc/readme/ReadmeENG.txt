=================================================
CmdFreak 1.2.0
=================================================
1.CmdFreak summary

Several information systems and application software suites use a RDBMS
(Relational Database Management System) as a persistence mechanism for their
own data. The range of RDBMS usage shows a tendency of expansion.
Most RDBMSs are used as a background service for information systems and
application software. However, such systems can also be accessed directly
for data mining, quicker commercial operation and maintenance.
An appropriate RDBMS can be chosen to match the characteristics of the target
information system and application software. As a result, multiple RDBMSs are
often used simultaneously in the same business operation and maintenance
environment. However, multiple RDBMS usage is characterized by the following
problems:

- Different data reference software applications need to be provided, which
  may increase the learning cost.
- Data reference software needs to be installed on each client-side PC.
  The cost of introduction is therefore higher with a greater number of
  client PCs.

CmdFreak is a relational database viewer (data reference software) that
provides solutions for the above problems.

- CmdFreak supports the major free RDBMS MySQL, PostgreSQL and MariaDB
  systems, providing a common user interface for these RDBMSs and enabling
  data reference as a common operation. It does not need to learn different
  reference software applications for multiple RDBMSs, which helps to reduce
  related learning costs.
- CmdFreak works as web application software, and comes with an all-in-one
  installer incorporating web server software. This eliminates the need for
  web server pre-configuration and the use of script language.
  The introduction cost is practically zero.

CmdFreak provides the following functions for database reference:

- List records/paging
- Table selection
- Table information reference
- Column filtering and sorting
- DBMS selection


=================================================
2.System requirements

StkFw works on systems with the following specifications:

CPU: 2 GHz or more

Memory:1 GB or more

Free disk space: 20 MB or more

Operating systems/editions supported:
 Windows 7
 Windows 10
 Windows Server 2012
 Windows Server 2012 R2
 Windows Server 2016

Web browsers supported:
 Microsoft Internet Explorer 11
 Microsoft Edge
 Google Chrome
 Mozilla Firefox

DBMSs supported:
 MySQL 5.5 (+ ODBC Driver: MySQL Connector/ODBC 5.2)
 MySQL 5.7 (+ ODBC Driver: MySQL Connector/ODBC 5.3)
 MariaDB 5.5 (+ ODBC Driver: MariaDB Connector/ODBC 2.0)
 MariaDB 10.3 (+ ODBC Driver: MariaDB Connector/ODBC 3.0)
 PostgreSQL 9.2 (+ ODBC Driver: psqlODBC 9.2)
 PostgreSQL 9.6 (+ ODBC Driver: psqlODBC 9.6)


=================================================
3.Change history
* Version 1.0.0 ---> 1.1.0
(1) Support for displaying unlimited record.
(2) Performance improvement for multiple user access.

* Version 1.1.0 ---> 1.2.0
(1) Responsive design has been adopted for smartphone user.
(2) User management function has been added.

=================================================
4.Installation

Execute cmdfreak.msi and launch the installer.
Follow the on-screen dialog prompts.
Connect to the URL shown below by Web browser for use of CmdFreak after
installation.

http://<Host name or IP address of CmdFreak>:<Port number of Web server>/cmdfreak.html

In case of installation with default parameters : http://localhost/cmdfreak.html


=================================================
5.Contact

Please use the E-mail address below for inquiries about this product.

sy-takeuchi@outlook.com
https://github.com/s-takeuchi

Shinya Takeuchi

