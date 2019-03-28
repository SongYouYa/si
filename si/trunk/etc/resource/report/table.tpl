{{%AUTOESCAPE context="HTML"}}
<div class="dtable" id="{{ID}}">{{BI_NEWLINE}}
	{{#TITLE}}
	<div class="title">{{BI_NEWLINE}}
		{{#IMG}}
		<img src="{{SRC}}" />
		{{/IMG}}
		
		<h{{LEVEL}}>
			<a name="{{ARCHOR_NAME}}"></a>
			{{#PARENT_ARCHOR}}
			<a href="#{{TARGET_NAME}}">{{TARGET_TEXT}}</a>
			{{/PARENT_ARCHOR}}
			{{TEXT}}
		</h{{LEVEL}}>
		
		<p class="stretch_btn" id="{{ID}}"></p>
	</div>{{BI_NEWLINE}}
	{{/TITLE}}

	<table class="table{{#TABLE_CLASS}} {{CLASS_NAME}}{{/TABLE_CLASS}}" {{#TB_PROPERTY}} {{PVALUE:none}} {{/TB_PROPERTY}}>{{BI_NEWLINE}}
		{{#HEAD}}
		<thead class="{{HEAD_CLASS}}">{{BI_NEWLINE}}
			<tr {{#HR_PROPERTY}} {{PVALUE:none}} {{/HR_PROPERTY}}>
				{{#CELL}}
				<td class="{{CLASS_NAME}}" {{#HD_PROPERTY}} {{PVALUE:none}} {{/HD_PROPERTY}}>{{CELL_DATA}}</td>
				{{/CELL}}
			</tr>{{BI_NEWLINE}}
		</thead>{{BI_NEWLINE}}
		{{/HEAD}}
		<tbody>{{BI_NEWLINE}}
		{{#ROW}}
		<tr>{{BI_NEWLINE}}
			{{#CELL}}
			<td{{#CLASS}} class="{{NAME}}"{{/CLASS}}{{#ROW_SPAN}} rowspan="{{SPAN_COUNT}}"{{/ROW_SPAN}}{{#COL_SPAN}} colspan="{{SPAN_COUNT}}"{{/COL_SPAN}}>

				{{#IMG}}
				<img src="{{SRC}}" />
				{{/IMG}}
				
				{{#ARCHOR}}
				<a href="#{{TARGET_NAME}}">{{TEXT}}</a>
				{{/ARCHOR}}
				
				{{#URL}}
				<a href="{{URL_ADD}}" target="_blank">{{URL_DISPLAY}}</a>
				{{/URL}}
				
				{{#MEDIA_IMG}}
				<a href="{{IMG_PATH}}" target="_blank">
				<img class="mediaImg" src="{{IMG_PATH}}"/>
				</a>
				{{/MEDIA_IMG}}
				
				{{#MEDIA_COMMON}}
				<a href="{{MEDIA_PATH}}" target="_blank">{{MEDIA_NAME}}</a>
				{{/MEDIA_COMMON}}

				{{#WEB_PAGE}}
				<a href="{{PIC_LINK}}" target="_blank">{{PIC_DISPLAY}}</a>
				<a href="{{URL}}" target="_blank">   {{TITLE}}</a>
				<br/>
				{{/WEB_PAGE}}
				
				{{CELL_DATA:html_escape_with_arg=snippet}}
			</td>
			{{/CELL}}
		</tr>{{BI_NEWLINE}}
		{{/ROW}}
		</tbody>{{BI_NEWLINE}}
		
	</table>{{BI_NEWLINE}}
	
	{{#MORE_TARGET}}
	<a href="{{MORE_TARGET_HTML}}" class="more" target="_blank">{{TEXT}} >></a>{{BI_NEWLINE}}
	{{/MORE_TARGET}}
</div>{{BI_NEWLINE}}