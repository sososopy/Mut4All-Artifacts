//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Generic_Lambda_Param_With_T_92
 */ 
class MutatorFrontendAction_92 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(92)
private:
    class MutatorASTConsumer_92 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_92(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Replace_Generic_Lambda_Param_With_T_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("lambdaCallOp")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;
        
        if (!MD->isLambda()) return;
        
        for (const auto *Param : MD->parameters()) {
            if (Param->getType()->isAutoType()) {
                if (availableTypes.empty()) continue;
                unsigned idx = getrandom::getRandomIndex(availableTypes.size());
                std::string replacement = availableTypes[idx];
                
                SourceLocation startLoc = Param->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
                SourceLocation endLoc = Param->getTypeSourceInfo()->getTypeLoc().getEndLoc();
                
                if (startLoc.isValid() && endLoc.isValid()) {
                    Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
                }
            }
        }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (RD->isCompleteDefinition()) {
            availableTypes.push_back(RD->getNameAsString());
        }
    }
}
  
void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher lambdaMatcher = cxxMethodDecl(
        isLambdaCallOperator(),
        hasAnyParameter(hasType(autoType()))
    ).bind("lambdaCallOp");
    
    DeclarationMatcher typeMatcher = cxxRecordDecl().bind("record");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}