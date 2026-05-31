//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constrained_Overload_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Constrained_Overload_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the requires clause with its negation
      size_t requiresPos = declaration.find("requires");
      if (requiresPos != std::string::npos) {
        size_t constraintStart = declaration.find_first_of("({", requiresPos);
        size_t constraintEnd = declaration.find_first_of(")}", constraintStart);
        std::string constraint = declaration.substr(constraintStart, constraintEnd - constraintStart + 1);
        std::string negatedConstraint = "!" + constraint;
        declaration.replace(requiresPos + 8, constraint.length(), negatedConstraint);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}