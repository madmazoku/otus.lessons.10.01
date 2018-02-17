echo ========= Build docker image
docker build -t vektor .
echo ========= Execute vektor
docker run --rm -i vektor vektor
echo ========= Remove docker image
docker rmi vektor
