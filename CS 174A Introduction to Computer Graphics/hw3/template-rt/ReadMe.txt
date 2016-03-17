======================================
    Raytracer Project by Chang Liu 
======================================
testAmbient 			PASSED
testBackground			PASSED
testBehind				PASSED
testDiffuse				PASSED
testIllum				ALMOST PASSED*
testImgPlane			ALMOST PASSED*
testIntersection		PASSED
testParsing				PASSED
testReflection			PASSED
testSample				PASSED
testShadow				PASSED
testSpecular			PASSED


* For both of these, the blue sphere in the front in my generated image is slightly larger than what the correct test images show. 

Originally, these spheres did not even show at all because in line 239 of my raytracer.cpp, I would check to make sure the hit times from the intersect function were greater than 1.0f because that's what the spec said. It seems like the spheres in these two tests were placed very close to the eye. When I changed it to just check if the hit time was greater than 0, then the blue spheres show up but slightly more enlarged and I have not been able to figure out why this is the case. 
