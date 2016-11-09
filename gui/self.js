var seed;
/*
Встроенные функции генерации псевдослучайных чисел не дают управлять зерном энтропии.
Возьмём не сложную свою. Исходя из иррациональности числа пи (ни одно натуральное
число не будет ему целократно), мы всегда будем получать новое значение в диапазоне [0,1].
*/
function rand() {
    var x = Math.sin(seed++) * 10000;
    return x - Math.floor(x);
}
function irand(max) {
	return Math.floor(rand() * max);
}

//Доопределение типов 
var Point = function(x, y) { return { x : x, y : y }; }
var Line = function(a, b) {	return [a, b]; }
//Прямоугольник, ограничивающий операционное поле
var space = { width : 50, height : 30 };
space.s = space.width * space.height;
var xScale;
var yScale;

//тут про разметочку и про решетку на фон...
window.onresize = function() {
	d3.select("svg").remove();
	space.step = Math.min($(window).width() / space.width,
						  $(window).height() / space.height);
	var svg = d3.select("body").append("svg")
		.attr("width", space.step * space.width)
		.attr("height", space.step * space.height);
	svg.width = function() { return this.attr("width"); };
	svg.height = function() { return this.attr("height"); };
	svg.style("margin-left", (document.documentElement.clientWidth - svg.width()) / 2);
	svg.style("margin-top", (document.documentElement.clientHeight - svg.height()) / 2);
	xScale = d3.scaleLinear().domain([0, space.width]).range([0, +svg.width()]);
	yScale = d3.scaleLinear().domain([0, space.height]).range([+svg.height(), 0]);
	svg.append("g").attr("class", "greed")
		.selectAll("line").data(function() {
			var r = [];
			for (var i = 0; i <= space.width; ++i) {
				r.push(Line(Point(i, 0), Point(i, space.height)));
			}
			for (var i = 0; i <= space.height; ++i) {
				r.push(Line(Point(0, i), Point(space.width, i)));
			}
			return r;
		}).enter().append("line")
		.attr('x1', function(d) { return xScale(d[0].x); })
		.attr('x2', function(d) { return xScale(d[1].x); })
		.attr('y1', function(d) { return yScale(d[0].y); })
		.attr('y2', function(d) { return yScale(d[1].y); });
	update();
};

function update() {
	seed = Math.floor(+$("#seed").val());
	$("#seed").val(seed);

	d3.select(".set").remove();
	var result = [];
	var set = d3.select("svg").append("g").attr("class", "set");

	var density = $("#density").val();
	var step = 10 / Math.pow(density,0.3);
	var delta = 3 * Math.sqrt(density);
	/*
	Сгенерируем опорные точки для будущих полигонов. Для этого
	будем идти по полю с фиксированным шагом step, отступая от
	детермениованой точки Point((i + 0.5) * step , (i + 0.5) * step)
	на случайную величину, равномерно распределённую
	в промежутке [-delte : delta]
	*/
	for (var i = 0; i < space.width / step; ++i) {
		for (var j = 0; j < space.height / step; ++j) {
			result.push(Point(Math.floor((i + 0.5) * step  + irand(delta * 2) - delta), 
							  Math.floor((j + 0.5) * step + irand(delta * 2) - delta)));
		}
	}
	/*
	Теперь осталось сгенерировать сами полигоны. Для этого будем 
	отступать от опорной точки на случайную величину, заданную в 
	радиальных координатах.
	*/
	result.forEach(function(center) {
		var set = [];
		center.vertices = set;
		/*
		i будет выступать в качестве угла. Изначально угол равен
		нулю, на каждом шаге он увеличивается на случайное значение,
		равномерно распределённое от [Math.PI / 4 - 0.15, Math.PI / 4 + 0.15]
		*/
		for (var i = 0; i < Math.PI * 2; i += (Math.PI / 4) + (rand() - 0.5) * 0.3) {
			/*
			В качестве радиуса можно использовать величину,
			заведомо большую, чем 0. Радиус распределён в отрезке
			[sqrt(2), sqrt(2) + delta]. Это обеспечивает
			генерацию небольших полигонов так, чтобы для всех
			вариантов существовал путь из начала в конец.
			*/
			var radius = rand() * delta + Math.sqrt(2);
			//В самом конце всё это округлим до целых чисел, чтобы результат выглядел аккуратнее
			set.push(Point(Math.floor(center.x + Math.sin(i) * radius),
						   Math.floor(center.y + Math.cos(i) * radius)));
		}
	});
	//Теперь осталось выбрать начальную и конечную точки. Путь они на две клетки отстоят от углов
	result.start  = Point(2,2);
	result.finish = Point(space.width - 2, space.height - 2);
	var dest = function(a, b) {
		return Math.sqrt(Math.pow(a.x - b.x, 2) + Math.pow(a.y - b.y, 2));
	}
	for (var i = result.length; i;) {
		--i;
		if (dest(result[i], result.start ) < step + delta ||
			dest(result[i], result.finish) < step + delta) {
			result.splice(i, 1);
		}
	}
	//Пару строк, чтобы всё это отобразилось на экране
	set.selectAll("g").data(result).enter().append("g").append("polygon")
		.attr("points", function(a) { var result = ""; a.vertices.forEach(function(i) { result += xScale(i.x) + ',' + yScale(i.y) + ' '; }); return result; })
	set.append("circle").attr("cx", xScale(result.start.x)).attr("cy", yScale(result.start.y)).attr("r", step).style("fill", "#00AA00");
	set.append("circle").attr("cx", xScale(result.finish.x)).attr("cy", yScale(result.finish.y)).attr("r", step).style("fill", "#DD0000");
	//И вывалить в текстовом виде, чтобы было удобно забрать
	$("#result").html(JSON.stringify({ start : result.start, finish : result.finish, polygons: result }, null, ' '));
	upshow();
}

/*
Тут всё про построение результата... Для этого предоставляется возможность создавать 
в рабочей области точки и прямые. Приведён пример для варианта 1...
*/
function upshow() {
	context = d3.select("svg").select(".solve");
	context.remove();
	context = d3.select("svg").append("g").attr("class", "solve");
	var solution;
	try {
		if ($("#line").val()) {
			var solution = JSON.parse($("#line").val());
		} else {
			return;
		}
	}  catch (err) { alert(err); return; }
	function isLine(obj) {
		var k = Object.keys(obj);
		return k.indexOf("x1") >= 0 && k.indexOf("x2") >= 0 && k.indexOf("y1") >= 0 && k.indexOf("y2") >= 0;
	}
	function isPoint(obj) {
		var k = Object.keys(obj);
		return k.indexOf("cx") >= 0 && k.indexOf("cy") >= 0;
	}
	solution.forEach(function(itm, i){
		if (isLine(itm)) {
			context.append("line")
				.attr("x1", xScale(itm.x1))
				.attr("x2", xScale(itm.x2))
				.attr("y1", yScale(itm.y1))
				.attr("y2", yScale(itm.y2));
		}
		if (isPoint(itm)) {
			context.append("circle")
				.attr("cx", xScale(itm.cx))
				.attr("cy", yScale(itm.cy))
				.attr("r", 10);
		}
	});
}

window.onload = function() {
	$("body").append('<ul>\
			<li>Вариант: <input type="number" id="seed" min="1" max="50" value="1" onchange="$(' + "'#line'" + ').val(String()); update()" /></li>\
			<li>Плотность препятствий:<br/>\
				<select id="density" onchange="$(' + "'#line'" + ').val(String()); update()">\
					<option value="1">Разреженно</option>\
					<option value="2">Нормально</option>\
					<option value="3">Густо</option>\
				</select>\
			</li>\
			<li>Исходные данные:<br /><textarea id="result" /></li>\
			<li>Решение:<br /><textarea id="line" onchange="upshow()" /></li>\
		</ul>');
	$("#line").val(JSON.stringify([
 {
  "x1": 2,
  "y1": 2,
  "x2": 8,
  "y2": 4
 },
 {
  "cx": 8,
  "cy": 4
 },
 {
  "x1": 13,
  "y1": 9,
  "x2": 8,
  "y2": 4
 },
 {
  "cx": 13,
  "cy": 9
 },
 {
  "x1": 42,
  "y1": 10,
  "x2": 13,
  "y2": 9
 },
 {
  "cx": 42,
  "cy": 10
 },
 {
  "cx": 43,
  "cy": 20
 },
 {
  "x1": 42,
  "y1": 10,
  "x2": 43,
  "y2": 20
 },
 {
  "x1": 48,
  "y1": 28,
  "x2": 43,
  "y2": 20
 }
], null, ' '))
	window.onresize();
}