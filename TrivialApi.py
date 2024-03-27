import requests
import json
import sqlite3
URL = 'https://opentdb.com/api.php?amount=10'

con = sqlite3.connect("Trivia\Trivia\MyDB1.1.sqlite")
cur = con.cursor()

req = requests.get('https://opentdb.com/api.php?amount=10&type=multiple')
dic = json.loads(req.text)
res = dic['results']
for question in res:
    sqlState = 'INSERT INTO QUESTIONS (question,correct,wrong1,wrong2,wrong3) VALUES ("' + question['question'] + '","' + question['correct_answer'] + '","' + question['incorrect_answers'][0] + '","' + question['incorrect_answers'][1] + '","' + question['incorrect_answers'][2] + '");'
    sqlState = sqlState.replace('&#039;',"").replace('&amp;',"&").replace('&quot;','')
    cur.execute(sqlState)
    con.commit()
    print(sqlState)