#!/usr/bin/env node

const fs = require('fs')
let file = 'keep-it-green.txt'
var program = require('commander')
var promise = require('promise')
var spawn = require('child_process').spawn
var pkg = require('./package.json')
var CWD = process.cwd()

function addAll () {
  return run('git', [ 'add', '--all' ])
}
function push () {
  return run('git', [ 'push' ])
}

function commit () {
  return run('git', [
    'commit',
    '-m',
    '"[GIT AUTO COMMIT]: ' + new Date().toString() + '"'
  ])
}
function run (command, args) {
  return new Promise(function (resolve, reject) {
    var task = spawn(command, args, {
      cwd: CWD
    })
    task.on('close', function (code) {
      if (code !== 0)
        reject(new Error(command + ' process exited with code ' + code))
      else resolve()
    })
    task.stdout.pipe(process.stdout)
    task.stderr.pipe(process.stderr)
  })
}

fs.access(file, (err) => {
  console.log(`${file} ${err ? 'does not exist' : 'exists'}`)
  if (!err) {
    fs.writeFile(file, 'Hello World!', function (err) {
      if (err) return console.log(err)
      Promise.resolve()
        .then(addAll)
        .then(commit)
        .then(push)
        .then(function () {
          console.log(
            ('[GIT AUTO COMMIT]: Commit success at ' + new Date().toString())
              .green
          )
        })
        .catch(function (e) {
          console.log(('[GIT AUTO COMMIT]: ' + e.message).red)
        })
    })
  } else {
    let stream = fs.createWriteStream(file)
    stream.once('open', function (fd) {
      stream.write('stream wrote this\n')
      stream.end()
    })
  }
})
