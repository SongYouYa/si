{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">{{BI_NEWLINE}}
<html>{{BI_NEWLINE}}
<head>{{BI_NEWLINE}}
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />{{BI_NEWLINE}}
	<script src="{{THEMES_DIR}}/themes/jquery-1.9.1.min.js" type="text/javascript"></script>{{BI_NEWLINE}}
	<link rel="stylesheet" href="{{THEMES_DIR}}/themes/jstree/style.min.css" />{{BI_NEWLINE}}
	<script src="{{THEMES_DIR}}/themes/jstree/jstree-3.3.4.min.js"></script>{{BI_NEWLINE}}
</head>{{BI_NEWLINE}}
<body class="body" style="margin-top:5px; font-size: 14px;">{{BI_NEWLINE}}
	<div id="jstree"></div>{{BI_NEWLINE}}
	
	<script>{{BI_NEWLINE}}
	var treeDataObj = JSON.parse('{
	"plugins": ["types"],
	"types": {"default": {"icon":false}},
	"core" : {
		"data" : {{TREE_DATA}}
	}
	}'); {{BI_NEWLINE}}
	
	$('#jstree')
		.on("changed.jstree", function (e, data) {
			if (data.selected.length) { {{BI_NEWLINE}}
				var node = data.instance.get_node(data.selected[0]); {{BI_NEWLINE}}
				var nodeData = node.data; {{BI_NEWLINE}}
				if (nodeData) { {{BI_NEWLINE}}
					var fs = window.parent.frames;{{BI_NEWLINE}}
					fs["contentframe"].location.href = nodeData.target;{{BI_NEWLINE}}
				}{{BI_NEWLINE}}
			}
		}) {{BI_NEWLINE}}
		.on("loaded.jstree", function (e, data) { {{BI_NEWLINE}}
			data.instance.open_node('1'); {{BI_NEWLINE}}
			data.instance.select_node('1'); {{BI_NEWLINE}}
		}) {{BI_NEWLINE}}
		.jstree(treeDataObj);
	{{BI_NEWLINE}}
	</script>{{BI_NEWLINE}}
</body>{{BI_NEWLINE}}
</html>