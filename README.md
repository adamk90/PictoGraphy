# PictoGraphy

## Usage

To start backend, ensure that *mongodb* is running and you have up to date c++ compiler and std library (compatible with c++11).

From /backend folder:

```
npm install
node index.js
```

To start the frontend, ensure that backend is running (it will start without, but you wont be able to do anything).

From /frontend folder:

```
npm install
npm run dev
```

If you have your 3000 and 3001 ports open, then backend will by default run on localhost:3000 and frontend on localhost:3001 (but see starting messages for exact information).

SSL certification is self-signed, so you have to add it to chrome before you can see the pictures. We have created one certification for both server and backend (using this [guide](https://letsencrypt.org/docs/certificates-for-localhost/)), note that on production environment you should use separate certificates signed by CA authority for your domain.

To add the current self-signed certificate you should consult e.g. this answer on [stackoverflow](https://stackoverflow.com/questions/7580508/getting-chrome-to-accept-self-signed-localhost-certificate#answer-59302039).
