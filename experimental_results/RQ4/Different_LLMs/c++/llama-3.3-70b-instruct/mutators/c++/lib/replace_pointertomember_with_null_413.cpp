//source file
#include "../include/Replace_PointerToMember_With_Null_413.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <string>

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryOperator>("UnaryOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      std::string str = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      if (MT->getOpcode() == UnaryOperatorKind::UO_Deref) {
        size_t pos = str.find("->");
        if (pos != std::string::npos) {
            str.replace(pos, 2, "0");
        }
      } else if (MT->getOpcode() == UnaryOperatorKind::UO_AddrOf) {
        size_t pos = str.find("&");
        if (pos != std::string::npos) {
            str.replace(pos, 1, "");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), str);
    }
}
  
void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = unaryOperator().bind("UnaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}