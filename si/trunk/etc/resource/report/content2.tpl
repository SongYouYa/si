{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>{{BI_NEWLINE}}
<head>{{BI_NEWLINE}}
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />{{BI_NEWLINE}}
<style type="text/css">{{BI_NEWLINE}}
<!-- {{BI_NEWLINE}}
body {padding:0; margin:0; font-family: "Microsoft YaHei"; font-size:14px; background: none; width:95%; margin:auto;} {{BI_NEWLINE}}

table {border-collapse:collapse; border:solid 1px #D7D7D7; width:100%; word-break: break-all;} {{BI_NEWLINE}}
table tr td {padding:6px 8px 6px 8px; border:solid 1px #D7D7D7; text-align:left;} {{BI_NEWLINE}}
table tr td.cHidden {display:none;} /*hide*/{{BI_NEWLINE}}
table thead td{background:#F3F3F3; min-width:30px} {{BI_NEWLINE}}
table thead td.cTime {min-width:80px;} /*time*/{{BI_NEWLINE}}
table thead td.cSize {min-width:60px} /*size*/ {{BI_NEWLINE}}

.keycol { width: 20%; } {{BI_NEWLINE}}
.numCell { width:45px; } {{BI_NEWLINE}}

-->{{BI_NEWLINE}}
</style>{{BI_NEWLINE}}
</head>{{BI_NEWLINE}}
<body class="body">{{BI_NEWLINE}}
	<br/>
	<center style="font-size:20px; color:#3F518A;"><b>{{HTML_TITLE}}</b></center><p>&nbsp;</p>{{BI_NEWLINE}}
	
	{{#CATOGARY}}
	{{#TITLE}}
	<h{{LEVEL}}>
		<a name="{{ARCHOR_NAME}}"></a>
		{{TEXT}}
	</h{{LEVEL}}>{{BI_NEWLINE}}
	{{/TITLE}}
	
	{{#INCL_TABLE}}
	{{>INCL_TPL}}
	<br/>
	{{/INCL_TABLE}}
	
	{{/CATOGARY}}
	
	{{#SIGNS}}
	<br/>
	<p>
	<a name="{{ARCHOR_NAME}}"></a>
	调查人（签字）：______________ 见证人（签字）：______________ 对象信息（签字）：______________
	</p>
	<br/>
	{{/SIGNS}}
</body>{{BI_NEWLINE}}
</html>{{BI_NEWLINE}}