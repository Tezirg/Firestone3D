float toonDiffuse(
  vec3 lightDirection,
  vec3 surfaceNormal,
  int nSteps) {
  float fSteps = float(abs(nSteps));
  float angle = max(0.0, dot(lightDirection, surfaceNormal));
  return round(angle * fSteps) / fSteps;
}

#pragma glslify: export(toonDiffuse)