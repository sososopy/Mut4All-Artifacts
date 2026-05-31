//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pragma_Keyword_With_Var_281
 */ 
class MutatorFrontendAction_281 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(281)

private:
    class MutatorASTConsumer_281 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_281(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> variableNames;
    };
};

//source file
#include "../include/Replace_Pragma_Keyword_With_Var_281.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        variableNames.push_back(VD->getNameAsString());
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("directive")) {
        if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(PD->getLocation()))
            return;
        SourceRange range = PD->getSourceRange();
        std::string directiveText = stringutils::rangetoStr(*Result.SourceManager, range);
        if (directiveText.find("#pragma") == std::string::npos && directiveText.find("__pragma") == std::string::npos)
            return;
        size_t openParen = directiveText.find('(');
        if (openParen == std::string::npos) return;
        size_t closeParen = directiveText.find(')');
        if (closeParen == std::string::npos) return;
        std::string args = directiveText.substr(openParen + 1, closeParen - openParen - 1);
        std::string keywordArg;
        for (size_t i = 0; i < args.size(); ++i) {
            if (isalpha(args[i])) {
                size_t j = i;
                while (j < args.size() && isalnum(args[j])) ++j;
                keywordArg = args.substr(i, j - i);
                break;
            }
        }
        if (keywordArg.empty() || variableNames.empty()) return;
        std::string replacementVar = variableNames[getrandom::getRandomIndex(variableNames.size())];
        std::string newDirectiveText = directiveText;
        size_t keywordPos = newDirectiveText.find(keywordArg);
        if (keywordPos != std::string::npos) {
            newDirectiveText.replace(keywordPos, keywordArg.size(), replacementVar);
            Rewrite.ReplaceText(range, newDirectiveText);
        }
    }
}
  
void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varMatcher = varDecl().bind("varDecl");
    DeclarationMatcher preprocMatcher = preprocessorDirective().bind("directive");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(preprocMatcher, &callback);
    matchFinder.matchAST(Context);
}