//header file
#pragma once
#include "Mutator_base.h"

/**
 * recursive_initialization_in_structs_630
 */ 
class MutatorFrontendAction_630 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(630)

private:
    class MutatorASTConsumer_630 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_630(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/recursive_initialization_in_structs_630.h"

// ========================================================================================================
#define MUT630_OUTPUT 1

void MutatorFrontendAction_630::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;

        if (!RD->isStruct())
            return;

        // Check if the struct has at least one field
        if (RD->field_empty())
            return;

        // Check if the struct has a constructor
        bool hasConstructor = false;
        for (auto *ctor : RD->ctors()) {
            if (ctor->isUserProvided()) {
                hasConstructor = true;
                break;
            }
        }

        // If no user-provided constructor, add one
        if (!hasConstructor) {
            std::string constructorCode = "/*mut630*/" + RD->getNameAsString() + "() {}";
            Rewrite.InsertTextAfterToken(RD->getBraceRange().getBegin(), constructorCode);
        }

        // Modify the initialization of the first member variable to involve a recursive call
        if (auto *FD = *RD->field_begin()) {
            std::string fieldName = FD->getNameAsString();
            std::string structName = RD->getNameAsString();
            std::string recursiveInit = "/*mut630*/" + fieldName + " = g(" + structName + "())";
            Rewrite.ReplaceText(FD->getSourceRange(), recursiveInit);
        }
    }
}

void MutatorFrontendAction_630::MutatorASTConsumer_630::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}