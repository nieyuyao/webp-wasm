echo -e "registry=https://registry.npmjs.org/\n_authToken=$NPM_AUTH_TOKEN" > .npmrc

npm publish --tag beta
