#!/bin/tcsh
echo "begin===="
set MID = `mid`
echo mid = $MID
cat licfile.sample_task2 | sed s/"9BD08696-595B1443-9431535B-5E5E297E"/$MID/ >  licfile.sample
createlic test/test_lic_task2.lic

cat licfile.sample_node | sed s/"9BD08696-595B1443-9431535B-5E5E297E"/$MID/ >  licfile.sample
createlic test/test_lic_node.lic

cat licfile.sample_user | sed s/"9BD08696-595B1443-9431535B-5E5E297E"/$MID/ >  licfile.sample
createlic test/test_lic_user.lic

cat licfile.sample_user10 | sed s/"9BD08696-595B1443-9431535B-5E5E297E"/$MID/ >  licfile.sample
createlic test/test_lic_user10.lic

head -n17 test/test_lic_task2.lic | sed s/"limit = 10"/"limit = 2"/ > test/test_borrowIn.lic
head -n17 test/test_lic_user10.lic | sed s/"limit = 10"/"limit = 2"/ > test/test_borrowInUser.lic
 

