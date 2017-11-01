# Alpha blending
Alpha blending in x86 and x86-64.

## Usage
Requirements:
```bash
sudo apt-get install liballegro5-dev nasm
```
Run example:
```bash
git clone https://github.com/kczarnota/alpha_blending
cd alpha_blending
make
./blend <file1.bmp> <file2.bmp>
```
Result will be saved in "result.bmp" file.

Controls:
* use arrow keys to move smaller picture inside the bigger one
* use Esc to close the window