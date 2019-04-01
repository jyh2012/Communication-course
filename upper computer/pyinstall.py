import  os
if __name__ == '__main__':
    from PyInstaller.__main__ import run
    opts=['callzsjyh.py','-F','-w','--icon=tools_48px.ico']
    run(opts)
