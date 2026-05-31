//source file
#include "../include/Replace_Template_Constraints_441.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace template constraints with equivalent constraints
      // For example, replace "requires IsTemplateInstanceOfT<TType, A>" with "requires std::is_same_v<TType, A<typename TType::Value>>"
      std::string replacement = declaration.str();
      size_t pos = replacement.find("requires IsTemplateInstanceOfT");
      if (pos != std::string::npos) {
        replacement.replace(pos, 25, "requires std::is_same_v");
        pos = replacement.find("TType, A");
        if (pos != std::string::npos) {
          replacement.replace(pos, 7, "TType, A<typename TType::Value>");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}