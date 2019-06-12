#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <zip.h>

#define MAX_PASSWD_LEN	7

using namespace std;

zip_t* zipHandle = NULL;

void usage();
bool tryPassword(const char* vPasswd);
bool openZipFile(char* filename);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		usage();
		return 0;
	}
/*
	if (!openZipFile(argv[1]))
	{
		printf(" error opening zipfile\n");
		return(-1);
	}

	zip_int64_t totEntries = zip_get_num_entries(zipHandle, ZIP_FL_UNCHANGED);

	cout << " archive has " << totEntries << " entries" << endl;

	char* files[totEntries];

	for (int k = 0; k < totEntries; ++k) {
		files[k] = (char*)zip_get_name(zipHandle, k, 0);
	}
*/
	// print files
	//for (int k = 0; k < totEntries; ++k) {
	//	cout << " " << files[k] << endl;
	//}

	unsigned char cStart = 32;
	unsigned char cStop = 126;
	unsigned char vPasswd[MAX_PASSWD_LEN+1];
	uint64_t nCount = 0;
	static char vWaitSymbols[4] = {'-','\\','|','/'};
	unsigned nWaitCounter = 0;
	bool anotherOne;

	for (unsigned int i = 0; i < MAX_PASSWD_LEN; i++)
	{
		vPasswd[i+1] = '\0';

		for (unsigned int j = 1; j <= i; ++j)
			vPasswd[j] = cStart;

		anotherOne = true;

		while (anotherOne)
		{
			//cout << "\r trying password " <<  vWaitSymbols[nWaitCounter++ % 4] << flush;
			if (nCount < 1000000000)
				cout << "\r trying password n. " <<  nCount+1 << " ..." << flush;
			else
				cout << "\r trying password n. " <<  fixed << setprecision(3) << nCount/1000000000.0 << " mld ..." << flush;

			for (unsigned char cCurrChar = cStart; cCurrChar <= cStop; cCurrChar++)
			{
				nCount++;
				vPasswd[0] = cCurrChar;

				if (!openZipFile(argv[1]))
				{
					printf(" error opening zipfile\n");
					return(-1);
				}

				if (tryPassword((char*)vPasswd))
				{
					cout << "\n password found '" << vPasswd << "'" << endl;
					exit(1);
				}

				zip_close(zipHandle);

			}

			anotherOne = false;

			for (unsigned int j = 1; j <= i; ++j)
			{
				if (vPasswd[j] < cStop)
				{
					vPasswd[j]++;
					anotherOne = true;
					break;
				}

				vPasswd[j] = cStart;
			}
		}
	}

	cout << "\r password not found in " << nCount << " try" << endl;

	return 0;
}

void usage()
{
	cout << "Usage:\n";
	cout << " ZipPasswordFinder <zip_file>\n";
}

bool tryPassword(const char* vPasswd)
{
	if (zip_fopen_index_encrypted(zipHandle, 0, ZIP_FL_UNCHANGED | ZIP_FL_COMPRESSED, vPasswd) == NULL)
	{
		const zip_error_t* z_err = zip_get_error(zipHandle);
		int zip_err = zip_error_code_zip(z_err);
		zip_error_clear(zipHandle);

		return false;
	}

	int zip_err = zip_error_code_zip(zip_get_error(zipHandle));
	zip_error_clear(zipHandle);

	//ZIP_ER_WRONGPASSWD

	return true;
}

bool openZipFile(char* filename)
{
	int errorp = 0;

	zipHandle = zip_open(filename, ZIP_RDONLY, &errorp);

	return zipHandle != NULL;
}