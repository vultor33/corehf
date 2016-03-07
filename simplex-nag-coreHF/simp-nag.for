        PROGRAM SIMPLEXNAG
        INTEGER           N, N1, I
        DOUBLE PRECISION, ALLOCATABLE :: X(:)
        DOUBLE PRECISION  FMIN
        LOGICAL           RESTART_EXIST
        
        ! Read input
        OPEN (UNIT=13, STATUS='OLD', FILE='point.txt')
        READ(13,*) N
        ALLOCATE(X(1:N))
        DO I=1,N
           READ(13,*) X(I)
        ENDDO
        CLOSE(13)
        ! Beware, check user defined funct.for
        
        ! Create all point output
        INQUIRE(FILE='restart.simp',EXIST=RESTART_EXIST)
        IF(RESTART_EXIST) THEN
              OPEN(38, FILE='restart.simp',form='unformatted')
              CLOSE(38, status='delete')
        ENDIF
        OPEN(UNIT=39,FILE='restart.simp',STATUS='new')
        CLOSE(39)
        ! Created

        N1 = N+1
        CALL GONAG(N,N1,X,FMIN)

        STOP
        END

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!       USER FUNCTION      !!!!!!!!!!!
        INCLUDE 'FUNCTION.FOR'
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        SUBROUTINE GONAG(N,N1,X,FMIN)
        INTEGER           IFAIL, MAXIT, N, N1
        DOUBLE PRECISION  EPS, FMIN
        DOUBLE PRECISION  P(N1,N), PBAR(N), PDSTAR(N), PSTAR(N), STEP(N)
        DOUBLE PRECISION  X(N), Y(N1), FXDUMMY

        IFAIL = 0 !ERROR
        MAXIT = 50000 !MAX ITERATIONS
        EPS = 2.0D0**(-56)

        CALL FUNCT(N,X,FXDUMMY)

        CALL E04CCF(N,X,FMIN,EPS,N1,PDSTAR,PSTAR,PBAR,STEP,Y,P,
     *                  FUNCT,MONIT,MAXIT,IFAIL)

        RETURN
        END
        
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!       NAG ROUTINE        !!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d
       SUBROUTINE E04CCF(N,X,FMIN,EPS,N1,PDSTAR,PSTAR,PBAR,STEP,Y,P,
     *                  FUNCT,MONIT,MAXIT,IFAIL)
C     MARK 1 RELEASE.  NAG COPYRIGHT 1971
C     MARK 3 REVISED.
C     MARK 4.5 REISSUE. LER-F7
C     MARK 8 REVISED. IER-220 (MAR 1980)
C     MARK 11.5(F77) REVISED. (SEPT 1985.)
C     MARK 13 REVISED. USE OF MARK 12 X02 FUNCTIONS (APR 1988).
C     MARK 14 REVISED. IER-716 (DEC 1989).
C     .. Parameters ..
      CHARACTER*6       SRNAME
      PARAMETER         (SRNAME='E04CCF')
C     .. Scalar Arguments ..
      DOUBLE PRECISION  EPS, FMIN
      INTEGER           IFAIL, MAXIT, N, N1

C     .. Array Arguments ..
      DOUBLE PRECISION  P(N1,N), PBAR(N), PDSTAR(N), PSTAR(N), STEP(N),
     *                  X(N), Y(N1)
C     .. Subroutine Arguments ..
      EXTERNAL          FUNCT, MONIT
C     .. Local Scalars ..
      DOUBLE PRECISION  A, B, C, CENT, COEFF, DERIV, DERIV2, F1, F2, F3,
     *                  FMAX, R, SERROR, X1, X2, X3, XMIN, YDSTAR,
     *                  YMEAN, YSTAR, F1FIX
      INTEGER           H, I, IV, J, K, L, LASTMX, MCOUNT, NCALL, NP1
C     .. Local Arrays ..
      CHARACTER*1       P01REC(1)
C     .. External Functions ..
      DOUBLE PRECISION  X02AJF
      INTEGER           P01ABF
      EXTERNAL          X02AJF, P01ABF
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, SIGN, SQRT, DBLE
C     .. Executable Statements ..
      FMAX = 100001.D0
      FMIN = 0.D0
      NCALL = 0
      R = 0
      IF (N.LT.1 .OR. EPS.LT.X02AJF()
     *    .OR. N1.NE.N+1 .OR. MAXIT.LT.1 .OR.
     *    (IFAIL.NE.1 .AND. IFAIL.NE.0)) GO TO 1220

      CALL FUNCT(N,X,F1FIX)
      NCALL = NCALL + 1
   20 DO 300 I = 1, N
         F1 = F1FIX
         X1 = 0.D0
         COEFF = 1.D0
         DO 80 J = 1, N
            IF (I-J) 60, 40, 60
   40       PSTAR(J) = X(J) + COEFF
            GO TO 80
   60       PSTAR(J) = X(J)
   80    CONTINUE
         CALL FUNCT(N,PSTAR,F2)
         NCALL = NCALL + 1
         X2 = 1.D0
         PSTAR(I) = PSTAR(I) + COEFF
         CALL FUNCT(N,PSTAR,F3)
         NCALL = NCALL + 1
         X3 = 2.D0
  100    IF (NCALL.GT.MAXIT) GO TO 1120
         DERIV = (X2-X3)*F1 + (X3-X1)*F2 + (X1-X2)*F3
         IF (ABS(DERIV)-X02AJF()) 140, 120, 120
  120    DERIV2 = DERIV/(X1-X2)/(X2-X3)/(X3-X1)
         IF (DERIV2) 240, 140, 140
  140    IF (F1-F3) 160, 200, 200
  160    IF (X1.LE.-5.0D0) GO TO 180
         F3 = F2
         X3 = X2
         F2 = F1
         X2 = X1
         X1 = X1 - COEFF
         PSTAR(I) = X(I) + X1
         CALL FUNCT(N,PSTAR,F1)
         NCALL = NCALL + 1
         GO TO 100
  180    XMIN = -5.0D0
         GO TO 280
  200    IF (X3.GE.5.0D0) GO TO 220
         F1 = F2
         X1 = X2
         F2 = F3
         X2 = X3
         X3 = X3 + COEFF
         PSTAR(I) = X(I) + X3
         CALL FUNCT(N,PSTAR,F3)
         NCALL = NCALL + 1
         GO TO 100
  220    XMIN = 5.0D0
         GO TO 280
  240    XMIN = .5D0*((X2**2-X3**2)*F1+(X3**2-X1**2)*F2+(X1**2-X2**2)
     *          *F3)/DERIV
         IF (XMIN.NE.0.0D0) GO TO 260
         XMIN = 0.1D0
         GO TO 280
  260    IF (ABS(XMIN).LT.0.1D0) XMIN = SIGN(0.1D0,XMIN)
         IF (ABS(XMIN).GT.5.0D0) XMIN = SIGN(5.0D0,XMIN)
  280    STEP(I) = XMIN
  300 CONTINUE
      NP1 = N + 1
      DO 380 I = 1, NP1
         DO 360 J = 1, N
            IF (I-J-1) 320, 340, 320
  320       PSTAR(J) = X(J)
            P(I,J) = X(J)
            GO TO 360
  340       PSTAR(J) = X(J) + STEP(J)
            P(I,J) = X(J) + STEP(J)
  360    CONTINUE
         NCALL = NCALL + 1
         CALL FUNCT(N,PSTAR,Y(I))
  380 CONTINUE
      A = 1.D0
      B = .5D0
      C = 2.D0
      LASTMX = 0
      MCOUNT = 0
      K = 0
  400 K = K + 1
      FMAX = Y(1)
      FMIN = Y(1)
      H = 1
      L = 1
      DO 480 I = 2, N1
         IF (Y(I)-FMAX) 440, 440, 420
  420    FMAX = Y(I)
         H = I
         GO TO 480
  440    IF (Y(I)-FMIN) 460, 480, 480
  460    FMIN = Y(I)
         L = I
  480 CONTINUE
      IF (LASTMX-H) 580, 500, 580
  500 MCOUNT = MCOUNT + 1
      IF (MCOUNT-5) 600, 520, 600
  520 IF (H-1) 560, 540, 560
  540 H = 2
      FMAX = Y(H)
      GO TO 600
  560 H = 1
      FMAX = Y(H)
      GO TO 600
  580 LASTMX = H
      MCOUNT = 0
  600 CONTINUE

! diogocomentou
!      CALL MONIT(FMIN,FMAX,P,N,N1,NCALL)

      IF (K.EQ.1) GO TO 620
      IF (SERROR-EPS) 1140, 620, 620
  620 IF (NCALL-MAXIT) 640, 1160, 1160
  640 DO 700 J = 1, N
         CENT = 0.D0
         DO 680 I = 1, N1
            IF (I-H) 660, 680, 660
  660       CENT = CENT + P(I,J)
  680    CONTINUE
         PBAR(J) = CENT/DBLE(N)
  700 CONTINUE
C     REFLECTION
      DO 720 I = 1, N
         PSTAR(I) = (1.D0+A)*PBAR(I) - A*P(H,I)
  720 CONTINUE
      CALL FUNCT(N,PSTAR,YSTAR)
      NCALL = NCALL + 1
      IF (YSTAR-FMIN) 740, 780, 780
C     EXPANSION
  740 DO 760 I = 1, N
         PDSTAR(I) = (1.D0+C)*PSTAR(I) - C*PBAR(I)
  760 CONTINUE
      CALL FUNCT(N,PDSTAR,YDSTAR)
      NCALL = NCALL + 1
      IF (YDSTAR-YSTAR) 980, 1020, 1020
C     CONTRACTION
  780 DO 820 I = 1, N
         IF (I-H) 800, 820, 800
  800    IF (YSTAR-Y(I)) 1020, 820, 820
  820 CONTINUE
      IF (FMAX-YSTAR) 880, 840, 840
  840 DO 860 I = 1, N
         P(H,I) = PSTAR(I)
  860 CONTINUE
  880 CONTINUE
      DO 900 I = 1, N
         PDSTAR(I) = B*P(H,I) + (1.D0-B)*PBAR(I)
  900 CONTINUE
      CALL FUNCT(N,PDSTAR,YDSTAR)
      NCALL = NCALL + 1
      IF (YDSTAR-FMAX) 980, 980, 920
  920 DO 960 I = 1, N1
         DO 940 J = 1, N
            PBAR(J) = (P(I,J)+P(L,J))*0.5D0
            P(I,J) = PBAR(J)
  940    CONTINUE
         CALL FUNCT(N,PBAR,Y(I))
         NCALL = NCALL + 1
  960 CONTINUE
      GO TO 1060
  980 DO 1000 J = 1, N
         P(H,J) = PDSTAR(J)
 1000 CONTINUE
      Y(H) = YDSTAR
      GO TO 1060
 1020 DO 1040 J = 1, N
         P(H,J) = PSTAR(J)
 1040 CONTINUE
      Y(H) = YSTAR
 1060 YMEAN = 0.D0
      SERROR = 0.D0
      DO 1080 I = 1, N1
         YMEAN = YMEAN + Y(I)
 1080 CONTINUE
      YMEAN = YMEAN/DBLE(N+1)
      DO 1100 I = 1, N1
         SERROR = SERROR + (Y(I)-YMEAN)**2
 1100 CONTINUE
      SERROR = SQRT(SERROR/DBLE(N+1))
      GO TO 400
 1120 IV = 2
      GO TO 1240
 1140 IV = 0
      GO TO 1180
 1160 IV = 2
 1180 DO 1200 I = 1, N
         X(I) = P(L,I)
 1200 CONTINUE
      GO TO 1240
 1220 IV = 1
 1240 IFAIL = P01ABF(IFAIL,IV,SRNAME,0,P01REC)

      RETURN
      END


      INTEGER FUNCTION P01ABF(IFAIL,IERROR,SRNAME,NREC,REC)
C     MARK 11.5(F77) RELEASE. NAG COPYRIGHT 1986.
C     MARK 13 REVISED. IER-621 (APR 1988).
C     MARK 13B REVISED. IER-668 (AUG 1988).
C
C     P01ABF is the error-handling routine for the NAG Library.
C
C     P01ABF either returns the value of IERROR through the routine
C     name (soft failure), or terminates execution of the program
C     (hard failure). Diagnostic messages may be output.
C
C     If IERROR = 0 (successful exit from the calling routine),
C     the value 0 is returned through the routine name, and no
C     message is output
C
C     If IERROR is non-zero (abnormal exit from the calling routine),
C     the action taken depends on the value of IFAIL.
C
C     IFAIL =  1: soft failure, silent exit (i.e. no messages are
C                 output)
C     IFAIL = -1: soft failure, noisy exit (i.e. messages are output)
C     IFAIL =-13: soft failure, noisy exit but standard messages from
C                 P01ABF are suppressed
C     IFAIL =  0: hard failure, noisy exit
C
C     For compatibility with certain routines included before Mark 12
C     P01ABF also allows an alternative specification of IFAIL in which
C     it is regarded as a decimal integer with least significant digits
C     cba. Then
C
C     a = 0: hard failure  a = 1: soft failure
C     b = 0: silent exit   b = 1: noisy exit
C
C     except that hard failure now always implies a noisy exit.
C
C     S.Hammarling, M.P.Hooper and J.J.du Croz, NAG Central Office.
C
C     .. Scalar Arguments ..
      INTEGER                 IERROR, IFAIL, NREC
      CHARACTER*(*)           SRNAME
C     .. Array Arguments ..
      CHARACTER*(*)           REC(*)
C     .. Local Scalars ..
      INTEGER                 I, NERR
      CHARACTER*72            MESS
C     .. External Subroutines ..
      EXTERNAL                P01ABZ, X04AAF, X04BAF
C     .. Intrinsic Functions ..
      INTRINSIC               ABS, MOD
C     .. Executable Statements ..
      IF (IERROR.NE.0) THEN
C        Abnormal exit from calling routine
         IF (IFAIL.EQ.-1 .OR. IFAIL.EQ.0 .OR. IFAIL.EQ.-13 .OR.
     *       (IFAIL.GT.0 .AND. MOD(IFAIL/10,10).NE.0)) THEN
C           Noisy exit
            CALL X04AAF(0,NERR)
            DO 20 I = 1, NREC
               CALL X04BAF(NERR,REC(I))
   20       CONTINUE
            IF (IFAIL.NE.-13) THEN
               WRITE (MESS,FMT=99999) SRNAME, IERROR
               CALL X04BAF(NERR,MESS)
               IF (ABS(MOD(IFAIL,10)).NE.1) THEN
C                 Hard failure
                  CALL X04BAF(NERR,
     *                     ' ** NAG hard failure - execution terminated'
     *                        )
                  CALL P01ABZ
               ELSE
C                 Soft failure
                  CALL X04BAF(NERR,
     *                        ' ** NAG soft failure - control returned')
               END IF
            END IF
         END IF
      END IF
      P01ABF = IERROR
      RETURN
C
99999 FORMAT (' ** ABNORMAL EXIT from NAG Library routine ',A,': IFAIL',
     *  ' =',I6)
      END


      SUBROUTINE P01ABZ
C     MARK 11.5(F77) RELEASE. NAG COPYRIGHT 1986.
C
C     Terminates execution when a hard failure occurs.
C
C     ******************** IMPLEMENTATION NOTE ********************
C     The following STOP statement may be replaced by a call to an
C     implementation-dependent routine to display a message and/or
C     to abort the program.
C     *************************************************************
C     .. Executable Statements ..
      WRITE(*,*) "PRESS ENTER TO EXIT"
      READ(*,*)
      STOP
      END   
      
      
      DOUBLE PRECISION FUNCTION X02AJF()
C     MARK 12 RELEASE. NAG COPYRIGHT 1986.
C
C     RETURNS  (1/2)*B**(1-P)  IF ROUNDS IS .TRUE.
C     RETURNS  B**(1-P)  OTHERWISE
C
C     .. Executable Statements ..
C     IN THEORY THIS SHOULD BE 2.0**(-56) BUT 2.0**(-55) HAS BEEN FOUND
C     TO BE MORE PRACTICAL IN THE PAST.
C     HOWEVER, FROM MARK 14 THE THEORETICAL VALUE WILL BE USED.

       X02AJF = 2.0D0**(-56)
c      X02AJF = 2.0D-40
      RETURN
      END


      SUBROUTINE X04AAF(I,NERR)
C     MARK 7 RELEASE. NAG COPYRIGHT 1978
C     MARK 7C REVISED IER-190 (MAY 1979)
C     MARK 11.5(F77) REVISED. (SEPT 1985.)
C     MARK 14 REVISED. IER-829 (DEC 1989).
C     IF I = 0, SETS NERR TO CURRENT ERROR MESSAGE UNIT NUMBER
C     (STORED IN NERR1).
C     IF I = 1, CHANGES CURRENT ERROR MESSAGE UNIT NUMBER TO
C     VALUE SPECIFIED BY NERR.
C
C     .. Scalar Arguments ..
      INTEGER           I, NERR
C     .. Local Scalars ..
      INTEGER           NERR1
C     .. Save statement ..
      SAVE              NERR1
C     .. Data statements ..
      DATA              NERR1/6/
C     .. Executable Statements ..
      IF (I.EQ.0) NERR = NERR1
      IF (I.EQ.1) NERR1 = NERR
      RETURN
      END


      SUBROUTINE X04BAF(NOUT,REC)
C     MARK 11.5(F77) RELEASE. NAG COPYRIGHT 1986.
C
C     X04BAF writes the contents of REC to the unit defined by NOUT.
C
C     Trailing blanks are not output, except that if REC is entirely
C     blank, a single blank character is output.
C     If NOUT.lt.0, i.e. if NOUT is not a valid Fortran unit identifier,
C     then no output occurs.
C
C     .. Scalar Arguments ..
      INTEGER           NOUT
      CHARACTER*(*)     REC
C     .. Local Scalars ..
      INTEGER           I
C     .. Intrinsic Functions ..
      INTRINSIC         LEN
C     .. Executable Statements ..
      IF (NOUT.GE.0) THEN
C        Remove trailing blanks
         DO 20 I = LEN(REC), 2, -1
            IF (REC(I:I).NE.' ') GO TO 40
   20    CONTINUE
C        Write record to external file
   40    WRITE (NOUT,FMT=99999) REC(1:I)
      END IF
      RETURN
C
99999 FORMAT (A)
      END


*   ******
