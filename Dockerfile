FROM mhart/alpine-node:8

COPY ants /usr/src/ants
COPY judge /usr/src/judge

WORKDIR /usr/src/judge

#Extra stuff
RUN apk add --no-cache python openjdk7-jre git

RUN npm install && npm run build

#Bug
RUN mkdir game_logs

#Listen here
EXPOSE 3000

#Start
CMD [ "npm", "start" ]

