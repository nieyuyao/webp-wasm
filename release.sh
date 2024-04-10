echo -e "registry=https://registry.npmjs.org/\n_authToken=$NPM_AUTH_TOKEN" > .npmrc


# get the version
NODE_VERSION=$(node -p -e "require('./package.json').version")

if [[ $NODE_VERSION =~ "beta" ]]; then
  # beta
  npm publish --tag beta
else
  npm publish 
fi