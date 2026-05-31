//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Lex/Preprocessor.h"

/**
 * ReorderEmbedParamsWithInvalidFirst_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)
private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_ReorderEmbedParamsWithInvalidFirst_227.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("embedDirective")) {
        // Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        FullSourceLoc loc(MT->getBeginLoc(), SM);
        std::string directiveText = Lexer::getSourceText(CharSourceRange::getCharRange(MT->getSourceRange()), SM, SM.getLangOpts());

        if (directiveText.find("#embed") != 0) {
            return;
        }

        // Remove the "#embed" part and leading whitespace
        std::string paramsStr = directiveText.substr(6);
        paramsStr = paramsStr.substr(paramsStr.find_first_not_of(" \t"));

        std::vector<std::string> params;
        size_t start = 0;
        while (start < paramsStr.length()) {
            size_t end = paramsStr.find_first_of(" \t", start);
            if (end == std::string::npos) {
                end = paramsStr.length();
            }
            std::string param = paramsStr.substr(start, end - start);
            if (!param.empty()) {
                params.push_back(param);
            }
            start = end + 1;
        }

        if (params.size() < 2) {
            return;
        }

        bool hasInvalid = false;
        int invalidIndex = -1;
        for (size_t i = 0; i < params.size(); ++i) {
            const std::string& param = params[i];
            size_t openParen = param.find('(');
            if (openParen == std::string::npos) {
                hasInvalid = true;
                invalidIndex = i;
                break;
            }
            size_t closeParen = param.rfind(')');
            if (closeParen != param.length() - 1) {
                hasInvalid = true;
                invalidIndex = i;
                break;
            }
        }

        if (hasInvalid) {
            std::string invalidParam = params[invalidIndex];
            params.erase(params.begin() + invalidIndex);
            params.insert(params.begin(), invalidParam);
        } else {
            if (params.empty()) {
                return;
            }
            std::string validParam = params[0];
            size_t openParen = validParam.find('(');
            if (openParen == std::string::npos) {
                return;
            }
            std::string identifier = validParam.substr(0, openParen);
            std::string args = validParam.substr(openParen + 1, validParam.length() - openParen - 2);
            std::string invalidParam = identifier + "X(" + args + ")";
            params[0] = invalidParam;
        }

        std::string newParams;
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) {
                newParams += " ";
            }
            newParams += params[i];
        }
        std::string newDirective = "#embed " + newParams;

        Rewrite.ReplaceText(MT->getSourceRange(), newDirective);
    }
}

void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = preprocessingDirective().bind("embedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}