import sqlite3
import pathlib
import os.path
import textwrap
import random
import shutil
from dictionary.box import Box

WIDTH = shutil.get_terminal_size().columns

class Dictionary:
    def __init__(self, word: str, no_ansi: bool):
        self.word = word
        self.db = os.path.join(str(pathlib.Path.home()), 
                          '.local/share/def/dictionary.db')
        self.con = sqlite3.connect(self.db)
        self.cur = self.con.cursor()
        self.no_ansi = no_ansi


    def display_definitions(self) -> None:
        defs_and_pos = self.get_definitions()

        box = Box(self.word, defs_and_pos, self.no_ansi)
        box.draw()


    def find_similar_words(self) -> None:
        '''
        Print words in the database that contain the input string.
        This is used when a requested word is not found in the database.
        '''
        word_id_res = self.cur.execute(
            f"select id from words where word like '%{self.word}%'").fetchall()

        if len(word_id_res) == 0:
            exit()

        # display superset words
        word_ids = [i[0] for i in word_id_res]

        words = [self.cur.execute(
                f'select word from words where id = {word_id}').fetchone()[0]
                    for word_id in word_ids]
        words = [word.strip('"') for word in words]
        words = ', '.join(words)
        words = textwrap.wrap(words, width=WIDTH)

        print('\nSimilar:\n────────')
        for line in words:
            print(line)


    def get_definitions(self) -> list[tuple[str, str]]:
        '''
        Query database for definitions and parts of speech for a given word.

        E.g.

        word = happy
        returns
            [('enjoying or showing or marked by joy or pleasure', 'adjective'),
             ('marked by good fortune', 'adjective'), 
             ('eagerly disposed to act or to be of service', 'adjective'), 
             ('well expressed and to the point', 'adjective')]
        '''

        word_id_res = self.cur.execute(
            f"select id from words where word = '\"{self.word}\"'").fetchone()

        if word_id_res is None:
            print('Cannot find word in working database.')
            self.find_similar_words()
            exit()

        word_id, = word_id_res
        defs_and_pos = self.cur.execute(
            f"select definition, pos\
                from definitions where word_id = '{word_id}'").fetchall()

        return defs_and_pos


    def edit_definition(self, index: int):
        index -= 1

        word_id_res = self.cur.execute(
            f"select id from words where word = '\"{self.word}\"'").fetchone()

        if not word_id_res:
            print("Database does not contain this word") 
            exit()

        word_id, = word_id_res
        pos, definition = self.cur.execute(
            f"select pos, definition\
                from definitions where word_id = '{word_id}'\
                limit 1 offset {index}").fetchone()
        
        print(f'Old part of speech: {pos}')
        new_pos = input('New part of speech (Leave empty to keep): ')
        if new_pos == '': new_pos = pos

        print()

        print(f'Old definition: {definition}')
        new_definition = input('New definition (Leave empty to keep): ')
        if new_definition == '': new_definition = definition
            
        definition_id, = self.cur.execute(
            f"select id from definitions where word_id = '{word_id}'\
                limit 1 offset {index}").fetchone()

        self.cur.execute(
            f"update definitions set definition = '{new_definition}',\
                pos = '{new_pos}'\
                where id = {definition_id}")
        self.con.commit()


    def remove_definition(self, index: int):
        index -= 1

        word_id_res = self.cur.execute(
            f"select id from words where word = '\"{self.word}\"'").fetchone()
        if not word_id_res:
            print('Database does not contain this word') 
            exit()
        word_id, = word_id_res

        definition_id = self.cur.execute(
            f"select id from definitions where word_id = '{word_id}'\
                limit 1 offset {index}").fetchone()
        if not definition_id:
            print('There is no definition with this index')
            exit()
        definition_id, = definition_id
        self.cur.execute(
            f'delete from definitions where id = {definition_id}')

        # remove word if no definitions remain
        remaining_defs = self.cur.execute(
            f'select definition from definitions where word_id = {word_id}'
        ).fetchall()
        if not remaining_defs:
            self.cur.execute(f'delete from words where id = {word_id}')

        self.con.commit()


    def add_definition(self, addword: str, pos: str, definition: str):
        addword = addword.replace("'", "''")
        pos = pos.replace("'", "''")
        definition = definition.replace("'", "''")

        self.cur.execute(
            f"insert or ignore into words (word) values ('\"{addword}\"')")
        word_id, = self.cur.execute(
            f"select id from words where word = '\"{addword}\"'").fetchone()

        self.cur.execute(
            f"insert or ignore into definitions (word_id, definition, pos)\
                values ({word_id}, '{definition}', '{pos}')")
        self.con.commit()


    def get_num_words(self) -> int:
        return self.cur.execute(
            'select count(*) from words').fetchone()[0]


    def get_num_defs(self) -> int:
        return self.cur.execute(
            'select count(*) from definitions').fetchone()[0]


    def get_db_size(self) -> float:
        size = os.path.getsize(self.db)
        size = size / (1024 ** 2) 
        return size # MB


    def get_all_pos(self) -> str:
        pos = self.cur.execute(
                'select distinct pos from definitions').fetchall()
        pos = ' | '.join([i[0] for i in pos])
        return pos


    def get_stats(self):
        words = self.get_num_words()
        definitions = self.get_num_defs()
        size = self.get_db_size()

        print(f'Words:        {words}')
        print(f'Definitions:  {definitions}')
        print(f'Disk size:    {size:.2f} MB')
        print('─' * (WIDTH // 2 + 1))
        print('Parts of speech:')
        for line in textwrap.wrap(self.get_all_pos(),
            width=(WIDTH // 2)):
            if line[-2:] == ' |':
                line = line[:-2]
            print('   ', line)


    def get_random(self) -> str:
        ind = random.randint(0, self.get_num_words() - 1)        
        word = self.cur.execute(
            'select word from words limit 1 offset ?', (ind,)).fetchone()[0]
        return word[1:-1]


    def __enter__(self):
        return self

    
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.con.close()
