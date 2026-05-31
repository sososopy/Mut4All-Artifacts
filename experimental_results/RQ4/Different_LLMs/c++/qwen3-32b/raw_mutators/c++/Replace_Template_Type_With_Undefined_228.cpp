//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_With_Undefined_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Type_With_Undefined_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                           FD->getLocation()))
            return;
        //Get the source range of the type
        auto typeRange = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        //Replace with undefined type
        Rewrite.ReplaceText(typeRange, "invalid_type_228");
    } 
    else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        //Filter nodes in header files
        if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                           PD->getLocation()))
            return;
        //Get the source range of the type
        auto typeRange = PD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        //Replace with undefined type
        Rewrite.ReplaceText(typeRange, "invalid_type_228");
    }
}
  
void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify template member variables and parameters
    DeclarationMatcher matcher = 
        fieldDecl(declContext(classTemplateDecl())).bind("field") ||
        parmVarDecl(declContext(functionTemplateDecl())).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}