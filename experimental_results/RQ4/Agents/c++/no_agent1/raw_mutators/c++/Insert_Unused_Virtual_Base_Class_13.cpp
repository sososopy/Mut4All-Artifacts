//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Virtual_Base_Class_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_classes;
    };
};

//source file
#include "../include/insert_unused_virtual_base_class_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        
        base_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
            return;
        if (!DL->isCompleteDefinition())
            return;

        if (base_classes.empty())
            return;

        const clang::CXXRecordDecl *baseClass = base_classes.back();
        std::string baseClassName = baseClass->getNameAsString();
        std::string derivedClassName = DL->getNameAsString();

        std::string virtualBaseInsertion = "class " + derivedClassName + " : virtual public " + baseClassName + " {};";

        Rewrite.InsertTextAfterToken(DL->getEndLoc(), "\n/*mut13*/" + virtualBaseInsertion);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}