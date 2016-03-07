        PROGRAM SIMPLEXNAG
        INTEGER           N, N1, I, IOS
        DOUBLE PRECISION, ALLOCATABLE :: X(:), XTEMP(:)
        DOUBLE PRECISION  FMIN, FX, FTEMP
        LOGICAL           RESTART_EXIST
        
        ! Read input
        OPEN (UNIT=13, STATUS='OLD', FILE='point.txt')
        READ(13,*) N
        CLOSE(13)

        ALLOCATE(X(1:N))
        ALLOCATE(XTEMP(1:N))

        FX = 1.0e10
        
        OPEN(UNIT=14, STATUS='OLD', FILE='restart.simp')
        READ(14,*,IOSTAT=IOS) FTEMP
        write(*,*) 'estou aqui', IOS
        DO WHILE (IOS .EQ. 0)
           IF( FTEMP .LT. FX ) THEN
               FX = FTEMP
               DO I=1,N
                  READ(14,*,IOSTAT=IOS) X(I)
               ENDDO
           ELSE
               DO I=1,N
                  READ(14,*,IOSTAT=IOS) XTEMP(I)
               ENDDO
           ENDIF
           READ(14,*,IOSTAT=IOS) FTEMP
        ENDDO
        CLOSE(14)

        OPEN(UNIT=15, STATUS='UNKNOWN', FILE='bestpoint.txt')
        write(15,*) FX
        write(15,*)
        DO I=1,N
           write(15,*) X(I)
        ENDDO
        CLOSE(15)

        STOP
        END


