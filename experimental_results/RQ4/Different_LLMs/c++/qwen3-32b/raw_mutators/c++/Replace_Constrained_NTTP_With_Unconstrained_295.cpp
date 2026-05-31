//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_NTTP_With_Unconstrained_295
 */ 
class MutatorFrontendAction_295 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(295)

private:
    class MutatorASTConsumer_295 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_295(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constrained_NTTP_With_Unconstrained_295.h"

// ========================================================================================================
#define MUT295_OUTPUT 1

void MutatorFrontendAction_295::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TTP = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("templateParam")) {
        if (!TTP || !Result.Context->getSourceManager().isWrittenInMainFile(TTP->getLocation()))
            return;

        for (const auto *Param : TTP->parameters()) {
            if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
                if (NTTP->hasConstraint()) {
                    SourceLocation startLoc = NTTP->getLocation();
                    SourceLocation endLoc = TTP->getSourceRange().getEnd();
                    std::string originalText = stringutils::rangetoStr(*Result.SourceManager, 
                                                                      SourceRange(startLoc, endLoc));
                    size_t constraintPos = originalText.find("C auto");
                    if (constraintPos != std::string::npos) {
                        std::string newText = originalText;
                        newText.replace(constraintPos, 4, "auto");
                        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newText);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_295::MutatorASTConsumer_295::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTemplateParmDecl(
        hasDeclContext(templateDecl(isAliasTemplate()))
    ).bind("templateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}