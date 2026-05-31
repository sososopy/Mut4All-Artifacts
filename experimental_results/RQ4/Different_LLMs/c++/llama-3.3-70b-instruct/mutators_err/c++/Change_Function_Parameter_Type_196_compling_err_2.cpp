//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Change_Function_Parameter_Type_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Change_Function_Parameter_Type_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = MT->getArgs();
      if (args.size() == 0)
        return;
      auto arg = args[0];
      auto argType = arg->getType();
      //Perform mutation on the source code text by applying string replacement
      std::string newType = "char"; // Change to a different type
      std::string mutatedArg = "('" + newType + "')";
      std::string declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      size_t pos = declaration.find(arg->getSourceRange().getBegin().printToString(*Result.SourceManager));
      declaration.replace(pos, arg->getSourceRange().getEnd().printToString(*Result.SourceManager) - pos, mutatedArg);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}