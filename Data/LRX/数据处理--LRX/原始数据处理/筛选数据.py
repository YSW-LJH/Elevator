filename= 'D:\学习\电梯项目\原始数据：1-2-20-32-33.txt'
origin = []
originData = []
with open(filename,'r') as fp:
    for line in fp:
        # print(line)
        origin.append(line)
# print((origin))


for i in origin:
    originData.append(dict(ID=i[57:60],Data=i[95:120],times=1))

del originData[0]
# print(originData)
with open('first_choice','w') as fp:
    for line in originData:
        fp.write(line['ID'] +'      '+line['Data'],)

# lines = [line.rstrip().ljust(maxLength) + '#' + str(index) + '\n'
#          for index, line in enumerate(lines)]

filterData = []
for item in originData:
    for i in filterData:
        if(item['Data']==i['Data'] and item['ID']==i['ID']):
            i['times'] = i['times']+1
    else:
        filterData.append(item)
# print(filterData)


lines=[line['ID']+'      '+line['Data'].rstrip()+'      '+str(line['times'])+'\n'
       for line in filterData]
with open('colletion','w') as fp:
    for line in lines:
        fp.write(line)

