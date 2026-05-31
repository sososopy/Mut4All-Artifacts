//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_builtin_function_call_arguments_11
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const CallExpr *> builtin_calls;
    };
};

//source file
#include "../include/modify_builtin_function_call_arguments_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getExprLoc()))
            return;

        if (Call->getDirectCallee() && Call->getDirectCallee()->getName().startswith("__builtin_")) {
            builtin_calls.push_back(Call);
            auto args = Call->arguments();
            std::string modified_call = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(Call->getSourceRange()),
                Result.Context->getSourceManager(), clang::LangOptions(), 0);

            if (!args.empty()) {
                // Modify argument count
                if (getrandom::getRandomIndex(2)) {
                    // Add an extra argument
                    modified_call.insert(modified_call.find_last_of(')'), ", 0");
                } else {
                    // Remove an argument (if possible)
                    if (args.size() > 1) {
                        auto arg_range = clang::SourceRange(args[0]->getBeginLoc(), args[1]->getBeginLoc().getLocWithOffset(-1));
                        modified_call.erase(clang::Lexer::getSourceText(
                            clang::CharSourceRange::getTokenRange(arg_range),
                            Result.Context->getSourceManager(), clang::LangOptions(), 0));
                    }
                }
            }

            // Change argument types
            if (!args.empty()) {
                unsigned index = getrandom::getRandomIndex(args.size());
                auto &arg = args[index];
                auto arg_text = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(arg->getSourceRange()),
                    Result.Context->getSourceManager(), clang::LangOptions(), 0);
                
                if (arg->getType()->isIntegerType())
                    modified_call.replace(modified_call.find(arg_text), arg_text.length(), "0.0");
                else if (arg->getType()->isFloatingType())
                    modified_call.replace(modified_call.find(arg_text), arg_text.length(), "1");
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Call->getSourceRange()), modified_call);
        }
    }
}

void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto builtin_function_call_matcher = callExpr(callee(functionDecl(hasName("__builtin_")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(builtin_function_call_matcher, &callback);
    matchFinder.matchAST(Context);
}