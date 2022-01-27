//import {BosClient} from '@baiducloud/sdk';
const BosClient = require('@baiducloud/sdk').BosClient;
const config = require('./config');
const u = require('underscore');
const H = require('./headers');
const bucket = 'yy-ai-train';
/*
const config = {
    endpoint: <EndPoint>,         //传入Bucket所在区域域名
    credentials: {
        ak: <AccessKeyID>,         //您的AccessKey
        sk: <SecretAccessKey>       //您的SecretAccessKey
    }
};
*/
const client = new BosClient(config);

function putObjectFromFile(object, filePath, options) {
    client.putObjectFromFile(bucket, object, filePath, options)
        .then(response => console.log(response))
        .catch(error => console.error(error));
}

function putObjectFromString(object, data, options) {
    client.putObjectFromString(bucket, object, data, options)
        .then(response => console.log(response))
        .catch(error => console.error(error));
}

function putObject(object, buf, options) {
    client.putObject(bucket, object, buf, options)
        .then(response => console.log(response))
        .catch(error => console.error(error));
}

function generatePresignedUrl(object) {
    let timestamp = 0;
    let expirationInSeconds = -1;
    return client.generatePresignedUrl(bucket, object, timestamp, expirationInSeconds)
}



module.exports = {
    putObjectFromFile,
    putObjectFromString,
    putObject,
    generatePresignedUrl,
}