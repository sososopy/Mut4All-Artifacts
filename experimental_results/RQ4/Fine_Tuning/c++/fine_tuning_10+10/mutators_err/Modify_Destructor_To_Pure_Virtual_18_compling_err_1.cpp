//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Destructor_To_Pure_Virtual_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_destructor_to_pure_virtual_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
            return;

        if (DT->isVirtual() && DT->hasBody()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       DT->getSourceRange());
            size_t bracePos = declaration.find("{");
            if (bracePos != std::string::npos) {
                declaration.replace(bracePos, declaration.size() - bracePos, "= 0;");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), declaration);
            }
        }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl(isVirtual(), hasBody()).bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}