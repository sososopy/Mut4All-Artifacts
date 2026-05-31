//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Destructor_With_Invalid_Default_Initializer_18
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
#include "../include/Replace_Destructor_With_Invalid_Default_Initializer_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Dtor")) {
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(DT->getLocation()))
            return;

        auto SM = Result.SourceManager;
        auto DTLoc = DT->getLocation();
        if (SM->isInSystemHeader(DTLoc))
            return;

        std::string dtorText = stringutils::rangetoStr(*SM, DT->getSourceRange());
        if (dtorText.empty())
            return;

        if (dtorText.find("= default;") != std::string::npos) {
            dtorText.replace(dtorText.find("= default;"), 10, "= {}");
        } else {
            size_t openBrace = dtorText.find('{');
            if (openBrace != std::string::npos) {
                dtorText.replace(openBrace, dtorText.length() - openBrace, " = {}");
            }
        }

        Rewrite.ReplaceText(DT->getSourceRange(), dtorText);
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Dtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}