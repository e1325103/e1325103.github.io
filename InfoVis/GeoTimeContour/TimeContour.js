var rendererSpace;
var rendererTime = [];

// 2D draw area, with margin for axis labels
var margin = { top: 20, right: 20, bottom: 20, left: 20 };

// related to threejs
var dataTexture;
var material;
var scene, camera;
var geometry, mesh;

var done = false;
var dayIndex = [];

function generateRenderer(index, year, width, height) {

    width = width - margin.left - margin.right;
    height = height - margin.top - margin.bottom;

    // linear mapping of X random variable points of interest to draw area x axis
    var x = d3.scale.linear()
        .domain([0, 365])
        .range([0, width]);

    // linear mapping of Y random variable points of interest to draw area y axis
    var y = d3.scale.linear()
        .domain([0, 1])
        .range([height, 0]);

    // svg container for axes
    d3.select("svg").remove(); // clear existing svg container
    var svg = d3.select("#timeContourDiv") // return DOM object (related to html element) body
        .append("svg") // in body object create new svg object and return it
        .attr("width", width + margin.left + margin.right) // set svg attr and return itself
        .attr("height", height + margin.top + margin.bottom)
        .append("g") // in svg object create group of svg shapes and return it
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")"); // etc.

    // d3 axes
    var xAxis = d3.svg.axis()
        .scale(x) // scale axis to range x
        .orient("bottom");

    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left");

    // draw x axis
    // note on "call" function:
    // below xAxis is a function returned by axis().
    // <whatever>.call(xAxis) is same as xAxis(<whatever>),
    // i.e. run xAxis() function on <whatever>, here on the svg group g object.
    svg.append("g") // group of svg shapes
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .attr("fill", "#888")
        .call(xAxis)
        .append("text")
        .attr("class", "label")
        .attr("x", width)
        .attr("y", 42)
        .style("text-anchor", "end")
        .text("departure delay (mins)");

    // draw y axis
    svg.append("g")
        .attr("class", "y axis")
        .attr("fill", "#888")
        .call(yAxis)
        .append("text")
        .attr("class", "label")
        .attr("transform", "rotate(-90)")
        .attr("y", -60)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text("arrival delay (mins)");

    geometry = new THREE.PlaneGeometry(width, height, 1, 1);


    material = new THREE.ShaderMaterial({
        vertexShader: document.getElementById('vertexShader').textContent,
        fragmentShader: document.getElementById('fragmentShader').textContent
    });

    var filePath = "Year/" + ("" + year) + ".csv";
    var dataWidth = 959;
    var dataHeight = 37;

    if (index < 0) {
        filePath = "Day/2016_20.csv";
        dataWidth = 152;
        dataHeight = 137;
    }

    d3.csv(filePath, function (error, data) {
        var textData = [];

        for (var i = 0; i < (dataWidth * dataHeight); i++) {
            textData.push(+data[i].V);
        }

        dataTexture = new THREE.DataTexture(new Float32Array(textData), dataWidth, dataHeight, THREE.LuminanceFormat, THREE.FloatType);
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

        material.uniforms = uniform;
        material.needsUpdate = true;
        mesh.needsUpdate = true;
        if (index >= 0) {
            rendererTime[index].render(scene, camera);
        } else {
            rendererSpace.render(scene, camera);
        }
        console.log("Load Complete");
        done = true;
    });

    // mesh
    mesh = new THREE.Mesh(geometry, material);

    // scene
    scene = new THREE.Scene();
    scene.add(mesh);

    // ortho camera
    // clipping plane distances: left, right, top, bottom, near, far
    camera = new THREE.OrthographicCamera(-width / 2, width / 2, height / 2, -height / 2, 1, 1000);
    camera.position.z = 1000;

    var renderer = new THREE.WebGLRenderer();
    renderer.setSize(width, height);

    renderer.domElement.style.zIndex = "1";

    if (index >= 0) {
        rendererTime[index] = renderer;
    } else {
        rendererSpace = renderer;
    }
}

function init() {
    for (var i = 0; i < 3; i++) {
        generateRenderer(i, 2014 + i, $("#timeContourDiv" + i).width() * 1.22, 400);
        document.getElementById("timeContourDiv" + i).appendChild(rendererTime[i].domElement);
    }
    generateRenderer(-1, 2014, $("#spaceContourDiv").width() * 0.8, $("#spaceContourDiv").width() * 0.8);
    document.getElementById("spaceContourDiv").appendChild(rendererSpace.domElement);

    d3.csv("Day/dayIndex.csv", function (error, data) {
        for (var i = 0; i < data.length; i++) {
            if (typeof dayIndex[+data[i].Y] == "undefined") {
                dayIndex[+data[i].Y] = [];
            }
            dayIndex[+data[i].Y].push(+data[i].D);
        }
    });
}

function update(index) {
    d3.csv("Year/2015.csv", function (error, data) {
        var textData = [];

        for (var i = 0; i < (959 * 37); i++) {
            textData.push(+data[i].V);
        }

        dataTexture = new THREE.DataTexture(new Float32Array(textData), 959, 37, THREE.LuminanceFormat, THREE.FloatType);
        dataTexture.wrapS = THREE.ClampToEdgeWrapping;
        dataTexture.wrapT = THREE.ClampToEdgeWrapping;
        dataTexture.magFilter = THREE.LinearFilter;
        dataTexture.minFilter = THREE.LinearFilter;
        dataTexture.repeat.set(1, 1);
        dataTexture.needsUpdate = true;


        material = new THREE.ShaderMaterial({
            vertexShader: document.getElementById('vertexShader').textContent,
            fragmentShader: document.getElementById('fragmentShader').textContent
        });
        uniform = {
            map: {
                value: dataTexture
            }
        };
        material.uniforms = uniform;
        scene.children[0].material = material;
        scene.children[0].material.needsUpdate = true;
        scene.children[0].needsUpdate = true;
        scene.needsUpdate = true;
        rendererTime[index].render(scene, camera);
        console.log("UPDATE");
    });
}

window.addEventListener("load", init, false);
