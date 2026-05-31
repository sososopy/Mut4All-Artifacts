//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * modify_pseudo_destructor_expression_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_pseudo_destructor_expression_16.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PD = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDestructor")) {
        //Filter nodes in header files
        if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto destructorType = PD->getDestroyedType().getAsString();

        //Perform mutation on the source code text by applying string replacement
        std::string newType = "NewType";
        std::string typedefDecl = "typedef int NewType;";

        //Replace the original AST node with the mutated one
        SourceLocation startLoc = PD->getBeginLoc();
        Rewrite.InsertTextBefore(startLoc, typedefDecl + "\n");
        Rewrite.ReplaceText(PD->getDestroyedTypeInfo()->getTypeLoc().getSourceRange(), newType);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxPseudoDestructorExpr().bind("PseudoDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}