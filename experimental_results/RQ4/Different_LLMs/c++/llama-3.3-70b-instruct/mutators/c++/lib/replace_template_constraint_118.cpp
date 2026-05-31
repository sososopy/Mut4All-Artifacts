//source file
#include "../include/Mutator_Replace_Template_Constraint_118.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUT118_OUTPUT 1

void MutatorFrontendAction_118::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      std::string declarationStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      //Replace the constraint with a different constraint
      auto constraintStart = declarationStr.find("requires");
      if (constraintStart != std::string::npos) {
        auto constraintEnd = declarationStr.find(")", constraintStart);
        if (constraintEnd != std::string::npos) {
          auto constraint = declarationStr.substr(constraintStart + 7, constraintEnd - constraintStart - 7);
          //Replace the constraint with a different constraint
          auto newConstraint = "sizeof(T) > 0";
          declarationStr.replace(constraintStart + 7, constraint.length(), newConstraint);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declarationStr);
    }
}
  
void MutatorFrontendAction_118::MutatorASTConsumer_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}