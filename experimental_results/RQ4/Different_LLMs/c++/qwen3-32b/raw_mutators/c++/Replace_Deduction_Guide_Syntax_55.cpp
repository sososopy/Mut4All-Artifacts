//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Deduction_Guide_Syntax_55
 */ 
class MutatorFrontendAction_55 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(55)

private:
    class MutatorASTConsumer_55 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_55(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Deduction_Guide_Syntax_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("deductionGuide")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        auto original = stringutils::rangetoStr(*Result.SourceManager, FT->getSourceRange());
        size_t arrow_pos = original.find("->");
        if (arrow_pos == std::string::npos)
            return;

        std::string before_arrow = original.substr(0, arrow_pos);
        std::string after_arrow = original.substr(arrow_pos + 2);
        std::string mutated = before_arrow + " " + after_arrow;

        Rewrite.ReplaceText(FT->getSourceRange(), mutated);
    }
}

void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(isDeductionGuide()).bind("deductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}