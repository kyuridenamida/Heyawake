
// Ajax
var times = 0;
function xhr(file, data, func) {
	var fh = createHttpRequest();
	fh.open("POST", file, "True");
	fh.onreadystatechange = function() {
		if (fh.readyState == 4 && fh.status == 200)
			func(
				fh.responseText.slice(fh.responseText.indexOf("<res>")+5, fh.responseText.indexOf("</res>")),
				fh.responseText.slice(fh.responseText.indexOf("<data>")+6, fh.responseText.indexOf("</data>"))
			);
	}
//	fh.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	fh.send(data + "&times=" + (++times));
}

function xhr2(file, datas, func, method) {
	var Datas = [], key;
	for (key in datas) {
		Datas.push( key + "=" + encodeURIComponent(datas[key] + "").replace("%0D", "") );
	}
	var data = Datas.join("&");

	
	if (method == null) method = "POST";
	var fh = createHttpRequest();
	fh.open(method, file, true);
	fh.onreadystatechange = function() {
		if (fh.readyState == 4 && fh.status == 200) {
			func(
				fh.responseText.slice(fh.responseText.indexOf("<res>")+5, fh.responseText.indexOf("</res>")),
				fh.responseText.slice(fh.responseText.indexOf("<data>")+6, fh.responseText.indexOf("</data>")),
				fh.responseText
			);
		}
	}
	//fh.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	fh.send(data);
	//fh.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
}

// スライダー
var slider_drag = function() {
	this.id = "";
	this.dx = 0;
}
var drag_functions = {};
function use_slider() {
	Event.observe(top.document, "mouseup", function() { slider_drag.id = ""; });
	Event.observe(document, "mousemove", function(event) {
		if (slider_drag.id) {
			var elem = $(slider_drag.id + "_button");
			var line = $(slider_drag.id);

			elem.x = event.clientX - line.offsetLeft + elem.min - slider_drag.dx;

			if (elem.x < elem.min) elem.x = elem.min;
			else if (elem.x > elem.max) elem.x = elem.max;

			elem.style.left = elem.x + "px";

			drag_functions[slider_drag.id]((elem.x - elem.min) / elem.len);
		}
	});
}
function slider(line_id, src, value, dw, dh, f) {
	f(value);

	var elem;
	var button_id = line_id + "_button";
	if (src == "") {
		// つまみの初期位置設定
		elem = $(button_id);
		elem.style.display = "";
		if (elem.style.position == "relative") {
			var elem_l = $(line_id);
			elem.style.left = elem_l.offsetLeft - elem.offsetLeft - Math.floor(elem.offsetWidth/2) + "px";
			elem.style.top = elem_l.offsetTop - elem.offsetTop - Math.floor(elem.offsetHeight/2) + "px";
		}
		else {
			var elem_l = $(line_id);
			elem.style.left = elem_l.offsetLeft - Math.floor(elem.offsetWidth/2) + "px";
			elem.style.top = elem_l.offsetTop - Math.floor(elem.offsetHeight/2) + "px";
		}

		elem.len = $(line_id).offsetWidth;
		elem.min = rpx(elem.style.left) * 1;
		elem.max = elem.min + elem.len;
		elem.x = elem.min + Math.floor(elem.len * value);

		elem.style.left = elem.x + "px";
	}
	else {
		elem = document.createElement("img");

		elem.setAttribute("id", button_id);
		elem.setAttribute("src", src);
		elem.style.cssText = "position: absolute; cursor: pointer;";

		elem.len = $(line_id).offsetWidth;
		elem.min = $(line_id).offsetLeft - dw;
		elem.max = elem.min + elem.len;
		elem.x = elem.min + Math.floor(elem.len * value);

		elem.style.left = elem.x + "px";
		elem.style.top = $(line_id).offsetTop - dh + "px";

		document.body.appendChild(elem);
	}

	Event.observe(button_id, "mousedown", function(event) {
		slider_drag.id = line_id;
		slider_drag.dx = event.clientX - this.offsetLeft;
	});

	Event.observe(line_id, "click", function(event) {
		var elem = $(this.id + "_button");
		elem.style.left = (elem.x = event.clientX - this.offsetLeft + elem.min) + "px";
		drag_functions[this.id]((elem.x - elem.min) / elem.len);
	});

	drag_functions[line_id] = f;
}

// CSS系
function over_change(id, f) {
	eval('Event.observe(' + id + ', "mouseover", ' + f + ')');
	eval('Event.observe(' + id + ', "mouseout", function() { this.style.cssText = "' + $(id).style.cssText + '"; })');
}
function over_line(id) {
	$(id).style.cursor = "pointer";
	eval('Event.observe("' + id + '", "mouseover", function() { this.style.textDecoration = "underline"; })');
	eval('Event.observe("' + id + '", "mouseout",  function() { this.style.textDecoration = ""; })');
}
function over_italic(id) {
	$(id).style.cursor = "pointer";
	eval('Event.observe("' + id + '", "mouseover", function() { this.style.fontStyle = "italic"; })');
	eval('Event.observe("' + id + '", "mouseout",  function() { this.style.fontStyle = ""; })');
}
function over(id, class1, class2) {
	eval('Event.observe("' + id + '", "mouseover", function() { this.className = class2; })');
	eval('Event.observe("' + id + '", "mouseout",  function() { this.className = class1; })');
}
function alpha(id, alp) {
	$(id).style.zoom = 1.0;
	$(id).style.filter = "alpha(opacity=" + alp + ")";
	$(id).style.opacity = alp / 100;
	if (navigator.userAgent.indexOf("MSIE") <= -1)
		$(id).style.setProperty("-moz-opacity", alp / 100, "");
}
var filter = alpha;
function round(id, r1, r2, r3, r4) {
	if (r2 || r3 || r4) {
		if (r1) {
			$(id).style.setProperty("border-top-left-radius", r1 + "px", "");
			$(id).style.setProperty("-webkit-border-top-left-radius", r1 + "px", "");
			$(id).style.setProperty("-moz-border-radius-topleft", r1 + "px", "");
		}
		if (r2) {
			$(id).style.setProperty("border-top-right-radius", r2 + "px", "");
			$(id).style.setProperty("-webkit-border-top-right-radius", r2 + "px", "");
			$(id).style.setProperty("-moz-border-radius-topright", r2 + "px", "");
		}
		if (r3) {
			$(id).style.setProperty("borde-bottom-leftr-radius", r3 + "px", "");
			$(id).style.setProperty("-webkit-bottom-left-border-radius", r3 + "px", "");
			$(id).style.setProperty("-moz-border-radius-bottomleft", r3 + "px", "");
		}
		if (r4) {
			$(id).style.setProperty("border-bottom-right-radius", r4 + "px", "");
			$(id).style.setProperty("-webkit-border-bottom-right-radius", r4 + "px", "");
			$(id).style.setProperty("-moz-border-radius-bottomright", r4 + "px", "");
		}
	}
	else {
		$(id).style.setProperty("border-radius", r1 + "px", "");
		$(id).style.setProperty("-webkit-border-radius", r1 + "px", "");
		$(id).style.setProperty("-moz-border-radius", r1 + "px", "");
	}
}
function make_button(id, r, g, b) {
	if (typeof r === "string") {
		b = parseInt(r.substr(4, 2), 16);
		g = parseInt(r.substr(2, 2), 16);
		r = parseInt(r.substr(0, 2), 16);
	}

	var light = 1.4, dark = 0.6;
	var elem = $(id);
	elem.style.backgroundColor = make_rgb(r, g, b);
	elem.style.borderLeft = elem.style.borderTop = "1px solid " + make_rgb(r*light, g*light, b*light);
	elem.style.borderRight = elem.style.borderBottom = "1px solid " + make_rgb(r*dark, g*dark, b*dark);
}
function over_line(id) {
	$(id).style.cursor = "pointer";
	eval('Event.observe("' + id + '", "mouseover", function() { this.style.textDecoration = "underline"; })');
	eval('Event.observe("' + id + '", "mouseout",  function() { this.style.textDecoration = ""; })');
}
function over_italic(id) {
	$(id).style.cursor = "pointer";
	eval('Event.observe("' + id + '", "mouseover", function() { this.style.fontStyle = "italic"; })');
	eval('Event.observe("' + id + '", "mouseout",  function() { this.style.fontStyle = ""; })');
}
function no_drag(id, f) {
	if (f == null) f = false;
	if (f) {
		$(id).onmousemove   = function() { return false; }
		$(id).onmousedown   = function() { return false; }
		$(id).oncontextmenu = function() { return false; }
		$(id).onselectstart = function() { return false; }
	}
	else {
		$(id).onmousemove   = function() {  }
		$(id).onmousedown   = function() {  }
		$(id).oncontextmenu = function() {  }
		$(id).onselectstart = function() {  }
	}
}
function shadow(id, s, r, g, b) {
	if (s) {
		var rgb = make_rgb(r, g, b);
		$(id).style.textShadow = "-" + s + "px -" + s + "px 0 " + rgb + ", " + s + "px -" + s + "px 0 " + rgb + ", -" + s + "px " + s + "px 0 " + rgb + ", " + s + "px " + s + "px 0 " + rgb;
	}
	else $(id).style.textShadow = "";
}
function draw(elem, flag) {
	if (flag == null) elem = $(elem);
	elem.style.left = elem.x + "px";
	elem.style.top = elem.y + "px";
//	elem.style.display = "";
}

// 取得系
function GetLeft(oj) {
	var x = 0;
	oj = $(oj);
	while (oj) {
		x += oj.offsetLeft;
		oj = oj.offsetParent;
	}
	return x;
}
function GetTop(oj) {
	var y = 0;
	oj = $(oj);
	while (oj) {
		y += oj.offsetTop;
		oj = oj.offsetParent;
	}
	return y;
}
function getBrowserWidth() {
        if ( window.innerWidth ) return window.innerWidth;
        else if ( document.documentElement && document.documentElement.clientWidth != 0 ) return document.documentElement.clientWidth;
        else if ( document.body ) return document.body.clientWidth;
        return 0;
}
function getBrowserHeight() {
        if ( window.innerHeight ) return window.innerHeight;
        else if ( document.documentElement && document.documentElement.clientHeight != 0 ) return document.documentElement.clientHeight;
        else if ( document.body ) return document.body.clientHeight;
        return 0;
}
function rpx(px) {
	return +px.replace(/px/, "");
}

// その他
function succeedClass(obj1, obj2) { // obj1 <= obj2
	var ret = cloneObject(obj1);

	for (var k in obj2) {
		if (typeof obj2[k] === "object") ret[k] = cloneObject(obj2[k]);
		else ret[k] = obj2[k];
	}

	return ret;
}
function cloneObject(obj) {
	var ret = {};

	for (var k in obj) {
		if (typeof obj[k] === "object") ret[k] = cloneObject(obj[k]);
		else ret[k] = obj[k];
	}

	return ret;
}
function make_array() {
	var ary = [];
	if (typeof arguments[0] === "object") ary = arguments[0];
	else 
		for (var i = 0; i < arguments.length; ++i)
			ary[i] = arguments[i];


	var sz = ary[0];
	var ret = new Array(sz);

	ary.splice(0, 1);
	if (!ary.length) return ret;

	for (var i = 0; i < sz; ++i) {
		ret[i] = arguments.callee(ary);
	}

	return ret;
}
function make_rgb(r, g, b) {
	return "rgb(" + (r | 0) + "," + (g | 0) + "," + (b | 0) + ")";
}
function createHttpRequest() {
	var xmlhttp = null;
	if (window.ActiveXObject) {
		try {
			// MSXML2以降用
			xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) {
				try {
				// 旧MSXML用
				xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
			}
			catch (e2) {
			}
		}
	}
	else if (window.XMLHttpRequest) {
		// Win Mac Linux m1,f1,o8 Mac s1 Linux k3用
		xmlhttp = new XMLHttpRequest();
	}
	else {
	}
	if (xmlhttp == null) {
		alert("Can not create an XMLHTTPRequest instance");
	}
	return xmlhttp;
}
function preload(imgs){ // 画像を先に読み込み
	for (var i = 0; i < imgs.length; ++i) {
		var imgObj = new Image();
		imgObj.src = imgs[i];
	}
}
function cat(ary1, ary2) {
	var ret = [], k = 0;
	for (var i = 0; i < ary1.length; ++i, ++k) ret[k] = ary1[i];
	for (var i = 0; i < ary2.length; ++i, ++k) ret[k] = ary2[i];
	return ret;
}

/*
// 戻る時にもonload
var is_back = false;
var is_onload = false, onload_func = function() { };
var observe_handlers = {};
var EventObserve = Event.observe;
Event.observe = function(element, eventName, handler, f) {
	if (element == window && eventName == "load") {
		if (f == null) {
			EventObserve(element, eventName, handler);
			window.onunload = function() { };
		}
		else {
			onload_func = handler;

			if (is_back) { onload_check(); }
			else EventObserve(element, eventName, function() { is_back = true; onload_func(); });
		}
	}
	else if (f) {
		observe_handlers[element + '_' + eventName] = handler;
		eval('EventObserve("' + element + '", "' + eventName + '", observe_handlers["' + element + '_' + eventName + '"])');
	}
	else EventObserve(element, eventName, handler);
}

function onload_check() {
	if (document.getElementById("last_tag") == null) setTimeout(onload_check, 500);
	else onload_func();
}
*/
Event._observe = Event.observe;
Event.observe = function(element, eventName, handler) {
	if( element == window && eventName == "unload") {
		this._unloadObserved = true;
	}
	if( element == window && eventName == "load" ) {
		if( !Prototype.Browser.Gecko ) {
			eventName = "load"
		}
		else eventName = "pageshow";

		if( Prototype.Browser.WebKit && !this._unloadObserved ) {
			Event._observe(window, "unload", function(){});
			this._unloadObserved = true;
		}
	}
	Event._observe(element, eventName, handler);
}
