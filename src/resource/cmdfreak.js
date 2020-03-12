
// User role
var userRole = 0;

// Table name which selected
var currentTablename = "";

// Selected user information on user management page
var selectedUserId = -1;

// User operation result
var userOpeStatus = 0;

function initClientMessage() {
    //
    // User information
    //
    addClientMessage('USERMGMT', {'en':'User Management', 'ja':'ユーザー管理'});
    addClientMessage('NOUSEREXIST', {'en':'<p>No user exists</p>', 'ja':'<p>ユーザーは存在しません</p>'});
    addClientMessage('USERNAME', {'en':'User Name', 'ja':'ユーザー名'});
    addClientMessage('USERROLE', {'en':'User Role', 'ja':'ユーザーロール'});
    addClientMessage('USERROLEADMIN', {'en':'Administrator', 'ja':'管理者'});
    addClientMessage('USERROLEUSER', {'en':'General User', 'ja':'一般ユーザー'});
    addClientMessage('USEROPECOMPLETED', {'en':'The operation has completed.', 'ja':'操作が完了しました。'});
    addClientMessage('USER_PASSWORD_ON', {'en':'Configure password', 'ja':'パスワードを設定する'});
    addClientMessage('USER_PASSWORD', {'en':'Password', 'ja':'パスワード'});

    //
    // ODBC configuration
    //
    addClientMessage('ODBC_CONNECTION', {'en':'ODBC Connections', 'ja':'ODBC接続'});
    addClientMessage('ODBC_CONNSTR_GUIDE', {
        'en':'Configure ODBC connection.<br>Select the connection target DBMS and specify the ODBC connection string.<br>As this is 32-bit application software, this string should be specified for the use of a 32-bit ODBC driver. As a prerequisite, a 32-bit ODBC driver needs to be installed on the operating system.<br>',
        'ja':'ODBC接続の設定を行います。<br>接続対象のDBMSを選択し，ODBCの接続文字列を指定してください。<br>本ソフトウェアは32ビットアプリケーションのため，ODBC接続文字列には，32ビット版ODBCドライバを使用するように指定を行ってください。オペレーティングシステムには，あらかじめ32ビット版ODBCドライバがインストールされている必要があります。<br>'
    });
    addClientMessage('ODBC_SELECTION', {'en':'Select DBMS : ', 'ja':'DBMSの選択 : '});
    addClientMessage('ODBC_CONFAILURE', {
        'en':'DBMS connection failed.<br>This may be caused by one of the following issues:<br>- The DBMS is not working properly.<br>- No ODBC driver is installed.<br>- The ODBC connection string is invalid.<br>- There are issues with the DBMS on the network.<br>',
        'ja':'DBMSとの接続に失敗しました。<br>次の要因が考えられます。<br>- DBMSが適切に起動していない。<br>- ODBCドライバがインストールされていない。<br>- ODBCの接続文字列が不正。<br>- DBMSとの接続経路に何らかの問題がある。<br>'
    });

    //
    // Service information
    //
    addClientMessage('SVCINFO', {'en':'Information', 'ja':'情報'});
    addClientMessage('SVCINFO_OPEN_MANUAL', {'en':'Open online manual', 'ja':'オンラインマニュアルを開く'});
    addClientMessage('SVCINFO_OPEN_MANUAL_URL', {'en':'manual/eng/index.htm', 'ja':'manual/jpn/index.htm'});
    addClientMessage('SVCINFO_TRACELOG', {
        'en':'The trace log is displayed below.<br>Times are as per the time zone of the web server\'s location.',
        'ja':'以下にトレースログが表示されます。<br>表示される時刻はWebサーバーが配置された地域の時刻となります。'
    });
    addClientMessage('SVCINFO_LOGTIME', {'en':'Logging time', 'ja':'ログ出力時刻'});
    addClientMessage('SVCINFO_LOGMSG', {'en':'Message', 'ja':'メッセージ'});

    //
    // Filtering dialog
    //
    addClientMessage('FILTERING', {'en':'Filtering', 'ja':'フィルタリング'});
    addClientMessage('FILTERING_NO_TABLE', {'en':'The operation cannot be performed because the target table for filtering does not exist.', 'ja':'フィルタリングの対象となるテーブルが存在しないため操作を継続できません。'});
    addClientMessage('FILTERING_DESC', {
        'en':'Filter settings can be specified here.<br>Records are filtered according to the specified criteria.<br>Up to five filter items can be specified.<br>All items are treated as logical AND operators.',
        'ja':'ここではフィルタリングの設定を行うことができます。<br>指定した条件でレコード情報がフィルタリングされます。<br>フィルタリング項目は最大5件を指定することができます。<br>各フィルタリング項目間は論理積[AND]で結合されます。'
    });
    addClientMessage('FILTERING_UNSPECIFY', {'en':'Unspecified', 'ja':'未指定'});
    addClientMessage('FILTERING_INVALID', {
        'en':'The filter conditions set do not match the current table schema.<br>Do you want to clear the filter conditions?',
        'ja':'設定されているフィルタリング条件は現在のテーブルのスキーマと一致していません。<br>フィルタリング条件をクリアしてよいですか？'
    });
    addClientMessage('FILTERING_ON', {'en':'Filtering On', 'ja':'フィルタリング有効'});

    //
    // Table information
    //
    addClientMessage('TABLEINFO', {'en':'Table Information', 'ja':'テーブル情報'});
    addClientMessage('TABLEINFO_SHOWN', {'en':'Detailed information on the specified table is shown below.', 'ja':'以下に指定されたテーブルの詳細情報を表示します。'});
    addClientMessage('TABLEINFO_TARGET', {'en':'The target table: ', 'ja':'対象テーブル: '});
    addClientMessage('TABLEINFO_NOTEXIST', {'en':'The operation cannot be performed because the target table for display does not exist.', 'ja':'表示対象のテーブルが存在しないため操作を継続できません。'});
    addClientMessage('TABLEINFO_COLUMNNAME', {'en':'Column name', 'ja':'カラム名'});
    addClientMessage('TABLEINFO_COLUMNTYPE', {'en':'Column type', 'ja':'カラム種別'});
    addClientMessage('TABLEINFO_ISNULL', {'en':'Is nullable', 'ja':'NULL指定可否'});

    //
    // Errors, Common
    //
    addClientMessage('WELCOME_MSG', {'en':'Welcome to the CmdFreak page!', 'ja':'ようこそ，CmdFreakのページです！'});
    addClientMessage('WELCOME_CONFIGODBC', {
        'en':'To refer to data in a database using this software, the ODBC connection needs to be configured first.',
        'ja':'本ソフトウェアを使用してデータベースのデータを参照するには，まずはじめにODBC接続の設定を行ってください。'
    });
    addClientMessage('CONNERR', {
        'en':'Connection with REST API service failed. This may be caused by one of the following issues:<br>(1) REST API service cannot be started.<br>(2) REST API service is not registered as a firewall exception.<br>(3) The definition file [nginx.conf and/or sample.conf] for the host name and port number in the network connectivity settings is invalid.<br>(4) A timeout has occurred when waiting for data from REST API server.<br>',
        'ja':'REST APIサービスとの通信が失敗しました。次の要因が考えられます。<br>(1) REST APIサービスが開始されていない。<br>(2) REST APIサービスがファイアウォールに例外登録されていない。<br>(3) 接続先ホスト名およびポート番号の定義ファイル [nginx.conf , sample.conf] が不正。<br>(4) REST APIサーバからのデータ取得中にタイムアウトが発生した。<br>'
    });
    addClientMessage('DLG_CLOSE', {'en':'Close', 'ja':'閉じる'});
    addClientMessage('DLG_OK', {'en':'OK', 'ja':'OK'});
    addClientMessage('DLG_CANCEL', {'en':'Cancel', 'ja':'キャンセル'});
    addClientMessage('SERVER_ERROR', {'en':'An error occurred at server side.', 'ja':'サーバ側でエラーが発生しました。'});
    addClientMessage('COMADD', {'en':'Add', 'ja':'追加'});
    addClientMessage('COMUPDATE', {'en':'Update', 'ja':'更新'});
    addClientMessage('COMDELETE', {'en':'Delete', 'ja':'削除'});
}

function getSvrMsg(respDat) {
    if (getClientLanguage() == 1) {
        return getClientMessage('SERVER_ERROR') + '<br/>' + respDat.MsgJpn;
    } else {
        return getClientMessage('SERVER_ERROR') + '<br/>' + respDat.MsgEng;
    }
}

function getArray(targetObject) {
    if (targetObject === undefined) {
        return null;
    }
    var targetArray = [];
    if (targetObject instanceof Array) {
        return targetObject;
    } else {
        targetArray.push(targetObject);
        return targetArray;
    }
}

function escapeString(tmpStr) {
    var csReplaced = tmpStr;
    csReplaced = csReplaced.replace(/&/g, "&amp;");
    csReplaced = csReplaced.replace(/</g, "&lt;");
    csReplaced = csReplaced.replace(/>/g, "&gt;");
    csReplaced = csReplaced.replace(/\"/g, "&quot;");
    csReplaced = csReplaced.replace(/\'/g, "&#39;");
    return csReplaced;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayUser() {
    userOpeStatus = 0;
    var contents = [{ method: 'GET', url: '/api/user/?target=all', request: null, keystring: 'API_GET_USERS' }];
    MultiApiCall(contents, displayUser);
}

function displayUser() {
    var userMgmt = $('<div id="usermgmt">');
    showInputModal(getClientMessage('USERMGMT'), userMgmt);

    if (statusCode['API_GET_USERS'] == -1 || statusCode['API_GET_USERS'] == 0) {
        displayAlertDanger('#usermgmt', getClientMessage('CONNERR'));
        $('#usermgmt').append('<p><button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else if (statusCode['API_GET_USERS'] != 200) {
        displayAlertDanger('#usermgmt', getSvrMsg(responseData['API_GET_USERS']));
        $('#usermgmt').append('<p><button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }
    var userList = getArray(responseData['API_GET_USERS'].Data.User);
    if (userList == null) {
        $('#usermgmt').append(getClientMessage('NOUSEREXIST'));
    }

    selectedUserId = -1;

    if (responseData['API_GET_USERS'].Data.User !== undefined) {
        var userListTable = $('<table>');
        userListTable.addClass('table table-striped');

        var tHead = $('<thead class="thead-dark">');
        tHead.append('<tr><th>' + getClientMessage('USERNAME') + '</th><th>' + getClientMessage('USERROLE') + '</th></tr>');
        userListTable.append(tHead);

        var tBody = $('<tbody>');
        for (var Loop = 0; Loop < userList.length; Loop++) {
            var StrUserRole = '';
            if (userList[Loop].Role == 0) {
                StrUserRole = getClientMessage('USERROLEADMIN');
            } else {
                StrUserRole = getClientMessage('USERROLEUSER');
            }
            tBody.append('<tr><td><div class="radio"><label><input type="radio" id="radioUser' + userList[Loop].Id + '" name="optradio" onclick="selectUser(\''+ userList[Loop].Id + '\')"/>' + userList[Loop].Name + '</label></div></td><td>' + StrUserRole + '</td></tr>');
        }
        userListTable.append(tBody);
        $('#usermgmt').append(userListTable);
    }
    $('#usermgmt').append('<div class="form-group"><label for="userName">' + getClientMessage('USERNAME') + '</label><input type="text" class="form-control" id="userName" placeholder="' + getClientMessage('USERNAME') + '"/></div>');
    $('#usermgmt').append('<div class="form-group"><label for="userType">' + getClientMessage('USERROLE') + '</label><select class="form-control" id="userRole"><option>' + getClientMessage('USERROLEADMIN') + '</option><option>' + getClientMessage('USERROLEUSER') + '</option></select></div>');

    var userPwOn = '';
    $('#usermgmt').append($('<div class="form-check"><input class="form-check-input" type="checkbox" id="userPwOn" onClick="clickUserPwOn()" ' + userPwOn + '><label class="form-check-label" for="userPwOn">' + getClientMessage('USER_PASSWORD_ON') + '</label><input type="password" class="form-control" id="userPassword" placeholder="' + getClientMessage('USER_PASSWORD') + '" disabled/></div>'));

    $('#usermgmt').append('<br/>');
    $('#usermgmt').append('<div id="usermgt_msg"/>');
    if (userOpeStatus == 0) {
    } else if (userOpeStatus == 1) {
        displayAlertSuccess('#usermgt_msg', getClientMessage('USEROPECOMPLETED'));
    } else {
        displayAlertDanger('#usermgt_msg', getSvrMsg(responseData['API_OPE_USER']));
    }
    $('#usermgmt').append('<button type="button" id="userBtnAdd" class="btn btn-dark" onclick="updateUser(false)">' + getClientMessage('COMADD') + '</button> ');
    $('#usermgmt').append('<button type="button" id="userBtnUpdate" class="btn btn-dark disabled"">' + getClientMessage('COMUPDATE') + '</button> ');
    $('#usermgmt').append('<button type="button" id="userBtnDelete" class="btn btn-dark disabled"">' + getClientMessage('COMDELETE') + '</button> ');
    $('#usermgmt').append('<button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button>');
    $('#usermgmt').append('<p></p>');
    $('td').css('vertical-align', 'middle');
}

function clickUserPwOn()
{
    if ($('#userPwOn').prop('checked') == true) {
        $('#userPassword').prop('disabled', false);
    } else {
        $('#userPassword').prop('disabled', true);
    }
}

function updateUser(opeFlag) {
    var specifiedUserName = $('#userName').val().replace(/[\n\r]/g, '');
    var specifiedUserRole = $('#userRole').val();
    var specifiedUserPassword = $('#userPassword').val().replace(/[\n\r]/g, '');
    var tmpRole = -1;
    if (specifiedUserRole === getClientMessage('USERROLEADMIN')) {
        tmpRole = 0;
    } else if (specifiedUserRole === getClientMessage('USERROLEUSER')) {
        tmpRole = 1;
    } else {
        closeInputModal();
    }
    if (opeFlag == false) {
        selectedUserId = -1;
    }

    var reqDatDf = {};
    if ($('#userPwOn').prop('checked') == true) {
        reqDatDf = { 'Id': selectedUserId, 'Name': specifiedUserName, 'Role': tmpRole, 'Password': specifiedUserPassword };
    } else {
        reqDatDf = { 'Id': selectedUserId, 'Name': specifiedUserName, 'Role': tmpRole };
    }
    apiCall('POST', '/api/user/', reqDatDf, 'API_OPE_USER', userOpeFinal);
}

function deleteUser() {
    apiCall('DELETE', '/api/user/' + selectedUserId + '/', null, 'API_OPE_USER', userOpeFinal);
}

function userOpeFinal() {
    if (statusCode['API_OPE_USER'] == 200 && statusCode['API_OPE_USER'] == 200) {
        userOpeStatus = 1;
    } else {
        userOpeStatus = 2;
    }
    
    var contents = [{ method: 'GET', url: '/api/user/?target=all', request: null, keystring: 'API_GET_USERS' }];
    MultiApiCall(contents, displayUser);
}

function selectUser(userId) {
    var userList = getArray(responseData['API_GET_USERS'].Data.User);
    for (loop = 0; loop < userList.length; loop++) {
        if (userList[loop].Id == userId) {
            selectedUserId = userList[loop].Id;
            var roleStr = '';
            if (userList[loop].Role == 0) {
                roleStr = getClientMessage('USERROLEADMIN');
            } else {
                roleStr = getClientMessage('USERROLEUSER');
            }
            $('#userName').val(userList[loop].Name);
            $('#userRole').val(roleStr);
            $('#userPwOn').prop('checked', false);
            $('#userPassword').prop('disabled', true);
            $('#userPassword').val('');
            $('#userBtnUpdate').removeClass('disabled');
            $('#userBtnUpdate').click(function() {updateUser(true);});
            $('#userBtnDelete').removeClass('disabled');
            $('#userBtnDelete').click(function() {deleteUser();});
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayOdbcConfig() {
    var contents = [{ method: 'GET', url: '/api/odbcinfo/', request: { 'query': 'default' }, keystring: 'API_GET_ODBCINFO_DEFAULT' },
                    { method: 'GET', url: '/api/odbcinfo/', request: { 'query': 'configured' }, keystring: 'API_GET_ODBCINFO_CONFIGURED' }
    ];
    MultiApiCall(contents, displayOdbcConfig);
}

function displayOdbcConfig() {
    var odbcConfig = $('<div id="odbcconfig">');
    showInputModal(getClientMessage('ODBC_CONNECTION'), odbcConfig);

    if (statusCode['API_GET_ODBCINFO_DEFAULT'] == 200 && statusCode['API_GET_ODBCINFO_CONFIGURED'] == 200) {
        // Nothing to do
    } else if (statusCode['API_GET_ODBCINFO_DEFAULT'] == 400 || statusCode['API_GET_ODBCINFO_CONFIGURED'] == 400) {
        if (statusCode['API_GET_ODBCINFO_DEFAULT'] == 400) {
            displayAlertDanger('#odbcconfig', getSvrMsg(responseData['API_GET_ODBCINFO_DEFAULT']));
        }
        if (statusCode['API_GET_ODBCINFO_CONFIGURED'] == 400) {
            displayAlertDanger('#odbcconfig', getSvrMsg(responseData['API_GET_ODBCINFO_CONFIGURED']));
        }
        odbcConfig.append('<p><button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else if (statusCode['API_GET_ODBCINFO_DEFAULT'] == 401) {
        displayAlertDanger('#odbcconfig', getSvrMsg(responseData['API_GET_ODBCINFO_CONFIGURED']));
        odbcConfig.append('<p><button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else {
        displayAlertDanger('#odbcconfig', getClientMessage('CONNERR'));
        odbcConfig.append('<p><button type="button" id="closeOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }

    var odbcInfos = getArray(responseData['API_GET_ODBCINFO_DEFAULT'].Data.OdbcInfo);
    var odbcInfoConfigured = responseData['API_GET_ODBCINFO_CONFIGURED'].Data.OdbcInfo;

    odbcConfig.append(getClientMessage('ODBC_CONNSTR_GUIDE'));

    var configuredOdbc = '';
    if (odbcInfoConfigured.DbType === 'MariaDB') {
        configuredOdbc = 'MariaDB';
    } else if (odbcInfoConfigured.DbType === 'PostgreSQL' || odbcInfoConfigured.DbType === 'Init') {
        configuredOdbc = 'PostgreSQL';
    } else if (odbcInfoConfigured.DbType === 'MySQL') {
        configuredOdbc = 'MySQL';
    }
    var btnGrp = $('<div class="btn-group">');
    btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="dbmsselector">' + configuredOdbc + '</span><span class="caret"></span></button>');
    var ddMenu = $('<ul class="dropdown-menu" role="menu">');
    ddMenu.append('<li role="presentation"><a onclick="eventDbmsSelected(0)" role="menuitem" tabindex="-1" href="#">MariaDB</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="eventDbmsSelected(1)" role="menuitem" tabindex="-1" href="#">PostgreSQL</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="eventDbmsSelected(2)" role="menuitem" tabindex="-1" href="#">MySQL</a></li>');
    btnGrp.append(ddMenu);
    odbcConfig.append('<p>');
    odbcConfig.append(getClientMessage('ODBC_SELECTION'));
    odbcConfig.append(btnGrp);
    odbcConfig.append('</p>');

    odbcConfig.append('<textarea class="form-control" id="connstr" rows="3" style="margin-top: 0px; margin-bottom: 0px; height: 100px;"></textarea>');
    if (odbcInfoConfigured.DbType === 'Init') {
        $('#connstr').val(odbcInfos[1].ConnStr);
    } else {
        var csReplaced = odbcInfoConfigured.ConnStr;
        csReplaced = csReplaced.replace(/&/g, "&amp;");
        csReplaced = csReplaced.replace(/</g, "&lt;");
        csReplaced = csReplaced.replace(/>/g, "&gt;");
        csReplaced = csReplaced.replace(/\"/g, "&quot;");
        $('#connstr').append(csReplaced);
    }

    odbcConfig.append('<p>');
    odbcConfig.append('<button type="button" id="okOdbcConfig" class="btn btn-dark" onclick="closeDbmsConfig()">' + getClientMessage('DLG_OK') + '</button> ');
    odbcConfig.append('<button type="button" id="cancelOdbcConfig" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CANCEL') + '</button> ');
    odbcConfig.append('</p>');
}

function eventDbmsSelected(dbmsindex) {
    var odbcInfos = getArray(responseData['API_GET_ODBCINFO_DEFAULT'].Data.OdbcInfo);
    $('#dbmsselector').text(odbcInfos[dbmsindex].DbType);
    $('#connstr').val(odbcInfos[dbmsindex].ConnStr);
}

function closeDbmsConfig() {
    var selectedDbmsName = $('#dbmsselector').text();
    var tmpstr = $('#connstr').val().replace(/[\n\r]/g, '');
    var reqDatDf = { 'DbType': selectedDbmsName, 'ConnStr': tmpstr };
    apiCall('POST', '/api/odbcinfo/', reqDatDf, 'API_POST_ODBCINFO', refreshInfo);
    closeInputModal();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayInformation() {
    var contents = [{ method: 'GET', url: '/api/logs/', request: null, keystring: 'API_GET_LOGS' },
                    { method: 'GET', url: '/api/system/', request: null, keystring: 'API_GET_SYSTEM' }
    ];
    MultiApiCall(contents, displayInformation);
}

function displayInformation() {
    var svcInfo = $('<div id="svcinfo">');
    showInputModal(getClientMessage('SVCINFO'), svcInfo);
    var buildTime = "";
    var startTime = "";
    var productName = "";
    var version = "";
    if (statusCode['API_GET_SYSTEM'] == 200) {
        buildTime = responseData['API_GET_SYSTEM'].Data.BuildTime;
        startTime = responseData['API_GET_SYSTEM'].Data.StartTime;
        productName = responseData['API_GET_SYSTEM'].Data.ProductName;
        version = responseData['API_GET_SYSTEM'].Data.Version;
    } else if (statusCode['API_GET_SYSTEM'] == 401) {
        displayAlertDanger('#svcinfo', getSvrMsg(responseData['API_GET_SYSTEM']));
        svcInfo.append('<p><button type="button" id="closeSvcInfo" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else {
        displayAlertDanger('#svcinfo', getClientMessage('CONNERR'));
        svcInfo.append('<p><button type="button" id="closeSvcInfo" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }
    var Logs = [];
    if (statusCode['API_GET_LOGS'] == 200) {
        if (responseData['API_GET_LOGS'].Data.Log instanceof Array) {
            Logs = responseData['API_GET_LOGS'].Data.Log;
        } else if (responseData['API_GET_LOGS'].Data.Log) {
            Logs.push(responseData['API_GET_LOGS'].Data.Log);
        }
    } else {
        displayAlertDanger('#svcinfo', getClientMessage('CONNERR'));
        svcInfo.append('<p><button type="button" id="closeSvcInfo" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }

    svcInfo.append('<div style="float:left;padding-right:15px;"><img src="img/cristal_image48c.png"/></div>');
    svcInfo.append('<div style="float:left;padding-top:20px;height:55px;"><strong>' + productName + ' ' + version + '</strong></div>');
    svcInfo.append('<div style="clear:both;">');
    svcInfo.append('Copyright (C) 2020 Shinya Takeuchi<br/>All Rights Reserved.');
    svcInfo.append('<p><button type="button" id="openManual" class="btn btn-dark" onclick="openManual()">' + getClientMessage('SVCINFO_OPEN_MANUAL') + '</button></p>');
    svcInfo.append('<p>');
    svcInfo.append('Build Time = ' + buildTime + '<br/>');
    svcInfo.append('Service Start Time = ' + startTime + '<br/>');
    svcInfo.append('</p>');
    svcInfo.append('<p>' + getClientMessage('SVCINFO_TRACELOG') + '</p>');

    var logData = $('<table>');
    logData.addClass('table table-striped');

    var tHead = $('<thead class="thead-dark">');
    tHead.append('<tr><th>' + getClientMessage('SVCINFO_LOGTIME') + '</th><th>' + getClientMessage('SVCINFO_LOGMSG') + '</th></tr>');
    logData.append(tHead);

    var tBody = $('<tbody>');
    for (var Loop = 0; Loop < Logs.length; Loop++) {
        if (getClientLanguage() == 1) {
            tBody.append('<tr><td>' + Logs[Loop].Time + '</td><td>' + Logs[Loop].MsgJa + '</td></tr>');
        } else {
            tBody.append('<tr><td>' + Logs[Loop].Time + '</td><td>' + Logs[Loop].MsgEn + '</td></tr>');
        }
    }
    logData.append(tBody);

    svcInfo.append(logData);
    svcInfo.append('<p><button type="button" id="closeSvcInfo" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
}

function openManual() {
    window.open(getClientMessage('SVCINFO_OPEN_MANUAL_URL'));
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function checkOdbcConnection() {
    $('.container-fluid').remove();
    var containerFluidWorkSpace = $('<div class="container-fluid" style="margin:0px;padding:0px">');
    containerFluidWorkSpace.append('<div id="cmdfreakdata"></div>');
    $('body').append(containerFluidWorkSpace);

    if (statusCode['API_GET_ODBCINFO_CONFIGURED'] == 200 && statusCode['API_GET_TABLEINFO'] == 200) {
        // Nothing to do
    } else {
        displayAlertDanger('#cmdfreakdata', getClientMessage('CONNERR'));
        return;
    }
    if (responseData['API_GET_ODBCINFO_CONFIGURED'].Data.OdbcInfo.DbType === 'Init') {
        displayAlertInfo('#cmdfreakdata', getClientMessage('WELCOME_MSG') + '<p>' + getClientMessage('WELCOME_CONFIGODBC') + '</p>');
        return;
    }
    if (responseData['API_GET_ODBCINFO_CONFIGURED'].Data.OdbcInfo.Status === 'unconnectable') {
        displayAlertDanger('#cmdfreakdata', getClientMessage('ODBC_CONFAILURE'));
        return;
    }
    displayTableList();
}

function displayTableList() {
    if (responseData['API_GET_TABLEINFO'].Data.TableInfo === undefined) {
        return;
    }

    var tableInfos = getArray(responseData['API_GET_TABLEINFO'].Data.TableInfo);
    var foundIndex = -1;
    for (var tableIndex = 0; tableIndex < tableInfos.length; tableIndex++) {
        var targetName = escapeString(tableInfos[tableIndex].Name);
        addDropDownMenu(tableIndex, targetName, 'selectTable(' + tableIndex + ')');
        if (tableInfos[tableIndex].Name === currentTablename) {
            foundIndex = tableIndex;
        }
        if (tableIndex == 0 && currentTablename == "") {
            currentTablename = tableInfos[tableIndex].Name;
            foundIndex = tableIndex;
        }
    }
    if (foundIndex != -1) {
        selectTable(foundIndex);
    } else {
        currentTablename = "";
    }
}

function selectTable(index) {
    var tableInfos = getArray(responseData['API_GET_TABLEINFO'].Data.TableInfo);
    currentTablename = tableInfos[index].Name;
    changeDropDownTitle(getDropDownMenu(index));
    var reqDatDf = { 'query': tableInfos[index].Name };
    var contents = [{ method: 'GET', url: '/api/tableinfo/', request: reqDatDf, keystring: 'API_GET_TABLEINFO_WITH_COL' },
                    { method: 'GET', url: '/api/records/', request: reqDatDf, keystring: 'API_GET_RECORDS' }
    ];
    MultiApiCall(contents, displayData);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function displayData() {
    $('.container-fluid').remove();
    var containerFluidWorkSpace = $('<div class="container-fluid" style="margin:0px;padding:0px">');
    containerFluidWorkSpace.append('<div id="cmdfreakdata"></div>');
    $('body').append(containerFluidWorkSpace);

    if (statusCode['API_GET_TABLEINFO_WITH_COL'] == -1 || statusCode['API_GET_TABLEINFO_WITH_COL'] == 0) {
        displayAlertDanger('#cmdfreakdata', getClientMessage('CONNERR'));
        return;
    } else if (statusCode['API_GET_TABLEINFO_WITH_COL'] != 200) {
        displayAlertDanger('#cmdfreakdata', getSvrMsg(responseData['API_GET_TABLEINFO_WITH_COL']));
        return;
    }
    if (statusCode['API_GET_RECORDS'] == -1 || statusCode['API_GET_RECORDS'] == 0) {
        displayAlertDanger('#cmdfreakdata', getClientMessage('CONNERR'));
        return;
    } else if (statusCode['API_GET_RECORDS'] != 200) {
        displayAlertDanger('#cmdfreakdata', getSvrMsg(responseData['API_GET_RECORDS']));
        return;
    }

    if (responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo === undefined ||
        responseData['API_GET_RECORDS'].Data.Record === undefined) {
        $('#cmdfreakdata').append('No data');
        return;
    }

    var colCount = responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo.length;
    var colInfo = getArray(responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo);
    var recData = getArray(responseData['API_GET_RECORDS'].Data.Record);

    $('#cmdfreakdata').append('<div id="dispTableName" style="overflow:hidden;text-overflow:ellipsis;text-nowrap;height:25px;background-color:#3030c0"><a onclick="transDisplayTableInfo()" href="#">' + currentTablename + '</a></div>');

    var cmdfreakDiv = $('<div class="table-responsive text-nowrap">')
    var cmdfreakData = $('<table class="table table-striped table-bordered table-sm">');

    var tHead = $('<thead class="thead-dark">');
    var headStr = '<tr>';
    for (var loop = 0; loop < colCount; loop++) {
        var targetTitle = escapeString(colInfo[loop].title);
        headStr += '<th>' + targetTitle + '</th>';
    }
    headStr += '</tr>';
    tHead.append(headStr);
    cmdfreakData.append(tHead);

    var tBody = $('<tbody>');
    for (var loopRec = 0; loopRec < recData.length; loopRec++) {
        var bodyStr = '<tr>';
        for (var loopCol = 0; loopCol < colCount; loopCol++) {
            var targetData = escapeString(recData[loopRec][loopCol]);
            bodyStr += '<td>' + targetData + '</td>';
        }
        bodyStr += '</tr>';
        tBody.append(bodyStr);
    }
    cmdfreakData.append(tBody);
    cmdfreakDiv.append(cmdfreakData);

    $('#cmdfreakdata').append(cmdfreakDiv);

    resizeComponent();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayFilterModal() {
    var contents = [{ method: 'GET', url: '/api/tableinfo/', request: { 'query': currentTablename }, keystring: 'API_GET_TABLEINFO_WITH_COL' },
                    { method: 'GET', url: '/api/filterinfo/', request: null, keystring: 'API_GET_FILTERINFO' }
    ];
    MultiApiCall(contents, displayFilterModal);
}

function displayFilterModal() {
    var filteringDlg = $('<div id="filter">');
    showInputModal(getClientMessage('FILTERING'), filteringDlg);

    if (currentTablename == "") {
        displayAlertDanger('#filter', getClientMessage('FILTERING_NO_TABLE'));
        filteringDlg.append('<p><button type="button" id="closeFilterDlg" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }
    if (statusCode['API_GET_FILTERINFO'] == 200 && statusCode['API_GET_TABLEINFO_WITH_COL'] == 200) {
        // Nothing to do
    } else if (statusCode['API_GET_TABLEINFO_WITH_COL'] == 404) {
        displayAlertDanger('#filter', getSvrMsg(responseData['API_GET_TABLEINFO_WITH_COL']));
        filteringDlg.append('<p><button type="button" id="closeFilterDlg" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else if (statusCode['API_GET_FILTERINFO'] == 401) {
        displayAlertDanger('#filter', getSvrMsg(responseData['API_GET_FILTERINFO']));
        filteringDlg.append('<p><button type="button" id="closeFilterDlg" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else {
        displayAlertDanger('#filter', getClientMessage('CONNERR'));
        filteringDlg.append('<p><button type="button" id="closeFilterDlg" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }

    var columnInfos = getArray(responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo);

    var filterOn = '';
    if (responseData['API_GET_FILTERINFO'].Data.FilterInfo.Function === 'enable') {
        filterOn = 'checked';
    }
    filteringDlg.append($('<div class="form-check"><input class="form-check-input" type="checkbox" id="filterOn" ' + filterOn + '><label class="form-check-label" for="filterOn">' + getClientMessage('FILTERING_ON') + '</label></div>'));

    filteringDlg.append('<p>');
    filteringDlg.append(getClientMessage('FILTERING_DESC'));
    filteringDlg.append('</p>');

    for (var loopFil = 0; loopFil < 5; loopFil++) {
        var btnCol = $('<div class="filter-col-responsive" style="margin-right:5px;margin-bottom:5px"><button id="paramCol' + loopFil + '" type="button" class="filter-wid-responsive btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="paramColTxt' + loopFil + '">' + getClientMessage('FILTERING_UNSPECIFY') + '</span><span class="caret"></span></button></div>');
        var colMenu = $('<ul id="paramColMenu' + loopFil + '" class="dropdown-menu" role="menu">');
        btnCol.append(colMenu);

        var btnOpe = $('<div class="filter-ope-responsive" style="margin-right:5px;margin-bottom:5px"><button id="paramOpe' + loopFil + '" type="button" class="filter-wid-responsive btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="paramOpeTxt' + loopFil + '"></span><span class="caret"></span></button></div>');
        var opeMenu = $('<ul id="paramOpeMenu' + loopFil + '" class="dropdown-menu" role="menu">');
        btnOpe.append(opeMenu);

        var frmVal = $('<div class="filter-val-responsive filter-wid-responsive"" style="margin-right:5px;margin-bottom:5px"><input type="text" class="form-control" id="paramVal' + loopFil + '"></div>');

        filteringDlg.append('<p>');
        filteringDlg.append(btnCol);
        filteringDlg.append(btnOpe);
        filteringDlg.append(frmVal);
        filteringDlg.append($('<div style="clear:both"/>'));
        filteringDlg.append('</p>');
    }
    resizeComponent();

    var filterInfo = responseData['API_GET_FILTERINFO'].Data.FilterInfo.Criteria;

    for (var loopFil = 0; loopFil < 5; loopFil++) {
        $('#paramOpe' + loopFil).addClass('disabled');
        $('#paramVal' + loopFil).prop('disabled', true);
        var filFndFlag = false;
        $('#paramColMenu' + loopFil).append('<li role="presentation"><a onclick="eventParamColChanged(' + loopFil + ', -1)" role="menuitem" tabindex="-1" href="#">' + getClientMessage('FILTERING_UNSPECIFY') + '</a></li>');
        for (var loop = 0; columnInfos != null && loop < columnInfos.length; loop++) {
            $('#paramColMenu' + loopFil).append('<li role="presentation"><a onclick="eventParamColChanged(' + loopFil + ', ' + loop + ')" role="menuitem" tabindex="-1" href="#">' + escapeString(columnInfos[loop].title) + '</a></li>');
            if (columnInfos[loop].title == filterInfo[loopFil].columnname) {
                $('#paramColTxt' + loopFil).text(columnInfos[loop].title);
                filFndFlag = true;
                eventParamColChanged(loopFil, loop);
            }
        }
        if (!filFndFlag && filterInfo[loopFil].columnname != '*') {
            filteringDlg.empty();
            filteringDlg.append(getClientMessage('FILTERING_INVALID'));
            filteringDlg.append('<p>');
            filteringDlg.append('<button type="button" id="clearFiltering" class="btn btn-dark" onclick="clearFilterModal()">' + getClientMessage('DLG_OK') + '</button> ');
            filteringDlg.append('<button type="button" id="cancelFiltering" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CANCEL') + '</button> ');
            filteringDlg.append('</p>');
            return;
        }
        if (filFndFlag) {
            $('#paramOpe' + loopFil).removeClass('disabled');
            $('#paramVal' + loopFil).prop('disabled', false);
        }
        switch (filterInfo[loopFil].opetype) {
            case 1: $('#paramOpeTxt' + loopFil).text('='); break;
            case 2: $('#paramOpeTxt' + loopFil).text('!='); break;
            case 3: $('#paramOpeTxt' + loopFil).text('&lt;='); break;
            case 4: $('#paramOpeTxt' + loopFil).text('&lt;'); break;
            case 5: $('#paramOpeTxt' + loopFil).text('&gt;='); break;
            case 6: $('#paramOpeTxt' + loopFil).text('&gt;'); break;
            case 10: $('#paramOpeTxt' + loopFil).text('contains'); break;
            case 11: $('#paramOpeTxt' + loopFil).text('does not contain'); break;
            case 20: $('#paramOpeTxt' + loopFil).text('is NULL'); $('#paramVal' + loopFil).prop('disabled', true); break;
            case 21: $('#paramOpeTxt' + loopFil).text('is not NULL'); $('#paramVal' + loopFil).prop('disabled', true); break;
        }
        $('#paramVal' + loopFil).val(filterInfo[loopFil].value);
    }

    filteringDlg.append('<p>');
    filteringDlg.append('<button type="button" id="okFiltering" class="btn btn-dark" onclick="okFilterModal()">' + getClientMessage('DLG_OK') + '</button> ');
    filteringDlg.append('<button type="button" id="cancelFiltering" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CANCEL') + '</button> ');
    filteringDlg.append('</p>');
}

function eventParamColChanged(indexFil, indexCol) {
    var columnInfos = getArray(responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo);
    if (indexCol == -1) {
        $('#paramColTxt' + indexFil).text(getClientMessage('FILTERING_UNSPECIFY'));
        $('#paramOpe' + indexFil).addClass('disabled');
        $('#paramVal' + indexFil).prop('disabled', true);
        return;
    }
    $('#paramColTxt' + indexFil).text(columnInfos[indexCol].title);
    $('#paramOpe' + indexFil).removeClass('disabled');
    $('#paramVal' + indexFil).prop('disabled', false);

    $('#paramOpeMenu' + indexFil).empty();
    $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'=\')" role="menuitem" tabindex="-1" href="#">=</a></li>');
    $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'!=\')" role="menuitem" tabindex="-1" href="#">!=</a></li>');
    if (columnInfos[indexCol].dataType === 'integer' ||
        columnInfos[indexCol].dataType === 'float') {
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'&lt;=\')" role="menuitem" tabindex="-1" href="#">&lt;=</a></li>');
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'&lt;\')" role="menuitem" tabindex="-1" href="#">&lt;</a></li>');
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'&gt;=\')" role="menuitem" tabindex="-1" href="#">&gt;=</a></li>');
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'&gt;\')" role="menuitem" tabindex="-1" href="#">&gt;</a></li>');
    }
    if (columnInfos[indexCol].dataType === 'string') {
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'contains\')" role="menuitem" tabindex="-1" href="#">contains</a></li>');
        $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'does not contain\')" role="menuitem" tabindex="-1" href="#">does not contain</a></li>');
    }
    $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'is NULL\')" role="menuitem" tabindex="-1" href="#">is NULL</a></li>');
    $('#paramOpeMenu' + indexFil).append('<li role="presentation"><a onclick="eventParamOpeChanged(' + indexFil + ', \'is not NULL\')" role="menuitem" tabindex="-1" href="#">is not NULL</a></li>');
    $('#paramOpeTxt' + indexFil).text('=');
}

function eventParamOpeChanged(indexFil, ope) {
    if (ope == "is NULL" || ope == "is not NULL") {
        $('#paramVal' + indexFil).prop('disabled', true);
    } else {
        $('#paramVal' + indexFil).prop('disabled', false);
    }
    $('#paramOpeTxt' + indexFil).text(ope);
}

function okFilterModal() {
    var criteria = [{ index: 0, columnname: "", opetype: 0, value: "" },
                    { index: 0, columnname: "", opetype: 0, value: "" },
                    { index: 0, columnname: "", opetype: 0, value: "" },
                    { index: 0, columnname: "", opetype: 0, value: "" },
                    { index: 0, columnname: "", opetype: 0, value: "" }
    ];
    for (var loop = 0; loop < 5; loop++) {
        var paramCol = $('#paramColTxt' + loop).text();
        var paramOpe = $('#paramOpeTxt' + loop).text();
        var paramVal = $('#paramVal' + loop).val();
        var paramOpeInt = 0;

        if (typeof paramCol === 'undefined' || typeof paramOpe === 'undefined' || typeof paramVal === 'undefined') {
            paramCol = '*';
            paramVal = '';
        } else if (paramCol === getClientMessage('FILTERING_UNSPECIFY') || paramOpe === '') {
            paramCol = '*';
            paramVal = '';
        } else {
            switch (paramOpe) {
                case '=': paramOpeInt = 1; break;
                case '!=': paramOpeInt = 2; break;
                case '>=': paramOpeInt = 3; break;
                case '>': paramOpeInt = 4; break;
                case '<=': paramOpeInt = 5; break;
                case '<': paramOpeInt = 6; break;
                case 'contains': paramOpeInt = 10; break;
                case 'does not contain': paramOpeInt = 11; break;
                case 'is NULL': paramOpeInt = 20; break;
                case 'is not NULL': paramOpeInt = 21; break;
            }
        }
        criteria[loop].index = loop + 1;
        criteria[loop].columnname = paramCol;
        criteria[loop].opetype = paramOpeInt;
        criteria[loop].value = paramVal;
    }

    var reqDatDf = { "Criteria": criteria };
    var reqDatSw = {};
    if ($('#filterOn').prop('checked') == true) {
        reqDatSw = { "Function": "enable" };
    } else {
        reqDatSw = { "Function": "disable" };
    }
    var reqDatRc = { 'query': currentTablename };
    var contents = [{ method: 'POST', url: '/api/filterinfo/', request: reqDatDf, keystring: 'API_POST_FILTERINFO' },
                    { method: 'POST', url: '/api/filterinfo/', request: reqDatSw, keystring: 'API_POST_FILTERINFO' },
                    { method: 'GET', url: '/api/tableinfo/', request: reqDatRc, keystring: 'API_GET_TABLEINFO_WITH_COL' },
                    { method: 'GET', url: '/api/records/', request: reqDatRc, keystring: 'API_GET_RECORDS' }
    ];
    MultiApiCall(contents, completeFilterModal);
    closeInputModal();
}

function clearFilterModal() {
    var criteria = [{ index: 1, columnname: "*", opetype: 0, value: "" },
                    { index: 2, columnname: "*", opetype: 0, value: "" },
                    { index: 3, columnname: "*", opetype: 0, value: "" },
                    { index: 4, columnname: "*", opetype: 0, value: "" },
                    { index: 5, columnname: "*", opetype: 0, value: "" }
    ];
    var reqDatDf = { "Criteria": criteria };
    reqDatSw = { "Function": "disable" };
    var reqDatRc = { 'query': currentTablename };
    var contents = [{ method: 'POST', url: '/api/filterinfo/', request: reqDatDf, keystring: 'API_POST_FILTERINFO' },
                    { method: 'POST', url: '/api/filterinfo/', request: reqDatSw, keystring: 'API_POST_FILTERINFO' },
                    { method: 'GET', url: '/api/tableinfo/', request: reqDatRc, keystring: 'API_GET_TABLEINFO_WITH_COL' },
                    { method: 'GET', url: '/api/records/', request: reqDatRc, keystring: 'API_GET_RECORDS' }
    ];
    MultiApiCall(contents, completeFilterModal);
    closeInputModal();
}

function completeFilterModal() {
    displayData();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayTableInfo() {
    var contents = [{ method: 'GET', url: '/api/tableinfo/', request: { 'query': currentTablename }, keystring: 'API_GET_TABLEINFO_WITH_COL' }];
    MultiApiCall(contents, displayTableInfo);
}

function displayTableInfo() {
    var tableInfo = $('<div id="tableinfo" style="overflow:hidden;text-overflow:ellipsis;">');
    showInputModal(getClientMessage('TABLEINFO'), tableInfo);

    if (currentTablename == "") {
        displayAlertDanger('#tableinfo', getClientMessage('TABLEINFO_NOTEXIST'));
        tableInfo.append('<p><button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }
    if (statusCode['API_GET_TABLEINFO_WITH_COL'] == 200) {
        // Nothing to do
    } else if (statusCode['API_GET_TABLEINFO_WITH_COL'] == 404 || statusCode['API_GET_TABLEINFO_WITH_COL'] == 401) {
        displayAlertDanger('#tableinfo', getSvrMsg(responseData['API_GET_TABLEINFO_WITH_COL']));
        tableInfo.append('<p><button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    } else {
        displayAlertDanger('#tableinfo', getClientMessage('CONNERR'));
        tableInfo.append('<p><button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }

    var tblInfo = responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo;
    var columnInfos = [];
    if (tblInfo.Name === currentTablename) {
        columnInfos = getArray(responseData['API_GET_TABLEINFO_WITH_COL'].Data.TableInfo.ColumnInfo);
    } else {
        displayAlertDanger('#tableinfo', getClientMessage('TABLEINFO_NOTEXIST'));
        tableInfo.append('<p><button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
        return;
    }

    tableInfo.append('<p>' + getClientMessage('TABLEINFO_SHOWN') + '</p>');
    tableInfo.append(getClientMessage('TABLEINFO_TARGET') + escapeString(currentTablename));

    var tblDataDiv = $('<div style="overflow:auto;">')
    var tblData = $('<table>');
    tblData.addClass('table table-striped');
    var tHead = $('<thead class="thead-dark">');
    tHead.append('<tr><th>' + getClientMessage('TABLEINFO_COLUMNNAME') + '</th><th>' + getClientMessage('TABLEINFO_COLUMNTYPE') + '</th><th>' + getClientMessage('TABLEINFO_ISNULL') + '</th></tr>');
    tblData.append(tHead);
    var tBody = $('<tbody>');
    for (var colLoop = 0; colLoop < columnInfos.length; colLoop++) {
        tBody.append('<tr><td>' + escapeString(columnInfos[colLoop].title) + '</td><td>' + columnInfos[colLoop].coltype + '</td><td>' + columnInfos[colLoop].isnull + '</td></tr>');
    }
    tblData.append(tBody);
    tblDataDiv.append(tblData);
    tableInfo.append(tblDataDiv);

    tableInfo.append('<p>');
    tableInfo.append('<p><button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('DLG_CLOSE') + '</button></p>');
    tableInfo.append('</p>');
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function initCmdFreak() {
    if (statusCode['API_GET_LANG'] == -1 || statusCode['API_GET_LANG'] == 0) {
        displayAlertDanger('body', getClientMessage('CONNERR'));
        return;
    }
    if (responseData['API_GET_LANG'].Data.ClientLanguage == 'ja') {
        setClientLanguage(1);
    } else {
        setClientLanguage(0);
    }
    showLoginModal(checkLogin);
}

function refreshInfo() {
    currentTablename = "";
    clearRsCommand();
    addDropDown('Tables');
    addRsCommand('transDisplayFilterModal()', 'icon-filter', true);
    var contents = [{ method: 'GET', url: '/api/tableinfo/', request: null, keystring: 'API_GET_TABLEINFO' },
                    { method: 'GET', url: '/api/odbcinfo/', request: { 'query': 'configured' }, keystring: 'API_GET_ODBCINFO_CONFIGURED' }
    ];
    MultiApiCall(contents, checkOdbcConnection);
}

function checkLogin() {
    sendRequestRecvResponse('GET', '/api/user/', null, 'API_GET_USER', false);
    if (statusCode['API_GET_USER'] != 200) {
        return false;
    } else {
        var menuContents = [
            { id: 'cmdfreakconfig', actApiName: 'activateTopic', title: getClientMessage('ODBC_CONNECTION') },
            { id: 'cmdfreakusermgmt', actApiName: 'activateTopic', title: getClientMessage('USERMGMT') },
            { id: 'cmdfreakinfo', actApiName: 'activateTopic', title: getClientMessage('SVCINFO') }
        ];
        initMainPage('CmdFreak', 'img/cristal_image48c.png', menuContents);
        var userRole = responseData['API_GET_USER'].Data.User.Role;
        if (userRole == 0) {
            $('#menu-cmdfreakinfo').show();
            $('#menu-cmdfreakusermgmt').show();
            $('#menu-cmdfreakconfig').show();
        } else {
            $('#menu-cmdfreakinfo').show();
        }
        refreshInfo();
        return true;
    }
}

function activateTopic(id) {
    if (id === 'cmdfreakinfo') {
        transDisplayInformation();
    }
    if (id === 'cmdfreakconfig') {
        transDisplayOdbcConfig();
    }
    if (id === 'cmdfreakusermgmt') {
        transDisplayUser();
    }
}

function resizeComponent() {
    var wsize = $(window).width();

    var horizontal = false;
    if (wsize >= 1200) {
        horizontal = true;
        wsize = 300;
    } else if (wsize >= 992) {
        horizontal = true;
        wsize = 240;
    } else if (wsize >= 576) {
        horizontal = false;
        wsize = 400;
    } else if (wsize >= 400) {
        horizontal = false;
        wsize = 300;
    } else {
        horizontal = false;
        wsize = 240;
    }
    if (horizontal == true) {
        $(".filter-col-responsive").css("margin-left", "0px");
        $(".filter-ope-responsive").css("margin-left", "0px");
        $(".filter-val-responsive").css("margin-left", "0px");
        $(".filter-col-responsive").css("float", "left");
        $(".filter-ope-responsive").css("float", "left");
        $(".filter-val-responsive").css("float", "left");
    } else {
        $(".filter-col-responsive").css("margin-left", "0px");
        $(".filter-ope-responsive").css("margin-left", "15px");
        $(".filter-val-responsive").css("margin-left", "30px");
        $(".filter-col-responsive").css("float", "top");
        $(".filter-ope-responsive").css("float", "top");
        $(".filter-val-responsive").css("float", "top");
    }
    $(".filter-wid-responsive").css("width", wsize + "px");

    var hsize = $(window).height() - 82;
    $(".table-responsive").css("height", hsize + "px");
}

$(document).ready(function () {
    resizeComponent();
});

$(window).resize(function () {
    resizeComponent();
});

window.onload = function () {
    initClientMessage();
    var contents = [{ method: 'GET', url: '/api/language/', request: null, keystring: 'API_GET_LANG' }];
    MultiApiCall(contents, initCmdFreak);
}
