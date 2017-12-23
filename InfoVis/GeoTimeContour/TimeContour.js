

// 2D draw area, with margin for axis labels
var margin = { top: 20, right: 20, bottom: 20, left: 20 };

const TIME_DATA_WIDTH = 959;
const TIME_DATA_HEIGHT = 37;
const SPACE_DATA_WIDTH = 152;
const SPACE_DATA_HEIGHT = 137;

// related to threejs
var dataTexture;
var material;
var scene, camera;
var geometry, mesh;

var rendererSpace;
var rendererTime = [];

var sceneSpace;
var sceneTime = [];
var cameraSpace;
var cameraTime;

var dayIndex = [];
var dayBuffer = [];

var pDataTexture;
var nDataTexture;
var interpDataTexture;

function updateTime(index, year) {
    var filePath = "Year/" + ("" + year) + ".csv";

    d3.csv(filePath, function (error, data) {

        var textData = [];

        for (var i = 0; i < (TIME_DATA_WIDTH * TIME_DATA_HEIGHT); i++) {
            textData.push(+data[i].V);
        }

        dataTexture = new THREE.DataTexture(new Float32Array(textData), TIME_DATA_WIDTH, TIME_DATA_HEIGHT, THREE.LuminanceFormat, THREE.FloatType);
        dataTexture.wrapS = THREE.ClampToEdgeWrapping;
        dataTexture.wrapT = THREE.ClampToEdgeWrapping;
        dataTexture.magFilter = THREE.LinearFilter;
        dataTexture.minFilter = THREE.LinearFilter;
        dataTexture.repeat.set(1, 1);
        dataTexture.needsUpdate = true;

        uniform = {
            map: {
                value: dataTexture
            }
        };

        sceneTime[index].children[0].material.uniforms = uniform;
        sceneTime[index].children[0].material.needsUpdate = true;
        sceneTime[index].children[0].needsUpdate = true;
        rendererTime[index].render(sceneTime[index], cameraTime);
    });
}

function updateSpace(time) {
    interpDataTexture = time[2][1];

    var textData = [];

    for (var i = 0; i < (SPACE_DATA_WIDTH * SPACE_DATA_HEIGHT); i++) {
        textData.push(+dayBuffer[time[0][0]][dayIndex[time[0][0]][time[0][1]]][i]);
    }

    pDataTexture = new THREE.DataTexture(new Float32Array(textData), SPACE_DATA_WIDTH, SPACE_DATA_HEIGHT, THREE.LuminanceFormat, THREE.FloatType);
    pDataTexture.wrapS = THREE.ClampToEdgeWrapping;
    pDataTexture.wrapT = THREE.ClampToEdgeWrapping;
    pDataTexture.magFilter = THREE.LinearFilter;
    pDataTexture.minFilter = THREE.LinearFilter;
    pDataTexture.generateMipmaps = false;
    pDataTexture.repeat.set(1, 1);
    pDataTexture.needsUpdate = true;

    var textData = [];

    for (var i = 0; i < (SPACE_DATA_WIDTH * SPACE_DATA_HEIGHT); i++) {
        textData.push(+dayBuffer[time[1][0]][dayIndex[time[1][0]][time[1][1]]][i]);
    }

    nDataTexture = new THREE.DataTexture(new Float32Array(textData), SPACE_DATA_WIDTH, SPACE_DATA_HEIGHT, THREE.LuminanceFormat, THREE.FloatType);
    nDataTexture.wrapS = THREE.ClampToEdgeWrapping;
    nDataTexture.wrapT = THREE.ClampToEdgeWrapping;
    nDataTexture.generateMipmaps = false;
    nDataTexture.magFilter = THREE.LinearFilter;
    nDataTexture.minFilter = THREE.LinearFilter;
    nDataTexture.repeat.set(1, 1);
    nDataTexture.needsUpdate = true;

    uniform = {
        pMap: {
            value: pDataTexture
        },
        nMap: {
            value: nDataTexture
        },
        d: {
            value: interpDataTexture
        }
    };

    var material = new THREE.ShaderMaterial({
        vertexShader: document.getElementById('vertexShader').textContent,
        fragmentShader: document.getElementById('fragmentShaderSpace').textContent
    });
    material.uniforms = uniform;
    material.needsUpdate = true;

    sceneSpace.children[0].material = material;
    sceneSpace.children[0].needsUpdate = true;
    console.log("RENDER");
    rendererSpace.render(sceneSpace, cameraSpace);
}

function generateRenderer(index, year, width, height) {

    width = width - margin.left - margin.right;
    height = height - margin.top - margin.bottom;

    var geometry = new THREE.PlaneGeometry(width, height, 1, 1);
    var material;
    if (index >= 0) {
        material = new THREE.ShaderMaterial({
            vertexShader: document.getElementById('vertexShader').textContent,
            fragmentShader: document.getElementById('fragmentShaderTime').textContent
        });
    } else {
        material = new THREE.ShaderMaterial({
            vertexShader: document.getElementById('vertexShader').textContent,
            fragmentShader: document.getElementById('fragmentShaderTime').textContent
        });
    }

    // mesh
    var mesh = new THREE.Mesh(geometry, material);

    // scene
    var scene = new THREE.Scene();
    scene.add(mesh);

    // ortho camera
    // clipping plane distances: left, right, top, bottom, near, far
    var camera = new THREE.OrthographicCamera(-width / 2, width / 2, height / 2, -height / 2, 1, 1000);
    camera.position.z = 1000;

    var renderer = new THREE.WebGLRenderer();
    renderer.setSize(width, height);

    //renderer.domElement.style.position = "relative";
    //renderer.domElement.style.top = (margin.top) + "px";
    //renderer.domElement.style.left = (margin.left) + "px";
    renderer.domElement.style.zIndex = "1";

    if (index >= 0) {
        rendererTime[index] = renderer;
        sceneTime[index] = scene;
        cameraTime = camera;
    } else {
        rendererSpace = renderer;
        sceneSpace = scene;
        cameraSpace = camera;
    }
}

function init() {
    for (var i = 0; i < 3; i++) {
        generateRenderer(i, 2014 + i, $("#timeContourDiv" + i).width() * 1.22, 400);
        document.getElementById("timeContourDiv" + i).appendChild(rendererTime[i].domElement);
        rendererTime[i].domElement.id = "timeContourPlot" + i;
        document.getElementById("timeContourDiv" + i).addEventListener("mousemove", mousemove);
    }
    generateRenderer(-1, 2014, $("#spaceContourDiv").width() * 0.8, $("#spaceContourDiv").width() * 0.8);
    document.getElementById("spaceContourDiv").appendChild(rendererSpace.domElement);
    rendererSpace.domElement.id = "spaceContourPlot";
    document.getElementById("spaceContourDiv").addEventListener("mousemove", mousemove);

    d3.csv("Day/dayIndex.csv", function (error, data) {
        for (var i = 0; i < data.length; i++) {
            if (typeof dayIndex[+data[i].Y] == "undefined") {
                dayIndex[+data[i].Y] = [];
            }
            dayIndex[+data[i].Y].push(+data[i].D);
        }
        initYear(2014);
        initYear(2015);
        initYear(2016);
        updateTime(0, 2014);
        updateTime(1, 2015);
        updateTime(2, 2016);
        var time = findNextDate(2014, 125);
        updateSpace(time);
    });

}

function initYear(year) {
    if ((typeof dayBuffer[year] == "undefined") || (dayBuffer[year].length <= 0)) {
        dayBuffer[year] = [];
        for (var i = 0; i < dayIndex[year].length; i++) {
            (function (day) {
                d3.csv("Day/" + year + "_" + dayIndex[year][day] + ".csv", function (error, data) {
                    var textData = [];

                    for (var j = 0; j < (SPACE_DATA_WIDTH * SPACE_DATA_HEIGHT); j++) {
                        textData.push(+data[j].V);
                    }
                    dayBuffer[year][dayIndex[year][day]] = textData;
                });
            })(i);
        }
    }
}

function mousemove(e) {
    if (e.target.id.includes("ContourPlot")) {
        var x = (e.clientX - $("#" + e.target.id).position().left) / $("#" + e.target.id).width();
        x = Math.min(Math.max(x, 0.0), 1.0);
        var y = (e.clientY - $("#" + e.target.id).position().top) / $("#" + e.target.id).height();
        y = 1.0 - Math.min(Math.max(y, 0.0), 1.0);

        //console.log("x: " + x + " - y: " + y);

        if (e.target.id.includes("time")) {
            var index = +e.target.id.substring(e.target.id.length - 1);
            var time = findNextDate(2014 + index, x * 365);
            updateSpace(time);
        }
    }
}

function findNextDate(year, day) {
    var nDay;
    var pDay;
    var nYear = year;
    var pYear = year;
    for (nDay = 0; nDay < dayIndex[year].length && dayIndex[year][nDay] <= day; nDay++);
    pDay = nDay - 1;
    var d = (day - dayIndex[pYear][pDay]) / (dayIndex[nYear][nDay] - dayIndex[pYear][pDay]);
    if (nDay == dayIndex[year].length) {
        pDay = dayIndex[year].length - 1;
        nDay = 1;
        nYear = nYear + 1;
        d = (day - dayIndex[pYear][pDay]) / (366 - dayIndex[pYear][pDay]);
    } else if (nDay == 0) {
        pDay = dayIndex[year - 1].length - 1;
        pYear = pYear - 1;
        d = (day + 1) / (dayIndex[nYear][nDay] + 1);
    }
    console.log(pYear + " " + dayIndex[pYear][pDay] + " <" + (1 - d) + "> " + year + " " + Math.round(day) + " <" + d + "> " + nYear + " " + dayIndex[nYear][nDay]);
    var result = [];
    result[0] = [pYear, pDay];
    result[1] = [nYear, nDay];
    result[2] = [(1 - d), d];
    return result;
}

window.addEventListener("load", init, false);
