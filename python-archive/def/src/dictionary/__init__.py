import argparse
from dictionary.dictionary import Dictionary


def main():
    parser = argparse.ArgumentParser(description='Offline dictionary')
    parser.add_argument('word', nargs='?', help='Word to be defined') 
    parser.add_argument('-n', '--no-ansi', action='store_true', 
                        help="Don't make certain words bold")
    parser.add_argument('-s', '--stats', action='store_true', 
                        help='Show database statistics')
    parser.add_argument('-R', '--random', action='store_true', 
                        help='Return a random word')
    # this is kinda redundant
    parser.add_argument('-e', '--edit', type=int,
        help='Edit a definition specified by its index')
    parser.add_argument('-r', '--remove', type=int,
        help='Remove a definition specified by its index')
    parser.add_argument('-a', '--add', action='store_true',
        help='Add a new entry to the dictionary (must be used with -w -p -d)')
    parser.add_argument('-w', '--addword', type=str, help='Word to add')
    parser.add_argument('-p', '--pos', type=str, help='Part of speech to add')
    parser.add_argument('-d', '--definition', type=str, 
        help='Definition to add')
    args = parser.parse_args()

    if args.stats:
        Dictionary('', args.no_ansi).get_stats()
    elif args.random:
        print(Dictionary('', args.no_ansi).get_random())
    elif args.edit:
        with Dictionary(args.word, args.no_ansi) as dictionary:
            dictionary.edit_definition(args.edit)
    elif args.remove:
        with Dictionary(args.word, args.no_ansi) as dictionary:
            dictionary.remove_definition(args.remove)
    elif args.add and args.addword and args.pos and args.definition:
        with Dictionary('', args.no_ansi) as dictionary:
            dictionary.add_definition(args.addword, args.pos, args.definition)
    elif args.word is None:
        parser.print_help()
    else:
        with Dictionary(args.word, args.no_ansi) as dictionary:
            dictionary.display_definitions()
