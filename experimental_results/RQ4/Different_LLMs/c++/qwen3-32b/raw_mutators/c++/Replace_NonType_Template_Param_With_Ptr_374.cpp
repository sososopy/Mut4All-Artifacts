//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonType_Template_Param_With_Ptr_374
 */ 
class MutatorFrontendAction_374 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(374)

private:
    class MutatorASTConsumer_374 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_374(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonType_Template_Param_With_Ptr_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("declRefExpr")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getLocation()))
            return;
        SourceLocation StartLoc = DRE->getBeginLoc();
        SourceLocation EndLoc = DRE->getEndLoc();
        std::string Text = Rewrite.getRewrittenText(SourceRange(StartLoc, EndLoc));
        std::string NewText = "*" + Text;
        Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), NewText);
    }
}
  
void MutatorFrontendAction_374::MutatorASTConsumer_374::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateIdType(hasTemplateArgument(0, referent(declRefExpr(to(decl(nonTypeTemplateParm(), hasType(autoType())))).bind("declRefExpr")))).bind("templateArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}