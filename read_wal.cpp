// read_wal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma pack(1)
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <filesystem>
#include <algorithm>

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdint>
#include <cmath>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <scope_guard.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog_easy.hpp>
#include <args.hpp>

using namespace std;
namespace fs = std::filesystem;

struct wal_header_t
{
	uint32_t magic_number;
	uint32_t file_format_version;
	uint32_t database_page_size;
	uint32_t checkpoint_sequence_number;
	uint32_t salt_1;
	uint32_t salt_2;
	uint32_t checksum_1;
	uint32_t checksum_2;
};

struct wal_frame_header_t
{
	uint32_t page_number;
	uint32_t size_after_commit_or_zero;
	uint32_t salt_1;
	uint32_t salt_2;
	uint32_t checksum_1;
	uint32_t checksum_2;
};

template<typename B, typename E>
std::string hex(B b, E e)
{
	using namespace std;
	string hex;
	boost::algorithm::hex(b, e, back_inserter(hex));
	return std::move(hex);
}

std::string hex(uint32_t n)
{
	using namespace std;
	string hex;
	auto start = reinterpret_cast<char*>(&n);
	boost::algorithm::hex(start, start + sizeof(n), back_inserter(hex));
	return std::move(hex);
}

std::string hex(uint16_t n)
{
	using namespace std;
	string hex;
	auto start = reinterpret_cast<char*>(&n);
	boost::algorithm::hex(start, start + sizeof(n), back_inserter(hex));
	return std::move(hex);
}

void log_wal_frame_header(const std::unique_ptr<wal_frame_header_t> &first_frame_header)
{
	LOG_INFO("log", "page_number:{0} hex:{1}", first_frame_header->page_number, hex(first_frame_header->page_number));
	LOG_INFO("log", "size_after_commit_or_zero:{0} hex:{1}", first_frame_header->size_after_commit_or_zero, hex(first_frame_header->salt_2));
	LOG_INFO("log", "salt_1:{0} hex:{1}", first_frame_header->salt_1, hex(first_frame_header->salt_1));
	LOG_INFO("log", "salt_2:{0} hex:{1}", first_frame_header->salt_2, hex(first_frame_header->salt_2));
	LOG_INFO("log", "checksum_1:{0} hex:{1}", first_frame_header->checksum_1, hex(first_frame_header->checksum_1));
	LOG_INFO("log", "checksum_1:{0} hex:{1}", first_frame_header->checksum_2, hex(first_frame_header->checksum_2));
}

struct database_header_t
{
	char header_string[16];
	uint16_t page_size;
	uint8_t file_format_write_version;
	uint8_t file_format_read_version;
	uint8_t unused_of_page;
	uint8_t maximum_embedded_payload_fraction;
	uint8_t minimum_embedded_payload_fraction;
	uint8_t leaf_payload_fraction;
	uint32_t file_change_counter;
	uint32_t size_of_the_database_file_in_pages;
	uint32_t page_number_of_the_first_freelist_trunk_page;
	uint32_t total_number_of_freelist_pages;
	uint32_t schema_cookie;
	uint32_t schema_format_number;
	uint32_t page_cache_size;
	uint32_t page_number_of_the_largest_root_b_tree_page;
	uint32_t database_text_encoding;
	uint32_t user_version;
	uint32_t incremental_vacuum_mode;
	uint32_t application_id;
	char reserved_for_expansion[20];
	uint32_t version_valid_for_number;
	uint32_t SQLITE_VERSION_NUMBER;
};

//std::string split_hex_string(std::string str)
//{
//	
//}

void print_hex(std::string hex_db_content, std::size_t line)
{
	auto part = line / 2;
	for (size_t i = 0; i < hex_db_content.size(); i += 2)
	{
		if (i % (part * 4) == 0)
		{
			cout << endl;
			if (i % ((part * 4) * part) == 0)
			{
				cout << fmt::format("size:{0}", i) << endl;
			}
		}
		if (i % (part * 2) == (part - 1) * 2)
		{
			cout << hex_db_content.substr(i, 2) << "  ";
		}
		else
		{
			cout << hex_db_content.substr(i, 2) << " ";
		}
	}
}

int main(int argc, char *argv[])
{
	auto logger = spdlog::stdout_color_mt("log");
	args_t args(argc, argv);
	auto path = get(args, 1);
	auto db_name = get(args, 2);
	//auto wal = get(args, 2, "--wal");
	fs::path db_path;
	fs::path wal_path;

	LOG_INFO("log", "database_header_t:{0}", sizeof(database_header_t));
	//return 1;

	if (!path.empty() && !db_name.empty())
	{
		LOG_INFO("log", "path:{0}", path[0]);
		LOG_INFO("log", "db:{0}", db_name[0]);
		fs::path current_path(path[0]);
		db_path = current_path / (db_name[0] + ".db");
		wal_path = current_path / (db_name[0] + ".db-wal");
		LOG_INFO("log", "db:{0}", db_path.string());
		LOG_INFO("log", "wal:{0}", wal_path.string());
		if (!fs::exists(db_path) || !fs::exists(wal_path))
		{
			LOG_ERROR("log", "file not exists");
			return 1;
		}
	}

	LOG_INFO("log", "db path:{0} size:{1}", db_path.string(), fs::file_size(db_path));
	LOG_INFO("log", "wal path:{0} size:{1}", wal_path.string(), fs::file_size(wal_path));

	auto db_file = fopen(db_path.string().c_str(), "rb");
	auto wal_file = fopen(wal_path.string().c_str(), "rb");

	if (db_file == nullptr || wal_file == nullptr)
	{
		LOG_ERROR("log", "fopen:{0}", strerror(errno));
		return 1;
	}

	scope_guard_t file_guard(
		[&db_file, &wal_file]()
	{
		if (wal_file != nullptr)
		{
			fclose(db_file);
			LOG_INFO("log", "fclose db_file");
		}
		if (wal_file != nullptr)
		{
			fclose(wal_file);
			LOG_INFO("log", "fclose wal_file");
		}
	});

	auto database_header = make_unique<database_header_t>();
	if (fread(database_header.get(), sizeof(database_header_t), 1, db_file) != 1)
	{
		LOG_ERROR("log", "fread:{0}", strerror(errno));
		return 1;
	}

	LOG_INFO("log", "header_string:{0}", database_header->header_string);
	LOG_INFO("log", "page_size:{0} hex:{1}", database_header->page_size, hex(database_header->page_size));

	std::size_t page_size = pow(2, database_header->page_size);

	LOG_INFO("log", "page_size:{0}", page_size);

	vector<char> db_content;

	fseek(db_file, 0, SEEK_SET);

	while (true)
	{
		vector<char> buffer(1024);
		auto size = fread(buffer.data(), sizeof(char), buffer.size(), db_file);
		if (size > 0)
		{
			copy_n(buffer.begin(), size, back_inserter(db_content));
		}
		if (size < buffer.size())
		{
			break;
		}
	}

	LOG_INFO("log", "{0} {1}", db_content.size(), hex(db_content.begin(), db_content.end()));
	
	auto hex_db_content = hex(db_content.begin(), db_content.end());

	print_hex(hex_db_content, 10);

	//for (size_t i = 0; i < hex_db_content.size(); i += 2)
	//{
	//	if (i % 32 == 0)
	//	{
	//		cout << endl;
	//		if (i % (32 * 8) == 0)
	//		{
	//			cout << fmt::format("size:{0}", i) << endl;
	//		}
	//	}
	//	if (i % 32 == 14)
	//	{
	//		cout << hex_db_content.substr(i, 2) << "  ";
	//	}
	//	else
	//	{
	//		cout << hex_db_content.substr(i, 2) << " ";
	//	}
	//}

	return 1;

	//if (!wal.empty())
	//{
	//	LOG_INFO("log", "wal:{0}", wal[0]);
	//}

	//auto file_name = "D:/sqlite3/test.db-wal";
	//auto file = fopen(file_name, "rb");

	//filesystem::path wpath(file_name);
	//LOG_INFO("log", "file_size:{0}", filesystem::file_size(wpath));

	//if (file == nullptr)
	//{
	//	LOG_ERROR("log", "fopen error:{0}", strerror(errno));
	//	return 1;
	//}



	//vector<char> buffer(1024 * 1024 * 10);

	auto header = make_unique<wal_header_t>();
	LOG_INFO("log", "wal_header_t:{0}", sizeof(wal_header_t));
	fread(header.get(), sizeof(wal_header_t), 1, wal_file);
	LOG_INFO("log", "wal header");
	LOG_INFO("log", "magic_number:{0} hex:{1}", header->magic_number, hex(header->magic_number));
	LOG_INFO("log", "file_format_version:{0} hex:{1}", header->file_format_version, hex(header->file_format_version));
	LOG_INFO("log", "database_page_size:{0} hex:{1}", header->database_page_size, hex(header->database_page_size));
	LOG_INFO("log", "salt_1:{0} hex:{1}", header->salt_1, hex(header->salt_1));
	LOG_INFO("log", "salt_2:{0} hex:{1}", header->salt_2, hex(header->salt_2));
	LOG_INFO("log", "checksum_1:{0} hex:{1}", header->checksum_1, hex(header->checksum_1));
	LOG_INFO("log", "checksum_1:{0} hex:{1}", header->checksum_2, hex(header->checksum_2));

	auto first_frame_header = make_unique<wal_frame_header_t>();
	fread(first_frame_header.get(), sizeof(wal_frame_header_t), 1, wal_file);
	LOG_INFO("log", "first frame header");
	log_wal_frame_header(first_frame_header);

	//LOG_INFO("log", "fseek:{0}", fseek(file, 0, SEEK_SET));
	//LOG_INFO("log", "fseek:{0}", fseek(file, header->database_page_size, SEEK_SET));
	//auto second_frame_header = make_unique<wal_frame_header_t>();
	//fread(second_frame_header.get(), sizeof(wal_frame_header_t), 1, file);
	//LOG_INFO("log", "first second_frame_header header");
	//log_wal_frame_header(second_frame_header);

	return 0;
}
