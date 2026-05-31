//source file
#include "../include/Replace_Lambda_Default_Argument_Type_426.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Error.h"
#include <string>

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
          std::string lambdaTextStr = lambdaText.str();
          size_t pos = lambdaTextStr.find(defaultArgType);
          if (pos != std::string::npos) {
            lambdaTextStr.replace(pos, defaultArgType.length(), newType);
          }
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), lambdaTextStr);
          break;
        }
      }
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