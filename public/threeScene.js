let cone;
const canvas = document.getElementById('canvas')
const renderer = new THREE.WebGLRenderer({ canvas: canvas, antialias: true });
const canvasContainer = document.getElementById('canvas_container');
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, canvasContainer.clientWidth / canvasContainer.clientHeight, 0.1, 1000);
renderer.setSize(canvasContainer.clientWidth, canvasContainer.clientHeight);
renderer.setClearColor(0xe6e6e6);
renderer.render( scene, camera );
camera.position.z = 5;

window.addEventListener('resize', function() {
    camera.aspect = canvasContainer.clientWidth / canvasContainer.clientHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(canvasContainer.clientWidth, canvasContainer.clientHeight);
});

function cleanScene(scene) {
    scene.children.forEach((object) => {
        scene.remove(object);
        if (object.geometry) object.geometry.dispose();
        if (object.material) {
            if (Array.isArray(object.material)) {
                object.material.forEach((material) => {
                    material.dispose();
                });
            } else {
                object.material.dispose();
            }
        }
    });
    scene.children.length = 0;
}

function setMesh (cone) {
    cleanScene(scene);
    const geometry = new THREE.BufferGeometry();
    geometry.computeVertexNormals();
    const material = new THREE.MeshBasicMaterial({ color: 0x888888 });

    const vertices = cone.vertices;
    const indices = cone.indices;

    const verticesArray = new Float32Array(vertices);
    const indicesArray = new Uint32Array(indices);

    const positionAttribute = new THREE.BufferAttribute(verticesArray, 3);
    geometry.setIndex( indices );
    geometry.setAttribute( 'position', positionAttribute );

    cone = new THREE.Mesh(geometry, material);
    cone.name = 'cone'
    scene.add(cone);
    animate(cone);
    renderer.render( scene, camera );
}

function animate(cone) {
    if(scene.getObjectByName('cone')) {
        requestAnimationFrame(() => animate(cone));
    }
    cone.rotation.x += 0.01;
    cone.rotation.y += 0.01;
    cone.rotation.z += 0.01;
    renderer.render( scene, camera );
}