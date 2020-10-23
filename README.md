# C Code Socket Programming
Client side is under Windows base and Server side is under Linux RED-Hat
This programming is used for FOXCONN EPC of license generate and value setting

# Tool 
1. Server Side     => Linux RED-HAT
2. Client Side     => Windows base
3. PHP Web         => XAMPP
4. VM tool         => VMware
5. C code compiler => Code::Block
6. SQL             => MySQL in Server side

# C Code Server
1. Code activate command : ./LicenseGenerator_server "-1" "2 4 4 8 8"
  - Value "-1" is giving license using time setting
  - Value "2 4 4 8 8" is giving license function setting
  
2. Code execution command: gcc LicenseGenerator_server.c -o LicenseGenerator_server `mysql_config --cflags --libs`
  
# C Code Client
1.Code activate command : licenseGenerate_Client.exe "Server IP Address" "User Name"
  - Code will compeiler in Code::Block to create .exe file
  - Used Windows terminal to execute "licenseGenerate_Client.exe"
  
# PHP Web Page
1. Used XAMPP and php file to create local Web page 
  - PHP file upload path : C:\xampp\htdocs
  - XAMPP app need start "Apache" "MySQL" and "FileZilla" function
