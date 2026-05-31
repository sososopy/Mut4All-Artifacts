//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Dependent_Conversion_To_NonDependent_245
 */ 
class MutatorFrontendAction_245 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(245)

private:
    class MutatorASTConsumer_245 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_245(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Dependent_Conversion_To_NonDependent_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *opCall = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("conversionCall")) {
        // Filter nodes in header files
        if (!opCall || !Result.Context->getSourceManager().isWrittenInMainFile(opCall->getBeginLoc()))
            return;

        // Check if the return type is dependent
        QualType returnType = opCall->getType();
        if (!returnType->isDependentType())
            return;

        // Get the source code of the operator call
        std::string originalCode = stringutils::rangetoStr(*Result.SourceManager, opCall->getSourceRange());
        size_t opPos = originalCode.find("operator ");
        if (opPos != std::string::npos) {
            size_t endPos = originalCode.find("(", opPos);
            if (endPos != std::string::npos) {
                std::string typeStr = originalCode.substr(opPos + 9, endPos - (opPos + 9));
                std::string newCode = originalCode;
                newCode.replace(opPos + 9, typeStr.length(), "int");
                Rewrite.ReplaceText(opCall->getSourceRange(), newCode);
            }
        }
    }
}

void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxOperatorCallExpr(
        hasOperatorName("operator"),
        hasCallee(cxxConversionDecl())
    ).bind("conversionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}