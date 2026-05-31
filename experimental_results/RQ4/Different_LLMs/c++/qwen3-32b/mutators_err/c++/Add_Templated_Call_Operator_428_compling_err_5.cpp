//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Templated_Call_Operator_428
 */ 
class MutatorFrontendAction_428 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(428)

private:
    class MutatorASTConsumer_428 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_428(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Templated_Call_Operator_428.h"

// ========================================================================================================
#define MUT428_OUTPUT 1

void MutatorFrontendAction_428::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->getNameAsString() != "operator()") return;
        
        SourceManager &SM = Rewrite.getSourceMgr();
        SourceRange range = MT->getSourceRange();
        std::string original = stringutils::rangetoStr(SM, range);

        size_t openParen = original.find('(');
        size_t closeParen = original.find(')');
        if (openParen == std::string::npos || closeParen == std::string::npos) return;

        std::string beforeParams = original.substr(0, openParen + 1);
        std::string params = original.substr(openParen + 1, closeParen - (openParen + 1));
        std::string afterParams = original.substr(closeParen);

        size_t firstComma = params.find(',');
        std::string firstParam = (firstComma == std::string::npos) ? params : params.substr(0, firstComma);
        size_t spacePos = firstParam.find(' ');
        if (spacePos != std::string::npos) {
            std::string newName = "T" + firstParam.substr(spacePos);
            if (firstComma == std::string::npos) {
                params = newName;
            } else {
                params = newName + params.substr(firstComma);
            }
        }

        std::string templateStr = "template<typename T> ";
        std::string newBeforeParams = templateStr + beforeParams;
        std::string newDecl = newBeforeParams + params + afterParams;

        Rewrite.ReplaceText(range, newDecl);
    }
}

void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        hasName("operator()"),
        unless(isTemplated()),
        hasNumParams(1)
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}