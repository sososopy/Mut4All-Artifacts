//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonDependent_Constraint_With_Dependent_94
 */ 
class MutatorFrontendAction_94 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(94)
private:
    class MutatorASTConsumer_94 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_94(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonDependent_Constraint_With_Dependent_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_94::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *innerClass = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("innerClass")) {
        if (!innerClass || !Result.Context->getSourceManager().isWrittenInMainFile(innerClass->getLocation()))
            return;

        auto *outerClass = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("outerClass");
        auto *nonTypeParam = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("nonTypeParam");

        auto outerParams = outerClass->getTemplateParameters();
        auto TParam = outerParams->getParam(0);
        std::string TName = TParam->getNameAsString();

        auto innerParams = innerClass->getTemplateParameters();
        auto UParam = innerParams->getParam(0);
        std::string UName = UParam->getNameAsString();

        SourceLocation constraintStart = nonTypeParam->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation constraintEnd = nonTypeParam->getTypeSourceInfo()->getTypeLoc().getEndLoc();

        if (constraintStart.isInvalid() || constraintEnd.isInvalid())
            return;

        std::string newConstraint = "requires_same_as<" + TName + ", " + UName + ">";
        Rewrite.ReplaceText(SourceRange(constraintStart, constraintEnd), newConstraint);
    }
}
  
void MutatorFrontendAction_94::MutatorASTConsumer_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        has(templateParameterList(
            has(nonTypeTemplateParmDecl(
                has(constraint(type(isNotDependentType()))),
                unless(isDependentType())
            ).bind("nonTypeParam")
        )),
        hasParent(classTemplateDecl().bind("outerClass"))
    ).bind("innerClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}