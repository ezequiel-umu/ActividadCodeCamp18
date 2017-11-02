FROM mhart/alpine-node:8

#Extra stuff
RUN apk add --no-cache python openjdk7-jre git

WORKDIR /usr/src/judge

COPY ants /usr/src/ants
COPY judge /usr/src/judge
COPY framework /usr/src/framework

RUN npm install && npm run build

#Listen here
EXPOSE 3000

#Start
CMD [ "npm", "start" ]

