import os
from setuptools import setup, find_packages

setup(
    name='zslparser',
    version='0.1.0',
    packages=find_packages('zslparser'),
    install_requires=[
        'pyparsing'
    ],
    author='Eser Kokturk',
    description='ZSL syntax parser and code generator',
    entry_points={
        'console_scripts': [
            'zslparser=zslparser.__main__:main',
        ],
    },
    python_requires='>=3.7',
    options={"egg_info": {"egg_base": os.path.relpath(f"{os.environ.get('VIRTUAL_ENV')}", os.path.dirname(os.path.abspath(__file__))) }},
)