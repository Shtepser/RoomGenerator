#include <yaml.h>
#include <cstring>
#include "Monster.hpp"


class MonstersParsingError
{
    char *message;

    public:
        
        MonstersParsingError(const char* msg)
        {
            message = strdup(msg);
        }    

        MonstersParsingError(const MonstersParsingError& other)
        {
            message = strdup(other.message);
        }

        ~MonstersParsingError()
        {
            delete[] message;
        }

        const char* GetMessage() const
        {
            return message;
        }

    private:

        char* strdup(const char* str)
        {
            char *result = new char[strlen(str) + 1];
            strcpy(result, str);
            return result;
        }
};


enum ParserState
{
    NOT_PARSING,

    PARSING_STARTED,
    PARSING_ENDED,

    STREAM_STARTED,
    STREAM_ENDED,

    DOCUMENT_STARTED,
    DOCUMENT_ENDED,

    READING_CONTENT_TYPE,

    READING_MONSTERS,
    READING_MONSTERS_ENDED,

    MONSTER_STARTED,
    MONSTER_IN_PROGRESS,
    MONSTER_ENDED,

    READING_NAME,
    READING_MINIMAL_COUNT,
    READING_MAXIMAL_COUNT,
    READING_AC,
    READING_HD,
    READING_HD_STARS,
    READING_HP,
    READING_HP_VALUE,
    READING_ATTACKS,
    READING_DAMAGE,
    READING_MOVEMENT_IN_TURN,
    READING_MOVEMENT_IN_ROUND,
    READING_SAVES,
    READING_MORALE,
};


class MonstersParser
{
    yaml_parser_t parser;
    yaml_event_t current_event;
    ParserState state;

    Monster *monsters, **current_monster;

    public:

        MonstersParser()
        {
            yaml_parser_initialize(&parser);
            state = NOT_PARSING;
            monsters = NULL;
            current_monster = &monsters;
        }


        void ParseStdin()
        {
            Parse(stdin);
        }

        void ParseFile(const char* path)
        {
            FILE* file = fopen(path, "r");
            if (file == NULL)
                throw MonstersParsingError("Cannot open file!");
            Parse(file);
            fclose(file);
        }

        ~MonstersParser()
        {
            yaml_parser_delete(&parser);
            yaml_event_delete(&current_event);
        }

    private:
        
        void Parse(FILE* file)
        {
            yaml_parser_set_input_file(&parser, file);
            state = PARSING_STARTED;
            
            while (state != PARSING_ENDED)
                ReadFromStream();

            state = NOT_PARSING;
        }

        void ReadFromStream()
        {
            yaml_parser_parse(&parser, &current_event);
            switch (current_event.type)
            {
                case YAML_STREAM_START_EVENT:
                case YAML_STREAM_END_EVENT:
                    ProcessStream();
                    break;
                case YAML_DOCUMENT_START_EVENT:
                case YAML_DOCUMENT_END_EVENT:
                    ProcessDocument();
                    break;
                case YAML_MAPPING_START_EVENT:
                case YAML_MAPPING_END_EVENT:
                    ProcessMapping();
                    break;
                case YAML_SEQUENCE_START_EVENT:
                case YAML_SEQUENCE_END_EVENT:
                    ProcessSequence();
                    break;
                case YAML_SCALAR_EVENT:
                    ProcessScalar();
                    break;
                default:
                    state = NOT_PARSING;
                    throw MonstersParsingError("Unexpected event");
            }
            yaml_event_delete(&current_event);
        }

        void ProcessStream()
        { 
            switch (state)
            {
                case PARSING_STARTED:
                    state = STREAM_STARTED;
                    break;
                case DOCUMENT_ENDED:
                    state = PARSING_ENDED;
                    break;
                default:
                    state = NOT_PARSING;
                    throw MonstersParsingError("Unexpected stream event");
            }
        }

        void ProcessDocument()
        {
            switch (state)
            {
                case STREAM_STARTED:
                    state = DOCUMENT_STARTED;
                    break;
                case READING_MONSTERS_ENDED:
                    state = DOCUMENT_ENDED;
                    break;
                default:
                    state = NOT_PARSING;
                    throw MonstersParsingError("Unexpected document event");
            }
        }

        void ProcessMapping()
        {
            switch (state)
            {
                case DOCUMENT_STARTED:
                    state = READING_CONTENT_TYPE;
                    break;
                case READING_MONSTERS:
                case READING_NAME:
                    switch (current_event.type)
                    {
                        case YAML_MAPPING_START_EVENT:
                            state = READING_NAME;
                            break;
                        case YAML_MAPPING_END_EVENT:
                            state = READING_MONSTERS_ENDED;
                            for (Monster *m = monsters; m != NULL; m = m->next)
                                m->print();
                            break;
                        default:
                            break;
                    }
                    break;
                case MONSTER_STARTED:
                    state = MONSTER_IN_PROGRESS;
                    break;
                case MONSTER_IN_PROGRESS:
                    state = READING_NAME;
                    break;
                default:
                    break;
            }

        }

        void ProcessSequence()
        {
            if (state != READING_HP && state != READING_HP_VALUE)
            {
                state = NOT_PARSING;
                throw MonstersParsingError("Unexpected sequence event");
            }
            if (current_event.type == YAML_SEQUENCE_END_EVENT)
                state = MONSTER_IN_PROGRESS;
            else
                state = READING_HP_VALUE;
        }

        void ProcessScalar()
        {
            char *value = (char*) current_event.data.scalar.value;
            switch (state)
            {
                case READING_CONTENT_TYPE:
                    if (strcmp(value, "monsters") == 0)
                        state = READING_MONSTERS;
                    else
                    {
                        state = NOT_PARSING;
                        throw MonstersParsingError("Unexpected scalar event");
                    }
                    break;
                case READING_NAME:
                    *current_monster = new Monster();
                    (*current_monster)->name = new char[strlen(value)];
                    strcpy((*current_monster)->name, value);
                    state = MONSTER_STARTED;
                    break;
                case MONSTER_IN_PROGRESS:
                    if (strcmp(value, "minimal-count") == 0)
                        state = READING_MINIMAL_COUNT;
                    else if (strcmp(value, "maximal-count") == 0)
                        state = READING_MAXIMAL_COUNT;
                    else if (strcmp(value, "AC") == 0)
                        state = READING_AC;
                    else if (strcmp(value, "HD") == 0)
                        state = READING_HD;
                    else if (strcmp(value, "HD-stars") == 0)
                        state = READING_HD_STARS;
                    else if (strcmp(value, "HP") == 0)
                        state = READING_HP;
                    else if (strcmp(value, "attacks") == 0)
                        state = READING_ATTACKS;
                    else if (strcmp(value, "damage") == 0)
                        state = READING_DAMAGE;
                    else if (strcmp(value, "movement-turn") == 0)
                        state = READING_MOVEMENT_IN_TURN;
                    else if (strcmp(value, "movement-round") == 0)
                        state = READING_MOVEMENT_IN_ROUND;
                    else if (strcmp(value, "saves-as") == 0)
                        state = READING_SAVES;
                    else if (strcmp(value, "morale") == 0)
                        state = READING_MORALE;
                    break;
                case READING_MINIMAL_COUNT:
                    (*current_monster)->minimal_count = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_MAXIMAL_COUNT:
                    (*current_monster)->maximal_count = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_AC:
                    (*current_monster)->armor_class = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_HD:
                    (*current_monster)->hit_dices = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_HD_STARS:
                    (*current_monster)->hit_dices_stars = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_ATTACKS:
                    (*current_monster)->attacks = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_DAMAGE:
                    (*current_monster)->damage = new char[strlen(value)];
                    strcpy((*current_monster)->damage, value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_MOVEMENT_IN_TURN:
                    (*current_monster)->movement_in_turn = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_MOVEMENT_IN_ROUND:
                    (*current_monster)->movement_in_round = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_SAVES:
                    (*current_monster)->saves_as = new char[strlen(value)];
                    strcpy((*current_monster)->saves_as, value);
                    state = MONSTER_IN_PROGRESS;
                    break;
                case READING_MORALE:
                    (*current_monster)->morale = atoi(value);
                    state = MONSTER_IN_PROGRESS;
                    current_monster = &((*current_monster)->next);
                    break;
                case READING_HP_VALUE:
                    HitPointsVariant **last;
                    last = &((*current_monster)->hit_points_variants);
                    while (*last != NULL)
                        last = &((*last)->next);
                    *last = new HitPointsVariant(atoi(value));
                    break;
                default:
                    break;
            }

        }

};


int main()
{
    MonstersParser mparser = MonstersParser();
    try
    {
        mparser.ParseFile("monsters.yaml");
    }
    catch (MonstersParsingError &err)
    {
        fprintf(stderr, "Parsing error: %s\n", err.GetMessage());
        return 1;
    }
    return 0;
}

