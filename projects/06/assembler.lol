OBTW
    I PROVIDE THIS MESSAGE TO ANY FUTURE SCHOLARS WISHING TO ADAPT OR USE THIS
    PROGRAM:
        1) THIS MUST BE RUN USING LCI ON THE FUTURE BRANCH 
            A) THE PROGRAM SHOULD WORK ON THIS COMMIT:
            https://github.com/justinmeza/lci/tree/327f001df0005ca4f48eb88c58b67c88add46dfd
        2) THE DOCUMENTATION IS INCREDIBLY POORLY MAINTAINED AND AS SUCH,
            FINDING METHOD NAMES FOR THINGS LIKE STDIO IS A PAIN
            A) I SPENT A SOLID HOUR LEARNING TO DO BASIC FILE LOADING AND LINE
            READING
            B) LOOK IN THE "binding.c" FILE TO FIND THE LIBRARIES' IMPLEMENTED
            METHODS
        3) GOOD LUCK. YOU WILL NEED IT.
        4) WHEN USING GTFO IN A LOOP WITHIN A FUNCTION, IT EXITS THE FUNCTION
TLDR


BTW THE VERSION NUMBER DOESN'T ACTUALLY DO ANYTHING
HAI 1.69


BTW LIBRARY IMPORTS
CAN HAS STDIO?
CAN HAS STRING?


BTW FUNCTIONS
OBTW
    OPENS A FILE WITH THE MODE OR PRINTS AN ERROR
    RETURNS FAIL ON ERROR AND THE FILE DESCRIPTOR ON SUCCESS
TLDR
HOW IZ I OPEN YR PATH AN YR MODE AN YR ERROR
    I HAS A FILE ITZ STDIO IZ OPEN YR PATH AN YR MODE MKAY
    STDIO IZ DIAF YR FILE MKAY, O RLY?
        YA RLY
            VISIBLE ERROR
            FOUND YR FAIL
    OIC
    FOUND YR FILE
IF U SAY SO

OBTW
    SPLITS THE EXTENSION FROM THE FILE, IF ANY
    ASSUMES ONLY SINGLE EXTENSION
    RETURNS BUKKIT CONTAINING "BASE" AND "EXT"
TLDR
HOW IZ I PRZPATH YR PATH
    I HAS A BASE ITZ ""
    I HAS A EXT ITZ ""
    I HAS A FNDBASE ITZ FAIL

    I HAS A STRLEN ITZ STRING IZ LEN YR PATH MKAY
    I HAS A REVIDX
    I HAS A CHAR
    BTW ITERATE BACKWARDS THROUGH STRING
    IM IN YR STRLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN STRLEN
        REVIDX R DIFF OF DIFF OF STRLEN AN IDX AN 1
        CHAR R STRING IZ AT YR PATH AN YR REVIDX MKAY
        BOTH SAEM FNDBASE AN WIN, O RLY?
            YA RLY
                BASE R SMOOSH CHAR AN BASE MKAY
            NO WAI
                EXT R SMOOSH CHAR AN EXT MKAY
        OIC
        BOTH SAEM CHAR AN ".", O RLY?
            YA RLY
                FNDBASE R WIN
        OIC
    IM OUTTA YR STRLOOP
    
    BTW WE HAVE NO EXTENSION, SWAP EXT AND BASE, ADD EXT
    BOTH SAEM BASE AN "", O RLY?
        YA RLY
            BASE R EXT
            EXT R ".asm"
    OIC

    DIFFRINT EXT AN ".asm", O RLY?
        YA RLY
            FOUND YR FAIL
    OIC

    O HAI IM REZ
        I HAS A BASE ITZ BASE
        I HAS A EXT ITZ EXT
    KTHX
    FOUND YR REZ
IF U SAY SO

OBTW
    RETURNS A BUKKIT CONTAINING TWO ITEMS (NAMEZ AND VALZ)
    THE FIRST ITEM IS A BUKKIT INDEXED BY "TBL_{IDX}", CONTAINING THE SYMBOL NAMES
    THE SECOND ITEM IS A BUKKIT INDEXED BY A SYMBOL NAME AND ITS VALUE
TLDR
HOW IZ I MKSYMTBL
    I HAS A NAMEZ ITZ A BUKKIT
    BTW WE MANUALLY SET THESE IN THE CASE THAT ITEMS GET REARANGED/ADDED TO NAMEZ
    I HAS A VALZ ITZ A BUKKIT

    BTW R0-R15
    I HAS A MXRREG ITZ 16
    IM IN YR REG UPPIN YR IDX TIL BOTH SAEM IDX MXRREG
        NAMEZ HAS A SRS SMOOSH "TBL_" AN IDX MKAY ITZ SMOOSH "R" AN IDX MKAY
        VALZ HAS A SRS SMOOSH "R" AN IDX MKAY ITZ IDX
    IM OUTTA YR REG
    NAMEZ HAS A TBL_16 ITZ "SP"
    VALZ HAS A SP ITZ 0
    NAMEZ HAS A TBL_17 ITZ "LCL"
    VALZ HAS A LCL ITZ 1
    NAMEZ HAS A TBL_18 ITZ "ARG"
    VALZ HAS A ARG ITZ 2
    NAMEZ HAS A TBL_19 ITZ "THIS"
    VALZ HAS A THIS ITZ 3
    NAMEZ HAS A TBL_20 ITZ "THAT"
    VALZ HAS A THAT ITZ 4
    NAMEZ HAS A TBL_21 ITZ "SCREEN"
    VALZ HAS A SCREEN ITZ 16384
    NAMEZ HAS A TBL_22 ITZ "KBD"
    VALZ HAS A KBD ITZ 24576
    NAMEZ HAS A MAX ITZ 22

    I HAS A SYMTBL ITZ A BUKKIT
    SYMTBL HAS A NAMEZ ITZ NAMEZ
    SYMTBL HAS A VALZ ITZ VALZ
    FOUND YR SYMTBL
IF U SAY SO

OBTW
    READS A LINE FROM A FILE. STRIPS COMMENTS AND NORMALIZES ENDINGS TO ":)"
    RETURNS THE LINE OF TEXT IF NOT EMPTY
    RETURNS FAIL IF EMPTY AND EOF
    RETURNS TRUE IF EMPTY AND EOL
    WE NEED TO READ 1 CHAR AT A TIME BECAUSE THERE IS NO BUILT IN METHOD TO
    READ A LINE AND I CBA TO PORT IT IN
TLDR
HOW IZ I LUKLINE YR FILE
    I HAS A LINE ITZ ""
    I HAS A LSTCHAR ITZ ""
    I HAS A CHAR
    I HAS A CMT ITZ FAIL
    BTW LOOPS UNTIL AN ENDLINE CHAR IS FOUND
    IM IN YR LUKCHAR
        CHAR R STDIO IZ LUK YR FILE AN YR 1 MKAY
        OBTW
            CHECK FOR EOF
        TLDR
        BOTH SAEM CHAR AN "", O RLY?
            YA RLY
                FOUND YR FAIL
        OIC

        BTW SEE IF WE HAVE A COMMENT
        BOTH SAEM SUM OF BOTH SAEM LSTCHAR AN "/" AN BOTH SAEM CHAR AN "/" AN 2, O RLY?
            YA RLY
                CMT R WIN
        OIC

        BTW WE ADD THE PREVIOUS CHAR TO PREVENT GETTING THE FIRST PART OF A COMMENT
        BOTH SAEM SUM OF SUM OF DIFFRINT LSTCHAR AN "" DIFFRINT LSTCHAR AN " " AN DIFFRINT LSTCHAR AN ":(000D)" 3, O RLY?
            YA RLY
                DIFFRINT CMT AN WIN, O RLY?
                    YA RLY
                        LINE R SMOOSH LINE AN LSTCHAR MKAY
                OIC
        OIC

        BTW CHECK FOR EOL
        BOTH SAEM CHAR AN ":)", O RLY?
            YA RLY
                BTW EMPTY LINE
                BOTH SAEM LINE AN "", O RLY?
                    YA RLY
                        FOUND YR WIN
                OIC
                FOUND YR LINE
        OIC
        LSTCHAR R CHAR
    IM OUTTA YR LUKCHAR
IF U SAY SO

OBTW
    TAKES A FILE AND RETURNS THE STRIPPED AND CLEANED LINEZ IN A BUKKIT
    THE BUKKIT CONTAINS "MAX" GIVING THE MAX IDX. "MAX" IS -1 IF EMPTY
    THE BUKKIT IS INDEXED BY "LINE_{IDX}"
    SEE LUKLINE FOR HOW LINEZ ARE RETURNED
TLDR
HOW IZ I CLNFILE YR FILE
    I HAS A LINEZCNT ITZ -1
    I HAS A LINEZ ITZ A BUKKIT

    I HAS A LINE
    IM IN YR CLCTLINE
    LINE R I IZ LUKLINE YR FILE MKAY
    BTW STOP AT EOF
    BOTH SAEM LINE AN FAIL, O RLY?
        YA RLY
            GTFO
    OIC

    BTW ONLY PRINT IF NOT EOL
    DIFFRINT LINE AN WIN, O RLY?
        YA RLY
            LINEZCNT R SUM OF LINEZCNT AN 1
            LINEZ HAS A SRS SMOOSH "LINE_" AN LINEZCNT MKAY ITZ LINE
    OIC
    IM OUTTA YR CLCTLINE
    LINEZ HAS A MAX ITZ LINEZCNT
    FOUND YR LINEZ
IF U SAY SO

OBTW
    RETURNS IF THE SYMBOL IS IN THE SYMBOL TABLE
TLDR
HOW IZ I HAZSYMBL YR SYMBL AN YR SYMTBL
    IM IN YR SYMBLLOOP UPPIN YR TBLIDX TIL BOTH SAEM TBLIDX AN SUM OF SYMTBL'Z NAMEZ'Z MAX AN 1 
        BOTH SAEM SYMBL AN SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY, O RLY?
            YA RLY
                FOUND YR WIN
        OIC
    IM OUTTA YR SYMBLLOOP
    FOUND YR FAIL
IF U SAY SO

OBTW
    FIRST PASS ON ALL LINEZ AND UPDATES THE SYMBOL TABLE
    OUTPUT OF CLNFILE SHOULD BE INPUT INTO THIS
    RETURNS THE UPDATED SYNTAX TABLE
TLDR
HOW IZ I UPDTSYMTBL YR LINEZ AN YR SYMTBL
    I HAS A LINE
    I HAS A PRZDINSTR
    I HAS A INSTIDX ITZ 0

    IM IN YR LINELOOP UPPIN YR LINEIDX TIL BOTH SAEM LINEIDX AN SUM OF LINEZ'Z MAX AN 1
        LINE R LINEZ'Z SRS SMOOSH "LINE_" AN LINEIDX MKAY
        PRZDINSTR R I IZ PRZINSTR YR LINE AN YR SYMTBL MKAY

        BTW A-INSTRUCTION
        PRZDINSTR'Z HAZSYMBL, O RLY?
            YA RLY
                PRZDINSTR'Z HAZLBL, O RLY?
                    YA RLY
                        I IZ HAZSYMBL YR PRZDINSTR'Z SYMBL AN YR SYMTBL MKAY, O RLY?
                            YA RLY
                                SYMTBL'Z VALZ'Z SRS PRZDINSTR'Z SYMBL R INSTIDX
                            NO WAI
                                SYMTBL'Z NAMEZ'Z MAX R SUM OF SYMTBL'Z NAMEZ'Z MAX AN 1
                                SYMTBL'Z NAMEZ HAS A SRS SMOOSH "TBL_" AN SYMTBL'Z NAMEZ'Z MAX MKAY ITZ PRZDINSTR'Z SYMBL
                                SYMTBL'Z VALZ HAS A SRS PRZDINSTR'Z SYMBL ITZ INSTIDX
                        OIC
                    NO WAI
                        DIFFRINT I IZ HAZSYMBL YR PRZDINSTR'Z SYMBL AN YR SYMTBL MKAY AN WIN, O RLY?
                            YA RLY
                                SYMTBL'Z NAMEZ'Z MAX R SUM OF SYMTBL'Z NAMEZ'Z MAX AN 1
                                SYMTBL'Z NAMEZ HAS A SRS SMOOSH "TBL_" AN SYMTBL'Z NAMEZ'Z MAX MKAY ITZ PRZDINSTR'Z SYMBL
                                BTW TEMP VAL, WE ASSIGN THE VALUE AT THE END IN CASE WE USE @{LABEL} BEFORE (LABEL)
                                SYMTBL'Z VALZ HAS A SRS PRZDINSTR'Z SYMBL ITZ -1
                        OIC
                        INSTIDX R SUM OF INSTIDX AN 1
                OIC
            NO WAI
                BTW C-INSTRUCTION
                PRZDINSTR'Z HAZCOMP, O RLY?
                    YA RLY
                        INSTIDX R SUM OF INSTIDX AN 1
                OIC
        OIC
    IM OUTTA YR LINELOOP

    I HAS A FRSTAVL ITZ 16
    BTW ASSIGN SYMBOLIC ADDREZEZ
    IM IN YR NAMELOOP UPPIN YR TBLIDX TIL BOTH SAEM TBLIDX AN SUM OF SYMTBL'Z NAMEZ'Z MAX AN 1
        BTW CHECK FOR UNINITIALIZED ADDREZEZ
        BOTH SAEM SYMTBL'Z VALZ'Z SRS SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY AN -1, O RLY?
            YA RLY
                I IZ NTCHARNUM YR STRING IZ AT YR SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY AN YR 0 MKAY MKAY, O RLY?
                    YA RLY
                        SYMTBL'Z VALZ'Z SRS SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY R FRSTAVL
                        FRSTAVL R SUM OF FRSTAVL AN 1
                    NO WAI
                        SYMTBL'Z VALZ'Z SRS SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY R SYMTBL'Z NAMEZ'Z SRS SMOOSH "TBL_" AN TBLIDX MKAY
                OIC
                
        OIC
    IM OUTTA YR NAMELOOP

    FOUND YR SYMTBL
IF U SAY SO

OBTW
    PARZEZ A SINGLE INSTRUCTION
    RETURNS A BUKKIT CONTAINING SYMBOL, DEST, COMP, AND JUMP
    ELEMENTS ARE "" IF N/A
TLDR
HOW IZ I PRZINSTR YR LINE AN YR SYMTBL
    O HAI IM PRZD
        I HAS A HAZSYMBL ITZ FAIL
        I HAS A HAZLBL ITZ FAIL
        I HAS A HAZCOMP ITZ FAIL

        I HAS A SYMBL ITZ ""
        I HAS A DEST ITZ ""
        I HAS A COMP ITZ ""
        I HAS A JUMP ITZ ""
    KTHX

    I HAS A CHAR
    CHAR R STRING IZ AT YR LINE AN YR 0 MKAY
    BTW LABEL, NO PARZING
    BOTH SAEM CHAR AN "(", O RLY?
        YA RLY
            IM IN YR CHARLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN STRING IZ LEN YR LINE MKAY
                CHAR R STRING IZ AT YR LINE AN YR SUM OF IDX AN 1 MKAY
                BOTH SAEM CHAR AN ")", O RLY?
                    YA RLY
                        GTFO
                OIC

                PRZD'Z SYMBL R SMOOSH PRZD'Z SYMBL AN CHAR MKAY
            IM OUTTA YR CHARLOOP
            PRZD'Z HAZSYMBL R WIN
            PRZD'Z HAZLBL R WIN
            FOUND YR PRZD
    OIC

    BTW A-INSTRUCTION
    BOTH SAEM CHAR AN "@", O RLY?
        YA RLY
            IM IN YR CHARLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN STRING IZ LEN YR LINE MKAY
                CHAR R STRING IZ AT YR LINE AN YR SUM OF IDX AN 1 MKAY
                BOTH SAEM CHAR AN ":)", O RLY?
                    YA RLY
                        GTFO
                OIC

                PRZD'Z SYMBL R SMOOSH PRZD'Z SYMBL AN CHAR MKAY
            IM OUTTA YR CHARLOOP
            PRZD'Z HAZSYMBL R WIN
            FOUND YR PRZD
    OIC

    I HAS A DEST ITZ FAIL
    I HAS A JUMP ITZ FAIL
    BTW C-INSTRUCTION
    IM IN YR CHARLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN STRING IZ LEN YR LINE MKAY
        CHAR R STRING IZ AT YR LINE AN YR IDX MKAY
        BOTH SAEM CHAR AN ":)", O RLY?
            YA RLY
                GTFO
        OIC

        BTW WE HAVE A DEST
        PRZD'Z HAZCOMP R WIN
        BOTH SAEM CHAR AN "=", O RLY?
            YA RLY
                DEST R WIN
                PRZD'Z DEST R PRZD'Z COMP
                PRZD'Z COMP R ""
            NO WAI
                BTW WE HAVE A JUMP
                BOTH SAEM CHAR AN ";", O RLY?
                    YA RLY
                        JUMP R WIN
                    NO WAI
                        JUMP, O RLY?
                            YA RLY
                                PRZD'Z JUMP R SMOOSH PRZD'Z JUMP AN CHAR MKAY
                            NO WAI
                                PRZD'Z COMP R SMOOSH PRZD'Z COMP AN CHAR MKAY
                        OIC
                OIC
        OIC
    IM OUTTA YR CHARLOOP
    FOUND YR PRZD
IF U SAY SO

OBTW
    RETURNS IF THE CHAR IS NUMERIC
TLDR
HOW IZ I NTCHARNUM YR CHAR
    FOUND YR BOTH SAEM SUM OF BOTH SAEM MAEK CHAR A NUMBR AN 0 AN DIFFRINT CHAR AN "0" 2
IF U SAY SO

OBTW
    CONVERTS A SYMBOLIC ADDRESS TO ADDRESS
TLDR
HOW IZ I SYM2VAL YR SYMBL AN YR SYMTBL
    I HAS A CHAR ITZ STRING IZ AT YR SYMBL AN YR 0 MKAY
    I IZ NTCHARNUM YR CHAR MKAY, O RLY?
        YA RLY
            SYMBL R SYMTBL'Z VALZ'Z SRS SYMBL
    OIC
    FOUND YR SYMBL
IF U SAY SO

OBTW
    CONVERTS AN UNSIGNED DECIMAL TO ITS 15-BIT REPRESENTATION
TLDR
HOW IZ I DEC2BIN YR DECIM
    I HAS A REMVAL ITZ MAEK DECIM A NUMBR
    I HAS A BNRY ITZ ""
    I HAS A NUM ITZ 32768

    IM IN YR BNRYLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN 15
        NUM R QUOSHUNT OF NUM AN 2
        
        BTW REMVAL <= NUM
        BOTH SAEM BIGGR OF REMVAL AN NUM AN REMVAL, O RLY?
            YA RLY
                REMVAL R DIFF OF REMVAL AN NUM
                BNRY R SMOOSH BNRY AN "1" MKAY
            NO WAI
                BNRY R SMOOSH BNRY AN "0" MKAY
        OIC
    IM OUTTA YR BNRYLOOP
    FOUND YR BNRY
IF U SAY SO

OBTW
    GENERATES THE OUTPUT BINARY INSTRUCTIONZ IN A BUKKIT
    THE BUKKIT HAS A VAR MAX INDICATING THE NUMBER OF INSTRUCTIONZ
    THE FILE IS NOT CLOZED
TLDR
HOW IZ I PRZASM YR LINEZ AN YR SYMTBL
    I HAS A BNRYINSTRS ITZ A BUKKIT

    I HAS A LINE
    I HAS A CHAR
    I HAS A PRZDINSTR
    I HAS A IDEN
    I HAS A INSTCNT ITZ 0
    IM IN YR LINELOOP UPPIN YR LINEIDX TIL BOTH SAEM LINEIDX AN SUM OF LINEZ'Z MAX AN 1
        LINE R LINEZ'Z SRS SMOOSH "LINE_" AN LINEIDX MKAY
        PRZDINSTR R I IZ PRZINSTR YR LINE AN YR SYMTBL MKAY
        IDEN R SMOOSH "INST_" AN INSTCNT MKAY

        BTW A-INSTRUCTION
        PRZDINSTR'Z HAZSYMBL, O RLY?
            YA RLY
                BTW LABELS DON'T GET ENCODED INTO BINARY
                PRZDINSTR'Z HAZLBL, O RLY?
                    YA RLY
                    NO WAI
                        BNRYINSTRS HAS A SRS IDEN ITZ SMOOSH "0" AN I IZ DEC2BIN YR I IZ SYM2VAL YR PRZDINSTR'Z SYMBL AN YR SYMTBL MKAY MKAY MKAY
                        INSTCNT R SUM OF INSTCNT AN 1
                OIC
            NO WAI
                BTW C-INSTRUCTION
                PRZDINSTR'Z HAZCOMP, O RLY?
                    YA RLY
                        BNRYINSTRS HAS A SRS IDEN ITZ "111"
                        
                        BTW THESE COULD PROB BE SPED UP IF USING DICTS, DON'T KNOW HOW TO NAME EASILY
                        BTW A
                        PRZDINSTR'Z COMP, WTF?
                            BTW BOTH SAEM A AN 0
                            OMG "0"
                            OMG "1"
                            OMG "-1"
                            OMG "D"
                            OMG "A"
                            OMG "!D"
                            OMG "!A"
                            OMG "-D"
                            OMG "-A"
                            OMG "D+1"
                            OMG "A+1"
                            OMG "D-1"
                            OMG "A-1"
                            OMG "D+A"
                            OMG "D-A"
                            OMG "A-D"
                            OMG "D&A"
                            OMG "D|A"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "0" MKAY
                                GTFO

                            BTW BOTH SAEM A AN 1
                            OMG "M"
                            OMG "!M"
                            OMG "-M"
                            OMG "M+1"
                            OMG "M-1"
                            OMG "D+M"
                            OMG "D-M"
                            OMG "M-D"
                            OMG "D&M"
                            OMG "D|M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "1" MKAY
                                GTFO
                        OIC
                        
                        BTW CCCCCC
                        PRZDINSTR'Z COMP, WTF?
                            OMG "0"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "101010" MKAY
                                GTFO
                            OMG "1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "111111" MKAY
                                GTFO
                            OMG "-1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "111010" MKAY
                                GTFO
                            OMG "D"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001100" MKAY
                                GTFO
                            OMG "A"
                            OMG "M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110000" MKAY
                                GTFO
                            OMG "!D"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001101" MKAY
                                GTFO
                            OMG "!A"
                            OMG "!M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110001" MKAY
                                GTFO
                            OMG "-D"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001111" MKAY
                                GTFO
                            OMG "-A"
                            OMG "-M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110011" MKAY
                                GTFO
                            OMG "D+1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "011111" MKAY
                                GTFO
                            OMG "A+1"
                            OMG "M+1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110111" MKAY
                                GTFO
                            OMG "D-1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001110" MKAY
                                GTFO
                            OMG "A-1"
                            OMG "M-1"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110010" MKAY
                                GTFO
                            OMG "D+A"
                            OMG "D+M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "000010" MKAY
                                GTFO
                            OMG "D-A"
                            OMG "D-M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "010011" MKAY
                                GTFO
                            OMG "A-D"
                            OMG "M-D"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "000111" MKAY
                                GTFO
                            OMG "D&A"
                            OMG "D&M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "000000" MKAY
                                GTFO
                            OMG "D|A"
                            OMG "D|M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "010101" MKAY
                                GTFO
                        OIC

                        BTW DDD
                        PRZDINSTR'Z DEST, WTF?
                            OMG ""
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "000" MKAY
                                GTFO
                            OMG "M"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001" MKAY
                                GTFO
                            OMG "D"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "010" MKAY
                                GTFO
                            OMG "DM"
                            OMG "MD"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "011" MKAY
                                GTFO
                            OMG "A"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "100" MKAY
                                GTFO
                            OMG "AM"
                            OMG "MA"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "101" MKAY
                                GTFO
                            OMG "AD"
                            OMG "DA"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110" MKAY
                                GTFO
                            OMG "ADM"
                            OMG "AMD"
                            OMG "DAM"
                            OMG "DMA"
                            OMG "MDA"
                            OMG "MAD"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "111" MKAY
                                GTFO
                        OIC

                        BTW JJJ
                        PRZDINSTR'Z JUMP, WTF?
                            OMG ""
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "000" MKAY
                                GTFO
                            OMG "JGT"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "001" MKAY
                                GTFO
                            OMG "JEQ"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "010" MKAY
                                GTFO
                            OMG "JGE"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "011" MKAY
                                GTFO
                            OMG "JLT"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "100" MKAY
                                GTFO
                            OMG "JNE"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "101" MKAY
                                GTFO
                            OMG "JLE"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "110" MKAY
                                GTFO
                            OMG "JMP"
                                BNRYINSTRS'Z SRS IDEN R SMOOSH BNRYINSTRS'Z SRS IDEN AN "111" MKAY
                                GTFO
                        OIC

                        INSTCNT R SUM OF INSTCNT AN 1
                OIC
        OIC
    IM OUTTA YR LINELOOP
    BNRYINSTRS HAS A MAX ITZ DIFF OF INSTCNT AN 1
    FOUND YR BNRYINSTRS
IF U SAY SO

OBTW
    TAKEZ THE OUTPUT OF PRZASM
    WRITES BINARY INSTRUCTIONZ TO FILE
TLDR
HOW IZ I SCRIBBELBINARY YR FILE AN YR BNRYINSTRS
    I HAS A STR ITZ ""
    IM IN YR BNRYLOOP UPPIN YR IDX TIL BOTH SAEM IDX AN SUM OF BNRYINSTRS'Z MAX AN 1
        BTW OUTPUT 50 LINEZ AT A TIME
        STR R SMOOSH STR AN BNRYINSTRS'Z SRS SMOOSH "INST_" AN IDX MKAY MKAY
        STR R SMOOSH STR AN ":)" MKAY
        STDIO IZ SCRIBBEL YR FILE AN YR STR MKAY
        STR R ""
    IM OUTTA YR BNRYLOOP
IF U SAY SO


BTW RUNNER CODE
I HAS A PATH
GIMMEH PATH

I HAS A REZ
REZ R I IZ PRZPATH YR PATH MKAY
BOTH SAEM REZ AN FAIL, O RLY?
    YA RLY
        VISIBLE SMOOSH "LMAO! FILE (" AN PATH AN ") MST B A ASM FILE LOL!" MKAY
        GTFO
OIC
PATH R SMOOSH REZ'Z BASE AN REZ'Z EXT MKAY

I HAS A FILE
FILE R I IZ OPEN YR PATH AN YR "r" AN YR SMOOSH "LMAO! FILE (" AN PATH AN ") DZNT EXZIZT LOL!" MKAY MKAY
BOTH SAEM FILE AN FAIL, O RLY?
    YA RLY
        GTFO
OIC

I HAS A SYMTBL
SYMTBL R I IZ MKSYMTBL MKAY

VISIBLE "REMOVING COMMENTS AND EXTRA WHITESPACE"
I HAS A LINEZ
LINEZ R I IZ CLNFILE YR FILE MKAY

VISIBLE "UPDATING SYMBOL TABLE"
SYMTBL R I IZ UPDTSYMTBL YR LINEZ AN YR SYMTBL MKAY

VISIBLE "GENERATING BINARY INSTRUCTIONS"
I HAS A BNRYINSTRS
BNRYINSTRS R I IZ PRZASM YR LINEZ AN YR SYMTBL MKAY

BTW WE FINISHED PROCESSING THE FILE
STDIO IZ CLOSE YR FILE MKAY

BTW OPEN THE FILE TO WRITE
PATH R SMOOSH REZ'Z BASE AN ".hack" MKAY
FILE R I IZ OPEN YR PATH AN YR "w" AN YR SMOOSH "LMAO! FILE (" AN PATH AN ") CNT B MADE LOL!" MKAY MKAY
BOTH SAEM FILE AN FAIL, O RLY?
    YA RLY
        GTFO
OIC

VISIBLE "WRITING BINARY INSTRUCTIONS TO FILE"
I IZ SCRIBBELBINARY YR FILE AN YR BNRYINSTRS MKAY

BTW WE FINISHED PROCESSING THE FILE
STDIO IZ CLOSE YR FILE MKAY

VISIBLE "FINISHED PROCESSING"

KTHXBYE