//source file
#include "../include/Replace_RValue_Reference_7.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Type.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDeclaration = declaration.str();
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
        } else {
          pos = mutatedDeclaration.find("&");
          if (pos != std::string::npos) {
            mutatedDeclaration.erase(pos, 1);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ParmVarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDeclaration = declaration.str();
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
        } else {
          pos = mutatedDeclaration.find("&");
          if (pos != std::string::npos) {
            mutatedDeclaration.erase(pos, 1);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher1 = clang::ast_matchers::varDecl(clang::ast_matchers::hasType(clang::ast_matchers::rValueReferenceType())).bind("VarDecl");
    clang::ast_matchers::DeclarationMatcher matcher2 = clang::ast_matchers::parmVarDecl(clang::ast_matchers::hasType(clang::ast_matchers::rValueReferenceType())).bind("ParmVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}