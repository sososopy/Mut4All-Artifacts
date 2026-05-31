//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_404.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *BaseClass = Result.Nodes.getNodeAs<CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getBeginLoc()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        std::string baseClassName = BaseClass->getNameAsString();
        if (baseClassName.empty() || visitedClasses.count(baseClassName))
            return;

        visitedClasses.insert(baseClassName);
    }
    else if (const CXXRecordDecl *DerivedClass = Result.Nodes.getNodeAs<CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getBeginLoc()))
            return;
        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        std::string derivedClassName = DerivedClass->getNameAsString();
        if (derivedClassName.empty())
            return;

        std::string virtualBaseInsertion = " : virtual public " + *visitedClasses.begin();
        std::string insertionText = "class " + derivedClassName + virtualBaseInsertion + " {};\n";
        SourceLocation insertionLoc = DerivedClass->getEndLoc();

        Rewrite.InsertTextAfterToken(insertionLoc, ";\n/*mut404*/" + insertionText);
    }
}

void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(unless(isDerivedFrom(anything()))).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}