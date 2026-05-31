#include <clang/AST/ASTContext.h>
#include <clang/AST/NestedNameSpecifier.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Frontend/FrontendAction.h>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <llvm/Support/CommandLine.h>
#include <memory>
#include <ostream>
#include <random>
#include <string>

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include "ProgressBar.hpp"
#include "conf.hpp"
#include "do_visit.hpp"
#include "mutator.hpp"

using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;

struct tmp_file
{
    string fname;
    tmp_file()
    {
        fname                        = "/tmp/cppmutate.";
        static const char alphanum[] = "0123456789"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz";
        std::string       tmp_s;
        tmp_s.reserve(6);

        for (int i = 0; i < 6; ++i)
        {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        fname += tmp_s + ".cpp";
    }
    ~tmp_file() {}
};

struct job_t
{
    shared_ptr<mutator_t> cur_mutator;

    job_t()
    {
        cur_mutator = make_shared<mutator_t>();
    }

    shared_ptr<file_ctx_t> parse_file(const string& file_path)
    {
        try
        {
            return make_shared<file_ctx_t>(file_path);
        }
        catch (...)
        {
            return nullptr;
        }
    }
    deque<string> get_file_path(const string& folder_path)
    {
        auto res = deque<string>();
        if (!filesystem::exists(folder_path))
        {
            cerr << "Folder does not exist: " << folder_path << endl;
            return res;
        }
        // Walking through the folders and parsing files
        auto   skip_load_files = conf().get<vector<string>>("skip_load_files");
        for (const auto& entry : filesystem::recursive_directory_iterator(folder_path))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }
            if (entry.path().extension() != ".cpp" && entry.path().extension() != ".c")
            {
                continue;
            }
            if (any_of(skip_load_files.begin(), skip_load_files.end(),
                       [&entry](const string& skip_file) {
                           return entry.path().string().find(skip_file) != string::npos;
                       }))
            {
                cout << "Skipping file: " << entry.path().string() << endl;
                continue;
            }
            res.push_back(entry.path().string());
        }
        return res;
    }
    deque<shared_ptr<file_ctx_t>> parse_folders(const string& folder_path)
    {
        auto res        = deque<shared_ptr<file_ctx_t>>();
        auto file_paths = get_file_path(folder_path);
        auto pbar       = ProgressBar('.', '#', 60);
        pbar.done       = 0;
        pbar.todo       = file_paths.size();
        for (const auto& file_path : file_paths)
        {
            auto data = parse_file(file_path);
            if (data)
            {
                res.push_back(data);
            }
            else
            {
                cerr << "Failed to parse file: " << file_path << endl;
            }
            pbar.done++;
            pbar.auto_fill_up();
            pbar.displayPercentage();
            cout << " | ";
            pbar.displayTasksDone();
        }
        pbar.end();
        return res;
    }

    void do_visit(shared_ptr<file_ctx_t>& data)
    {
        class SelfMutateASTActionFactory : public FrontendActionFactory
        {
        public:
            std::shared_ptr<file_ctx_t>& file_ctx;
            std::shared_ptr<mutator_t>   cur_mutator;
            SelfMutateASTActionFactory(std::shared_ptr<file_ctx_t>& data,
                                       std::shared_ptr<mutator_t>   cur_mutatot)
            : file_ctx(data), cur_mutator(cur_mutatot)
            {}
            std::unique_ptr<FrontendAction> create() override
            {
                return std::make_unique<MutateASTAction>(file_ctx, cur_mutator);
            }
        };
        auto action = shared_ptr<SelfMutateASTActionFactory>(
            new SelfMutateASTActionFactory(data, cur_mutator));
        try
        {
            data->tool->run(action.get());
        }
        catch (const std::exception& e)
        {
            cerr << "Error during AST visit: " << e.what() << endl;
        }
        catch (...)
        {
            cerr << "Unknown error during AST visit." << endl;
        }
    }

    void store_ast(shared_ptr<file_ctx_t>& data)
    {
        cur_mutator->job_type       = mutator_t::job_ty::ADD;
        cur_mutator->analysis_depth = 0;
        cur_mutator->mutate_done    = false;
        do_visit(data);
    }

    shared_ptr<file_ctx_t> modify_ast(shared_ptr<file_ctx_t>& data)
    {
        cur_mutator->job_type       = mutator_t::job_ty::MODIFY;
        cur_mutator->analysis_depth = 0;
        cur_mutator->mutate_done    = false;
        do_visit(data);
        return data;
    }

    string ast_to_code(const shared_ptr<file_ctx_t>& data)
    {
        return data->cur_code;
    }
};

bool HALT = false;

int run(job_t& jobber)
{
    // Notice: To modify the AST, we need to new file context, or we will modify the original one.
    auto all_files = jobber.get_file_path(conf().get<string>("input_folder"));
    if (all_files.empty())
    {
        cerr << "No files found in input folder: " << conf().get<string>("input_folder") << endl;
        return 1;
    }
    auto dev  = random_device();
    auto rng  = mt19937(dev());
    auto dist = uniform_int_distribution<size_t>(0, all_files.size() - 1);

    auto mutate_time   = 6;
    auto loop_cnt      = conf().get<size_t>("loop_cnt");
    auto inf_loop      = conf().get<bool>("inf_loop");
    auto output_folder = conf().get<string>("output_folder");
    auto tmp_f         = tmp_file();

    auto pbar = ProgressBar('.', '#', 60);
    pbar.done = 0;
    if (inf_loop)
    {
        pbar.todo = 0;
    }
    else
    {
        pbar.todo = loop_cnt;
    }

    for (size_t i = 0; (i < loop_cnt) || inf_loop; ++i)
    {
        if (HALT)
        {
            return 0;
        }
        size_t idx = dist(rng);

        pbar.done++;
        if (!inf_loop)
        {
            pbar.auto_fill_up();
            pbar.displayPercentage();
        }
        else
        {
            cout << '\r';
            cout << "Infinite loop mode, no percentage display." << endl;
        }
        cout << " | ";
        cout << "Processing: " << all_files[idx] << " | ";
        if (!inf_loop)
        {
            pbar.displayTasksDone();
        }
        else
        {
            cout << i << "/inf" << endl;
        }

        {
            ofstream ofs(tmp_f.fname);
            ifstream ifs(all_files[idx]);
            if (!ifs || !ofs)
            {
                cerr << "Failed to open input or temporary file." << endl;
                continue;
            }
            ofs << ifs.rdbuf();
            ifs.close();
            ofs.close();
        }

        for (size_t j = 0; j < mutate_time; j++)
        {
            auto file = jobber.parse_file(tmp_f.fname);
            if (!file)
            {
                cerr << "Failed to parse file: " << all_files[idx] << endl;
                continue;
            }
            jobber.modify_ast(file);
            ofstream out(tmp_f.fname);
            if (!out)
            {
                cerr << "Failed to open temporary file for writing." << endl;
                continue;
            }
            out << jobber.ast_to_code(file);
            out.close();
        }

        string   fname = "file_" + to_string(i) + ".cpp";
        ifstream ifs(tmp_f.fname);
        ofstream out(output_folder + "/" + fname);
        if (!out)
        {
            cerr << "Failed to open output file: " << output_folder + "/" + fname << endl;
            continue;
        }
        if (!ifs)
        {
            cerr << "Failed to open temporary file for reading: " << tmp_f.fname << endl;
            continue;
        }
        out << ifs.rdbuf();
        ifs.close();
        out.close();
    }

    pbar.end();
    return 0;
}

int main(int argc, char* argv[], char* envp[])
{
    if (argc < 2)
    {
        conf_init("conf.json");
    }
    else
    {
        string conf_file = argv[1];
        conf_init(conf_file);
    }

    auto jobber = job_t();

    if (!filesystem::exists(conf().get<string>("input_folder")))
    {
        cerr << "Input folder does not exist: " << conf().get<string>("input_folder") << endl;
        return 1;
    }
    cout << "Beginning to parse input files:" << endl;
    auto files = jobber.parse_folders(conf().get<string>("input_folder"));
    cout << "Beginning to load files as seeds:" << endl;
    auto pbar = ProgressBar('.', '#', 60);
    pbar.done = 0;
    pbar.todo = files.size();
    for (auto& file : files)
    {
        pbar.done++;
        pbar.auto_fill_up();
        pbar.displayPercentage();
        cout << " | ";
        cout << file->orig_file << " | ";
        pbar.displayTasksDone();
        jobber.store_ast(file);
    }
    pbar.end();
    cout << "Loaded " << files.size() << " files as seeds." << endl;
    for (const auto& [k, v] : jobber.cur_mutator->entity_stores)
    {
        cout << "Entity: " << k << " has " << v.entities.size() << " entities." << endl;
    }

    // Capture Ctrl-C to stop the program
    signal(SIGINT, [](int signum) {
        cout << "Caught signal " << signum << ", stopping..." << endl;
        HALT = true;
    });

    return run(jobber);
}
