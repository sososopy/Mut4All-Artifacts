//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Line_Continuation_In_Macro_Argument_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(409)

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Line_Continuation_In_Macro_Argument_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ME = Result.Nodes.getNodeAs<clang::MacroExpansion>("macroExpansion")) {
        if (!ME) return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(ME->getLocation()))
            return;
        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation Loc = ME->getLocation();
        FullSourceLoc StartLoc = SM.getFullLoc(Loc);
        if (!StartLoc.isValid()) return;
        std::string macroText = Lexer::getSourceText(CharSourceRange::getTokenRange(ME->getSourceRange()), SM, Result.Context->getLangOpts()).str();
        size_t openParen = macroText.find('(');
        if (openParen == std::string::npos) return;
        std::string macroName = macroText.substr(0, openParen);
        size_t closeParen = macroText.find(')');
        if (closeParen == std::string::npos) return;
        std::string argsText = macroText.substr(openParen + 1, closeParen - openParen - 1);
        std::vector<std::string> args;
        size_t start = 0;
        size_t end = 0;
        while (end < argsText.size()) {
            if (argsText[end] == ',' && argsText[end - 1] != '\\') {
                args.push_back(argsText.substr(start, end - start));
                start = end + 1;
                while (start < argsText.size() && argsText[start] == ' ')
                    ++start;
            }
            ++end;
        }
        if (start < argsText.size())
            args.push_back(argsText.substr(start));
        for (size_t i = 0; i < args.size(); ++i) {
            std::string arg = args[i];
            arg.erase(0, arg.find_first_not_of(" \t\n\r\f\v"));
            arg.erase(arg.find_last_not_of(" \t\n\r\f\v") + 1);
            if (arg.empty()) continue;
            if ((arg.front() == '"' && arg.back() == '"') || isalpha(arg.front())) {
                std::string newArg;
                if (arg.front() == '"') {
                    newArg = arg.substr(0, arg.size() - 1) + "\\ continuation\"";
                } else {
                    newArg = arg + "\\ _suffix";
                }
                args[i] = newArg;
                break;
            }
        }
        std::string newArgsText;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0)
                newArgsText += ", ";
            newArgsText += args[i];
        }
        std::string newMacroText = macroName + "(" + newArgsText + ")";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ME->getSourceRange()), newMacroText);
    }
}
  
void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::macroExpansion().bind("macroExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}