<%@ Language=VBScript %>
<%Option Explicit %>
<%Response.Buffer = True

'--------------------------------------------------------------------------
' SQL Thing 
' 
' Overview
' 
' I wrote this as a simple tool for my use to query a db when graphical 
' tools were not handy.  The tool requires ASP 3.0 to run as well as a 
' connection to a database (duh) using a DSN-less connection.  
' It should work with any ODBC data source.
' 
' Please note that I chose to place the CSS styles, DSN, and SQL queries 
' within this page in order to keep the number of necessary support 
' files to a minimum.
'
' Following you will find the SQL script I use to create 
' the table used by the application.  This script works for MS SQL Server 2k.
' 
' CREATE TABLE [dbo].[dyn_SQL] (
' 	[SQLID] [int] IDENTITY (1, 1) NOT NULL ,
' 	[SQLDescription] [varchar] (200) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
' 	[SQLString] [varchar] (4000) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
' 	[AddedDate] [datetime] NULL ,
' 	[Active] [char] (1) COLLATE SQL_Latin1_General_CP1_CI_AS NULL 
' ) ON [PRIMARY]
' GO
' 
' Design Issues:
' 
' When you run a non select query you will get an error messgae even though 
' the query may have run successfully.
' 
' I would like to make it so the user can change to different databases
' 
' I am not thrilled with the function CleanCode(). I works but is not very
' elegant.
' 
' If you have any ideas or make any changes I would appreciate you letting 
' me know and giving me your changes.
' 
' The code uses the query string more than I like.
' 
' I want to add logging to it so I can see what queries are being run.
' 
' I would like to add caregories to the saved queries so I can have more
' queries in an organized manner.
' 
' enjoy it
' 
' Brad Hughes
' braddoro@yahooo.com
'--------------------------------------------------------------------------

'--------------------------------------------------------------------------
' Connections string to connect to the db. Change to suit.
'--------------------------------------------------------------------------
dim str_ConnectionString
str_ConnectionString = "driver={SQL Server};SERVER=bear;UID=sa;PWD=alvahugh;DATABASE=blog"

'--------------------------------------------------------------------------
' Max length of a query, this should be the same size of the db table.
'--------------------------------------------------------------------------
dim int_maxlength
int_maxlength = 4000

'--------------------------------------------------------------------------
' This is the icon you will click on to delete a query.  
'--------------------------------------------------------------------------
dim str_DeleteIcon
str_DeleteIcon = "BD14753_.GIF"

'--------------------------------------------------------------------------
' Passoword to allow anything besides selects.
'--------------------------------------------------------------------------
dim str_Password
str_Password = "alvahugh"

'--------------------------------------------------------------------------
' Passoword to allow anything besides selects.
'--------------------------------------------------------------------------
dim str_DB
str_DB = "AdminDB"

'--------------------------------------------------------------------------
' This function is what I use to disable any type of command besides a 
' select statement.  I suppose there is a more elegant solution but this 
' works.
'--------------------------------------------------------------------------
function CleanCode (InString)
	dim OutString 
	OutString = InString
	if not lcase(request("override")) = str_Password then
		OutString = replace(OutString,"truncate","t------e")
		OutString = replace(OutString,"delete","d----e")
		OutString = replace(OutString,"update","u----e")
		OutString = replace(OutString,"insert","i----t")
		OutString = replace(OutString,"create","c----e")
		OutString = replace(OutString,"alter","a---r")
		OutString = replace(OutString,"exec","e--c")
		OutString = replace(OutString,"kill","k--l")
		OutString = replace(OutString,"drop","d--p")
		OutString = replace(OutString,"sp_","s-_")
		OutString = replace(OutString,"xp_","x-_")
	end if
	CleanCode = OutString
end function

'--------------------------------------------------------------------------
' Inline style to make the screen look better.  You can change this to suit
' your tastes.
'--------------------------------------------------------------------------
%>
<style>
.bigbox {
	top: 5px;
	left: 10px;
	position : absolute;
	background-color: #DCDCDC;
	padding: 5px 5px 5px 5px;
	border: thin solid Black;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 12px;
	margin-right: 5px;
	margin-bottom: 5px;
}

.title {
	background-color: #57799a;
	color: white;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 14px;
	font-weight: bold;
	border: thin solid Black;
	display: block;
	
}
.even {
	background: #FAFAD2;
	color: Black;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
}
.odd {
	background: #CECECE;
	color: black;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
}
.head {
	background: #7C7C7C;
	color: white;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
	font-weight: bold
}
.foot {
	background: #7C7C7C;
	color: white;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
}	
.body {
	background: #466587;
	color: black;	
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
}
.table {
	background: #E1E1E1;
	color: Black;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 10px;
	padding: 0px;
	border-collapse: collapse;
	border: thin solid;
}
</style>
<%
Dim objConn
Dim objRS
Dim x 
Dim Total
Dim SQL
Dim Cols
Dim SQLString
Dim SQLDescription
Set objConn = Server.CreateObject("ADODB.Connection")
objConn.ConnectionString = str_ConnectionString
Server.ScriptTimeout = 240
objConn.ConnectionTimeout = 30
objConn.CommandTimeout = 240
objConn.Open

'--------------------------------------------------------------------------
' Delete a saved query
'--------------------------------------------------------------------------
if cint(request("DEL")) > 0 then
	Set objRS=Server.CreateObject("ADODB.Recordset")
	SQL = "delete from " & str_DB & "..dyn_sql where SQLID = " & Request("del")
	objRS.Open SQL, objConn
	set objRS = nothing
end if

'--------------------------------------------------------------------------
' Get the saved queries
'--------------------------------------------------------------------------
if cint(request("ID")) > 0 then
	Set objRS=Server.CreateObject("ADODB.Recordset")
	SQL = "select SQLDescription, SQLString from " & str_DB & "..dyn_sql where SQLID = " & Request("ID")
	objRS.Open SQL, objConn
	Do While Not objRS.EOF
		SQLDescription = objRS("SQLDescription")
		SQLString = objRS("SQLString")
		objRS.movenext
	Loop
	objRS.close
else
	SQLString = CleanCode(request("TheSQL"))
end if

'--------------------------------------------------------------------------
' Input form.
'--------------------------------------------------------------------------
'#E7E7E7
response.write "<html>"
response.write "<head>"
response.write "<title>SQL Thing</title>"
response.write "</head>"
response.write "<body class='body'>"
response.write "<div class='bigbox'>"
response.write "<span class='title'>SQL Thing</span><br>"
response.write "<form action='sql.asp' method='post' name='send' id='send'>"
response.write "<textarea cols='100' rows='20' name='TheSQL' id='TheSQL'>" & CleanCode(SQLString) & "</textarea><br>"
response.write "Save Name <input type='text' name='QueryName' id='QueryName' value='" & SQLDescription & "' size='50' maxlength='200'> "
response.write "Check to Save<input type='checkbox' name='SaveQuery' id='SaveQuery' value='Y'> "
response.write "pwd <input type='text' name='OverRide' id='OverRide' value='" & request("OverRide") & "' size='8' maxlength='10'> "
response.write "<input type='submit' id='submit' value='submit'> "
response.write "</form>"
if len(CleanCode(SQLString)) > 0 then
	response.write "<h5>Query Length: " & len(trim(CleanCode(SQLString))) & " characters (" & (int_maxlength-len(trim(CleanCode(SQLString)))) & " left)</h5>"
end if

if len(SQLString) > 0 then
	'--------------------------------------------------------------------------
	' Save a query
	'--------------------------------------------------------------------------
	if request("SaveQuery") = "Y" then
		dim objRS2
		dim SQL2
		Set objRS2=Server.CreateObject("ADODB.Recordset")
		SQL2 = "insert into " & str_DB & "..dyn_SQL (SQLDescription, SQLString) select '" & request("QueryName") & "', '" & left(replace(SQLString,"'", "''"), int_maxlength) & "'"
		objRS2.Open SQL2, objConn
		'objRS2.close
		set objRS2 = nothing
	end if
	response.write "<span class='title'>Output</span><br>"
	Set objRS=Server.CreateObject("ADODB.Recordset")
	objRS.Open SQLString, objConn
	
	'--------------------------------------------------------------------------
	' Cycle through the queue recordsets.
	'--------------------------------------------------------------------------
	Do Until objRS Is Nothing
		response.write "<table class='table'>"
		response.write "<tr>"
		
		'----------------------------------------------------------------------
		' Write the headings.
		'----------------------------------------------------------------------
		Cols = 0
		For x = 0 to (objRS.Fields.count - 1)
			Cols = Cols + 1
			Response.Write "<td class='head'>" & objRS.Fields(x).Name & "</td>"
		Next 
		response.write "</tr>"
		Total = 0
		
		'----------------------------------------------------------------------
		' Write the detail for this queue.
		'----------------------------------------------------------------------
		Do While Not objRS.EOF
			response.write "<tr>"
			
			if Total MOD 2 = 0 then
				response.write "<tr class='odd'>"
			else
				response.write "<tr class='even'>"
			end if
			
			For x = 0 to (objRS.Fields.count - 1)
				Response.Write "<TD>" & objRS.Fields(x) & "</TD>"
			Next 
			response.write "</tr>"
			Total = Total + 1
			objRS.MoveNext
		Loop
		response.write "<tr><td colspan='" & Cols & "' class='foot'>Rows: " & Total & "</td></tr>"
		response.write "</table>"
		response.write "<BR>"
		Set objRS = objRS.NextRecordset
	Loop
	Set objRS = Nothing
	Set objConn = Nothing
end if

'----------------------------------------------------------------------
' Display the saved queries.
'----------------------------------------------------------------------
response.write "<span class='title'>Saved</span><br>"
Set objConn = Server.CreateObject("ADODB.Connection")
objConn.ConnectionString = str_ConnectionString
Server.ScriptTimeout = 240
objConn.ConnectionTimeout = 30
objConn.CommandTimeout = 240
objConn.Open
Set objRS=Server.CreateObject("ADODB.Recordset")
SQL = "select SQLID, SQLDescription, SQLString, AddedDate from " & str_DB & "..dyn_sql where Active = 'Y' order by SQLDescription, AddedDate"
objRS.Open SQL, objConn
response.write "<table class='table'>"
response.write "<tr>"
Response.Write "<td class='head'>Name</td>"
Response.Write "<td class='head'>Date</td>"
Response.Write "<td class='head'>Query</td>"
Response.Write "<td class='head'></td>"
response.write "</tr>"
Total = 0
Do While Not objRS.EOF
	response.write "<tr>"
	if Total MOD 2 = 0 then
		response.write "<tr class='odd'>"
	else
		response.write "<tr class='even'>"
	end if
	Response.Write "<TD>" & objRS.Fields("SQLDescription") & "</TD>"
	Response.Write "<TD>" & objRS.Fields("AddedDate") & "</TD>"
	Response.Write "<TD><a href='SQL.asp?ID=" & objRS.Fields("SQLID") & "'>" & objRS.Fields("SQLString") & "</a></TD>"
	Response.Write "<TD><a href='sql.asp?del=" & objRS.Fields("SQLID") & "'><img src='" & str_DeleteIcon & "' alt='delete' width='15' height='15' border='0'></a>&nbsp;</TD>"
	response.write "</tr>"
	Total = Total + 1
	objRS.MoveNext
Loop
response.write "<tr><td colspan='4' class='foot'>Rows: " & Total & "</td></tr>"
response.write "</table>"
response.write "<BR>"
Set objRS = Nothing
Set objConn = Nothing
response.write "</div>"
response.write "</body>"
response.write "</html>"
%>