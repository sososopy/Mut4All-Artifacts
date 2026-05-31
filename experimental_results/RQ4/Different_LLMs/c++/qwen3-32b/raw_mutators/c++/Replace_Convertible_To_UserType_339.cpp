//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Convertible_To_UserType_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(339)

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> userDefinedTypes;
    };
};

//source file
#include "../include/Replace_Convertible_To_UserType_339.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check if the matched node is a CXXRecordDecl (user-defined type)
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        // Add the name of the user-defined type to the list
        userDefinedTypes.push_back(RD->getNameAsString());
    }
    // Check if the matched node is a TypeTraitExpr for is_convertible
    else if (auto *TT = Result.Nodes.getNodeAs<clang::TypeTraitExpr>("traitExpr")) {
        if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(TT->getLocation()))
            return;
        // Check if it's is_convertible
        if (TT->getTrait() != clang::TT_isConvertible)
            return;
        // Get the second type argument (target type)
        if (TT->getNumArgs() < 2)
            return;
        auto *secondArg = TT->getTypeArg(1);
        if (!secondArg)
            return;
        // Get the source range of the second type argument
        SourceLocation startLoc = secondArg->getSourceRange().getBegin();
        SourceLocation endLoc = secondArg->getSourceRange().getEnd();
        // Select a random user-defined type
        if (userDefinedTypes.empty())
            return;
        std::string newTypeName = userDefinedTypes[getrandom::getRandomIndex(userDefinedTypes.size())];
        // Replace the second type argument with the new type
        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newTypeName);
    }
}

void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match CXXRecordDecls to collect user-defined types
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("recordDecl");
    // Match TypeTraitExpr for is_convertible
    DeclarationMatcher traitMatcher = expr(typeTraitExpr(hasTypeTrait(clang::TT_isConvertible))).bind("traitExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(traitMatcher, &callback);
    matchFinder.matchAST(Context);
}