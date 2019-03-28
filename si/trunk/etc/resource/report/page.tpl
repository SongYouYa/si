{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>{{BI_NEWLINE}}
<head>{{BI_NEWLINE}}
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />{{BI_NEWLINE}}
<link href="{{UPPER_DIR}}themes/css/page.css" rel="stylesheet" type="text/css" />{{BI_NEWLINE}}
</head>{{BI_NEWLINE}}
<body class="body">{{BI_NEWLINE}}

	{{#INCL_TABLE}}
	{{>INCL_TPL}}
	{{/INCL_TABLE}}
	
	{{#PAGE_JUMP}}
	<div class="pagejump">{{BI_NEWLINE}}
		<span>{{TOTAL_PAGE_TEXT}} : {{TOTAL_PAGE_COUNT}}  </span>
		<select onchange="location.href=this.options[this.selectedIndex].value;">{{BI_NEWLINE}}
			{{#OPTION}}
			<option value="{{VALUE}}" {{#SELECTED}}selected="selected"{{/SELECTED}}>{{OPTION_NUM}}</option>{{BI_NEWLINE}}
			{{/OPTION}}
		</select>{{BI_NEWLINE}}
		{{#PAGE_ARCHOR}}
		<a{{#HREF}} href="{{TARGET_HTML}}"{{/HREF}}{{#CLASS}} class="{{CLASS_NAME}}"{{/CLASS}}>{{TARGET_TEXT}}</a>{{BI_NEWLINE}}
		{{/PAGE_ARCHOR}}
	</div>
	{{/PAGE_JUMP}}
	
</body>{{BI_NEWLINE}}
</html>{{BI_NEWLINE}}