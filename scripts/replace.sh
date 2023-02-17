sed -i -r 's/[vt][0-9]+/reg0/g' myrtl.csv
sed -i -r 's/[f][0-9]+/regff0/g' myrtl.csv
sed -i -r 's/;;[a-zA-Z0-9 .]+//g' myrtl.csv

sed -i -r 's/[vt][0-9]+/reg0/g' sirrtl.csv
sed -i -r 's/[f][0-9]+/regff0/g' sirrtl.csv
sed -i -r 's/;;[a-zA-Z0-9 .]+//g' sirrtl.csv
