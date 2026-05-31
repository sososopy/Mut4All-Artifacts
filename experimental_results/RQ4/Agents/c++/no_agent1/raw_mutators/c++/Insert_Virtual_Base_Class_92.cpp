//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_92
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
        const clang::CXXRecordDecl *lastBaseClass = nullptr;
    };
};

//source file
#include "../include/Insert_Virtual_Base_Class_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        //Filter nodes in header files
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        // Record the base class for potential virtual inheritance
        lastBaseClass = BaseClass;
    } else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        //Filter nodes in header files
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        if (lastBaseClass && DerivedClass->isDerivedFrom(lastBaseClass)) {
            //Get the source code text of target node
            std::string baseName = lastBaseClass->getNameAsString();
            std::string mutationText = "virtual " + baseName;

            //Perform mutation on the source code text by applying string replacement
            for (auto &BaseSpec : DerivedClass->bases()) {
                if (BaseSpec.getType()->getAsCXXRecordDecl() == lastBaseClass) {
                    SourceLocation baseStartLoc = BaseSpec.getBeginLoc();
                    Rewrite.InsertText(baseStartLoc, "/*mut92*/virtual ", true, true);
                    break;
                }
            }
        }
    }
}

void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}