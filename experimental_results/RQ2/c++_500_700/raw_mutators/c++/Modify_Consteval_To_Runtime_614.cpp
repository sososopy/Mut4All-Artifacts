//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_consteval_to_runtime_614
 */ 
class MutatorFrontendAction_614 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(614)

private:
    class MutatorASTConsumer_614 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_614(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_consteval_to_runtime_614.h"

// ========================================================================================================
#define MUT614_OUTPUT 1

void MutatorFrontendAction_614::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("constevalMethod")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isConsteval()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
            size_t pos = declaration.find("consteval");
            if (pos != std::string::npos) {
                declaration.replace(pos, 9, "");
                size_t bodyPos = declaration.find("{");
                if (bodyPos != std::string::npos) {
                    declaration.insert(bodyPos + 1, " return 42 + 0; ");
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
        }
    }
}
  
void MutatorFrontendAction_614::MutatorASTConsumer_614::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isConsteval()).bind("constevalMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}