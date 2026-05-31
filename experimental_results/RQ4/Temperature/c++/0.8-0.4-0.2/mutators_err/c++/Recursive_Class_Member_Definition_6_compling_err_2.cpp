//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Class_Member_Definition_6
 */ 
class MutatorFrontendAction_6 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)

private:
    class MutatorASTConsumer_6 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/recursive_class_member_definition_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition() || CL->isLambda() || CL->isTemplateDecl())
            return;
        
        std::string className = CL->getNameAsString();
        if (className.empty())
            return;

        std::string memberDecl = "\n\t" + className + " member;";
        SourceLocation insertLoc = CL->getBraceRange().getEnd().getLocWithOffset(-1);
        
        Rewrite.InsertTextBefore(insertLoc, memberDecl);
    }
}

void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), unless(hasDescendant(templateDecl()))).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}