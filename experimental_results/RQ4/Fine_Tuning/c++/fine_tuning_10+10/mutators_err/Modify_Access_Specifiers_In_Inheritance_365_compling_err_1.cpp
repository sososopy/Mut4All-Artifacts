//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Access_Specifiers_In_Inheritance_365
 */ 
class MutatorFrontendAction_365 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(365)

private:
    class MutatorASTConsumer_365 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_365(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_access_specifiers_in_inheritance_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (const auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        // Filter nodes in header files
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;

        // Iterate over base classes and modify access specifier
        for (const auto &Base : DerivedClass->bases()) {
            if (Base.getAccessSpecifier() == clang::AS_private) {
                // Perform mutation on the access specifier
                std::string replacement = "protected";
                Rewrite.ReplaceText(Base.getAccessSpecifierLoc(), "private", replacement);
                break; // Modify only one base class
            }
        }
    }
}

void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(anything()), hasDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}