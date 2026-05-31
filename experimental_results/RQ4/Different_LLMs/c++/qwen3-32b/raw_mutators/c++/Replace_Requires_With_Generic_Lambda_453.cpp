//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_With_Generic_Lambda_453
 */ 
class MutatorFrontendAction_453 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(453)
private:
    class MutatorASTConsumer_453 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_453(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Requires_With_Generic_Lambda_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("req")) {
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(RE->getBeginLoc()))
            return;

        auto SM = Result.Context->getSourceManager();
        auto &LangOpts = Result.Context->getLangOpts();
        SourceLocation StartLoc = RE->getRequiresLoc();
        SourceLocation EndLoc = RE->getEndLoc();

        if (!StartLoc.isValid() || !EndLoc.isValid())
            return;

        // Find the type constraint expression within the requires clause
        if (auto *CE = dyn_cast<clang::CXXTypeidExpr>(RE->getSubExpr())) {
            if (auto *DC = dyn_cast<clang::DeclRefExpr>(CE->getOperand())) {
                std::string OriginalCode = stringutils::rangetoStr(*SM, CharSourceRange::getTokenRange(CE->getSourceRange()));
                
                // Generate lambda replacement
                std::string Lambda = "[&](auto x) { return std::same_as<decltype(x), ";
                Lambda += OriginalCode.substr(OriginalCode.find(',') + 1);
                Lambda += ">; }(T::tag{})";
                
                // Perform mutation
                Rewrite.ReplaceText(CE->getSourceRange(), Lambda);
            }
        }
    }
}
  
void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr(hasParent(conceptDefinition())).bind("req");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}