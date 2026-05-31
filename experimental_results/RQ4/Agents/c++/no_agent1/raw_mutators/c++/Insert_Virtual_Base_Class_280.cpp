//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_280
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
        const clang::CXXRecordDecl *lastBaseClass = nullptr;
    };
};

//source file
#include "../include/insert_virtual_base_class_280.h"

// ========================================================================================================
#define MUT280_OUTPUT 1

void MutatorFrontendAction_280::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        //Filter nodes in header files
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;

        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        // Record the last valid base class
        lastBaseClass = BaseClass;
    } else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        //Filter nodes in header files
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;

        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        if (!lastBaseClass)
            return;

        //Get the source code text of target node
        std::string baseClassName = lastBaseClass->getNameAsString();
        std::string derivedClassName = DerivedClass->getNameAsString();
        
        //Perform mutation on the source code text by applying string replacement
        std::string insertion = " : virtual public " + baseClassName;
        std::string mutation = "/*mut280*/class " + derivedClassName + insertion + " {};\n";
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(DerivedClass->getEndLoc(), mutation);
    }
}

void MutatorFrontendAction_280::MutatorASTConsumer_280::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher baseMatcher = cxxRecordDecl(isCompleteDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isCompleteDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}