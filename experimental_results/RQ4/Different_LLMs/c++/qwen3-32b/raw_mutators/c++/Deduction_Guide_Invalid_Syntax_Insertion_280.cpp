//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deduction_Guide_Invalid_Syntax_Insertion_280
 */ 
class MutatorFrontendAction_280 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(280)

private:
    class MutatorASTConsumer_280 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_280(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Deduction_Guide_Invalid_Syntax_Insertion_280.h"

// ========================================================================================================
#define MUT280_OUTPUT 1

void MutatorFrontendAction_280::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;
        
        auto sourceCode = stringutils::rangetoStr(*Result.SourceManager, DG->getSourceRange());
        size_t arrowPos = sourceCode.find("->");
        if (arrowPos == std::string::npos) return;
        
        std::string className = DG->getDeclName().getAsString();
        std::string mutatedCode = sourceCode;
        mutatedCode.insert(arrowPos, " " + className);
        
        Rewrite.ReplaceText(DG->getSourceRange(), mutatedCode);
    }
}

void MutatorFrontendAction_280::MutatorASTConsumer_280::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDeductionGuideDecl().bind("deductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}