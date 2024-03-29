3600 #!/usr/bin/perl -w
3601 
3602 # Generate vectors.S, the trap/interrupt entry points.
3603 # There has to be one entry point per interrupt number
3604 # since otherwise there's no way for trap() to discover
3605 # the interrupt number.
3606 
3607 print "# generated by vectors.pl - do not edit\n";
3608 print "# handlers\n";
3609 print ".globl alltraps\n";
3610 for(my $i = 0; $i < 256; $i++){
3611     print ".globl vector$i\n";
3612     print "vector$i:\n";
3613     if(!($i == 8 || ($i >= 10 && $i <= 14) || $i == 17)){
3614         print "  pushl \$0\n";
3615     }
3616     print "  pushl \$$i\n";
3617     print "  jmp alltraps\n";
3618 }
3619 
3620 print "\n# vector table\n";
3621 print ".data\n";
3622 print ".globl vectors\n";
3623 print "vectors:\n";
3624 for(my $i = 0; $i < 256; $i++){
3625     print "  .long vector$i\n";
3626 }
3627 
3628 # sample output:
3629 #   # handlers
3630 #   .globl alltraps
3631 #   .globl vector0
3632 #   vector0:
3633 #     pushl $0
3634 #     pushl $0
3635 #     jmp alltraps
3636 #   ...
3637 #
3638 #   # vector table
3639 #   .data
3640 #   .globl vectors
3641 #   vectors:
3642 #     .long vector0
3643 #     .long vector1
3644 #     .long vector2
3645 #   ...
3646 
3647 
3648 
3649 
