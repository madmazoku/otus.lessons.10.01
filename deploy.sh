echo Deploy script executed for 0.0.$TRAVIS_BUILD_NUMBER
cp -r html/* ../docs/
git add -f ../docs
git commit -m "update docs 0.0.$TRAVIS_BUILD_NUMBER"
git push "https://madmazoku:$GITHUB_API_KEY@github.com/madmazoku/otus.lessons.10.01.git" HEAD:master
