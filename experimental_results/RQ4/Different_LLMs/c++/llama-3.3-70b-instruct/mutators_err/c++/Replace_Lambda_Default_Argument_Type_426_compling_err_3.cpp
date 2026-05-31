//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Lambda_Default_Argument_Type_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)

private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl*> types;
    };
};

//source file
#include "../include/Replace_Lambda_Default_Argument_Type_426.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Error.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(L->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts()
      );
      //Perform mutation on the source code text by applying string replacement
      // Find default argument type
      auto defaultArgType = L->getCallOperator()->getParamDecl(0)->getType().getAsString();
      // Replace default argument type with another type that already exists in the program
      for (const auto &type : types) {
        auto newType = type->getNameAsString();
        if (newType != defaultArgType) {
          lambdaText.replace(lambdaText.find(defaultArgType), defaultArgType.length(), newType);
          break;
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), lambdaText);
    } else if (auto *T = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Type")) {
      // Record existing types in the program
      types.push_back(T);
    }
}

void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr().bind("Lambda");
    auto typeMatcher = cxxRecordDecl().bind("Type");
    auto paramMatcher = parmVarDecl(hasAncestor(lambdaExpr())).bind("ParmVarDecl");
    auto lambdaWithDefaultArgMatcher = lambdaExpr(has(paramMatcher)).bind("LambdaWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaWithDefaultArgMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}