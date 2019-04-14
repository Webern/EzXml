THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
EZXML_DIR="$THIS_DIR/.."
cd "$THIS_DIR"

docker build \
    -f "$THIS_DIR/Dockerfile" \
    -t matthewjamesbriggs/cmake:v001 \
    "$THIS_DIR"

docker rm -f lyretestcontainer || true;
docker rmi -f lyretestimage || true;

docker build \
    -f "$THIS_DIR/Dockerfile.test" \
    -t lyretestimage \
    "$EZXML_DIR"

docker run -it --name lyretestcontainer lyretestimage