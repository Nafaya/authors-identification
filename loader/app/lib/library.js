const Promise = require('bluebird');
const _ = require('underscore');
const path = require('path');
const fs = require('fs-extra');

function Library(config){
    this.booksDirectory = config.booksDirectory;
    this.catalogDirectory = config.catalogDirectory;
    this.max_books_in_list = 1000;
    this.max_new_books_in_temp_list = 100;
    this.newBooksList = [];
    this.booksHash = {};
    this.lists = {};
}
Library.prototype.init = async function(){
    fs.ensureDirSync(this.booksDirectory);
    fs.ensureDirSync(this.catalogDirectory);
    await this.forEach(async (bookData, { listId }) => {
        this.booksHash[bookData.id] = listId;
        this.lists[listId] = (this.lists[listId]||0)+1;
    });
};
Library.prototype.forEach = async function(func){
    let regexp = /^list_(\d+).json/;
    await Promise.each(fs.readdirSync(this.catalogDirectory), async (filename) => {
        let match = regexp.exec(filename);
        let filepath = path.join(this.catalogDirectory, filename);
        if(!match || !fs.statSync(filepath).isFile()) return;
        let listId = parseInt(match[1]);
        let list = JSON.parse(fs.readFileSync(filepath));
        await Promise.each(list,async (bookData) => {
            await func(bookData,{ listId });
        });
    });
};
Library.prototype.saveNewList = async function(){
    console.log(this.newBooksList.length+' new books in library.') ;
    console.log(_.keys(this.booksHash).length+' unique ids') ;

    while(this.newBooksList.length){
        let { listId } = await this.getListWithSpace();
        let list = [];
        let filepath = path.join(this.catalogDirectory, `list_${listId}.json`);
        if(fs.existsSync(filepath)){
            list = JSON.parse(fs.readFileSync(filepath));
        }
        let amount = this.max_books_in_list - list.length ;
        if(amount<=0) throw new Error('Strange list.length>=this.max_books_in_list.');
        let tempList = this.newBooksList.splice(0, amount);
        amount = tempList.length;
        fs.writeFileSync(filepath, JSON.stringify(list.concat(tempList), null, 4));
        this.lists[listId] = (this.lists[listId] || 0) + amount;
        console.log(amount+' new books in library.') ;

    }

    console.log('List saved.') ;
};
Library.prototype.getListWithSpace = async function(){
    for(let listId in this.lists){
        if(!this.lists.hasOwnProperty(listId)) continue;
        if(this.lists[listId]<this.max_books_in_list) return { listId, amount: this.max_books_in_list - this.lists[listId] };
    }
    let listId = 1;
    while(this.lists[listId])listId++;
    return { listId, amount: this.max_books_in_list };
};

Library.prototype.saveBook = async function(bookData){
    const { id, text } = bookData;
    if(this.booksHash[id]) {
        console.log(`book ${bookData.id} already exists.`) ;
        return bookData;
    }
    const bookPath = path.join(this.booksDirectory, id);
    fs.ensureDirSync(bookPath);
    fs.writeFileSync(path.join(bookPath,`book.txt`), text);

    bookData = _.omit(bookData,'text');
    fs.writeFileSync(path.join(bookPath,`bookData.json`), JSON.stringify(bookData, null, 4));
    this.newBooksList.push(bookData);
    this.booksHash[bookData.id] = 'new';

    if(this.newBooksList.length>=this.max_new_books_in_temp_list){
        await this.saveNewList();
    }

    return bookData;
};
Library.prototype.hasBook = function(id){
    return !!this.booksHash[id];
};
Library.prototype.getBook = async function(id){
    //if(!this.hasBook(id)) throw new Error(`Cannot get book with id=${id}`);
    let dirpath = path.join(this.booksDirectory, id);
    if(!fs.statSync(dirpath).isDirectory()) return;
    let textfilename = path.join(dirpath, 'book.txt');
    let bookDatafilename = path.join(dirpath, 'bookData.json');
    if(!fs.statSync(textfilename).isFile() || !fs.statSync(bookDatafilename).isFile()) throw new Error('Bad book dir.');
    let text = fs.readFileSync(textfilename);
    let bookData = JSON.parse(fs.readFileSync(bookDatafilename));
    return {
        text,
        ...bookData
    }
};
Library.prototype.aggregate = async function(){
    await Promise.each(fs.readdirSync(this.booksDirectory), async (dirname) => {
        //console.log('dirname = '+dirname) ;
        await this.saveBook(await this.getBook(dirname));
        //await Promise.delay(1000);
        //console.log('dirname = '+dirname+' ok') ;
    });
};

module.exports = Library ;
