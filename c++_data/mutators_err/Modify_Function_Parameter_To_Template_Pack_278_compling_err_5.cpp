//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Function_Parameter_To_Template_Pack_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Function_Parameter_To_Template_Pack_278.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->getNumParams() > 0 && FD->hasBody()) {
            auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            auto bodySource = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());

            std::string templatePrefix = "template<typename... Ts> ";
            std::string newParams = "Ts... params";
            std::string newBody = "{ (void)sizeof...(params); " + bodySource.substr(1);

            size_t paramStart = functionSource.find('(');
            size_t paramEnd = functionSource.find(')', paramStart);

            functionSource.replace(paramStart + 1, paramEnd - paramStart - 1, newParams);
            functionSource.insert(0, templatePrefix);
            functionSource.replace(functionSource.find(bodySource), bodySource.length(), newBody);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
        }
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameterCount(1), isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}