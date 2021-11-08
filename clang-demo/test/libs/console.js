extend(console, 'log', (...args) => {
  // document.writeln(args.join(' '));
  const fragment = document.createDocumentFragment();

  args.forEach(arg => {
    const p = document.createElement('p');
    p.innerText = arg.toString();
    fragment.append(p);
  });

  document.body.append(fragment);
});

extend(console, 'table', obj => {
  const trs = Object.entries(obj)
    .map(
      ([key, value]) => `
  <tr>
    <td>${key}</td>
    <td>${value}</td>
  </tr>`,
    )
    .join('');

  const table = html(`
    <table border=1 cellpadding=1 cellspacing=0>
      <tr>
        <th>Key</th>
        <th>Value</th>
      </tr>
      ${trs}
    </table>
  `);

  document.body.append(table);
});

function extend(obj, key, cb) {
  const old = obj[key];
  obj[key + 'old'] = old;
  obj[key] = (...args) => {
    cb(...args);
    old.apply(obj, args);
  };
}

function html(str) {
  const div = document.createElement('div');
  div.innerHTML = str;
  return div.children[0];
}
