; (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
;               All Right Reserved
;               SUNEXT CONFIDENTIAL

        AREA    Heap, DATA, NOINIT

        EXPORT bottom_of_heap

; Create dummy variable used to locate bottom of heap

bottom_of_heap    SPACE   1

        END
