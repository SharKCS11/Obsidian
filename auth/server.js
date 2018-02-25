//var host = "127.0.0.1"; //localhost
//var host = "obsidian.com";
var port = 8080; //port

//dependancies
var fs = require("fs");
var express = require("express");
var bodyParser = require('body-parser');
var http = require("https");
var async = require('async');
var app = express();

var PersonGroupName = "Shang";
var PersonGroupID = "shang";

var oxford = require('project-oxford'),
    client = new oxford.Client('8582373e04fb48bebebfb930669f18ae', 'eastus');

var options = {
    inflate: true,
    limit: '10mb',
    type: 'application/octet-stream'
};

app.use(bodyParser.raw(options));

app.use(express.json({ limit: '50mb' }));
app.use(express.urlencoded({ limit: '50mb' }));


app.post('/api', function(req, res) {

    console.log(req.body);

    console.log(typeof(req.body));

    client.face.detect({
        data: req.body,
        returnFaceId: true
    }).then(
        function(response) {
            console.log("recieved face id");
            person_faceid = response[0].faceId;
            person_faceid = [person_faceid];
            console.log(response[0].faceId);
            client.face.identify(person_faceid, PersonGroupID).then(
                function(responseIdentify) {
                    console.log("Person Exists");
                    console.log(responseIdentify[0].candidates[0].personId);
                    client.face.person.addFace(PersonGroupID, responseIdentify[0].candidates[0].personId, {
                        data: req.body
                    }).then(function(resAddPerson) {
                        client.face.personGroup.trainingStart(PersonGroupID).
                        then(function(resTrain) {
                            console.log("Training Done");
                        });
                    });
                    res.send();
                },
                function(identifyReject) {
                    console.log(identifyReject);
                    res.status(500).send('Something broke!')
                });
        },
        function(reason) {
            console.log(
                reason
            );
        });
});

app.post('/new', function(req, res) {
    console.log(req.body);

    var buffer = req.body;

    client.face.detect({
        data: req.body,
        returnFaceId: true
    }).then(function(resFaceID) {
        console.log("ID finished");
        console.log(req.headers.name);
        console.log(req.body);
        client.face.person.create(PersonGroupID, req.headers.name)
            .then(function(resCreate) {
                    console.log(resCreate.personId);
                    console.log(buffer);
                    client.face.person.addFace(PersonGroupID, resCreate.personId, { data: buffer })
                        .then(function(resAdd) {
                                client.face.personGroup.trainingStart(PersonGroupID)
                                    .then(function(restTraining) {
                                        console.log("New Training Ended!");
                                    })
                            },
                            function(reasons) {
                                console.log(reasons)
                            });
                },
                function(reason) {
                    console.log(reason);
                });
    });
    res.send();
});
app.listen(8080, function(){console.log("Server Started on port: " + port)});
