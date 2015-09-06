#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <shlwapi.h>
#include "MyMsgProc.h"
#include "BbbPage.h"
#include "..\Global.h"

BbbPage* BbbPage::ThisInstance;

BbbPage::BbbPage()
{
}

BbbPage::~BbbPage()
{
}

// Get this instance
BbbPage* BbbPage::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new BbbPage();
		Init = 0;
	}
	return ThisInstance;
}

void BbbPage::WriteText(TCHAR* Txt)
{
	int Utf8Size = WideCharToMultiByte(CP_UTF8, 0, Txt, -1, NULL, 0, NULL, NULL);
	if (Utf8Size > 0) {
		BYTE* Utf8Txt = new BYTE[Utf8Size + 1];
		Utf8Size = WideCharToMultiByte(CP_UTF8, 0, Txt, -1, (LPSTR)Utf8Txt, Utf8Size, NULL, NULL);
		Utf8Txt[Utf8Size] = '\0';
		printf_s("%s", Utf8Txt);
		delete Utf8Txt;
	}
}

void BbbPage::WriteTxtP(TCHAR* Txt, TCHAR* Param)
{
	int LenOfTxt = lstrlen(Txt);
	int LenOfParam = lstrlen(Param);
	if (LenOfTxt <= 0 || LenOfParam <= 0) {
		return;
	}
	int Len = LenOfTxt + LenOfParam;
	TCHAR* TxtWithParam = new TCHAR[Len + 1];
	_snwprintf_s(TxtWithParam, Len + 1, Len, Txt, Param);
	WriteText(TxtWithParam);
	delete TxtWithParam;
}

void BbbPage::WriteTxtP(TCHAR* Txt, int Param)
{
	int LenOfTxt = lstrlen(Txt);
	if (LenOfTxt <= 0) {
		return;
	}
	int Len = LenOfTxt + 20;
	TCHAR* TxtWithParam = new TCHAR[Len + 1];
	_snwprintf_s(TxtWithParam, Len + 1, Len, Txt, Param);
	WriteText(TxtWithParam);
	delete TxtWithParam;
}

void BbbPage::WriteTxtQ(TCHAR* Txt, TCHAR* Param1, TCHAR* Param2)
{
	int LenOfTxt = lstrlen(Txt);
	int LenOfParam1 = lstrlen(Param1);
	int LenOfParam2 = lstrlen(Param2);
	if (LenOfTxt <= 0 || LenOfParam1 <= 0 || LenOfParam2 <= 0) {
		return;
	}
	int Len = LenOfTxt + LenOfParam1 + LenOfParam2;
	TCHAR* TxtWithParam = new TCHAR[Len + 1];
	_snwprintf_s(TxtWithParam, Len + 1, Len, Txt, Param1, Param2);
	WriteText(TxtWithParam);
	delete TxtWithParam;
}

void BbbPage::OutputTitle()
{
	WriteText(_T("<title>CmdFreak</title>\n\n"));
}

void BbbPage::OutputHeadStart()
{
	WriteText(_T("<head>\n"));
}

void BbbPage::OutputHeadEnd()
{
	WriteText(_T("</head>\n\n"));
}

void BbbPage::OutputHeadBase()
{
	WriteText(_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"));
	WriteText(_T("<meta http-equiv=\"content-style-type\" content=\"text/css\" />\n"));
	WriteText(_T("<meta http-equiv=\"pragma\" content=\"non-cache\" />\n"));
	WriteText(_T("<meta http-equiv=\"cache-control\" content=\"non-cache\" />\n"));
	WriteText(_T("<meta http-equiv=\"expires\" content=\"0\" />\n"));

    WriteText(_T("<link rel=\"stylesheet\" href=\"./lib/jquery/jquery-ui.min.css\" />\n"));
    WriteText(_T("<script src=\"./lib/jquery/jquery-1.9.1.min.js\"></script>\n"));
    WriteText(_T("<script src=\"./lib/jquery/jquery-ui.min.js\"></script>\n"));
    WriteText(_T("<link rel=\"stylesheet\" href=\"./lib/grid/pqgrid.min.css\" />\n"));
    WriteText(_T("<script src=\"./lib/grid/pqgrid.min.js\"></script>\n"));
    WriteText(_T("<link rel=\"stylesheet\" href=\"./lib/grid/themes/office/pqgrid.css\" />\n"));
	WriteText(_T("<link rel=\"stylesheet\" href=\"./style.css\" />\n\n"));
}

void BbbPage::OutputHeadMain(int ActionType,
						TCHAR Param1[Global::MAX_PARAM_LENGTH],
						TCHAR Param2[Global::MAX_PARAM_LENGTH],
						TCHAR Param3[Global::MAX_PARAM_LENGTH],
						TCHAR Param4[Global::MAX_PARAM_LENGTH])
{
	WriteText(_T("<script type=\"text/javascript\">\n\n"));

	WriteText(_T("var currentTablename = \"\";\n"));
	WriteText(_T("var currentDbmsname = \"\";\n"));
	WriteText(_T("var columnData = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));

	WriteText(_T("window.onload = function() {\n"));
	WriteText(_T("    $('#adialog').dialog({autoOpen:false});\n"));
	WriteText(_T("    displaySheet();\n"));
	WriteText(_T("    boxResize();\n"));
	WriteText(_T("    document.getElementById('mainarea').style.display='block';\n"));
	WriteText(_T("    displayToolBar(0);\n"));
	if (ActionType != 0) {
		WriteTxtP(_T("    onMenuAction(%d);\n"), ActionType);
	}
	WriteText(_T("    var testResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("    if (sendRequestToWebServer('test', testResponse) != 0) {\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    initializeCurrentTablename();\n"));
	WriteText(_T("    showTable(currentTablename);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function boxResize() {\n"));
	WriteText(_T("    var box_info = document.getElementById('box_info');\n"));
	WriteText(_T("    var box_cont = document.getElementById('container');\n"));
	WriteText(_T("    var boxHeight = getshowHeight();\n"));
	WriteText(_T("    var boxWidth  = getshowWidth();\n"));
	WriteText(_T("    if (boxHeight < 440) {\n"));
	WriteText(_T("        boxHeight = 440;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (boxWidth < 580) {\n"));
	WriteText(_T("        boxWidth = 580;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    box_info.style.height = boxHeight - 20 + 'px';\n"));
	WriteText(_T("    box_info.style.width  = boxWidth  + 'px';\n"));
	WriteText(_T("    box_cont.style.height = boxHeight + 'px';\n"));
	WriteText(_T("    box_cont.style.width  = boxWidth  + 'px';\n"));
	WriteText(_T("    $(function () {\n"));
    WriteText(_T("        $('#grid_array').pqGrid('option', 'width', boxWidth - 2);\n"));
    WriteText(_T("        $('#grid_array').pqGrid('option', 'height', boxHeight - 62);\n"));
	WriteText(_T("    });\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function getshowHeight() {\n"));
	WriteText(_T("    if (window.innerHeight)\n"));
	WriteText(_T("        return window.innerHeight;\n"));
	WriteText(_T("    if (document.body.parentNode.clientHeight)\n"));
	WriteText(_T("        return document.body.parentNode.clientHeight;\n"));
	WriteText(_T("    if (document.body.clientHeight)\n"));
	WriteText(_T("        return document.body.clientHeight;\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function getshowWidth() {\n"));
	WriteText(_T("    if (window.innerWidth)\n"));
	WriteText(_T("        return window.innerWidth;\n"));
	WriteText(_T("    if (document.body.parentNode.clientWidth)\n"));
	WriteText(_T("        return document.body.parentNode.clientWidth;\n"));
	WriteText(_T("    if (document.body.clientWidth)\n"));
	WriteText(_T("        return document.body.clientWidth;\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function displayToolBar(type) {\n"));
	WriteText(_T("    $('#mainarea').empty();\n"));
	WriteText(_T("    var buttons = [\n"));
	WriteTxtP(_T("        ['mainarea', 'img/config_image32s.png', 'img/config_image32c.png', '%s', 100],\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ODBCCONNECTIONS));
	WriteText(_T("        ['mainarea', 'img/sep32.png', 'img/sep32.png', '', -1],\n"));
	WriteTxtP(_T("        ['mainarea', 'img/table_image32s.png', 'img/table_image32c.png', '%s', 200],\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TABLES));
	WriteTxtP(_T("        ['mainarea', 'img/refresh_image32s.png', 'img/refresh_image32c.png', '%s', 400],\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_REFRESHTABLELIST));
	WriteText(_T("        ['mainarea', 'img/sep32.png', 'img/sep32.png', '', -1],\n"));
	WriteTxtP(_T("        ['mainarea', 'img/filter_image32s.png', 'img/filter_image32c.png', '%s', 300],\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTERING));
	WriteText(_T("        ['mainarea', 'img/sep32.png', 'img/sep32.png', '', -1],\n"));
	WriteTxtP(_T("        ['mainarea', 'img/info_image32s.png', 'img/info_image32c.png', '%s', 600]\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_INFORMATION));
	WriteText(_T("    ];\n"));
	WriteText(_T("    for (var Loop = 0; Loop < 8; Loop++) {\n"));
	WriteText(_T("        displayButton(buttons[Loop][0], buttons[Loop][1], buttons[Loop][2], buttons[Loop][3], buttons[Loop][4]);\n"));
	WriteText(_T("        if (Loop == 0) {\n"));
	WriteText(_T("            var connstrResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("            if (sendRequestToWebServer('get-connstr', connstrResponse) != 0) {\n"));
	WriteText(_T("                return;\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            if (connstrResponse.data.DbmsType == 0) {\n"));
	WriteText(_T("                $('#mainarea').append('MySQL/MariaDB');\n"));
	WriteText(_T("            } else {\n"));
	WriteText(_T("                $('#mainarea').append('PostgreSQL');\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (Loop == 2) {\n"));
	WriteText(_T("            $('#mainarea').append('<select id=\"tableselector\" style=\"width:150px\" onchange=\"eventTableSelected()\" align=\"middle\">');\n"));
	WriteText(_T("            $('#mainarea').append('</select>');\n"));
	WriteText(_T("            refreshTableNameList();\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (Loop == 5) {\n"));
	WriteText(_T("            var filterswResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("            if (sendRequestToWebServer('getfiltersw', filterswResponse) != 0) {\n"));
	WriteText(_T("                return;\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            var switchflag = '';\n"));
	WriteText(_T("            if (filterswResponse.data.switchon == true) {\n"));
	WriteText(_T("                switchflag = 'checked';\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            $('#mainarea').append('<input type=\"checkbox\" id=\"filteringonoff\" value=\"filteringOn\" align=\"middle\" onclick=\"eventFilterOn(this)\" ' + switchflag + '>On');\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    appendBr('mainarea');\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function displayButton(target, mouseOutIcon, mouseOverIcon, toolTip, actionType) {\n"));
	WriteText(_T("    var element = document.createElement('img');\n"));
	WriteText(_T("    element.src=mouseOutIcon;\n"));
	WriteText(_T("    element.border='0';\n"));
	WriteText(_T("    element.align='middle';\n"));
	WriteText(_T("    if (actionType != -1) {\n"));
	WriteText(_T("        element.title=toolTip;\n"));
	WriteText(_T("        element.style.cursor='hand';\n"));
	WriteText(_T("        element.onmouseover = function(){\n"));
	WriteText(_T("            element.src=mouseOverIcon;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        element.onmouseout = function(){\n"));
	WriteText(_T("            element.src=mouseOutIcon;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        element.onclick = function(){\n"));
	WriteText(_T("            onMenuAction(actionType);\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("    }\n"));
    WriteText(_T("    var parentElem = document.getElementById(target);\n"));
	WriteText(_T("    parentElem.appendChild(element);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function appendBr(target) {\n"));
	WriteText(_T("    var element = document.createElement('br');\n"));
    WriteText(_T("    var parentElem = document.getElementById(target);\n"));
	WriteText(_T("    parentElem.appendChild(element);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function displaySheet() {\n"));
	WriteText(_T("    var obj = { width:300, height:300, title:'', resizable:false, draggable:false, freezeCols:0 };\n"));
	WriteText(_T("    var data = [];\n"));
	WriteText(_T("    obj.colModel = [{ title: '', width: 10, dataType: 'integer' }];\n"));
	WriteText(_T("    obj.dataModel = { data: data, paging: 'local', rPPOptions:[100, 200, 400], rPP:100 };\n"));
	WriteText(_T("    $('#grid_array').pqGrid(obj);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function showMessageDialog(title, message, actionType, width, height, icon, buttonType) {\n"));
	WriteText(_T("    $('#adialog').dialog({ autoOpen:true, title:title, modal:true, position:'center', draggable:true, resizable: false, height:height, width:width });\n"));
	WriteText(_T("    $('#adialog-icon').empty();\n"));
	WriteText(_T("    $('#adialog-msg').empty();\n"));
	WriteText(_T("    $('#adialog-content').empty();\n"));
	WriteText(_T("    $('#adialog-button').empty();\n"));
	WriteText(_T("    $('#adialog-icon').append('<img src=\"' + icon + '\"/>');\n"));
	WriteText(_T("    $('#adialog-msg').append(message);\n"));
	WriteTxtP(_T("    displayButton('adialog-button', 'img/ok_image32s.png', 'img/ok_image32c.png', '%s', actionType + 1);\n"), MyMsgProc::GetMsg(MyMsgProc::COMMON_OK));
	WriteText(_T("    if (buttonType == 1) {\n"));
	WriteTxtP(_T("        displayButton('adialog-button', 'img/ng_image32s.png', 'img/ng_image32c.png', '%s', actionType + 2);\n"), MyMsgProc::GetMsg(MyMsgProc::COMMON_CANCEL));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function sendRequestToWebServer(operation, response) {\n"));
	WriteText(_T("    var resultcode = 0;\n"));
	WriteText(_T("    $.ajax({\n"));
	WriteText(_T("        type: 'POST',\n"));
	WriteText(_T("        contentType: 'multipart/form-data',\n"));
	WriteText(_T("        url: '',\n"));
	WriteText(_T("        cache: false,\n"));
	WriteText(_T("        dataType: 'json',\n"));
	WriteText(_T("        data: 'operation=' + operation,\n"));
	WriteText(_T("        async: false,\n"));
	WriteText(_T("        success: function(msg){\n"));
	WriteText(_T("            resultcode = msg.resultcode;\n"));
	WriteText(_T("            response.operation = operation;\n"));
	WriteText(_T("            response.resultcode = msg.resultcode;\n"));
	WriteText(_T("            response.data = msg.data;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("    });\n"));
	WriteText(_T("    if (resultcode == 1100) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1100, 512, 230, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_UNEXPECTED));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1110) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1110, 512, 290, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_SERVICEDOWN));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1120) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1120, 512, 290, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_INVALIDDBCONN));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1130) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1130, 512, 290, 'img/cristal_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_WELCOME), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_WELCOME));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_EMPTYCONNSTR));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1140) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1140, 512, 230, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_TOOMUCHRECS));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1150) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1140, 512, 230, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_TOOLONGCONNSTR));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (resultcode == 1160) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', 1140, 512, 230, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_TOOLONGFILTER));
	WriteText(_T("    }\n"));
	WriteText(_T("    return resultcode;\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function refreshTableNameList() {\n"));
	WriteText(_T("    var tablenameResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("    if (sendRequestToWebServer('get-tablenames', tablenameResponse) != 0) {\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    $('#tableselector').empty();\n"));
	WriteText(_T("    for (var tableIndex = 0; tableIndex < tablenameResponse.data.length; tableIndex++) {\n"));
	WriteText(_T("        var tn = tablenameResponse.data[tableIndex].name;\n"));
	WriteText(_T("        var tnReplaced = tn;\n"));
	WriteText(_T("        tnReplaced = tnReplaced.replace(/&lt;/g, \"<\");\n"));
	WriteText(_T("        tnReplaced = tnReplaced.replace(/&gt;/g, \">\");\n"));
	WriteText(_T("        tnReplaced = tnReplaced.replace(/&amp;/g, \"&\");\n"));
	WriteText(_T("        tnReplaced = tnReplaced.replace(/&quot;/g, \"\\\"\");\n"));
	WriteText(_T("        var seld = '';\n"));
	WriteText(_T("        if (tableIndex == 0 && currentTablename == \"\") {\n"));
	WriteText(_T("            currentTablename = tnReplaced;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (tnReplaced == currentTablename) {\n"));
	WriteText(_T("            seld = 'selected';\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        $('#tableselector').append('<option value=\"' + tn + '\" ' + seld + '>' + tn + '</option>');\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function eventTableSelected() {\n"));
	WriteText(_T("    var selectedTableName = $('#tableselector').val();\n"));
	WriteText(_T("    currentTablename = selectedTableName;\n"));
	WriteText(_T("    showTable(currentTablename);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function eventFilterOn(checkboxObj) {\n"));
	WriteText(_T("    var filterswResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("    if (checkboxObj.checked == true) {\n"));
	WriteText(_T("        if (sendRequestToWebServer('filtersw_on', filterswResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        showTable(currentTablename);\n"));
	WriteText(_T("    } else {\n"));
	WriteText(_T("        if (sendRequestToWebServer('filtersw_off', filterswResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        showTable(currentTablename);\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function eventDbmsSelected() {\n"));
	WriteText(_T("    var selectedDbmsName = $('#dbmsselector').val();\n"));
	WriteText(_T("    currentDbmsname = selectedDbmsName;\n"));
	WriteText(_T("    var defconnstrResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("    if (sendRequestToWebServer('get-defconnstr&dbmstype=' + selectedDbmsName, defconnstrResponse) != 0) {\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    $('#connstr').empty();\n"));
	WriteText(_T("    $('#connstr').append(defconnstrResponse.data.defconnstr);\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function eventParamColChanged(selectObj) {\n"));
	WriteText(_T("    var valstr = selectObj.options[selectObj.selectedIndex].value;\n"));
	WriteText(_T("    var fetchedId = selectObj.id.substring(8, 9);\n"));
	WriteText(_T("    if (selectObj.selectedIndex == 0) {\n"));
	WriteText(_T("        $('#paramOpe' + fetchedId).attr('disabled', 'disabled');\n"));
	WriteText(_T("        $('#paramVal' + fetchedId).attr('disabled', 'disabled');\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    $('#paramOpe' + fetchedId).removeAttr('disabled');\n"));
	WriteText(_T("    $('#paramVal' + fetchedId).removeAttr('disabled');\n"));
	WriteText(_T("    var paramOpe = '#paramOpe' + fetchedId;\n"));
	WriteText(_T("    $(paramOpe).empty();\n"));
	WriteText(_T("    $(paramOpe).append('<option value=\"equals\">=</option>');\n"));
	WriteText(_T("    $(paramOpe).append('<option value=\"notequal\">!=</option>');\n"));
	WriteText(_T("    if (columnData.data[selectObj.selectedIndex - 1].dataType == 'integer' ||\n"));
	WriteText(_T("        columnData.data[selectObj.selectedIndex - 1].dataType == 'float') {\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"equalslesserthan\">&lt;=</option>');\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"lesserthan\">&lt;</option>');\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"equalsgreaterthan\">&gt;=</option>');\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"greaterthan\">&gt;</option>');\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (columnData.data[selectObj.selectedIndex - 1].dataType == 'string') {\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"contains\">contains</option>');\n"));
	WriteText(_T("        $(paramOpe).append('<option value=\"notcontain\">does not contain</option>');\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    $(paramOpe).append('<option value=\"null\">is NULL</option>');\n"));
	WriteText(_T("    $(paramOpe).append('<option value=\"notnull\">is not NULL</option>');\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function eventParamOpeChanged(selectObj) {\n"));
	WriteText(_T("    var fetchedId = selectObj.id.substring(8, 9);\n"));
	WriteText(_T("    if (selectObj.options[selectObj.selectedIndex].value == \"notnull\" ||\n"));
	WriteText(_T("        selectObj.options[selectObj.selectedIndex].value == \"null\") {\n"));
	WriteText(_T("        $('#paramVal' + fetchedId).attr('disabled', 'disabled');\n"));
	WriteText(_T("    } else {\n"));
	WriteText(_T("        $('#paramVal' + fetchedId).removeAttr('disabled');\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function showTable(tableName) {\n"));
	WriteText(_T("    if (tableName != \"\") {\n"));
	WriteText(_T("        var columnResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        var recordResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('get-columns&tablename=' + tableName, columnResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (sendRequestToWebServer('get-records&tablename=' + tableName, recordResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        var obj = { title: tableName, editable: false };\n"));
	WriteText(_T("        $('#grid_array').pqGrid({colModel: columnResponse.data});\n"));
	WriteText(_T("        obj.dataModel = { data: recordResponse.data, dataType: 'JSON', paging: 'local', rPPOptions:[100, 200, 400], rPP:100, curPage:1 };\n"));
	WriteText(_T("        $('#grid_array').pqGrid(obj);\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function initializeCurrentTablename() {\n"));
	WriteText(_T("    var selectedTableName = $('#tableselector').val();\n"));
	WriteText(_T("    if (selectedTableName == null) {\n"));
	WriteText(_T("        currentTablename = \"\";\n"));
	WriteText(_T("    } else {\n"));
	WriteText(_T("        currentTablename = selectedTableName;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("function onMenuAction(actionType) {\n"));
	WriteText(_T("    if (actionType == 100) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', actionType, 600, 440, 'img/config_image48c.png', 1);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ODBCCONNECTIONS), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ODBCINFO));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_DBMS));
	WriteText(_T("        $('#adialog-content').append('<select id=\"dbmsselector\" style=\"width:250px\" onchange=\"eventDbmsSelected()\">');\n"));
	WriteText(_T("        $('#adialog-content').append('</select><p>');\n"));
	WriteText(_T("        var connstrResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('get-connstr', connstrResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (connstrResponse.data.DbmsType == 0) {\n"));
	WriteText(_T("            $('#dbmsselector').append('<option value=\"mariadb\" selected>MySQL/MariaDB</option>');\n"));
	WriteText(_T("            currentDbmsname = 'mariadb';\n"));
	WriteText(_T("        } else {\n"));
	WriteText(_T("            $('#dbmsselector').append('<option value=\"mariadb\">MySQL/MariaDB</option>');\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (connstrResponse.data.DbmsType == 1) {\n"));
	WriteText(_T("            $('#dbmsselector').append('<option value=\"postgresql\" selected>PostgreSQL</option>');\n"));
	WriteText(_T("            currentDbmsname = 'postgresql';\n"));
	WriteText(_T("        } else {\n"));
	WriteText(_T("            $('#dbmsselector').append('<option value=\"postgresql\">PostgreSQL</option>');\n"));
	WriteText(_T("        }\n"));
	WriteTxtP(_T("        $('#adialog-content').append('%s<br>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_CONNSTR));
	WriteText(_T("        $('#adialog-content').append('<textarea id=\"connstr\" name=\"connstr\" style=\"width:560px;height:150px\" spellcheck=\"false\"/><p>');\n"));
	WriteText(_T("        $('#connstr').append(connstrResponse.data.ConnStr);\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 101) {\n"));
	WriteText(_T("        var DbmsType = 0;\n"));
	WriteText(_T("        var tmpstr = $('#connstr').val().replace(/[\\n\\r]/g, '');\n"));
	WriteText(_T("        if (currentDbmsname == 'postgresql') DbmsType = 1;\n"));
	WriteText(_T("        var connstrResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('set-connstr&dbmstype=' + DbmsType + '&connstr=' + tmpstr, connstrResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        $('#adialog').dialog('close');\n"));
	WriteText(_T("        currentTablename = \"\";\n"));
	WriteText(_T("        displaySheet();\n"));
	WriteText(_T("        boxResize();\n"));
	WriteText(_T("        displayToolBar(0);\n"));
	WriteText(_T("        var testResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('test', testResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        initializeCurrentTablename();\n"));
	WriteText(_T("        showTable(currentTablename);\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 200) {\n"));
	WriteText(_T("        if (currentTablename == \"\") {\n"));
	WriteTxtQ(_T("            showMessageDialog('%s', '%s', actionType, 600, 270, 'img/table_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TABLES), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TABLEINFO));
	WriteText(_T("            $('#adialog-content').empty();\n"));
	WriteTxtP(_T("            $('#adialog-content').append('<br>%s<br>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_NOTABLEINFO));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', actionType, 600, 440, 'img/table_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TABLES), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TABLEINFO));
	WriteTxtP(_T("        $('#adialog-content').append('%s' + currentTablename + '<br>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_TARGETTABLE));
	WriteText(_T("        $('#adialog-content').append('<table class=\"tblstyle\">');\n"));
	WriteText(_T("        $('.tblstyle').append('<tr><th>Column name</th><th>Column type</th><th>Is nullable</th></tr>');\n"));
	WriteText(_T("        var columnResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('get-columns&tablename=' + currentTablename, columnResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        for (var ColLoop = 0; ColLoop < columnResponse.data.length; ColLoop++) {\n"));
	WriteText(_T("            $('.tblstyle').append('<tr><td>' + columnResponse.data[ColLoop].title + '</td><td>' + columnResponse.data[ColLoop].coltype + '</td><td>' + columnResponse.data[ColLoop].isnull + '</td></tr>');\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        $('#adialog-content').append('</table>');\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 300) {\n"));
	WriteText(_T("        if (currentTablename == \"\") {\n"));
	WriteTxtQ(_T("            showMessageDialog('%s', '%s', actionType, 600, 270, 'img/filter_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTERING), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTER_DESC));
	WriteText(_T("            $('#adialog-content').empty();\n"));
	WriteTxtP(_T("            $('#adialog-content').append('<br>%s<br>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTERING_NOTABLE));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', actionType, 600, 440, 'img/filter_image48c.png', 1);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTERING), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTER_DESC));
	WriteText(_T("        for (var Loop = 1; Loop <= 5; Loop++) {\n"));
	WriteText(_T("            $('#adialog-content').append('' + Loop + ': ');\n"));
	WriteText(_T("            var paramCol = document.createElement('select');\n"));
	WriteText(_T("            paramCol.id = 'paramCol' + Loop;\n"));
	WriteText(_T("            paramCol.style.width = '130px';\n"));
	WriteText(_T("            paramCol.align = 'middle';\n"));
	WriteText(_T("            paramCol.onchange = function() {;\n"));
	WriteText(_T("                eventParamColChanged(this);\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            $('#adialog-content').append(paramCol);\n"));
	WriteText(_T("            $('#adialog-content').append(' ');\n"));
	WriteText(_T("            var paramOpe = document.createElement('select');\n"));
	WriteText(_T("            paramOpe.id = 'paramOpe' + Loop;\n"));
	WriteText(_T("            paramOpe.style.width = '140px';\n"));
	WriteText(_T("            paramOpe.align = 'middle';\n")); 
	WriteText(_T("            paramOpe.onchange = function() {;\n"));
	WriteText(_T("                eventParamOpeChanged(this);\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            $('#adialog-content').append(paramOpe);\n"));
	WriteText(_T("            $('#adialog-content').append(' ');\n"));
	WriteText(_T("            var paramVal = document.createElement('input');\n"));
	WriteText(_T("            paramVal.id = 'paramVal' + Loop;\n"));
	WriteText(_T("            paramVal.type = 'text';\n"));
	WriteText(_T("            paramVal.size = '30';\n"));
	WriteText(_T("            paramVal.align = 'middle';\n")); 
	WriteText(_T("            $('#adialog-content').append(paramVal);\n"));
	WriteText(_T("            $('#adialog-content').append('<p>');\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        if (sendRequestToWebServer('get-columns&tablename=' + currentTablename, columnData) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        var filtercdResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('getfiltercd', filtercdResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        for (var LoopCol = 1; LoopCol <= 5; LoopCol++) {\n"));
	WriteText(_T("            $('#paramOpe' + LoopCol).attr('disabled', 'disabled');\n"));
	WriteText(_T("            $('#paramVal' + LoopCol).attr('disabled', 'disabled');\n"));
	WriteTxtP(_T("            $('#paramCol' + LoopCol).append('<option value=\"*\">%s</option>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_UNSPEC));
	WriteText(_T("            var filFndFlag = false;\n"));
	WriteText(_T("            for (var Loop in columnData.data) {\n"));
	WriteText(_T("                var seld = '';\n"));
	WriteText(_T("                if (columnData.data[Loop].title == filtercdResponse.data[LoopCol - 1].columnname) {\n"));
	WriteText(_T("                    seld = 'selected';\n"));
	WriteText(_T("                    filFndFlag = true;\n"));
	WriteText(_T("                }\n"));
	WriteText(_T("                $('#paramCol' + LoopCol).append('<option value=\"' + columnData.data[Loop].title + '\" ' + seld + '>' + columnData.data[Loop].title + '</option>');\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            if (filFndFlag == false && filtercdResponse.data[LoopCol - 1].columnname != '*') {\n"));
	WriteText(_T("                $('#adialog-content').empty();\n"));
	WriteTxtP(_T("                $('#adialog-content').append('<br>%s<br>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_FILTER_INVALID));
	WriteText(_T("                return;\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            eventParamColChanged($('#paramCol' + LoopCol).get(0));\n"));
	WriteText(_T("            switch (filtercdResponse.data[LoopCol - 1].opetype) {\n"));
	WriteText(_T("                case 1  : $('#paramOpe' + LoopCol).val('equals'); break;\n"));
	WriteText(_T("                case 2  : $('#paramOpe' + LoopCol).val('notequal'); break;\n"));
	WriteText(_T("                case 3  : $('#paramOpe' + LoopCol).val('equalslesserthan'); break;\n"));
	WriteText(_T("                case 4  : $('#paramOpe' + LoopCol).val('lesserthan'); break;\n"));
	WriteText(_T("                case 5  : $('#paramOpe' + LoopCol).val('equalsgreaterthan'); break;\n"));
	WriteText(_T("                case 6  : $('#paramOpe' + LoopCol).val('greaterthan'); break;\n"));
	WriteText(_T("                case 10 : $('#paramOpe' + LoopCol).val('contains'); break;\n"));
	WriteText(_T("                case 11 : $('#paramOpe' + LoopCol).val('notcontain'); break;\n"));
	WriteText(_T("                case 20 : $('#paramOpe' + LoopCol).val('null'); $('#paramVal' + LoopCol).attr('disabled', 'disabled'); break;\n"));
	WriteText(_T("                case 21 : $('#paramOpe' + LoopCol).val('notnull'); $('#paramVal' + LoopCol).attr('disabled', 'disabled'); break;\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            $('#paramVal' + LoopCol).val(filtercdResponse.data[LoopCol - 1].value);\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 301) {\n"));
	WriteText(_T("        if (currentTablename == \"\") {\n"));
	WriteText(_T("            $('#adialog').dialog('close');\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        for (var Loop = 1; Loop <= 5; Loop++) {\n"));
	WriteText(_T("            var paramCol = $('#paramCol' + Loop + ' option:selected').val();\n"));
	WriteText(_T("            var paramOpe = $('#paramOpe' + Loop + ' option:selected').val();\n"));
	WriteText(_T("            var paramVal = $('#paramVal' + Loop).val();\n"));
	WriteText(_T("            var paramOpeInt = 0;\n"));
	WriteText(_T("            if (typeof paramCol === 'undefined' || typeof paramOpe === 'undefined' || typeof paramVal === 'undefined') {\n"));
	WriteText(_T("                paramCol = '*';\n"));
	WriteText(_T("                paramVal = '';\n"));
	WriteText(_T("            } else {;\n"));
	WriteText(_T("                switch (paramOpe) {\n"));
	WriteText(_T("                    case 'equals'            : paramOpeInt = 1; break;\n"));
	WriteText(_T("                    case 'notequal'          : paramOpeInt = 2; break;\n"));
	WriteText(_T("                    case 'equalslesserthan'  : paramOpeInt = 3; break;\n"));
	WriteText(_T("                    case 'lesserthan'        : paramOpeInt = 4; break;\n"));
	WriteText(_T("                    case 'equalsgreaterthan' : paramOpeInt = 5; break;\n"));
	WriteText(_T("                    case 'greaterthan'       : paramOpeInt = 6; break;\n"));
	WriteText(_T("                    case 'contains'          : paramOpeInt = 10; break;\n"));
	WriteText(_T("                    case 'notcontain'        : paramOpeInt = 11; break;\n"));
	WriteText(_T("                    case 'null'              : paramOpeInt = 20; break;\n"));
	WriteText(_T("                    case 'notnull'           : paramOpeInt = 21; break;\n"));
	WriteText(_T("                }\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("            var filterResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("            if (sendRequestToWebServer('setfiltercd&paramidx=' + Loop + '&paramcol=' + paramCol + '&paramope=' + paramOpeInt + '&paramval=' + paramVal, filterResponse) != 0) {\n"));
	WriteText(_T("                return;\n"));
	WriteText(_T("            }\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        showTable(currentTablename);\n"));
	WriteText(_T("        $('#adialog').dialog('close');\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 400) {\n"));
	WriteText(_T("        displayToolBar(0);\n"));
	WriteText(_T("        initializeCurrentTablename();\n"));
	WriteText(_T("        showTable(currentTablename);\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 600) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', actionType, 600, 440, 'img/info_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_INFORMATION), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_COMPOINFO));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_COMPOINFO2));
	WriteText(_T("        $('#adialog-content').append('<p>');\n"));
	WriteTxtQ(_T("        $('#adialog-content').append('<a href=\"%s\" target=\"manual\">%s</a><p>');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_MANUAL), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_MANUALMSG));
	WriteText(_T("        var timeResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('config-time', timeResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteText(_T("        $('#adialog-content').append('Build Time = ' + timeResponse.data.buildtime + '<br/>');\n"));
	WriteText(_T("        $('#adialog-content').append('Service Start Time = ' + timeResponse.data.starttime + '<br/>');\n"));
	WriteText(_T("        $('#adialog-content').append('<br/>');\n"));
	WriteText(_T("        var logResponse = {'operation' : '', 'resultcode' : 0, 'data' : {}};\n"));
	WriteText(_T("        if (sendRequestToWebServer('config-checklog', logResponse) != 0) {\n"));
	WriteText(_T("            return;\n"));
	WriteText(_T("        }\n"));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_LOG_DESC));
	WriteText(_T("        $('#adialog-content').append('' + logResponse.data.log + '<br/>');\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType == 1100) {\n"));
	WriteTxtQ(_T("        showMessageDialog('%s', '%s', actionType, 512, 230, 'img/error_image48c.png', 0);\n"), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED), MyMsgProc::GetMsg(MyMsgProc::AN_ERROR_OCCURRED));
	WriteTxtP(_T("        $('#adialog-content').append('%s');\n"), MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_UNEXPECTED));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("    if (actionType % 10 == 1 || actionType % 10 == 2) {\n"));
	WriteText(_T("        $('#adialog').dialog('close');\n"));
	WriteText(_T("        return;\n"));
	WriteText(_T("    }\n"));
	WriteText(_T("}\n\n"));

	WriteText(_T("</script>\n"));
}

void BbbPage::OutputBodyMainStart()
{
	WriteText(_T("\n<body style=\"overflow: hidden;\" onresize=\"boxResize()\">\n<div id=\"container\"><div class=\"spc\">\n"));
	WriteText(_T("<div class=\"headtailbar\">\n"));
	WriteText(_T("    <table class=\"ttblstyle\" width=\"100%\"><tr>\n"));
	WriteText(_T("        <td align=\"left\"><b>CmdFreak</b></td>\n"));
	WriteText(_T("        <td align=\"right\">Optimized for <img src=\"img/browser_ie.png\">8,9,10 & <img src=\"img/browser_ff.png\">3,10,17 </td>\n"));
	WriteText(_T("    </tr></table>\n"));
	WriteText(_T("</div>\n\n"));
}

void BbbPage::OutputBodyMainEnd()
{
	WriteText(_T("</div></div>\n"));
	WriteText(_T("</body>\n\n"));
}

void BbbPage::OutputMain(int ActionType,
						TCHAR Param1[Global::MAX_PARAM_LENGTH],
						TCHAR Param2[Global::MAX_PARAM_LENGTH],
						TCHAR Param3[Global::MAX_PARAM_LENGTH],
						TCHAR Param4[Global::MAX_PARAM_LENGTH])
{
	WriteText(_T("<div id=\"box_info\">\n"));

	WriteText(_T("    <noscript>\n"));
	WriteText(_T("        "));
	WriteText(MyMsgProc::GetMsg(MyMsgProc::CMDFRK_ERROR_JAVASCRIPTERR));
	WriteText(_T("    </noscript>\n\n"));

	WriteText(_T("    <div id=\"mainarea\" style=\"display:none\">\n"));
	WriteText(_T("        <div id=\"adialog\">\n"));
	WriteText(_T("            <table class=\"ttblstyle\" width=\"100%\"><tr>\n"));
	WriteText(_T("            <td id=\"adialog-icon\" width=\"60px\"></td><td id=\"adialog-msg\"></td>\n"));
	WriteText(_T("            </tr></table><p>\n"));
	WriteText(_T("            <div id=\"adialog-content\" align=\"left\"></div>\n"));
	WriteText(_T("            <hr/>\n"));
	WriteText(_T("            <div id=\"adialog-button\" align=\"right\"></div>\n"));
	WriteText(_T("        </div>\n"));
	WriteText(_T("    </div>\n"));
	WriteText(_T("    <div id=\"grid_array\">\n"));
	WriteText(_T("    </div>\n"));

	WriteText(_T("</div>\n\n"));
}

void BbbPage::MainPage(int ActionType,
						TCHAR Param1[Global::MAX_PARAM_LENGTH],
						TCHAR Param2[Global::MAX_PARAM_LENGTH],
						TCHAR Param3[Global::MAX_PARAM_LENGTH],
						TCHAR Param4[Global::MAX_PARAM_LENGTH])
{
	WriteText(_T("Content-Type: text/html\n\n"));
	WriteText(_T("<!doctype html>\n"));
	WriteText(_T("<html>\n"));
	OutputHeadStart();
	OutputTitle();
	OutputHeadBase();
	OutputHeadMain(ActionType, Param1, Param2, Param3, Param4);
	OutputHeadEnd();

	OutputBodyMainStart();
	OutputMain(ActionType, Param1, Param2, Param3, Param4);
	OutputBodyMainEnd();
	WriteText(_T("</html>\n"));
}

void BbbPage::OutputCommandResult(TCHAR Operation[BbbPage::MAX_OPERATION_LENGTH], int ResultCode, TCHAR Data[Global::MAX_PARAM_LENGTH])
{
	WriteText(_T("Content-Type: application/json\n\n"));
	WriteText(_T("{"));
	WriteTxtP(_T("\"operation\": \"%s\""), Operation);
	WriteTxtP(_T(", \"resultcode\": %d"), ResultCode);
	if (Data != NULL) {
		if (lstrcmp(Data, _T(""))) {
			WriteTxtP(_T(", \"data\": %s"), Data);
		} else {
			WriteText(_T(", \"data\": {}"));
		}
	} else {
			WriteText(_T(", \"data\": {}"));
	}
	WriteText(_T("}\n"));
}
