       SUBROUTINE FUNCT(N,X,FX)
       INTEGER N
       DOUBLE PRECISION X(N), FX
       LOGICAL pointExist
       CHARACTER (LEN=7) :: projectName

C      deleta o ponto
       INQUIRE(FILE='coreHFparameterizationInput.txt',
     &         EXIST=pointExist)
       IF(pointExist) THEN
             OPEN(11, FILE='coreHFparameterizationInput.txt')

             READ(11,*) projectName

             CLOSE(11, status='delete')
       ENDIF

C      escreve de novo
       open(unit=12,file='coreHFparameterizationInput.txt',status='new')
       write(12,*) projectName
       write(12,*) "/home/fred/gamess/rungms"
       write(12,*) "/home/fred/scr/"
       write(12,*) X(1)
       do i=2,N
              write(12,*) X(i)
       enddo
       close(12)

       call system("./CoreHFparam.exe")

       open(unit=13,file='fitness.txt',status='old')
       read(13,*) FX
       close(13)

       ! WRITE ALL POINTS AT RESTART FILE
        OPEN(UNIT=40,FILE='restart.simp',STATUS='old',ACCESS = 'append')
        WRITE(40,*) FX
        DO i=1,N
           WRITE(40,*) X(i)
        ENDDO
        CLOSE(40)
        ! WROTE

       RETURN
       END
