################################################################
### this is a manual to use leaderboard and submit functions ###
### by lingfei                                               ###
################################################################

To compile it in ubuntu

for leaderboard:

g++ -std=c++14 leaderboard.cpp -ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -g -I/usr/include/jsoncpp -o leaderboard

for submit:

g++ -std=c++14 submit.cpp -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client -o submit


#################################################################

Files needed:

key.json contains the correct labels and other things.

    size is the total number of labels,
    reduced_size is the number of labels that are used to calculate the ARI that are available to the students,
    number_of_clusters is the number of classes in the labels, for example k,
    max_submission_times is the maximum number of submissions the leaderboard will take,
    labels contains the correct label for each data, from numbers 0~k-1.

record.json contains the record of the newest submission for each group.

    ARI is the ARI that is calculated using all the data points,
    ARI_reduced is the ARI that is seen by the students, that are caculated using reduced_size of the data points,
    submission_times is the current submission times,
    vsID is the virtual ID for each group, note that the second and third digits from the left are the group number, all the other digits are just random characters.


####################################################################

To run leaderboard server:

put key.json, record.json, and leaderboard in the same folder.

run:

./leaderboard 8000 # 8000 is the port number, contact the admin to get access to a valid port


####################################################################

To submit:

save the result in a submission.json file, instruction is in the final project jupyter notebook file, then put it in the same folder as submit.

run:

./submit

####################################################################

