//source file
#include "../include/Replace_Container_Operations_462.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Tooling.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("ContainerOperations")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      std::string strDeclaration = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      if (strDeclaration.find("insert") != std::string::npos) {
        strDeclaration.replace(strDeclaration.find("insert"), 6, "emplace");
      } else if (strDeclaration.find("push_back") != std::string::npos) {
        strDeclaration.replace(strDeclaration.find("push_back"), 9, "emplace_back");
      } else if (strDeclaration.find("erase") != std::string::npos) {
        strDeclaration.replace(strDeclaration.find("erase"), 5, "assign");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), strDeclaration);
    }
}
  
void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr(
        clang::ast_matchers::callee(
            clang::ast_matchers::functionDecl(
                clang::ast_matchers::anyOf(
                    clang::ast_matchers::hasName("insert"), 
                    clang::ast_matchers::hasName("push_back"), 
                    clang::ast_matchers::hasName("erase")
                )
            )
        )
    ).bind("ContainerOperations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}