import re
import string

class Kwic:
    def __init__(self, excluded, lines):
        self.excluded = excluded[:]
        self.index_lines = lines[:]
        temp=[]
        for phrase in self.index_lines:
            temp+= [word.lower() for word in phrase.split() if not(word in self.excluded or word.lower() in self.excluded)]  
        self.idx_words = list(set(sorted(temp)))
        self.idx_words.sort()
        self.kwic = []
        self.print_idx
        
    # Goes through each key word in context in alphabetical order
    # Prints any phrases containing the current key word in context
    def print_idx(self, phrases, idx_words):
        for idx_word in idx_words:
            for phrase in phrases:
                match = re.search(r'\b' + idx_word + r'\b', phrase.lower())
                match2 = re.search(r'\b' + idx_word + r'$', phrase.lower())
                match3 = re.match(idx_word + r'\b', phrase.lower())
                if match or match2 or match3:
                    self.print_phrase(phrase, idx_word)
                    
    # Capitalizes the key word in context in its sentence
    # Prints the phrase in accordance with the formatting specified in the assignment description    
    def print_phrase(self, phrase, idx_word):
        index = [word.lower() for word in phrase.split()].index(idx_word)
        new_phrase = [line.strip() for line in phrase.split()]
        new_phrase[index] = re.sub(r'(.*)', new_phrase[index].upper(), new_phrase[index])
        start = ''
        for word in reversed(new_phrase[:index]):
            if (len(word) + len(start)) < 20:
                start = re.sub(r'(.*)', word + r' \1', start)
            else:
                break
        end = ''
        for word in new_phrase[index+1:]:
            if (len(word) + len(end)) <= (30-len(idx_word)):
                end = re.sub(r'(.*)', r'\1 ' + word, end)
            else:
                break
        space = " "
        length = len(idx_word)
        if len(start) == 0:   
            str1 = ("{:>29}" .format(space*29)) 
            str2 = (new_phrase[index] + end[:31-length])
            self.kwic.append(str1 + str2)
        else:
            self.kwic.append(("{:>29}" .format(start)) + (new_phrase[index] + end[:31-length]))
            
    def output (self):
        self.print_idx(self.index_lines, self.idx_words)
        return self.kwic
