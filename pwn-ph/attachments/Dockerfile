FROM php:8.1-apache

RUN apt-get update
RUN apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive

RUN ln -sf /usr/share/zoneinfo/Asia/Tokyo /etc/localtime
RUN apt-get install -y net-tools

ENV FLAG=flag{test_flag}

COPY ./php.ini /usr/local/etc/php
COPY ./vuln.so /usr/local/lib/php/extensions/no-debug-non-zts-20210902
COPY ./readflag /
COPY ./flag /flag
COPY ./start.sh /start.sh
COPY ./index.php /var/www/html


RUN chmod au+sx /readflag
RUN chmod au+sx /start.sh
RUN chmod -R 777 /var/www/html

CMD ["/start.sh"]
EXPOSE 80